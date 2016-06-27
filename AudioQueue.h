#pragma once
#include <memory>
#include "SynchornizedRingBuffer.h"
#include <mutex>
using namespace std;

class AudioBufferQ
{
   private:
      SynchronizedRingBuffer<char> _ringBuffer;
      static std::shared_ptr<AudioBufferQ> _audioBufferQ;
      static mutex _Mutex;
      AudioBufferQ( size_t queueSize );

   public:
      AudioBufferQ() = delete;
      AudioBufferQ(const AudioBufferQ&) = delete;
      AudioBufferQ& operator = (const AudioBufferQ& ) = delete;
      ~AudioBufferQ();

      static shared_ptr<AudioBufferQ> get( void );
      void enqueue( const vector<char>& data );
      size_t dequeue( vector<char>& data, size_t bytes );

};
