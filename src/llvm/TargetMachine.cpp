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
  BIND_ITERATOR_CLASS(PymTargetIterator, "TargetIterator")

  auto TargetMachineClass =
    nb::class_<PymTargetMachine, PymLLVMObject<PymTargetMachine, LLVMTargetMachineRef>>
      (m, "TargetMachine", "TargetMachine");

  auto TargetMachineOptionsClass =
    nb::class_<PymTargetMachineOptions,
               PymLLVMObject<PymTargetMachineOptions, LLVMTargetMachineOptionsRef>>
      (m, "TargetMachineOptions", "TargetMachineOptions");

  auto TargetClass =
    nb::class_<PymTarget, PymLLVMObject<PymTarget, LLVMTargetRef>>(m, "Target", "Target");


  m.def("get_default_target_triple",
        [](){
          auto res = LLVMGetDefaultTargetTriple();
          RETURN_MESSAGE(res);
        });
  m.def("normalize_target_triple",
        [](const char*triple) {
          auto res = LLVMNormalizeTargetTriple(triple);
          RETURN_MESSAGE(res);
        });
  m.def("get_host_cpu_name",
        []() {
          auto res = LLVMGetHostCPUName();
          RETURN_MESSAGE(res);
        });
  m.def("get_host_cpu_features",
        []() {
          auto res = LLVMGetHostCPUFeatures();
          RETURN_MESSAGE(res);
        });


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
      .def("__iter__", [](PymTarget &self) { return PymTargetIterator(self); })
      .def_static("get_first",
                  []() {
                    return PymTarget(LLVMGetFirstTarget());
                  },
                  "Returns the first llvm::Target in the registered targets list.")
      .def_static("get_from_name",
                  [](const char *name) {
                    return PymTarget(LLVMGetTargetFromName(name));
                  },
                  "name"_a,
                  "Finds the target corresponding to the given name")
      .def_static("get_from_triple",
                  [](const char *triple) {
                    char *errorMessage;
                    LLVMTargetRef T;
                    auto res = LLVMGetTargetFromTriple(triple, &T, &errorMessage) == 0;
                    THROW_ERORR_DISPOSE_MESSAGE_IF_FALSE(res, errorMessage);
                    return PymTarget(T);
                  },
                  "triple"_a,
                  "Finds the target corresponding to the given triple."
                  "Raises:\n"
                  "\tRuntimeError")
      .def_prop_ro("name",
                   [](PymTarget &self) {
                     return LLVMGetTargetName(self.get());
                   })
      .def_prop_ro("description",
                   [](PymTarget &self) {
                     return LLVMGetTargetDescription(self.get());
                   })
      .def_prop_ro("has_jit",
                   [](PymTarget &self) {
                     return LLVMTargetHasJIT(self.get()) != 0;
                   })
      .def_prop_ro("has_target_machine",
                   [](PymTarget &self) {
                     return LLVMTargetHasTargetMachine(self.get()) != 0;
                   })
      .def_prop_ro("has_asm_backend",
                   [](PymTarget &self) {
                     return LLVMTargetHasTargetMachine(self.get()) != 0;
                   })
      .def_prop_ro("next",
                   [](PymTarget &self) -> std::optional<PymTarget> {
                     auto res = LLVMGetNextTarget(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymTarget);
                   },
                   "Returns the next llvm::Target given a previous one (or null "
                   "if there's none)");

  
  TargetMachineOptionsClass
      .def("__init__",
           [](PymTargetMachineOptions *tmo) {
             new (tmo) PymTargetMachineOptions(LLVMCreateTargetMachineOptions());
           })
      .def("set_cpu",
           [](PymTargetMachineOptions &self, const char *cpu) {
             return LLVMTargetMachineOptionsSetCPU(self.get(), cpu);
           },
           "cpu"_a)
      .def("set_features",
           [](PymTargetMachineOptions &self, const char *features) {
             return LLVMTargetMachineOptionsSetFeatures(self.get(), features);
           },
           "features"_a)
      .def("set_abi",
           [](PymTargetMachineOptions &self, const char *abi) {
             return LLVMTargetMachineOptionsSetABI(self.get(), abi);
           },
           "abi"_a)
      .def("set_code_gen_opt_level",
           [](PymTargetMachineOptions &self, LLVMCodeGenOptLevel level) {
             return LLVMTargetMachineOptionsSetCodeGenOptLevel(self.get(), level);
           },
           "level"_a)
      .def("set_reloc_mode",
           [](PymTargetMachineOptions &self, LLVMRelocMode reloc) {
             return LLVMTargetMachineOptionsSetRelocMode(self.get(), reloc);
           },
           "reloc"_a)
      .def("set_code_model",
           [](PymTargetMachineOptions &self, LLVMCodeModel codeModel) {
             return LLVMTargetMachineOptionsSetCodeModel(self.get(), codeModel);
           },
           "code_model"_a);

  TargetMachineClass
      .def("__init__",
           [](PymTargetMachine *tm, PymTarget &T, const char *triple,
              PymTargetMachineOptions options) {
             auto res = LLVMCreateTargetMachineWithOptions
                          (T.get(), triple, options.get());
             new (tm) PymTargetMachine(res);
           },
           "target"_a, "triple"_a, "options"_a)
      .def("__init__",
           [](PymTargetMachine *tm, PymTarget &T, const char *triple, const char *cpu,
              const char *features, LLVMCodeGenOptLevel level, LLVMRelocMode reloc,
              LLVMCodeModel codeModel) {
             auto res = LLVMCreateTargetMachine
                          (T.get(), triple, cpu, features, level, reloc, codeModel);
             new (tm) PymTargetMachine(res);
           })
      .def_prop_ro("target",
                   [](PymTargetMachine &self) {
                     return PymTarget(LLVMGetTargetMachineTarget(self.get()));
                   })
      .def_prop_ro("triple",
                   [](PymTargetMachine &self) {
                     auto res = LLVMGetTargetMachineTriple(self.get());
                     RETURN_MESSAGE(res);
                   })
      .def_prop_ro("cpu",
                   [](PymTargetMachine &self) {
                     auto res = LLVMGetTargetMachineCPU(self.get());
                     RETURN_MESSAGE(res);
                   })
      .def_prop_ro("feature_string",
                   [](PymTargetMachine &self) {
                     auto res = LLVMGetTargetMachineFeatureString(self.get());
                     RETURN_MESSAGE(res);
                   })
      .def_prop_ro("data_layout",
                   [](PymTargetMachine &self) {
                     return PymTargetData(LLVMCreateTargetDataLayout(self.get()));
                   })
      .def("set_asm_verbosity",
           [](PymTargetMachine &self, bool verbose) {
             return LLVMSetTargetMachineAsmVerbosity(self.get(), verbose);
           },
           "verbose"_a)
      .def("set_fast_isel",
           [](PymTargetMachine &self, bool enable) {
             return LLVMSetTargetMachineFastISel(self.get(), enable);
           },
           "enable"_a)
      .def("set_global_isel",
           [](PymTargetMachine &self, bool enable) {
             return LLVMSetTargetMachineGlobalISel(self.get(), enable);
           },
           "enable"_a)
      .def("set_global_isel_abort",
           [](PymTargetMachine &self, LLVMGlobalISelAbortMode mode) {
             return LLVMSetTargetMachineGlobalISelAbort(self.get(), mode);
           },
           "mode"_a)
      .def("set_machine_outliner",
           [](PymTargetMachine &self, bool enable) {
             return LLVMSetTargetMachineMachineOutliner(self.get(), enable);
           },
           "enable"_a)
      .def("emit_to_file",
           [](PymTargetMachine &self, PymModule &m, const char *filename,
              LLVMCodeGenFileType codegen) {
             char *errorMessage;
             auto res = LLVMTargetMachineEmitToFile
                          (self.get(), m.get(), filename, codegen, &errorMessage);
             THROW_ERORR_DISPOSE_MESSAGE_IF_FALSE(res, errorMessage);
           },
           "module"_a, "filename"_a, "codegen"_a,
           "Emits an asm or object file for the given module to the filename. This"
           "wraps several c++ only classes (among them a file stream).\n\n"
           "Raises:\n"
           "\tRuntimeError")
      .def("emit_to_memory_buffer",
           [](PymTargetMachine &self, PymModule &m, LLVMCodeGenFileType codegen) {
             LLVMMemoryBufferRef outMemBuf;
             char *errorMessage;
             auto res = LLVMTargetMachineEmitToMemoryBuffer
                          (self.get(), m.get(), codegen, &errorMessage, &outMemBuf);
             THROW_ERORR_DISPOSE_MESSAGE_IF_FALSE(res, errorMessage);
           },
           "module"_a, "codegen"_a)
       .def("add_analysis_passes",
            [](PymTargetMachine &self, PymPassManager &pm) {
              return LLVMAddAnalysisPasses(self.get(), pm.get());
            },
            "pm"_a,
            "Adds the target-specific analysis passes to the pass manager.");
}
