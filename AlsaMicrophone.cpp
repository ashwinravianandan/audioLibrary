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
      cout<<"Read is not in progress, creating thread"<<endl;
      readThread = thread{ &AlsaMicrophone::pollAudioDevice, this };
      cout<<"Thread created"<<endl;
   }
   vector<char> data;
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
   /*
    * @todo : close the pcm node
    */
   
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
   cout<<"Thread to poll device has started"<<endl;
   auto count = snd_pcm_poll_descriptors_count( _deviceHandle );
   cout<< "Polling "<<count<<" descriptors"<<endl;
   auto descriptors = new pollfd[count];
   auto nrOfConfiguredDesc = snd_pcm_poll_descriptors( _deviceHandle, descriptors, count );
   cout<<"Populated "<<nrOfConfiguredDesc<<" descriptors"<<endl;
   while( 1 )
   {
      if( false == _readInProgress )
         break;

      char* data = new char[512];
      auto retVal = snd_pcm_readi( _deviceHandle, data, 256 );
      if( retVal == -EPIPE )
      {
         cout<<"underrun"<<endl;
         retVal = snd_pcm_prepare( _deviceHandle );
         if( retVal < 0 )
         {
            cout<<"could not prepare node"<<endl;
            break;
         }
      }
      else if( retVal < 0 )
      {
         cout<<"read error"<<endl;
      }
      else
      {
         retVal*=2;
         cout<<"Reader Thread: "<<retVal<<"bytes read"<<endl;
         vector<char> vectorData;
         for( auto ptr = data; retVal > 0; --retVal,ptr++ )
         {
            vectorData.push_back( *ptr );
         }

         AudioBufferQ::get()->enqueue( vectorData );
         vectorData.clear();
      }
      delete []data;

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
            cout<<"Read and enqueue data"<<endl;
            auto frames = snd_pcm_avail( _deviceHandle );
            char* buffer = new char[ frames*2];
            auto retVal = snd_pcm_readi( _deviceHandle, buffer, frames );
            if( retVal == -EPIPE )
            {
               cout<<"underrun"<<endl;
               retVal = snd_pcm_prepare( _deviceHandle );
               if( retVal < 0 )
               {
                  cout<<"could not prepare node"<<endl;
                  break;
               }
            }
            else if( retVal < 0 )
            {
               cout<<"error reading data"<<endl;
               break;
            }
            else
            {
               cout<<"reader thread, Poll phase => read "<<retVal*2<<"bytes"<<endl;
               retVal*=2;
               vector<char> data;
               for( auto ptr = buffer; retVal > 0; --retVal,ptr++ )
               {
                  data.push_back( *ptr );
               }
               AudioBufferQ::get()->enqueue( data );
               data.clear();
            }
            delete []buffer;
         }
         else
         {
            continue;
         }
      }
   }
}


