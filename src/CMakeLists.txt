cmake_minimum_required(VERSION 3.0)

project(Dependencies)

FILE(GLOB Libs_H "${Checkers_SOURCE_DIR}/include/*.h")
FILE(GLOB Libs_C "${Checkers_SOURCE_DIR}/src/*.cpp")

add_library(
    dep_lib SHARED ${Libs_C} ${Libs_H}
)