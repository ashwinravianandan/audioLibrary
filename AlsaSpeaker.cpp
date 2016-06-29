#include "AlsaSpeaker.h"

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
   auto retVal = snd_pcm_close( _deviceHandle );
   if( retVal < 0)
   {
      success = false;
   }
   return success;/*bool*/
}


