add_library(seriallib STATIC seriallib/serialib.cpp)

target_include_directories(seriallib PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/seriallib)
