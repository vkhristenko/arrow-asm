#include <iostream>

#include "arrowasm/include/test.hpp"

#include "clang/AST/ASTConsumer.h"

int main(int argc, char** argv) {
    test();

    std::cout << "hello world" << std::endl;
}
