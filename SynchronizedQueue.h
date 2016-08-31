#pragma once

#include <queue>
#include <mutex>
#include <list>
#include <condition_variable>
#include <iostream>
using namespace std;

using namespace std;

template <typename T>
class SynchronizedQueue
{
   private:
      queue<T> _queue;
      mutex _mutex;
      condition_variable _QNotEmpty;
   public:
      void enqueue( const list<T>& data )
      {
         lock_guard<mutex> lock( _mutex );
         for( auto datum: data )
         {
            _queue.push( datum );
         }
         if( _queue.size() > 0 )
         {
            cout<<"Number of bytes in queue is "<<_queue.size()<<endl;
            _QNotEmpty.notify_one();
         }
      }

      void dequeue( list<T>& data, size_t numberOfBytes )
      {
         unique_lock<mutex> lock( _mutex );
         cout<<"Waiting for queue to unblock"<<endl;
         if( true == _queue.empty() )
         {
            _QNotEmpty.wait( lock, [this](){ return !( this->_queue.empty());} );
         }
         cout<<"Queue unblocked"<<endl;

         auto bytesToRead = numberOfBytes;
         while( ( bytesToRead > 0 ) && ( _queue.size() > 0 ) )
         {
            data.push_back( _queue.front() );
            _queue.pop();
            --bytesToRead;
         }
      }
      bool isEmpty(){ return _queue.empty(); }
};

