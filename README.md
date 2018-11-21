# arrow-asm
 - assembly/disassembly of general purpose row based `c++` classes to/from _arrow_ based data structures

## TODO
- understand `clang/llvm`
    - parse headers
    - build `AST`
    - dump/analyze nodes of AST
    - understand how to generate functionality based on the parsed class declarations
    - _do we need cling in here???_
- ...

## Workflow
The workflow should be very similar in the beginning to the generation of reflection functionality ine [_ROOT_](https://root.cern.ch) (dictionaries)
 - at the compilation stage,
 - parse headers with class declarations,
 - generate the required functionality
 - link it to the rest of the code base/compilation pipeline
