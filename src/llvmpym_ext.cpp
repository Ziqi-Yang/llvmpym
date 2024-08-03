#include <nanobind/nanobind.h>
#include "llvm/Core.h"
#include "llvm/ErrorHandling.h"
#include "llvm/Utils.h"
#include "llvm/_types.h"

namespace nb = nanobind;
using namespace nb::literals;


NB_MODULE(llvmpym_ext, m) {
  m.doc() = "LLVM Python Native Extension";
  
  nb::class_<PyLLVMObject<PyValue, LLVMValueRef>>
    (m, "LLVMObject", "The base of for all LLVM object classes.")
      .def("__bool__", &PyLLVMObject<PyValue, LLVMValueRef>::__bool__)
      .def("__eq__", &PyLLVMObject<PyValue, LLVMValueRef>::__equal__)
      .def("__hash__", &PyLLVMObject<PyValue, LLVMValueRef>::__hash__);
  
  auto coreModule = m.def_submodule("core", "LLVM Core");
  populateCore(coreModule);

  auto errorHandlingModule = m.def_submodule("error_handling", "Error Handling");
  populateErrorHandling(errorHandlingModule);

  auto utilsModule = m.def_submodule("utils", "Utilities");
  populateUtils(utilsModule);
}
