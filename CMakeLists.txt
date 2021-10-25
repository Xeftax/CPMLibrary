cmake_minimum_required(VERSION 3.0.0)
project(CPMMessageStorage VERSION 0.1.0)

#include(CTest)
#enable_testing()

#lib
file(GLOB CPP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/include/*.h  "${CMAKE_CURRENT_SOURCE_DIR}/src/Storage/include/*.h")
file(GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/*.cpp  "${CMAKE_CURRENT_SOURCE_DIR}/src/Storage/*.cpp")

add_library(CPMMessageStorage ${CPP_FILES} ${CPP_HEADERS})
target_include_directories(CPMMessageStorage PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/include" "${CMAKE_CURRENT_SOURCE_DIR}/src/Storage/include")
set_property(TARGET CPMMessageStorage PROPERTY CXX_STANDARD 17)

#test

add_executable (manueltest ${CMAKE_CURRENT_SOURCE_DIR}/tester/manualtest.cpp)
target_link_libraries(manueltest CPMMessageStorage)
set_property(TARGET manueltest PROPERTY CXX_STANDARD 17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
