#include "globalFunc.h"
#include <llvm-c/Core.h>
#include <llvm-c/Core.h>
#include "../types_priv.h"

namespace nb = nanobind;
using namespace nb::literals;


void bindGlobalFunctions(nb::module_ &m) {
  // TODO
  m.def("shutdown", &LLVMShutdown, "Deallocate and destroy all ManagedStatic variables.");
  
  m.def("get_version",
        []() {
          unsigned major, minor, patch;
          LLVMGetVersion(&major, &minor, &patch);
          return std::make_tuple(major, minor, patch);
        },
        "Return the major, minor, and patch version of LLVM\n"
        "The version components are returned via the function's three output "
        "parameters or skipped if a NULL pointer was supplied.");

  // NOTE these two functions seems useless in Pymthon
  // m.def("create_message", &LLVMCreateMessage, "message"_a);
  // m.def("dispose_message", &LLVMDisposeMessage, "message"_a); // error, may need a wrapper for created message

  m.def("llvm_is_multithreaded",
        []() {
          return LLVMIsMultithreaded() != 0;
        });
}
