#pragma once
#include<alsa/asoundlib.h>
#include "AudioInterface.h"
#include <mutex>
#include <thread>
using namespace std;

class AlsaMicrophone : public AudioInput<snd_pcm_t>
{
   private:
      bool _readInProgress;
      thread readThread;
   public:
      AlsaMicrophone():_readInProgress(false){}
      ~AlsaMicrophone(){}
      bool open( const char* deviceName ) override final;
      bool close( void ) override final;
      unsigned int read( char* buffer, unsigned int bufferSize ) override final;
      void pollAudioDevice( );
};

