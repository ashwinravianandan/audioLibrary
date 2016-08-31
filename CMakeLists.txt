cmake_minimum_required( VERSION 2.6 )

project( AudioRec )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set( CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} )

set (SOURCE  AudioQueue.cpp AlsaMicrophone.cpp AlsaBaseConfig.cpp main.cpp )
set (SOURCE1 AudioOuptputQueue.cpp AlsaSpeaker.cpp AlsaBaseConfig.cpp main2.cpp )

find_library( ASOUND_INC asound )

find_path( ALSA_INCLUDE alsa/asoundlib.h )

include_directories( "${ALSA_INCLUDE}" )

add_executable( audiorec ${SOURCE}) 
add_executable( audioplay ${SOURCE1}) 

target_link_libraries( audiorec ${ASOUND_INC} ) 
target_link_libraries( audioplay ${ASOUND_INC} )

install (TARGETS audiorec DESTINATION bin)
install (TARGETS audioplay DESTINATION bin)

