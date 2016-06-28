#include <iostream>
#include "AudioQueue.h"
using namespace std;

std::shared_ptr<AudioBufferQ> AudioBufferQ::_audioBufferQ;
mutex AudioBufferQ::_Mutex;

AudioBufferQ::AudioBufferQ( size_t queueSize ): _ringBuffer(queueSize ){}
AudioBufferQ::~AudioBufferQ(){}

shared_ptr<AudioBufferQ> AudioBufferQ::get( void )
{
   if( nullptr == _audioBufferQ )
   {
      lock_guard<mutex> lock( _Mutex );
      if( nullptr == _audioBufferQ )
      {
         _audioBufferQ = shared_ptr<AudioBufferQ>{ new AudioBufferQ{ 20000 }};
      }
   }
   return _audioBufferQ;
}

void AudioBufferQ::enqueue( const vector<char>& data )
{
   _ringBuffer.enqueue( data );
}

size_t AudioBufferQ::dequeue( vector<char>& data, size_t bytes )
{
   return _ringBuffer.dequeue( data, bytes );
}
