cmake_minimum_required(VERSION 3.7)
project(CREST)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp cmake-build-debug/Tier.cpp cmake-build-debug/Tier.h cmake-build-debug/Cell.cpp cmake-build-debug/Cell.h)
add_executable(CREST ${SOURCE_FILES})