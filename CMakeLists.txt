cmake_minimum_required(VERSION 3.0.0)
project(Serial VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

find_package(wxWidgets REQUIRED net gl core base)

if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()

include_directories(incl)
include_directories(external)
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    incl/*.h
    external/*.cpp
    external/*.h
)

add_executable(Serial 
${SRC_FILES})

target_link_libraries(Serial ${wxWidgets_LIBRARIES})