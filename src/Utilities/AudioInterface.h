#pragma once
template <typename T>
class AudioDevice
{
   protected:
      T* _deviceHandle;
   public:
      virtual bool open( const char* ) = 0;
      virtual bool close( void ) = 0;
      virtual T* getHandle( void ) { return _deviceHandle; }
};

template <typename T>
class AudioInput : public AudioDevice<T>
{
   public:
      virtual unsigned int read( char* buffer, unsigned int bufferSize ) = 0;
};

template <typename T>
class AudioOutput : public AudioDevice<T>
{
   public:
      virtual unsigned int write( char* buffer, unsigned int bufferSize ) = 0;
};

template <typename T, typename U, typename Device = AudioDevice<U> >
class AudioConfiguration
{
   protected:
      T* _config;
   public:
      virtual bool initializeConfig( Device* audioDevice ) = 0;
      virtual bool setSampleRate( unsigned int , Device* audioDevice) = 0;
      virtual bool setBitDepth( unsigned int, Device* audioDevice ) = 0;
      virtual bool setNumberOfChannels( unsigned int, Device* audioDevice ) = 0;
      virtual bool applySetting( Device* audioDevice ) = 0;
      virtual bool setPeriodSize( unsigned int numberOfFrames, Device* audioDevice ) = 0;
      virtual unsigned int getFrameSize() = 0;
};


