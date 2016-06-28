cmake_minimum_required( VERSION 2.6 )

project( AudioRec )

set( CMAKE_CXX_FLAGS "-std=c++1y -m32")

set (SOURCE  AudioQueue.cpp AlsaMicrophone.cpp AlsaMicrophoneConfig.cpp main.cpp )

add_executable( audiorec ${SOURCE}) 

target_link_libraries( audiorec asound )

install (TARGETS audiorec DESTINATION bin)

