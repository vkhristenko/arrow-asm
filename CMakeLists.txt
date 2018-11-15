cmake_minimum_required(VERSION 3.8)

# keep project version in here for now
project (arrow-asm VERSION 0.0.0)

# search module path
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# find arrow
find_package(Arrow)
if (NOT ARROW_FOUND)
    message ("*** ERROR: arrow is not found ***")
else()
    include_directories(${ARROW_INCLUDE_DIR})
endif()

# inclusion
include_directories(${PROJECT_SOURCE_DIR}/src)

# traverse all the components
if (ARROW_FOUND)
    add_subdirectory(src/arrowasm)
endif()
