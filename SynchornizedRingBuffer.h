#pragma once
#include <mutex>
#include <queue>
#include <iostream>
#include <condition_variable>
#include <list>
using namespace std;

template <typename T>
class SynchronizedRingBuffer{
   private:
      queue<T> _Buffer;
      size_t _MaxRecords;
      mutex _Mutex;
      condition_variable _QNotEmpty;
   public:

      SynchronizedRingBuffer( size_t queueSize ):_MaxRecords(queueSize ){}
      ~SynchronizedRingBuffer(){}

      void setBufferSize( const size_t bytes ){ _MaxRecords = bytes; }

      void enqueue( const list<T>& data)
      {
         lock_guard<std::mutex> lock( _Mutex );
         for( auto item : data )
         {
            if( _Buffer.size() == _MaxRecords )
            {
               cout<<"Error: Throwing away data"<<endl;
               _Buffer.pop();
            }
            _Buffer.push( item );
         }
         if( data.size() > 0 )
         {
            _QNotEmpty.notify_one();
         }
      }

      size_t dequeue( list<T>& data, size_t nrOfRecords )
      {
         unique_lock<std::mutex> condLock( _Mutex );

         if( _Buffer.empty() )
         {
            cout<<"Waiting for queue to be filled"<<endl;
            _QNotEmpty.wait( condLock,
                  [this](){ return !(this->_Buffer.empty()); } );
         }

         cout<<"Dequeuing data"<<endl;
         auto bytesRead = 0;

         auto count = nrOfRecords;
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

