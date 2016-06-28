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
   unique_ptr<AudioInput<snd_pcm_t>> microphone{ new AlsaMicrophone{} };
   auto micConfig = unique_ptr<AudioConfiguration<snd_pcm_hw_params_t, snd_pcm_t>>
   { new AlsaMicConfig{} };

   cout<<microphone->open( "default" );
   cout<<micConfig->initializeConfig( microphone.get() );
   cout<<micConfig->setBitDepth( 16, microphone.get() );
   cout<<micConfig->setSampleRate( 44100, microphone.get() );
   cout<<micConfig->setNumberOfChannels( 1, microphone.get() );
   micConfig->applySetting( microphone.get() );

   ofstream utterance{ "utter.pcm",ios::binary};

   for( auto i = 0; i < 1000; ++i )
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

