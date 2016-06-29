#include<iostream>
#include "AlsaMicrophone.h"
#include "AlsaMicrophoneConfig.h"
#include <memory>
#include <fstream>
#include "SynchornizedRingBuffer.h"
#include "AudioQueue.h"
#include <unistd.h>
using namespace std;


int main()
{
   auto micConfig = unique_ptr<AudioConfiguration<snd_pcm_hw_params_t, snd_pcm_t>>
   { new AlsaMicConfig{} };
   unique_ptr<AudioInput<snd_pcm_t>> microphone{ new AlsaMicrophone{micConfig.get()} };

   microphone->open( "default" );
   micConfig->initializeConfig( microphone.get() );
   micConfig->setBitDepth( 16, microphone.get() );
   micConfig->setSampleRate( 44100, microphone.get() );
   micConfig->setNumberOfChannels( 1, microphone.get() );
   micConfig->setPeriodSize( 1024, microphone.get() );
   micConfig->applySetting( microphone.get() );

   AudioBufferQ::get()->setBufferSize( 1024 * 2 * 4 );
   

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

