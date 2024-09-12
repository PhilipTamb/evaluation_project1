cmake_minimum_required(VERSION 3.17)
project(ini_lib VERSION 1.0.1 DESCRIPTION "Library that facilitate Read/Write of .ini file in volatile memory.")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
include(CMakePrintHelpers)
project (ini_lib)

set(CMAKE_BUILD_TYPE Release)
include_directories( ${PROJECT_SOURCE_DIR}/include)

############################################################
# Create a library  [.so]
############################################################
#Generate the shared library from the library sources
add_library(test_ini SHARED  ${PROJECT_SOURCE_DIR}/src/ini.cpp)
target_include_directories(test_ini PUBLIC ${PROJECT_SOURCE_DIR}/include)

############################################################
#  Create a executable 
############################################################
# Add an executable with the above sources
add_executable(server ${PROJECT_SOURCE_DIR}/server.cpp)
add_executable(client ${PROJECT_SOURCE_DIR}/client.cpp)


############################################################
# Target Link Libaries
############################################################
# link the library target with the main target
target_link_libraries(server PRIVATE test_ini)
target_link_libraries(client)
