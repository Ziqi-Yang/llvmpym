#include "Disassembler.h"

#include <nanobind/nanobind.h>
#include <llvm-c/Disassembler.h>
#include "types_priv.h"

namespace nb = nanobind;
using namespace nb::literals;

void populateDisassembler(nb::module_ &m) {
  auto DisasmContextClass =
    nb::class_<PymDisasmContext, PymLLVMObject<PymDisasmContext, LLVMDisasmContextRef>>
      (m, "DisasmContext", "DisasmContext");

  // DisasmContextClass
  //     .def("__init__",
  //          [](PymDisasmContext *dc, const char *tripleName, nb::any DisInfo,
  //          int tagType, LLVM));
}

