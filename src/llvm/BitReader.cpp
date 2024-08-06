#include "BitReader.h"

#include <nanobind/nanobind.h>
#include <llvm-c/BitReader.h>
#include <stdexcept>
#include "types_priv.h"

namespace nb = nanobind;
using namespace nb::literals;

void populateBitReader(nb::module_ &m) {
  // NOTE LLVMParseBitcode is deprecated in favor of LLVMParseBitcode2
  m.def("parse_bit_code",
        [](PymMemoryBuffer &memBuf) {
          LLVMModuleRef module;
          auto res = LLVMParseBitcode2(memBuf.get(), &module) == 0;
          if (!res) {
            throw std::runtime_error("Error!");
          }
          // TODO check memBuf availability
          return PymModule(module);
        },
        "mem_buf"_a,
        "Builds a module from the bitcode in the specified memory buffer, returning a"
        "reference to the module via the OutModule parameter.");

  // LLVMGetBitcodeModule is deprecated in favor of LLVMGetBitcodeModule2
  m.def("get_bitcode_module",
        [](PymMemoryBuffer &memBuf) {
          LLVMModuleRef module;
          auto res = LLVMGetBitcodeModule2(memBuf.get(), &module) == 0;
          if (!res) {
            throw std::runtime_error("Error!");
          }
          // TODO check memBuf availability
          return PymModule(module);
        },
        "mem_buf"_a);
}
