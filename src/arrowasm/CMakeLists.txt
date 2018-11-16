file(GLOB SOURCES src/*.cpp)
add_library(arrow-asm SHARED ${SOURCES})
target_link_libraries(arrow-asm ${ARROW_SHARED_LIB})

add_subdirectory(tests)
