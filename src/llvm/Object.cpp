#include "Object.h"

#include <nanobind/nanobind.h>
#include <llvm-c/Object.h>
#include <stdexcept>
#include "types_priv.h"

namespace nb = nanobind;
using namespace nb::literals;

void populateObject(nb::module_ &m) {
  nb::enum_<LLVMBinaryType>(m, "BinaryType", "BinaryType")
      .value("LLVMBinaryTypeArchive",
             LLVMBinaryType::LLVMBinaryTypeArchive,
             "Archive file.")
      .value("LLVMBinaryTypeMachOUniversalBinary",
             LLVMBinaryType::LLVMBinaryTypeMachOUniversalBinary,
             "Mach-O Universal Binary file.")
      .value("LLVMBinaryTypeCOFFImportFile",
             LLVMBinaryType::LLVMBinaryTypeCOFFImportFile,
             "COFF Import file.")
      .value("LLVMBinaryTypeIR",
             LLVMBinaryType::LLVMBinaryTypeIR,
             "LLVM IR.")
      .value("LLVMBinaryTypeWinRes",
             LLVMBinaryType::LLVMBinaryTypeWinRes,
             "Windows resource (.res) file.")
      .value("LLVMBinaryTypeCOFF",
             LLVMBinaryType::LLVMBinaryTypeCOFF,
             "COFF Object file.")
      .value("LLVMBinaryTypeELF32L",
             LLVMBinaryType::LLVMBinaryTypeELF32L,
             "ELF 32-bit, little endian.")
      .value("LLVMBinaryTypeELF32B",
             LLVMBinaryType::LLVMBinaryTypeELF32B,
             "ELF 32-bit, big endian.")
      .value("LLVMBinaryTypeELF64L",
             LLVMBinaryType::LLVMBinaryTypeELF64L,
             "ELF 64-bit, little endian.")
      .value("LLVMBinaryTypeELF64B",
             LLVMBinaryType::LLVMBinaryTypeELF64B,
             "ELF 64-bit, big endian.")
      .value("LLVMBinaryTypeMachO32L",
             LLVMBinaryType::LLVMBinaryTypeMachO32L,
             "MachO 32-bit, little endian.")
      .value("LLVMBinaryTypeMachO32B",
             LLVMBinaryType::LLVMBinaryTypeMachO32B,
             "MachO 32-bit, big endian.")
      .value("LLVMBinaryTypeMachO64L",
             LLVMBinaryType::LLVMBinaryTypeMachO64L,
             "MachO 64-bit, little endian.")
      .value("LLVMBinaryTypeMachO64B",
             LLVMBinaryType::LLVMBinaryTypeMachO64B,
             "MachO 64-bit, big endian.")
      .value("LLVMBinaryTypeWasm",
             LLVMBinaryType::LLVMBinaryTypeWasm,
             "Web Assembly.")
      .value("LLVMBinaryTypeOffload",
             LLVMBinaryType::LLVMBinaryTypeOffload,
             "Offloading fatbinary.");
}
