cmake_minimum_required(VERSION 3.7)
project(CREST)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Base/Tier.cpp Base/Tier.h Base/Cell.cpp Base/Cell.h Core/Core.cpp Core/Core.h Base/Basin.cpp Base/Basin.h Spatial/SpatialDriver.cpp Spatial/SpatialDriver.h)
add_executable(CREST ${SOURCE_FILES})

find_package(GDAL REQUIRED)
include_directories(${GDAL_INCLUDE_DIRS})

target_link_libraries(CREST ${GDAL_LIBRARY})