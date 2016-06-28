#include<iostream>
#include "AlsaMicrophone.h"
#include "AlsaMicrophoneConfig.h"
#include <memory>
#include <fstream>
#include "SynchornizedRingBuffer.h"
#include <unistd.h>
using namespace std;


int main()
{

//   snd_pcm_t *micHandle = NULL;
//   snd_pcm_hw_params_t *hwParams = NULL;
//
//   do
//   {
//      auto retVal = snd_pcm_open( &micHandle, "default", SND_PCM_STREAM_CAPTURE, 0 );
//      if( retVal < 0 )
//      {
//         cout<<"could not open node for capture"<<endl;
//         break;
//      }
//      retVal = snd_pcm_hw_params_malloc( &hwParams );
//      if( retVal < 0 )
//      {
//         cout<<"could not allocate memory for params"<<endl;
//         break;
//      }
//      retVal = snd_pcm_hw_params_any( micHandle, hwParams );
//      if( retVal < 0 )
//      {
//         cout<<"init of hw params failed"<<endl;
//         break;
//      }
//
//      retVal = snd_pcm_hw_params_set_access( 
//            micHandle, hwParams, SND_PCM_ACCESS_RW_INTERLEAVED );
//      if( retVal < 0 )
//      {
//         cout<<"could not set access"<<endl;
//         break;
//      }
//      retVal = snd_pcm_hw_params_set_format(
//            micHandle, hwParams, SND_PCM_FORMAT_S16_LE );
//      if( retVal < 0 )
//      {
//         cout<<"could not set capture format"<<endl;
//         break;
//      }
//
//      retVal = snd_pcm_hw_params_set_channels( micHandle, hwParams, 1 );
//      if( retVal < 0 )
//      {
//         cout<<"could not select mono channel"<<endl;
//         break;
//      }
//
//      //snd_pcm_uframes_t frame = 128;
//      //int dir;
//      //retVal = snd_pcm_hw_params_set_period_size_near( micHandle, hwParams, &frame, &dir );
//      //if( retVal < 0 )
//      //{
//      //   cout<<"could not set params"<<endl;
//      //   break;
//      //}
//
//      retVal = snd_pcm_hw_params( micHandle, hwParams );
//      if( retVal < 0 )
//      {
//         cout<<"could not set params"<<endl;
//         break;
//      }
//
//   }
//   while(0);
//
//   short buffer[512];
//
//   ofstream pcmFile( "utternace.pcm", ios::binary );
//   for( auto i = 0; i < 500; ++i )
//   {
//      auto retVal = snd_pcm_readi( micHandle, buffer, 512 );
//      if( retVal == -EPIPE )
//      {
//         cout<<"underrun"<<endl;
//         retVal = snd_pcm_prepare( micHandle );
//         if( retVal < 0 )
//         {
//            cout<<"could not prepare node"<<endl;
//            break;
//         }
//      }
//      else if( retVal < 0 )
//      {
//         cout<<"error reading data"<<endl;
//         break;
//      }
//      else
//      {
//         cout<<retVal<<" bytes read"<<endl;
//         pcmFile.write( (char*) buffer, retVal*2 );
//      }
//   }
//   pcmFile.close();
//
//   snd_pcm_close( micHandle );
//

   unique_ptr<AudioInput<snd_pcm_t>> microphone{ new AlsaMicrophone{} };
   auto micConfig = unique_ptr<AudioConfiguration<snd_pcm_hw_params_t, snd_pcm_t>>
   { new AlsaMicConfig{} };



   cout<<microphone->open( "default" );
   cout<<micConfig->initializeConfig( microphone.get() );
   cout<<micConfig->setBitDepth( 16, microphone.get() );
   cout<<micConfig->setSampleRate( 16000, microphone.get() );
   cout<<micConfig->setNumberOfChannels( 1, microphone.get() );
   micConfig->applySetting( microphone.get() );
   //snd_pcm_sw_params_t *swParams;

   //snd_pcm_sw_params_alloca( &swParams );
   //int retVal = snd_pcm_sw_params_current( microphone->getHandle(), swParams );

   //if( retVal < 0 )
   //{
   //   cout<<"error getting current software params"<<endl;
   //}

   //retVal = snd_pcm_sw_params_set_start_threshold( microphone->getHandle(),
   //      swParams, 50 );
   //if( retVal < 0 )
   //{
   //   cout<<"error setting start threshold"<<endl;
   //}

   //retVal = snd_pcm_sw_params_set_avail_min( microphone->getHandle(),
   //      swParams, 50 );
   //if( retVal < 0 )
   //{
   //   cout<<"error setting start threshold"<<endl;
   //}

   //retVal = snd_pcm_sw_params_set_period_event( microphone->getHandle(), swParams, 1 );
   //if( retVal < 0 )
   //{
   //   cout<<"error setting start threshold"<<endl;
   //}

   //retVal = snd_pcm_sw_params( microphone->getHandle(), swParams );
   //if( retVal < 0 )
   //{
   //   cout<<"error setting start threshold"<<endl;
   //}
   ofstream utterance{ "utter.pcm",ios::binary};

   for( auto i = 0; i < 200; ++i )
   {
      char buffer[1024];
      auto retVal = microphone->read( buffer, 1024);
      cout<<"Read "<<retVal<<" bytes"<<endl;
      utterance.write( buffer, retVal );
   }
   utterance.close();
   microphone->close();

   return 0;
}

