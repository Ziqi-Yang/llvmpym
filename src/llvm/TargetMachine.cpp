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

  auto TargetMachineClass =
    nb::class_<PyTargetMachine, PyLLVMObject<PyTargetMachine, LLVMTargetMachineRef>>
      (m, "TargetMachine", "TargetMachine");

  auto TargetMachineOptionsClass =
    nb::class_<PyTargetMachineOptions,
               PyLLVMObject<PyTargetMachineOptions, LLVMTargetMachineOptionsRef>>
      (m, "TargetMachineOptions", "TargetMachineOptions");

  auto TargetClass =
    nb::class_<PyTarget, PyLLVMObject<PyTarget, LLVMTargetRef>>(m, "Target", "Target");


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

  TargetClass
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
                   "Returns the next llvm::Target given a previous one (or null "
                   "if there's none)");
  TargetMachineOptionsClass
      .def("__init__",
           [](PyTargetMachineOptions *tmo) {
             new (tmo) PyTargetMachineOptions(LLVMCreateTargetMachineOptions());
           })
      .def("set_cpu",
           [](PyTargetMachineOptions &self, const char *cpu) {
             return LLVMTargetMachineOptionsSetCPU(self.get(), cpu);
           },
           "cpu"_a)
      .def("set_features",
           [](PyTargetMachineOptions &self, const char *features) {
             return LLVMTargetMachineOptionsSetFeatures(self.get(), features);
           },
           "features"_a)
      .def("set_abi",
           [](PyTargetMachineOptions &self, const char *abi) {
             return LLVMTargetMachineOptionsSetABI(self.get(), abi);
           },
           "abi"_a)
      .def("set_code_gen_opt_level",
           [](PyTargetMachineOptions &self, LLVMCodeGenOptLevel level) {
             return LLVMTargetMachineOptionsSetCodeGenOptLevel(self.get(), level);
           },
           "level"_a)
      .def("set_reloc_mode",
           [](PyTargetMachineOptions &self, LLVMRelocMode reloc) {
             return LLVMTargetMachineOptionsSetRelocMode(self.get(), reloc);
           },
           "reloc"_a)
      .def("set_code_model",
           [](PyTargetMachineOptions &self, LLVMCodeModel codeModel) {
             return LLVMTargetMachineOptionsSetCodeModel(self.get(), codeModel);
           },
           "code_model"_a);

  TargetMachineClass
      .def("__init__",
           [](PyTargetMachine *tm, PyTarget &T, const char *triple,
              PyTargetMachineOptions options) {
             auto res = LLVMCreateTargetMachineWithOptions
                          (T.get(), triple, options.get());
             new (tm) PyTargetMachine(res);
           },
           "target"_a, "triple"_a, "options"_a)
      .def("__init__",
           [](PyTargetMachine *tm, PyTarget &T, const char *triple, const char *cpu,
              const char *features, LLVMCodeGenOptLevel level, LLVMRelocMode reloc,
              LLVMCodeModel codeModel) {
             auto res = LLVMCreateTargetMachine
                          (T.get(), triple, cpu, features, level, reloc, codeModel);
             new (tm) PyTargetMachine(res);
           })
      .def_prop_ro("target",
                   [](PyTargetMachine &self) {
                     return PyTarget(LLVMGetTargetMachineTarget(self.get()));
                   })
      .def_prop_ro("triple",
                   [](PyTargetMachine &self) {
                     auto res = LLVMGetTargetMachineTriple(self.get());
                     RETURN_MESSAGE(res);
                   })
      .def_prop_ro("cpu",
                   [](PyTargetMachine &self) {
                     auto res = LLVMGetTargetMachineCPU(self.get());
                     RETURN_MESSAGE(res);
                   })
      .def_prop_ro("feature_string",
                   [](PyTargetMachine &self) {
                     auto res = LLVMGetTargetMachineFeatureString(self.get());
                     RETURN_MESSAGE(res);
                   });
      // TODO nedd 
      // .def_prop_ro("data_layout",
      //              [](PyTargetMachine &self) {
      //                return LLVMCreateTargetDataLayout(self.get());
      //              });
}
