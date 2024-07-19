#include <nanobind/nanobind.h>
#include "llvm/Core.h"
#include "llvm/ErrorHandling.h"

namespace nb = nanobind;
using namespace nb::literals;


NB_MODULE(llvmpym_ext, m) {
  m.doc() = "LLVM Python Native Extension";
  
  auto coreModule = m.def_submodule("core", "LLVM Core");
  populateCore(coreModule);

  auto errorHandlingModule = m.def_submodule("error_handling", "Error Handling");
  populateErrorHandling(errorHandlingModule);
}
