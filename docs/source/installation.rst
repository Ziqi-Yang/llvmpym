Installation
=====================

Requirements
------------

- **Python 3.11+**
- If you install this library through PyPI, then your system needs to also meet the following
  requirements, otherwise you need to manually build the library:

  - **Linux**
    
    - **x86-64 architecture**. *aarch64* build is on the plan list.
    - **glibc>=2.28** (you can check it using command ``ldd --version``). *musl*
      build is currently not planned (though you can try to build it).
      
  - **MacOS>=14** (apple silicon)
  - **Windows**: no restriction.

Through package managers
------------------------

You can install the library from PyPI.

.. code-block:: bash
                
   pip install llvmpym

   
The MacOS bundle is a lot larger than the Linux and Windows bundles. Though
at this time they are of reasonable small size (Linux and Windows bundle are around
1.7 mebibytes, whereas MacOS bundle is around 12.4 mebibytes).

Build from source
-----------------

Requirement
^^^^^^^^^^^

- **Python 3.12+**
- **CMake 3.20.0+**
- **A C++17 compiler**: Clang 7+, GCC 8+, and MSVC2019+ are officially supported.
  Others (MinGW, Intel, NVIDIA, ..) may work as well but will not receive support. 
- **LLVM 18** (18.1.8) Either from your system or a manual build. Note that you may also need
  *zlib* and other libraries required by LLVM. But as long as there is no error message
  during the build, you don't need to worry it. 
- `Ninja <https://github.com/ninja-build/ninja>`_ (optional)

Install the build requirements
""""""""""""""""""""""""""""""

.. code-block:: bash

   pip install -r ./requirements.txt


Build the library
"""""""""""""""""

There are two situation:

1. **LLVM installed from system package manager**
   
In this case, you can run this command to build and install the package locally:

.. code-block:: bash
                
   pip install .

Or to build it into a python wheel:

.. code-block:: bash

   python -m build

The result files can be found in the *dist* directory.

2. **LLVM installed/built manually**

To accelerate the process, you may want to download other's build, which can be found
on `LLVM release page <https://releases.llvm.org/>`_. NB: you should download the file whose
name looks like ``clang+llvm-18.1.8-x86_64-linux-gnu-ubuntu-18.04.tar.xz``.

You can follow the instruction on `Building LLVM with CMake <https://llvm.org/docs/CMake.html>`_
web page to manually build LLVM. It would take around 40 minutes on a ``11th Gen Intel i7-11370H (8) @ 4.800GHz`` CPU to build it probably. For reference, here are the steps to do a
manual build:

#. Download llvm-project 18.1.8 release source code. `LLVM release page <https://releases.llvm.org/>`_
#. Extract the source code in a random directory such as `~/proj/probe/llvm-project`. Here we set the path as `<llvm-project-path>` for later reference.
#. Change directory into the this project (`llvmpym`).
#. Do the following commands:

   .. code-block:: bash

      mkdir -p ./llvm-build
      cd ./llvm-build
      cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=. -DCMAKE_INSTALL_INCLUDEDIR=include -G Ninja <llvm-project-path>/llvm/
      cmake --build .
      ninja install-llvm-headers
  
Then do the following command to build and install this *llvmpym* library:

.. code-block:: bash
                
   env SKBUILD_CMAKE_ARGS="-DLLVM_DIR=$(pwd)/llvm-build/cmake/modules/CMakeFiles" pip install .

Or to build it into a python wheel:

.. code-block:: bash
                
   env SKBUILD_CMAKE_ARGS="-DLLVM_DIR=$(pwd)/llvm-build/cmake/modules/CMakeFiles" python -m build

Additional notes for NixOS Users
--------------------------------

When using this library, NixOS user may face dynamic link problem::
  
  ImportError: libz.so.1: cannot open shared object file: No such file or directory

You can use the following command to inspect what shared libraries are not founded

.. code-block:: bash
                
   ldd ./.venv/lib/python3.12/site-packages/llvmpym/llvmpym_ext.abi3.so

After this command, for example, you find *libz.so.1* and *libstdc++.so.6* are missing.
You can use the following command to temporarily solve the issue:

.. code-block:: bash

   env LD_LIBRARY_PATH="$(nix path-info nixpkgs#zlib)/lib:$(nix path-info nixpkgs#stdenv.cc.cc.lib)/lib:$LD_LIBRARY_PATH" <your-command>

If you uses Nix development shell, you can set a *shell-hook* 

.. code-block:: nix

  devShells = forAllSystems ({ pkgs }: {
    default = pkgs.mkShell {
        packages = with pkgs; [
          (python312.withPackages (ppkgs: with ppkgs; [
            pip
          ]))
        ];
        shellHook = ''
          export LD_LIBRARY_PATH="${pkgs.zlib}/lib:${pkgs.stdenv.cc.cc.lib.outPath}/lib:$LD_LIBRARY_PATH"
        '';
      };
  });

Currently the issue is not solved from its root. If you know how to solve it, it
would be really helpful of you to propose a PR. 
