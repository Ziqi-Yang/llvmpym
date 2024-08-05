#include "TargetMachine.h"

#include <nanobind/nanobind.h>
#include <nanobind/stl/optional.h>
#include <llvm-c/Core.h>
#include <llvm-c/TargetMachine.h>
#include <optional>
#include <stdexcept>
#include "types_priv.h"
#include "utils_priv.h"

namespace nb = nanobind;
using namespace nb::literals;

void populateTargetMachine(nanobind::module_ &m) {
  BIND_ITERATOR_CLASS(PyTargetIterator, "TargetIterator")

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

  nb::class_<PyTarget, PyLLVMObject<PyTarget, LLVMTargetRef>>(m, "Target", "Target")
      .def("__iter__", [](PyTarget &self) { return PyTargetIterator(self); })
      .def_static("get_first",
                  []() {
                    return PyTarget(LLVMGetFirstTarget());
                  },
                  "Returns the first llvm::Target in the registered targets list.")
      .def_static("get_from_name",
                  [](const char *name) {
                    return PyTarget(LLVMGetTargetFromName(name));
                  },
                  "name"_a,
                  "Finds the target corresponding to the given name")
      .def_static("get_from_triple",
                  [](const char *triple) {
                    char *errorMessage;
                    LLVMTargetRef T;
                    auto res = LLVMGetTargetFromTriple(triple, &T, &errorMessage) == 0;
                    WRAP_DISPOSE_MESSAGE_RUNTIME_ERROR(res, errorMessage, PyTarget);
                  },
                  "triple"_a,
                  "Finds the target corresponding to the given triple."
                  "Raises:\n"
                  "\tRuntimeError")
      .def_prop_ro("name",
                   [](PyTarget &self) {
                     return LLVMGetTargetName(self.get());
                   })
      .def_prop_ro("description",
                   [](PyTarget &self) {
                     return LLVMGetTargetDescription(self.get());
                   })
      .def_prop_ro("has_jit",
                   [](PyTarget &self) {
                     return LLVMTargetHasJIT(self.get()) != 0;
                   })
      .def_prop_ro("has_target_machine",
                   [](PyTarget &self) {
                     return LLVMTargetHasTargetMachine(self.get()) != 0;
                   })
      .def_prop_ro("has_asm_backend",
                   [](PyTarget &self) {
                     return LLVMTargetHasTargetMachine(self.get()) != 0;
                   })
      .def_prop_ro("next",
                   [](PyTarget &self) -> std::optional<PyTarget> {
                     auto res = LLVMGetNextTarget(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyTarget);
                   },
                   "Returns the next llvm::Target given a previous one (or null if there's none)");

  nb::class_<PyTargetMachine, PyLLVMObject<PyTargetMachine, LLVMTargetMachineRef>>
    (m, "TargetMachine", "TargetMachine");
}
