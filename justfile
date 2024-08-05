set dotenv-load

LLVM-CONFIG := env('LLVM_CONFIG', "llvm-config")

install:
    pip install --no-build-isolation -ve .

test: install-normal
    python -m pytest

build-wheels: install-normal
    python -m build

ipython: install-normal
    ipython

install-normal:
    pip install -v .
    
install-dev-requirements:
    pip install -r ./requirements.txt

build-docs:
    cd ./docs && make html

run-all-examples:
    #!/usr/bin/env bash
    set -euo pipefail
    for file in ./example/*.py; do
      python $file > /dev/null || echo $file failed
    done
    echo done!

opendoc:
    xdg-open ./docs/build/html/index.html

# Example: just find-components ./build/cp312-abi3-linux_x86_64/ LLVMCloneModule
# build_dir is the pip(scikit-build-core) output directory, where the cmake cache
# file contains the used LLVM library path
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
    


