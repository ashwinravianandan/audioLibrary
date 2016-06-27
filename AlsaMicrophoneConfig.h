#pragma once
#include<alsa/asoundlib.h>
#include "AudioInterface.h"

class AlsaMicConfig: public AudioConfiguration<snd_pcm_hw_params_t, snd_pcm_t>
{
   public:
      AlsaMicConfig(){}
      ~AlsaMicConfig(){}
      virtual bool initializeConfig( AudioDevice<snd_pcm_t>* ) override final ;

      virtual bool setSampleRate( unsigned int ,
             AudioDevice<snd_pcm_t>* ) override final;

      virtual bool setBitDepth( unsigned int,
             AudioDevice<snd_pcm_t>* ) override final;

      virtual bool setNumberOfChannels( unsigned int,
            AudioDevice<snd_pcm_t>* ) override final;

      virtual bool applySetting( AudioDevice<snd_pcm_t>* ) override final;
};
