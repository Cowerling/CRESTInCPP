cmake_minimum_required(VERSION 3.7)
project(CREST)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Tier.cpp Tier.h Cell.cpp Cell.h)
add_executable(CREST ${SOURCE_FILES})