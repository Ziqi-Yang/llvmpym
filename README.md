# LLVMPY Meow!

Note this project is still in the very early stage, not even half completed.   

## Manual Build of LLVM

1. Download llvm-project 18.1.8 release source code. [LLVM release page](https://releases.llvm.org/).
2. Extract the source code in a random directory such as `~/proj/probe/llvm-project`. Here we set the path as `<llvm-project-path>` for later reference.
3. Change directory into the this project (`llvmpym`).
4. Do the following command to manually build llvm project
``` shell
mkdir ./llvm-build
cd ./llvm-build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=. -DCMAKE_INSTALL_INCLUDEDIR=include -G Ninja ~/proj/llvm/llvm-project_18.1.8/llvm/
cmake --build .
ninja install-llvm-headers
```


## First time building this project

``` shell
env SKBUILD_CMAKE_ARGS="-DLLVM_DIR=$(pwd)/llvm-build/cmake/modules/CMakeFiles" just
```

## Enable nanobind Debug

If you are using `just`, then you can create an `.env` file at project root

``` plain
SKBUILD_CMAKE_BUILD_TYPE=Debug
```

otherwise you need to expose it as an environment variable


