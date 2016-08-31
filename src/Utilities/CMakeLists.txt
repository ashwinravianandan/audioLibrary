cmake_minimum_required( VERSION 2.6 )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set( CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} )

set (SOURCE 
   ${SOURCE} 
   ${CMAKE_CURRENT_SOURCE_DIR}/AudioOuptputQueue.cpp 
   ${CMAKE_CURRENT_SOURCE_DIR}/AudioQueue.cpp 
   PARENT_SCOPE )

set (SOURCE1 
   ${SOURCE1} 
   ${CMAKE_CURRENT_SOURCE_DIR}/AudioOuptputQueue.cpp 
   ${CMAKE_CURRENT_SOURCE_DIR}/AudioQueue.cpp 
   PARENT_SCOPE ) 
