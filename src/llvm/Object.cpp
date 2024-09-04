#include "Object.h"

#include <nanobind/nanobind.h>
#include <llvm-c/Object.h>
#include <stdexcept>
#include "types_priv.h"
#include "utils_priv.h"

namespace nb = nanobind;
using namespace nb::literals;

void populateObject(nb::module_ &m) {
  
  auto BinaryClass =
    nb::class_<PymBinary,
               PymLLVMObject<PymBinary, LLVMBinaryRef>>
      (m, "Binary", "Binary");
  

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


  BinaryClass
      .def("__init__",
           [](PymBinary *b, PymMemoryBuffer &memBuf, PymContext &cxt) {
             char *errorMessage;
             auto res = LLVMCreateBinary(memBuf.get(), cxt.get(), &errorMessage);
             auto success = res != NULL;
             THROW_ERORR_DISPOSE_MESSAGE_IF_FALSE(success, errorMessage);
             new (b) PymBinary(res);
           },
           "mem_buf"_a, "context"_a.none(),
           "Create a binary file from the given memory buffer.\n"
           "The exact type of the binary file will be inferred automatically, and the"
           "appropriate implementation selected.  The context may be NULL except if"
           "the resulting file is an LLVM IR file.")
      .def_prop_ro("memory_buffer",
                   [](PymBinary &self) {
                     return PymMemoryBuffer(LLVMBinaryCopyMemoryBuffer(self.get()));
                   },
                   "Retrieves a copy of the memory buffer associated with this object file.\n\n"
                   "The returned buffer is merely a shallow copy and does not own the actual"
                   "backing buffer of the binary.")
      .def_prop_ro("type",
                   [](PymBinary &self) {
                     return LLVMBinaryGetType(self.get());
                   },
                   "Retrieve the specific type of a binary.");
}
