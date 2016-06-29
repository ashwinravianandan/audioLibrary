#pragma once
#include<alsa/asoundlib.h>
#include "AudioInterface.h"

typedef AudioDevice<snd_pcm_t> AlsaDevice;

class AlsaMicConfig: public AudioConfiguration<snd_pcm_hw_params_t, snd_pcm_t>
{
   unsigned int _frameSize;
   unsigned int _numberOfChannels;
   public:
      AlsaMicConfig():_frameSize(4),_numberOfChannels(2){}
      ~AlsaMicConfig(){}
      virtual bool initializeConfig( AlsaDevice* ) override final ;

      virtual bool setSampleRate( unsigned int ,
             AlsaDevice* ) override final;

      virtual bool setBitDepth( unsigned int,
             AlsaDevice* ) override final;

      virtual bool setNumberOfChannels( unsigned int,
            AlsaDevice* ) override final;

      virtual bool applySetting( AlsaDevice* ) override final;

      virtual unsigned int getFrameSize() override final;

      virtual bool setPeriodSize( unsigned int numberOfFrames, 
            AlsaDevice* device ) override final;
};
