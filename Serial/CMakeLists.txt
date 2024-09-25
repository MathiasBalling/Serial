set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add source files
file(GLOB SRC_FILES
    src/*.cpp
)

find_package(wxWidgets REQUIRED net gl core base)

if(wxWidgets_USE_FILE) # not defined in CONFIG mode
  include(${wxWidgets_USE_FILE})
endif()

find_package(fmt REQUIRED)

include_directories(include)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE
-Wall -Wextra -Wpedantic -Wshadow
-Wnon-virtual-dtor -Wcast-align -Wunused
-Woverloaded-virtual -Werror=vla
-Wmisleading-indentation -Wnull-dereference)

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} seriallib fmt::fmt)
