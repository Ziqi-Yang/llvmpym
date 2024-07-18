+ Users can specify the `LLVM_DIR` variable: https://llvm.org/docs/CMake.html#embedding-llvm-in-your-project
`env SKBUILD_CMAKE_ARGS="-DLLVM_DIR=/home/meowking/proj/llvm/llvm-project_18.1.8/mybuilddir/cmake/modules/CMakeFiles" just`

= nanobind example
- https://github.com/Krande/nanobind-minimal/blob/main/src/nano_min.cpp
- https://github.com/leapmotion/pyopticam/blob/main/src/pyopticam_ext.cpp

= CI related res
- https://github.com/vovkos/llvm-package-windows
- https://github.com/c3lang/win-llvm?tab=readme-ov-file
== ci reference
- https://github.com/c3lang/c3c/blob/master/.github/workflows/main.yml
- https://github.com/revng/llvmcpy/blob/master/.github/workflows/ci.yml
- https://github.com/leapmotion/pyopticam/blob/main/src/pyopticam_ext.cpp


= Misc
== Stub File
The generated library file structure for stub file is correct. \
If you are testing files under `example` directory, the pyright language server
may not working correctly, since it will looks into our source code directly
first I think. For in-place testing, you can copy the generated `llvmpym_ext`
directory(e.g. `.venv/lib/python3.12/site-packages/llvmpym/llvmpym_ext`) to corresponding place in source code directory.

