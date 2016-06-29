#include <sys/asoundlib.h>
#include "AudioInterface.h"
#include <thread>
using namespace std;

class AlsaSpeaker: public AudioOutput<snd_pcm_t>
{
   private:
      AudioConfiguration<snd_pcm_hw_params_t,snd_pcm_t>* _config;
      thread _writeThread;
      bool _writingInProgress;
   protected:

   public:
      bool open( const char* name ) override final;
      bool close( ) override final;
      unsigned int write( char* buffer, unsigned int numberOfBytes )override final;
      AlsaSpeaker( );
      virtual ~AlsaSpeaker( );
};
