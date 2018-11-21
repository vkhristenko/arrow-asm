file(GLOB TESTS *.cpp)
link_directories(${LLVM_LIBRARY_DIRS})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "${LLVM_CXXFLAGS} --std=c++17")
foreach(TEST ${TESTS})
    get_filename_component(EXE ${TEST} NAME_WE)
    add_executable(${EXE} ${TEST})
    target_compile_options(${EXE} PUBLIC)
    target_link_libraries(${EXE} PUBLIC 
        arrow-asm ${LLVM_LDFLAGS} ${LLVM_LIBS} ${CLANG_LIBS} 
        ${LLVM_SYSTEM_LIBS})
endforeach(TEST)
