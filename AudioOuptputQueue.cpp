#include "AudioOuptputQueue.h"

shared_ptr<AudioOutputQ> AudioOutputQ::_sigletonInstance;
mutex AudioOutputQ::_Mutex;

/*..............................................................................
 * @brief AudioOutputQ
 *
 * Input Parameters:
 *    @param:  parameters
 * Return Value:
 *    @returns 
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
 AudioOutputQ::AudioOutputQ (  )
{
}

/*..............................................................................
 * @brief ~AudioOutputQ
 *
 * Input Parameters:
 *    @param:  parameters
 * Return Value:
 *    @returns 
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
 AudioOutputQ::~AudioOutputQ (  )
{
}
/*..............................................................................
 * @brief get
 *
 * Input Parameters:
 *    @param: void
 * Return Value:
 *    @returns shared_ptr<AudioOutputQ>
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
shared_ptr<AudioOutputQ> AudioOutputQ::get ( void )
{
   if( nullptr == _sigletonInstance )
   {
      lock_guard<mutex> lock{ _Mutex };
      if( nullptr == _sigletonInstance )
      {
         _sigletonInstance = shared_ptr<AudioOutputQ>{new AudioOutputQ};
      }
   }
   return _sigletonInstance;/*shared_ptr<AudioOutputQ>*/
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
void AudioOutputQ::enqueue ( const list<char>& data )
{
   _queue.enqueue( data );
   return ;/*void*/
}

/*..............................................................................
 * @brief dequeue
 *
 * Input Parameters:
 *    @param: list<char>& data, 
 *        numberOfBytes
 * Return Value:
 *    @returns void
 *
 * External methods/variables:
 *    @extern
 *............................................................................*/
void AudioOutputQ::dequeue ( list<char>& data, size_t numberOfBytes )
{
   _queue.dequeue( data, numberOfBytes );
   return ;/*void*/
}

