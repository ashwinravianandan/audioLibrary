#pragma once
#include "SynchronizedQueue.h"
#include <mutex>
#include <memory>
using namespace std;

class AudioOutputQ
{
   private:
      SynchronizedQueue<char> _queue;
      static mutex _Mutex;
      static shared_ptr<AudioOutputQ> _sigletonInstance;
      AudioOutputQ( );
   protected:

   public:
      AudioOutputQ( const AudioOutputQ& ) = delete;
      AudioOutputQ& operator = ( const AudioOutputQ& ) = delete;

      virtual ~AudioOutputQ( );
      static shared_ptr<AudioOutputQ> get( void );
      void enqueue( const list<char>& );
      void dequeue( list<char>&, size_t numberOfBytes );
      bool isEmpty(){ return _queue.isEmpty(); }
};

