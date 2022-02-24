cmake_minimum_required(VERSION 3.0.0)
project(CPMMessageStorage VERSION 0.1.0)

#include(CTest)
#enable_testing()

#lib
file(GLOB CPP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/Extensions/include/*.h)
file(GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/Extensions/*.cpp)

add_library(CPMMessageStorage ${CPP_FILES} ${CPP_HEADERS})
target_include_directories(CPMMessageStorage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/include ${CMAKE_CURRENT_SOURCE_DIR}/src/Extensions/include)
set_property(TARGET CPMMessageStorage PROPERTY CXX_STANDARD 17)

#file(GLOB CPP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/Extensions/include/*.h)
#file(GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Extensions/*.cpp)

#add_library(CPMMessageStorageExtensions ${CPP_FILES} ${CPP_HEADERS})
#target_include_directories(CPMMessageStorageExtensions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Extensions/include)
#set_property(TARGET CPMMessageStorageExtensions PROPERTY CXX_STANDARD 17)

#test

set(CMAKE_XCODE_GENERATE_SCHEME TRUE)
set(CMAKE_CXX_STANDARD 17)
add_executable (simulation_cpm ${CMAKE_CURRENT_SOURCE_DIR}/tester/cpm_simul.cpp)
target_link_libraries(simulation_cpm CPMMessageStorage)
set_property(TARGET simulation_cpm PROPERTY CXX_STANDARD 17)
set_property(TARGET simulation_cpm PROPERTY CMAKE_XCODE_GENERATE_SCHEME)

find_package(CxxTest)
if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR})
	CXXTEST_ADD_TEST(unittest_cpm_message_storage tester/cpm_test.cc
	${CMAKE_CURRENT_SOURCE_DIR}/tester/cpm_test.h)
    target_link_libraries(unittest_cpm_message_storage CPMMessageStorage) # as needed
endif()
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
