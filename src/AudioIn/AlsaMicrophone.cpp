#include "AlsaMicrophone.h"
#include "AudioQueue.h"
#include <thread>
#include <sys/poll.h>
#include <iostream>
using namespace std;

/*..............................................................................
 * @brief open
 *
 * Input Parameters:
 *    @param: 
 *        char* deviceName
 * Return Value:
 *    @returns void
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
bool AlsaMicrophone::open ( const char* deviceName )
{
   bool success = true;
   auto retVal = snd_pcm_open( &_deviceHandle, deviceName, SND_PCM_STREAM_CAPTURE, 0 );

   if( retVal < 0 )
      success = false;

   return success;/*bool*/
}

void test()
{
}
/*..............................................................................
 * @brief read
 *
 * Input Parameters:
 *    @param: char* buffer, 
 *        intbufferSize
 * Return Value:
 *    @returns unsigned int
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
unsigned int AlsaMicrophone::read ( char* buffer, unsigned int bufferSize )
{
   unsigned int readBytes = 0;
   if ( false == _readInProgress )
   {
      /// create thread
      _readInProgress = true;
      readThread = thread{ &AlsaMicrophone::pollAudioDevice, this };
   }
   list<char> data;
   readBytes = AudioBufferQ::get()->dequeue( data , bufferSize);
   for( auto byte : data )
   {
      *buffer = byte; ++buffer;
   }
   
   return readBytes;/*unsigned int*/
}

/*..............................................................................
 * @brief close
 *
 * Input Parameters:
 *    @param: void
 * Return Value:
 *    @returns bool
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
bool AlsaMicrophone::close ( void )
{
   bool success = true;
   _readInProgress = false;

   auto retVal = snd_pcm_close( _deviceHandle );
   if( retVal < 0)
   {
      success = false;
   }
   return success;/*bool*/
}

/*..............................................................................
 * @brief triggerRead
 *
 * Input Parameters:
 *    @param: void
 * Return Value:
 *    @returns void
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
void AlsaMicrophone::triggerRead ( void )const
{
   char *data = new char[1024];
   cout<<"Read triggered"<<endl;
   auto retVal = snd_pcm_readi( _deviceHandle, data, 1024/_config->getFrameSize() );
   if( retVal == -EPIPE )
   {
      cout<<"underrun"<<endl;
      retVal = snd_pcm_prepare( _deviceHandle );
      if( retVal < 0 )
      {
         cout<<"could not prepare node"<<endl;
      }
   }
   else if( retVal < 0 )
   {
      cout<<"read error"<<endl;
   }
   else
   {
      cout<<"Reader Thread: "<<retVal<<"bytes read"<<endl;
      list<char> vectorData;
      retVal *= _config->getFrameSize();
      for( auto ptr = data; retVal > 0; --retVal,ptr++ )
      {
         vectorData.push_back( *ptr );
      }
      AudioBufferQ::get()->enqueue( vectorData );
      vectorData.clear();
   }
   delete []data;

   return ;/*void*/
}
/*..............................................................................
 * @brief processMicData
 *
 * Input Parameters:
 *    @param: void
 * Return Value:
 *    @returns void
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
bool AlsaMicrophone::processMicData ( void )const
{
   bool success = true;
   auto frames = snd_pcm_avail( _deviceHandle );
   cout<<"Number of frames available from polling: "<<frames;
   char* buffer = new char[ frames*_config->getFrameSize() ];
   auto retVal = snd_pcm_readi( _deviceHandle, buffer, frames );
   if( retVal == -EPIPE )
   {
      cout<<"underrun"<<endl;
      retVal = snd_pcm_prepare( _deviceHandle );
      if( retVal < 0 )
      {
         cout<<"could not prepare node"<<endl;
         success = false;
      }
   }
   else if( retVal < 0 )
   {
      cout<<"error reading data"<<endl;
      success = false;
   }
   else
   {
      cout<<"reader thread, Poll phase => read "<<retVal*_config->getFrameSize()<<"bytes"<<endl;
      list<char> data;
      retVal*=_config->getFrameSize();
      for( auto ptr = buffer; retVal > 0; --retVal,ptr++ )
      {
         data.push_back( *ptr );
      }
      AudioBufferQ::get()->enqueue( data );
      data.clear();
   }
   delete []buffer;
   return success;/*bool*/
}
/*..............................................................................
 * @brief pollAudioDevice
 *
 * Input Parameters:
 *    @param:  parameters
 * Return Value:
 *    @returns void
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
void AlsaMicrophone::pollAudioDevice (  )
{
   auto count = snd_pcm_poll_descriptors_count( _deviceHandle );
   auto descriptors = new pollfd[count];
   (void)snd_pcm_poll_descriptors( _deviceHandle, descriptors, count );
   triggerRead();
   while( 1 )
   {
      if( false == _readInProgress )
         break;

      if( 0 > poll( descriptors, count, 500 ) )
      {
         cout<<"error polling for data"<<endl;
         break;
      }
      else
      {
         cout<<"Poll unblocked"<<endl;
         unsigned short pollEvent = 0;
         if( 0 > snd_pcm_poll_descriptors_revents( _deviceHandle, descriptors, count, &pollEvent ) )
         {
            cout<<"Error getting poll events";
            break;
         }
         else if( POLLIN == pollEvent )
         {
            if(!processMicData())
            {
               break;
            }
         }
         else
         {
            continue;
         }
      }
   }
}


