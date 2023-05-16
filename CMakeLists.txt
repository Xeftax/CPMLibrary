cmake_minimum_required(VERSION 3.0.0)
project(CPMMessageStorage VERSION 0.1.0)
find_package( log4cxx REQUIRED )

#include(CTest)

file(GLOB CPP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/Extensions/*.h)
file(GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/Extensions/*.cpp)

add_library(CPMMessageStorage ${CPP_FILES} ${CPP_HEADERS})
target_include_directories(CPMMessageStorage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects ${CMAKE_CURRENT_SOURCE_DIR}/src/Extensions)
set_property(TARGET CPMMessageStorage PROPERTY CXX_STANDARD 17)
target_link_libraries(CPMMessageStorage log4cxx)

set(CMAKE_XCODE_GENERATE_SCHEME TRUE)
set(CMAKE_CXX_STANDARD 17)

find_package(CxxTest)
if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR})
    enable_testing()
	CXXTEST_ADD_TEST(unittest_cpm_message_storage tests/cpmmanagertestsuite.cc 
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/cpmmanagertestsuite.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/messagetestsuite.h)
    target_link_libraries(unittest_cpm_message_storage CPMMessageStorage)
    CXXTEST_ADD_TEST(globalrandomtest_cpm_message_storage tests/globalrandomreadwritetest.cc ${CMAKE_CURRENT_SOURCE_DIR}/tests/globalrandomreadwritetest.h)
    target_link_libraries(globalrandomtest_cpm_message_storage CPMMessageStorage)
endif()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
