#include <iostream>
#include "AlsaMicrophoneConfig.h"
using namespace std;

/*..............................................................................
 * @brief ini
 *
 * Input Parameters:
 *    @param:  parameters
 * Return Value:
 *    @returns void
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
bool AlsaMicConfig::initializeConfig ( AudioDevice<snd_pcm_t>* audioDevice  )
{
   bool success = true;
   do
   {
      auto retVal = snd_pcm_hw_params_malloc( &_config );
      if( retVal < 0 )
      {
         cout<<"could not allocate memory for params"<<endl;
         success = false;
         break;
      }
      retVal = snd_pcm_hw_params_any( audioDevice->getHandle(), _config );
      if( retVal < 0 )
      {
         cout<<"init of hw params failed"<<endl;
         success = false;
         break;
      }
      retVal = snd_pcm_hw_params_set_access( 
            audioDevice->getHandle(), _config, SND_PCM_ACCESS_RW_INTERLEAVED );
      if( retVal < 0 )
      {
         cout<<"could not set access"<<endl;
         break;
      }
   }
   while( 0 );
   return success;
}

/*..............................................................................
 * @brief setBitDepth
 *
 * Input Parameters:
 *    @param: 
 *        intdepth, AudioDevice<snd_pcm_t>* audioDevice
 * Return Value:
 *    @returns bool
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
bool AlsaMicConfig::setBitDepth ( unsigned int depth, AudioDevice<snd_pcm_t>* audioDevice )
{
   bool success = true;
   if( 16 == depth )
   {
      auto retVal = snd_pcm_hw_params_set_format(
            audioDevice->getHandle(), _config, SND_PCM_FORMAT_S16_LE );
      if( retVal < 0 )
      {
         cout<<"could not set capture format"<<endl;
         success = false;
      }
   }
   return success;/*bool*/
}

/*..............................................................................
 * @brief setSampleRate
 *
 * Input Parameters:
 *    @param: 
 *        intrate, AudioDevice<snd_pcn_t>* device
 * Return Value:
 *    @returns bool
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
bool AlsaMicConfig::setSampleRate ( unsigned int rate, 
      AudioDevice<snd_pcm_t>* device )
{
   bool success = true;
   //auto retVal = snd_pcm_hw_params_set_rate_near( device->getHandle(), 
   //      _config, &rate, 0 );
   auto retVal = snd_pcm_hw_params_set_rate( device->getHandle(), 
         _config, rate, 0 );
   if( retVal < 0 )
   {
      cout<<"error setting rate"<<endl;
      success = false;
   }
   return success;/*bool*/
}

/*..............................................................................
 * @brief setNumberOfChanels
 *
 * Input Parameters:
 *    @param: 
 *        intnrOfChannels
     , AudioDevice<snd_pcm_t>* device
 * Return Value:
 *    @returns bool
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
bool AlsaMicConfig::setNumberOfChannels ( unsigned int nrOfChannels
     , AudioDevice<snd_pcm_t>* device )
{
   bool success = true;
   auto retVal = snd_pcm_hw_params_set_channels( device->getHandle(),
         _config, nrOfChannels );

   if( retVal < 0 )
   {
      cout<<"could not select mono channel"<<endl;
      success = false;
   }
   return success;/*bool*/
}

/*..............................................................................
 * @brief applySettiing
 *
 * Input Parameters:
 *    @param: AudioDevice<snd_pcm_t>* device
 * Return Value:
 *    @returns bool
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
bool AlsaMicConfig::applySetting( AudioDevice<snd_pcm_t>* device )
{
   bool success = true;
   auto retVal = snd_pcm_nonblock( device->getHandle(), 1 );
   if( retVal < 0 )
   {
      success = false;
      cout<<"could not set non block mode"<<endl;
   }
   retVal = snd_pcm_hw_params( device->getHandle(),
         _config );

   if( retVal < 0 )
   {
      success = false;
      cout<<"could not set params"<<endl;
   }
   return success;/*bool*/
}

