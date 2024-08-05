#include "Analysis.h"

#include <nanobind/nanobind.h>
#include <llvm-c/Analysis.h>
#include "types_priv.h"

namespace nb = nanobind;
using namespace nb::literals;

void populateAnalysis(nb::module_ &m) {
  nb::enum_<LLVMVerifierFailureAction>(m, "VerifierFailureAction",
                                       "VerifierFailureAction")
      .value("AbortProcess", LLVMVerifierFailureAction::LLVMAbortProcessAction,
             "verifier will print to stderr and abort()")
      .value("PrintMessage", LLVMVerifierFailureAction::LLVMPrintMessageAction,
             "verifier will print to stderr and return 1")
      .value("ReturnStatus", LLVMVerifierFailureAction::LLVMReturnStatusAction,
             "verifier will just return 1");

  m.def("view_function_cfg",
        [](PymFunction &f) {
          return LLVMViewFunctionCFG(f.get());
        },
        "fn"_a,
        "Open up a ghostview window that displays the CFG of the current function. "
        "Useful for debugging.");

  m.def("view_function_cfg_only",
        [](PymFunction &f) {
          return LLVMViewFunctionCFGOnly(f.get());
        },
        "fn"_a,
        "Open up a ghostview window that displays the CFG of the current function. "
        "Useful for debugging.");
}

