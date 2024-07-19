#include <nanobind/nanobind.h>
#include "llvm/Core.h"

namespace nb = nanobind;
using namespace nb::literals;


NB_MODULE(llvmpym_ext, m) {
  m.doc() = "LLVM Python Native Extension";
  
  auto coreModule = m.def_submodule("core", "LLVM Core");
  populateCore(coreModule);
}
