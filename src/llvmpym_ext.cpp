#include <nanobind/nanobind.h>
#include "llvm/Core.h"
#include "llvm/ErrorHandling.h"
#include "llvm/Utils.h"
#include "llvm/Support.h"
#include "llvm/types_priv.h"

namespace nb = nanobind;
using namespace nb::literals;


NB_MODULE(llvmpym_ext, m) {
  m.doc() = "LLVM Python Native Extension";
  
  BIND_PYLLVMOBJECT();
  
  auto coreModule = m.def_submodule("core", "LLVM Core");
  populateCore(coreModule);

  auto errorHandlingModule = m.def_submodule("error_handling", "Error Handling");
  populateErrorHandling(errorHandlingModule);
  
  auto supportModule = m.def_submodule("support", "Support");
  populateSupport(supportModule);
   
  auto utilsModule = m.def_submodule("utils", "Utilities");
  populateUtils(utilsModule);
}
