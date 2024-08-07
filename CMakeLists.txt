cmake_minimum_required(VERSION 3.20.0...3.29.6)

# default for both c and c++ (c is needed for MacOS build)
project(${SKBUILD_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# required for linking fmt library during cibuildwheels build on manylinux_2_28
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        ffdc3fdbd90385429dd2ea6a774848e39d4f957a)
FetchContent_MakeAvailable(fmt)


if (NOT SKBUILD)
  message(WARNING "\
  This CMake file is meant to be executed using 'scikit-build'. Running
  it directly will almost certainly not produce the desired result.")
endif()

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Try to import all Python components potentially needed by nanobind
find_package(Python
  REQUIRED COMPONENTS Interpreter Development.Module
  OPTIONAL_COMPONENTS Development.SABIModule)

find_package(nanobind CONFIG REQUIRED)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

set(LLVM_LIBRARY_DIRS ${LLVM_LIBRARY_DIRS} CACHE STRING "LLVM library directories" FORCE)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

include_directories(${CMAKE_SOURCE_DIR}/src/llvm)
include_directories(${CMAKE_SOURCE_DIR}/src/llvm/types_priv)
include_directories(${CMAKE_SOURCE_DIR}/src/llvm/Core)
include_directories(${LLVM_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE SOURCE_HEADERS src/*.h)

nanobind_add_module(
  llvmpym_ext

  # Target the stable ABI for Python 3.12+, which reduces
  # the number of binary wheels that must be built (but has small performance cost).
  # This does nothing on older Python versions.
  STABLE_ABI

  NB_STATIC
  
  ${LLVM_INCLUDE_DIRS}
  
  ${SOURCE_HEADERS}

  ${SOURCES}
)


if ($ENV{LLVMPYM_SHARED})
  set(llvm_libs LLVM)
else()
  # the following code will lead to `error: cannot use ‘typeid’ with ‘-fno-rtti’`
  # problem on ubuntu build
  # set(llvm_libs ${LLVM_AVAILABLE_LIBS})
  # # List of libraries that are part of the monolithic LLVM shared library (possibly)
  # # and should be removed. They will cause
  # #   CommandLine Error: Option 'verify-dom-info' registered more than once!
  # # error when not removed
  # list(REMOVE_ITEM llvm_libs LLVMTableGenCommon LLVMDebuginfod LTO LLVMCFIVerify LLVMDiff
  #   Remarks LLVM)

  # Dynamically get all LLVM components and map them to library names
  execute_process(
    COMMAND llvm-config --components
    OUTPUT_VARIABLE LLVM_COMPONENTS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  llvm_map_components_to_libnames(llvm_libs core transformutils analysis support
    target bitwriter bitreader
    aarch64disassembler amdgpudisassembler armdisassembler avrdisassembler bpfdisassembler hexagondisassembler lanaidisassembler loongarchdisassembler mcdisassembler mipsdisassembler msp430disassembler powerpcdisassembler riscvdisassembler sparcdisassembler systemzdisassembler vedisassembler webassemblydisassembler x86disassembler xcoredisassembler
  )

endif()

message(INFO " ${llvm_libs}")

target_link_libraries(llvmpym_ext PRIVATE ${llvm_libs} fmt::fmt)

# -flto and --exclude-libs allow us to remove those parts of LLVM we don't use
# although we are going build a full llvm python binding :-
# TODO for MacOS immitate llvmlite only expose certain symbols. Can it work?
# https://github.com/numba/llvmlite/blob/78ebf9bf188379b2642112aff388480384306c6b/ffi/CMakeLists.txt#L76C53-L76C61

if(UNIX AND CMAKE_BUILD_TYPE STREQUAL "Release")
  set_property(TARGET llvmpym_ext APPEND_STRING PROPERTY LINK_FLAGS "-flto")
  if(APPLE)
    set_property(TARGET llvmpym_ext APPEND_STRING PROPERTY LINK_FLAGS " -Wl,-dead_strip")
  else()
    set_property(TARGET llvmpym_ext APPEND_STRING PROPERTY LINK_FLAGS " -Wl,--exclude-libs,ALL")
  endif()
endif()

# Stub Files
nanobind_add_stub(
  llvmpym_ext_stub
  MODULE llvmpym_ext
  OUTPUT __init__.pyi
  PYTHON_PATH $<TARGET_FILE_DIR:llvmpym_ext>
  DEPENDS llvmpym_ext
)

nanobind_add_stub(
  llvmpym_ext_stub_core
  MODULE llvmpym_ext.core
  OUTPUT core.pyi
  PYTHON_PATH $<TARGET_FILE_DIR:llvmpym_ext>
  DEPENDS llvmpym_ext
)

nanobind_add_stub(
  llvmpym_ext_stub_error_handling
  MODULE llvmpym_ext.error_handling
  OUTPUT error_handling.pyi
  PYTHON_PATH $<TARGET_FILE_DIR:llvmpym_ext>
  DEPENDS llvmpym_ext
)

nanobind_add_stub(
  llvmpym_ext_stub_support
  MODULE llvmpym_ext.support
  OUTPUT support.pyi
  PYTHON_PATH $<TARGET_FILE_DIR:llvmpym_ext>
  DEPENDS llvmpym_ext
)

nanobind_add_stub(
  llvmpym_ext_stub_utils
  MODULE llvmpym_ext.utils
  OUTPUT utils.pyi
  PYTHON_PATH $<TARGET_FILE_DIR:llvmpym_ext>
  DEPENDS llvmpym_ext
)

nanobind_add_stub(
  llvmpym_ext_stub_analysis
  MODULE llvmpym_ext.analysis
  OUTPUT analysis.pyi
  PYTHON_PATH $<TARGET_FILE_DIR:llvmpym_ext>
  DEPENDS llvmpym_ext
)

nanobind_add_stub(
  llvmpym_ext_stub_target
  MODULE llvmpym_ext.target
  OUTPUT target.pyi
  PYTHON_PATH $<TARGET_FILE_DIR:llvmpym_ext>
  DEPENDS llvmpym_ext
)


nanobind_add_stub(
  llvmpym_ext_stub_target_machine
  MODULE llvmpym_ext.target_machine
  OUTPUT target_machine.pyi
  PYTHON_PATH $<TARGET_FILE_DIR:llvmpym_ext>
  DEPENDS llvmpym_ext
)


# Install directive for scikit-build-core
install(TARGETS llvmpym_ext LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME})

# stub files
install(FILES
  ${CMAKE_BINARY_DIR}/__init__.pyi
  ${CMAKE_BINARY_DIR}/core.pyi
  ${CMAKE_BINARY_DIR}/error_handling.pyi
  ${CMAKE_BINARY_DIR}/support.pyi
  ${CMAKE_BINARY_DIR}/utils.pyi
  ${CMAKE_BINARY_DIR}/analysis.pyi
  ${CMAKE_BINARY_DIR}/target.pyi
  ${CMAKE_BINARY_DIR}/target_machine.pyi
  
  DESTINATION ${SKBUILD_PROJECT_NAME}/llvmpym_ext)
