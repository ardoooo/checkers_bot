cmake_minimum_required(VERSION 3.0)

project("Checkers")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Checkers_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${Checkers_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_PATH ${Checkers_SOURCE_DIR}/lib)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

include_directories(include)
link_directories(${Checkers_SOURCE_DIR}/lib)

add_executable(Checkers playground/main.cpp)
target_link_libraries(Checkers dep_lib)