#include "TargetMachine.h"

#include <nanobind/nanobind.h>
#include <nanobind/stl/optional.h>
#include <llvm-c/TargetMachine.h>
#include <optional>
#include "types_priv.h"
#include "utils_priv.h"

namespace nb = nanobind;
using namespace nb::literals;

void populateTargetMachine(nanobind::module_ &m) {
  nb::enum_<LLVMCodeGenOptLevel>(m, "CodeGenOptLevel", "CodeGenOptLevel")
      // None is a python keyword
      .value("Null", LLVMCodeGenOptLevel::LLVMCodeGenLevelNone)
      .value("Less", LLVMCodeGenOptLevel::LLVMCodeGenLevelLess)
      .value("Default", LLVMCodeGenOptLevel::LLVMCodeGenLevelDefault)
      .value("Aggressive", LLVMCodeGenOptLevel::LLVMCodeGenLevelAggressive);


  nb::enum_<LLVMRelocMode>(m, "RelocMode", "RelocMode")
      .value("Default", LLVMRelocMode::LLVMRelocDefault)
      .value("Static", LLVMRelocMode::LLVMRelocStatic)
      .value("PIC", LLVMRelocMode::LLVMRelocPIC)
      .value("DynamicNoPic", LLVMRelocMode::LLVMRelocDynamicNoPic)
      .value("ROPI", LLVMRelocMode::LLVMRelocROPI)
      .value("RWPI", LLVMRelocMode::LLVMRelocRWPI)
      .value("ROPI_RWPI", LLVMRelocMode::LLVMRelocROPI_RWPI);

  nb::enum_<LLVMCodeModel>(m, "CodeModel", "CodeModel")
      .value("Default", LLVMCodeModel::LLVMCodeModelDefault)
      .value("JITDefault", LLVMCodeModel::LLVMCodeModelJITDefault)
      .value("Tiny", LLVMCodeModel::LLVMCodeModelTiny)
      .value("Small", LLVMCodeModel::LLVMCodeModelSmall)
      .value("Kernel", LLVMCodeModel::LLVMCodeModelKernel)
      .value("Medium", LLVMCodeModel::LLVMCodeModelMedium)
      .value("Large", LLVMCodeModel::LLVMCodeModelLarge);

  nb::class_<PyTarget>(m, "Target", "Target")
      .def_static("get_first",
                  []() {
                    return PyTarget(LLVMGetFirstTarget());
                  },
                  "Returns the first llvm::Target in the registered targets list.")
      .def_prop_ro("next",
                   [](PyTarget &self) -> std::optional<PyTarget> {
                     auto res = LLVMGetNextTarget(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyTarget);
                   },
                   "Returns the next llvm::Target given a previous one (or null if there's none)");

  
}
