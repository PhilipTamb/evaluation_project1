cmake_minimum_required(VERSION 3.17)
project(ini_lib VERSION 1.0.1 DESCRIPTION "Library that facilitate Read/Write of .ini file in volatile memory.")
set(CMAKE_CXX_STANDARD 20)
include(CMakePrintHelpers)
project (ini_lib)


#set(CMAKE_VERBOSE_MAKEFILE on) #enable print message
message(STATUS "CMake version   : " ${CMAKE_SYSTEM_VERSION})
message(STATUS "Compiler        : " ${CMAKE_CXX_COMPILER})
message(STATUS "Operating System: " ${CMAKE_SYSTEM})
message(STATUS "SYSTEM PROCESSOR: " ${CMAKE_SYSTEM_PROCESSOR})
message(STATUS "PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
message(STATUS "${CMAKE_SOURCE_DIR}" ${CMAKE_SOURCE_DIR})
message(STATUS "${CARGO_TARGET_DIR}" ${CARGO_TARGET_DIR}) 
message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}" ${CMAKE_CURRENT_SOURCE_DIR})

cmake_print_variables("${PROJECT_SOURCE_DIR}: " ${PROJECT_SOURCE_DIR})
cmake_print_variables(STATUS "CMake version   : " ${CMAKE_SYSTEM_VERSION})
cmake_print_variables(STATUS "Compiler        : " ${CMAKE_CXX_COMPILER})
cmake_print_variables(STATUS "Operating System: " ${CMAKE_SYSTEM})
cmake_print_variables(STATUS "SYSTEM PROCESSOR: " ${CMAKE_SYSTEM_PROCESSOR})

set(CMAKE_BUILD_TYPE Release)

include_directories( ${PROJECT_SOURCE_DIR}/include)

############################################################
# Create a library  [.so]
############################################################
#Generate the shared library from the library sources
add_library(test_ini SHARED  ${PROJECT_SOURCE_DIR}/src/ini.cpp)

target_include_directories(test_ini
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        )

############################################################
#  Create a executable 
############################################################
# Add an executable with the above sources
add_executable(server_exe
        ${PROJECT_SOURCE_DIR}/server.cpp
        )

############################################################
# Target Link Libaries
############################################################
# link the library target with the main target
target_link_libraries(server_exe
PRIVATE
test_ini
)
# cmake_minimum_required(VERSION 3.9)
# project(mylib VERSION 1.0.1 DESCRIPTION "mylib description")
# set(CMAKE_CXX_STANDARD 17)
# include(GNUInstallDirs)
# #add_compile_definitions("SOURCE_ROOT=${CMAKE_SOURCE_DIR}")
# set(header ${CMAKE_CURRENT_SOURCE_DIR}/include/ini.hpp)
# set(source ${CMAKE_CURRENT_SOURCE_DIR}/src/ini.cpp)
# add_library(mylib SHARED ${source})

# message("${CMAKE_CURRENT_SOURCE_DIR}/src"${CMAKE_CURRENT_SOURCE_DIR}/src)
# message("${CMAKE_SOURCE_DIR}" ${CMAKE_SOURCE_DIR})
# message("${CARGO_TARGET_DIR}" ${CARGO_TARGET_DIR}) 
# message("${CMAKE_CURRENT_SOURCE_DIR}" ${CMAKE_CURRENT_SOURCE_DIR})

# #set_target_properties(mylib PROPERTIES VERSION ${PROJECT_VERSION})
# set_target_properties(mylib PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
# set_target_properties(mylib PROPERTIES PUBLIC_HEADER include/ini.hpp)
# set_target_properties(mylib PROPERTIES
#     VERSION ${PROJECT_VERSION}
#     SOVERSION ${PROJECT_VERSION_MAJOR}
#     PUBLIC_HEADER include/ini.hpp)
    
# target_include_directories(mylib PUBLIC include)
# target_include_directories(mylib PRIVATE src)
# target_include_directories(mylib PRIVATE .)

# target_include_directories(mylib PRIVATE .)
# install(TARGETS mylib
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install(FILES ${CMAKE_BINARY_DIR}/mylib.pc
#     DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)