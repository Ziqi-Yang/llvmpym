+ Users can specify the `LLVM_DIR` variable: https://llvm.org/docs/CMake.html#embedding-llvm-in-your-project
`env SKBUILD_CMAKE_ARGS="-DLLVM_DIR=/home/meowking/proj/llvm/llvm-project_18.1.8/mybuilddir/cmake/modules/CMakeFiles" just`

= nanobind example
- https://github.com/ml-explore/mlx/tree/main/python/src
- https://github.com/Krande/nanobind-minimal/blob/main/src/nano_min.cpp
- https://github.com/leapmotion/pyopticam/blob/main/src/pyopticam_ext.cpp
- https://github.com/laggykiller/apngasm-python/blob/master/src/apngasm_python.cpp

= CI related res
- https://github.com/vovkos/llvm-package-windows
- https://github.com/c3lang/win-llvm?tab=readme-ov-file
== ci reference
- https://github.com/makslevental/mlir-wheels
- https://github.com/c3lang/c3c/blob/master/.github/workflows/main.yml
- https://github.com/revng/llvmcpy/blob/master/.github/workflows/ci.yml


= Misc
== Stub File
The generated library file structure for stub file is correct. \
If you are testing files under `example` directory, the pyright language server
may not working correctly, since it will looks into our source code directly
first I think. For in-place testing, you can copy the generated `llvmpym_ext`
directory(e.g. `.venv/lib/python3.12/site-packages/llvmpym/llvmpym_ext`) to corresponding place in source code directory.

== Add new submodule checklist
1. c++ code
2. python code: `__init__.py` import clause
3. CMakefile: `nanobind_add_module` and stub file definition and install blocks


= NOTE Note
+ Currently Pickling (thus deepcopy) is not supported, consider the we are not supported to know the internal change of LLVM when built on LLVM-C layer

= TODO
== Check
=== Core
+ LLVMAttributeIndex: should we create another type or just alias it at the python side (but it is an unsigned int)? (Maybe we just use the function name to represent it, but unsigned int?)
    + and check two anonymous enum values just before its typedef
    
=== Multi-thread


== Misc
- Doc: if document has `@see LLVMContext::setYieldCallback()`, then we need to also include documents for that function (in C++ header)
- TODO change all possible unsigned return value into corresponding enum type
    - also parameter (like `kindID`?)
- LATER Add cache to object properties
- TODO delete all `destory` function
- Create a simple tool to verify the api
    - e.g. https://github.com/cdisselkoen/llvm-ir-analysis

== More Python Style
1. iterator: next, prev ?
2. `with` clause
3. More see mlir python binding (doc & c++ code )
4. default value for function parameters (like name default to empty string)

=== Python methods:
+ `__eq__`
+ `__hash__`
+ `__str__`
+ `__repr__` (TODO subclasses for type, value)
+ with clause: `__enter__` and `__exit__`
+ iterator for methods like `functions` for Module




= Example
== Example custom initiator
```cpp
.def("__init__",
     [](PyContext *t, PyContext &&other) {
       new (t) PyContext(std::move(other));
     });
```
