# Notes on clang/llvm usage

## Running libTooling 
- need to explicitly provide include path directories `./clang-examle <sources> -- clang++ -I<include path> -I<include path>`

for instance: `./src/arrowasm/tests/test_visitor /Users/vk/software/test-cpp/stroustroup/22/visitors.cpp -- clang++ -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -I/usr/local/opt/llvm/include -I/usr/local/opt/llvm/include/c++/v1  > log 2>&1`

- when parsing just headers, clang assumes parsing `c` source code if extension is `.h`, to force `c++` add `clang++ -x c++` right after `--`
