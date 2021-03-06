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
      AudioConfiguration<snd_pcm_hw_params_t, snd_pcm_t>* _config;
      void triggerRead ( void )const;
      bool processMicData ( void )const;

   public:
      AlsaMicrophone():_readInProgress(false){}
      AlsaMicrophone(AudioConfiguration<snd_pcm_hw_params_t, snd_pcm_t>* config):
         _readInProgress(false),
         _config(config)
   {
      ;
   }
      ~AlsaMicrophone(){}
      bool open( const char* deviceName ) override final;
      bool close( void ) override final;
      unsigned int read( char* buffer, unsigned int bufferSize ) override final;
      void pollAudioDevice( );
};

