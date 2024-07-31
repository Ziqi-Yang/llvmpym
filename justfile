set dotenv-load

LLVM-CONFIG := env('LLVM_CONFIG', "./llvm-build/bin/llvm-config")

install:
    pip install --no-build-isolation -ve .

test: install-normal
    python -m pytest

build-wheels: install-normal
    python -m build

ipython: install-normal
    ipython

install-normal:
    pip install .
    
install-dev-requirements:
    pip install -r ./requirements.txt

build-docs:
    cd ./docs && make html

opendoc:
    open ./docs/build/html/index.html

# Example: just find-components ./build/cp312-abi3-linux_x86_64/ LLVMCloneModule
# Find which llvm archive file contains the desired symbol
find-components build_dir symbol:
    #!/usr/bin/env bash
    LLVM_LIBRARY_DIRS=$(cmake -L '{{build_dir}}' | grep LLVM_LIBRARY_DIRS | cut -d '=' -f 2)
    for lib in "$LLVM_LIBRARY_DIRS"/*.a; do
        nm -gC "$lib" | grep '{{symbol}}' && echo "Found in $lib"
    done
    echo Done!

list-llvm-components:
    '{{LLVM-CONFIG}}' --components | tr " " "\n" | less
    


