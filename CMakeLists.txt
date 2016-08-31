cmake_minimum_required( VERSION 2.6 )

project( AudioRec )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set( CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} )

find_library( ASOUND_INC asound )

find_path( ALSA_INCLUDE alsa/asoundlib.h )

add_subdirectory( "${PROJECT_SOURCE_DIR}/src" )
add_subdirectory( "${PROJECT_SOURCE_DIR}/src/AudioConfiguration" )
add_subdirectory( "${PROJECT_SOURCE_DIR}/src/AudioIn" )
add_subdirectory( "${PROJECT_SOURCE_DIR}/src/AudioOut" )
add_subdirectory( "${PROJECT_SOURCE_DIR}/src/Utilities" )

include_directories ( "${PROJECT_SOURCE_DIR}/src/Utilities" 
   "${PROJECT_SOURCE_DIR}/src/AudioConfiguration" 
   "${PROJECT_SOURCE_DIR}/src/AudioIn" 
   "${PROJECT_SOURCE_DIR}/src/AudioOut" 
   "${PROJECT_SOURCE_DIR}/src/Interfaces" )

include_directories( "${ALSA_INCLUDE}" )

add_executable( audiorec ${SOURCE}) 
add_executable( audioplay ${SOURCE1}) 

target_link_libraries( audiorec ${ASOUND_INC} ) 
target_link_libraries( audioplay ${ASOUND_INC} )

install (TARGETS audiorec DESTINATION bin)
install (TARGETS audioplay DESTINATION bin)

