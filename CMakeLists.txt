cmake_minimum_required(VERSION 3.8)
project(SugarBombEngine)

option(SBE_SINGLE_BINARY "Build everything as a single binary" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(src)