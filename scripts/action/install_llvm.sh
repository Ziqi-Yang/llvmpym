sys="$(uname -o)"
arch="$(uname -m)"


# Linux
if [[ "$sys" == "GNU/Linux" ]]; then 
    
    if [[ "$arch" == "x86_64" ]]; then
        url="https://github.com/llvm/llvm-project/releases/download/llvmorg-18.1.8/clang+llvm-18.1.8-x86_64-linux-gnu-ubuntu-18.04.tar.xz"
        root_llvm_dir_name="clang+llvm-18.1.8-x86_64-linux-gnu-ubuntu-18.04"
        
    elif [[ "$arch" == "aarch64" ]]; then
        url="https://github.com/llvm/llvm-project/releases/download/llvmorg-18.1.8/clang+llvm-18.1.8-aarch64-linux-gnu.tar.xz"
        root_llvm_dir_name="clang+llvm-18.1.8-aarch64-linux-gnu.tar.xz"
    fi

    # install libraries
    DISTRO_NMAE=$(awk -F= '$1=="NAME" { print $2 ;}' /etc/os-release)
    if [[ "$DISTRO_NMAE" == "\"AlmaLinux\"" ]]; then
        dnf install -y ncurses-devel # Terminfo::terminfo required by LLVM
    fi

    
# Windows
elif [[ "$sys" == "Msys" ]]; then
    url="https://github.com/llvm/llvm-project/releases/download/llvmorg-18.1.8/clang+llvm-18.1.8-x86_64-pc-windows-msvc.tar.xz"
    root_llvm_dir_name="clang+llvm-18.1.8-x86_64-pc-windows-msvc"

    # install libraries
    # vcpkg install zlib
    
# MacOS
elif [[ "$sys" == "Darwin" ]]; then # Mac (only for 14 or later, i.e. apple silicon)
    # install library
    brew install llvm@18
    exit
fi


# Linux & Windows
curl -L -o ./llvm-build.tar.xz "$url"
tar -xf ./llvm-build.tar.xz
mv "$root_llvm_dir_name" ./llvm-build

