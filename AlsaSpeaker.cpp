#include "AlsaSpeaker.h"
#include "AudioOuptputQueue.h"
#include <iostream>
using namespace std;

/*..............................................................................
 * @brief open
 *
 * Input Parameters:
 *    @param:  parameters
 * Return Value:
 *    @returns bool
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
bool AlsaSpeaker::open (  const char* name )
{
   bool success = true;

   auto retVal = snd_pcm_open( &_deviceHandle, name, SND_PCM_STREAM_PLAYBACK, 0 );
   if( retVal < 0 )
   {
      success = false;
   }
   return success;/*bool*/
}

/*..............................................................................
 * @brief close
 *
 * Input Parameters:
 *    @param:  parameters
 * Return Value:
 *    @returns bool
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
bool AlsaSpeaker::close ( )
{
   bool success = true;
   _writingInProgress = false;
   _writeThread.join();
   auto retVal = snd_pcm_close( _deviceHandle );
   if( retVal < 0)
   {
      success = false;
   }
   return success;/*bool*/
}
/*..............................................................................
 * @brief writeToAudioDevice
 *
 * Input Parameters:
 *    @param: 
 * Return Value:
 *    @returns void
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
void AlsaSpeaker::writeToAudioDevice (  )
{
   auto count = snd_pcm_poll_descriptors_count( _deviceHandle );
   auto descriptors = new pollfd[ count ];
   snd_pcm_poll_descriptors( _deviceHandle, descriptors, count );
   while( 1 )
   {
      if( ( false == _writingInProgress ) && AudioOutputQ::get()->isEmpty())
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
         else if( POLLOUT == ( POLLOUT & pollEvent) /*|| 
               POLLPRI == ( POLLPRI & pollEvent )*/ )
         {
            auto frames = snd_pcm_avail_update( _deviceHandle );
            if( frames < 0 )
            {
               cout<<"Error in getting available buffer, with code "<< frames<<"\n";
               if( frames == -EPIPE )
               {
                  snd_pcm_prepare( _deviceHandle);
               }
               else if( frames == -ESTRPIPE )
               {
                  while (-EAGAIN == snd_pcm_resume( _deviceHandle))
                     usleep(2000);
               }
               else
               {
                  snd_pcm_prepare( _deviceHandle);
               }
            }
            else
            {
               cout<<"Number of available frames is "<<frames<<endl;
               list<char> data;
               AudioOutputQ::get()->dequeue(data, frames*_config->getFrameSize() );
               char* buffer = new char[ data.size() ];
               auto ptr = buffer;
               for( auto&& byte : data )
               {
                  *ptr = byte;
                  ++ptr;
               }

               cout<<"Writing "<<data.size()<<" bytes to device\n";
               auto retval = snd_pcm_writei( 
                     _deviceHandle, buffer, data.size()/_config->getFrameSize() );
               if( retval < 0 )
               {
                  cout<<"Error in getting available buffer, with code "<< retval<<"\n";
                  if( retval == -EPIPE )
                  {
                     snd_pcm_prepare( _deviceHandle);
                  }
                  else if( retval == -ESTRPIPE )
                  {
                     while (-EAGAIN == snd_pcm_resume( _deviceHandle))
                        usleep(2000);
                  }
                  else
                  {
                     snd_pcm_prepare( _deviceHandle);
                  }
               }
            }
         }
         else
         {
            cout<<" event "<<pollEvent<<" received\n";
            auto frames = snd_pcm_avail_update( _deviceHandle );
            if( frames < 0 )
            {
               cout<<"Error in getting available buffer, with code "<< frames<<"\n";
               if( frames == -EPIPE )
               {
                  snd_pcm_prepare( _deviceHandle);
               }
               else if( frames == -ESTRPIPE )
               {
                  while (-EAGAIN == snd_pcm_resume( _deviceHandle))
                     usleep(2000);
               }
               else
               {
                  snd_pcm_prepare( _deviceHandle);
               }
            }
            else
            {
               cout<<"Number of available frames is "<<frames<<endl;
               list<char> data;
               AudioOutputQ::get()->dequeue(data, frames*_config->getFrameSize() );
               char* buffer = new char[ data.size() ];
               auto ptr = buffer;
               for( auto&& byte : data )
               {
                  *ptr = byte;
                  ++ptr;
               }

               cout<<"Writing "<<data.size()<<" bytes to device\n";
               auto retval = snd_pcm_writei( 
                     _deviceHandle, buffer, data.size()/_config->getFrameSize() );
               if( retval < 0 )
               {
                  cout<<"Error in getting available buffer, with code "<< retval<<"\n";
                  if( retval == -EPIPE )
                  {
                     snd_pcm_prepare( _deviceHandle);
                  }
                  else if( retval == -ESTRPIPE )
                  {
                     while (-EAGAIN == snd_pcm_resume( _deviceHandle))
                        usleep(2000);
                  }
                  else
                  {
                     snd_pcm_prepare( _deviceHandle);
                  }
               }
            }

            continue;
         }
      }
   }
   //list<char> data;
   //AudioOutputQ::get()->dequeue(data, 1024 );
   //char* buffer = new char[ data.size() ];
   //auto ptr = buffer;
   //for( auto&& byte : data )
   //{
   //   *ptr = byte;
   //   ++ptr;
   //}

   //auto retVal = snd_pcm_writei( 
   //      _deviceHandle, buffer, data.size()/_config->getFrameSize() );

   return ;/*void*/
}
/*..............................................................................
 * @brief write
 *
 * Input Parameters:
 *    @param: char* buffer, 
 *        intnumberOfBytes
 * Return Value:
 *    @returns unsigned int
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
unsigned int AlsaSpeaker::write ( char* buffer, unsigned int numberOfBytes )
{
   list<char> data;
   for( auto ptr = buffer; numberOfBytes > 0; ++ptr,--numberOfBytes)
   {
      data.push_back( *ptr );
   }
   AudioOutputQ::get()->enqueue( data );

   if ( false == _writingInProgress )
   {
      _writingInProgress = true;
      _writeThread = thread{ &AlsaSpeaker::writeToAudioDevice, this };
   }

   return data.size();/*unsigned int*/
}

