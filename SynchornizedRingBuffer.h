#pragma once
#include <mutex>
#include <queue>
#include <iostream>
#include <condition_variable>
using namespace std;

template <typename T>
class SynchronizedRingBuffer{
   private:
      queue<T> _Buffer;
      mutex _Mutex;
      size_t _MaxRecords;
      condition_variable _QNotEmpty;
   public:
      SynchronizedRingBuffer( size_t queueSize ):_MaxRecords(queueSize ){}
      ~SynchronizedRingBuffer(){}

      void enqueue( const vector<T>& data)
      {
         cout<<"Queueing "<<data.size()<< "bytes of data"<<endl;
         lock_guard<std::mutex> lock( _Mutex );
         for( auto item : data )
         {
            if( _Buffer.size() == _MaxRecords )
            {
               _Buffer.pop();
            }
            _Buffer.push( item );
         }
         if( data.size() > 0 )
         {
            _QNotEmpty.notify_one();
         }
      }

      size_t dequeue( vector<T>& data, size_t nrOfRecords )
      {
         unique_lock<std::mutex> condLock( _Mutex );
         cout<<"Waiting for queue to be filled"<<endl;
         _QNotEmpty.wait( condLock, [this](){ return !(this->_Buffer.empty()); } );
         cout<<"Dequeue unblocked"<<endl;

         auto bytesRead = 0;
         auto count = nrOfRecords;
         cout<<"Dequeueing "<<count<<" bytes"<<endl;
         while( ( count > 0 ) && ( _Buffer.size() > 0 ) )
         {
            ++bytesRead;
            data.push_back( _Buffer.front() );
            _Buffer.pop();
            --count;
         }
         return bytesRead;
      }
};

