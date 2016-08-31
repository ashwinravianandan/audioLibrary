#include<iostream>
#include "AlsaSpeaker.h"
#include "AlsaBaseConfig.h"
#include <memory>
#include <fstream>
#include "AudioOuptputQueue.h"
#include <unistd.h>
using namespace std;


int main()
{
   unique_ptr<AudioConfiguration<snd_pcm_hw_params_t, snd_pcm_t>> micConfig{ new AlsaBaseConfig{} };
   unique_ptr<AudioOutput<snd_pcm_t>> speaker{ new AlsaSpeaker{micConfig.get()} };

   speaker->open( "media_p" );
   micConfig->initializeConfig( speaker.get() );
   micConfig->setBitDepth( 16, speaker.get() );
   micConfig->setSampleRate( 16000, speaker.get() );
   micConfig->setNumberOfChannels( 1, speaker.get() );
   micConfig->setPeriodSize( 1024, speaker.get() );
   micConfig->applySetting( speaker.get() );

   
   ifstream pcmFile{ "utter.pcm", ios::binary };
   while( ! pcmFile.eof() )
   {
      char buffer[512] = { '\0' };
      pcmFile.read( buffer, 512 );
      speaker->write( buffer, 512 );
   }
   pcmFile.close();
   speaker->close();

   return 0;
}

