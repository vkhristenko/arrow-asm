cmake_minimum_required(VERSION 3.8.0)

project(arrowasm-basic-example LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

find_package(Arrow)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${ARROW_INCLUDE_DIR})

add_executable(basic driver.cpp)
target_compile_options(basic PUBLIC)
target_link_libraries(basic PUBLIC
    ${ARROW_SHARED_LIB})
