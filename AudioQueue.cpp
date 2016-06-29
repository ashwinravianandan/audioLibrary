#include <iostream>
#include "AudioQueue.h"
using namespace std;

std::shared_ptr<AudioBufferQ> AudioBufferQ::_audioBufferQ;
mutex AudioBufferQ::_Mutex;

/*..............................................................................
 * @brief AudioBufferQ
 *
 * Input Parameters:
 *    @param:  parameters
 * Return Value:
 *    @returns 
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
AudioBufferQ::AudioBufferQ( size_t queueSize ):
   _ringBuffer(queueSize )
{
   ;
}

/*..............................................................................
 * @brief ~AudioBufferQ
 *
 * Input Parameters:
 *    @param:  parameters
 * Return Value:
 *    @returns 
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
AudioBufferQ::~AudioBufferQ()
{
   ;
}

/*..............................................................................
 * @brief get
 *
 * Input Parameters:
 *    @param: void
 * Return Value:
 *    @returns shared_ptr<AudioBufferQ>
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
shared_ptr<AudioBufferQ> AudioBufferQ::get( void )
{
   if( nullptr == _audioBufferQ )
   {
      lock_guard<mutex> lock( _Mutex );
      if( nullptr == _audioBufferQ )
      {
         _audioBufferQ = shared_ptr<AudioBufferQ>{ new AudioBufferQ{ 8*1024 }};
      }
   }
   return _audioBufferQ;
}

/*..............................................................................
 * @brief enqueue
 *
 * Input Parameters:
 *    @param: 
 *        list<char>& data
 * Return Value:
 *    @returns void
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
void AudioBufferQ::enqueue( const list<char>& data )
{
   _ringBuffer.enqueue( data );
}

/*..............................................................................
 * @brief dequeue
 *
 * Input Parameters:
 *    @param: list<char>& data, 
 *        bytes
 * Return Value:
 *    @returns size_t
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
size_t AudioBufferQ::dequeue( list<char>& data, size_t bytes )
{
   return _ringBuffer.dequeue( data, bytes );
}
/*..............................................................................
 * @brief setBufferSize
 *
 * Input Parameters:
 *    @param:  parameters
 * Return Value:
 *    @returns void
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
void AudioBufferQ::setBufferSize ( const size_t nrOfBytes  )
{
   _ringBuffer.setBufferSize( nrOfBytes );
}

