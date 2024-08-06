#include "miscClasses.h"

#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/optional.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitReader.h>
#include <llvm-c/BitWriter.h>
#include <fmt/core.h>
#include <optional>
#include <stdexcept>
#include "../types_priv.h"
#include "../utils_priv.h"
#include "utils.h"

namespace nb = nanobind;
using namespace nb::literals;

template <typename T>
using optional = std::optional<T>;


void bindOtherClasses(nb::module_ &m) {
  auto ContextClass =
    nb::class_<PymContext, PymLLVMObject<PymContext, LLVMContextRef>>
      (m, "Context",
       "Contexts are execution states for the core LLVM IR system.\n\n"
       "Most types are tied to a context instance. Multiple contexts can"
       "exist simultaneously. A single context is not thread safe. However,"
       "different contexts can execute on different threads simultaneously.");

  auto AttributeClass =
    nb::class_<PymAttribute, PymLLVMObject<PymAttribute, LLVMAttributeRef>>
      (m, "Attribute", "Attribute");
  auto EnumAttributeClass = nb::class_<PymEnumAttribute, PymAttribute>
                              (m, "EnumAttribute", "EnumAttribute");
  auto TypeAttributeClass = nb::class_<PymTypeAttribute, PymAttribute>
                              (m, "TypeAttribute", "TypeAttribute");
  auto StringAttributeClass = nb::class_<PymStringAttribute, PymAttribute>
                                (m, "StringAttribute", "StringAttribute");
  
  
  auto BasicBlockClass =
    nb::class_<PymBasicBlock, PymLLVMObject<PymBasicBlock, LLVMBasicBlockRef>>
      (m, "BasicBlock", "BasicBlock");
  auto DiagnosticInfoClass =
    nb::class_<PymDiagnosticInfo, PymLLVMObject<PymDiagnosticInfo, LLVMDiagnosticInfoRef>>
      (m, "DiagnosticInfo", "DiagnosticInfo");

  auto NamedMDNodeClass =
    nb::class_<PymNamedMDNode, PymLLVMObject<PymNamedMDNode, LLVMNamedMDNodeRef>>
      (m, "NamedMDNode", "NamedMDNode");
  
  auto ModuleClass =
    nb::class_<PymModule, PymLLVMObject<PymModule, LLVMModuleRef>>
      (m, "Module",
       "Modules represent the top-level structure in an LLVM program. An LLVM"
       "module is effectively a translation unit or a collection of translation "
       "units merged together.");

  auto ModuleFlagEntriesClass =
    nb::class_<PymModuleFlagEntries, PymLLVMObject<PymModuleFlagEntries, LLVMModuleFlagEntries>>
      (m, "ModuleFlagEntry", "ModuleFlagEntry");

  
  auto MetadataClass =
    nb::class_<PymMetadata, PymLLVMObject<PymMetadata, LLVMMetadataRef>>
      (m, "Metadata", "Metadata");
  auto MDNodeClass = nb::class_<PymMDNode, PymMetadata>(m, "MDNode", "MDNode");
  auto MDStringClass = nb::class_<PymMDString, PymMetadata>(m, "MDString", "MDString");
  auto ValueAsMetadata = nb::class_<PymValueAsMetadata, PymMetadata>
                           (m, "ValueAsMetadata", "ValueAsMetadata");

  
  auto MetadataEntriesClass =
    nb::class_<PymMetadataEntries, PymLLVMObject<PymMetadataEntries, LLVMValueMetadataEntries>>
      (m, "MetadataEntry", "MetadataEntry");
  
  auto UseClass =
    nb::class_<PymUse, PymLLVMObject<PymUse, LLVMUseRef>>
      (m, "Use", "Use");

  auto IntrinsicClass =
    // nb::class_<PymIntrinsic, PymLLVMObject<PymIntrinsic, unsigned>>
    nb::class_<PymIntrinsic, PymLLVMObject<PymIntrinsic, unsigned>>
      (m, "Intrinsic", "Intrinsic");
  auto OperandBundleClass =
    nb::class_<PymOperandBundle, PymLLVMObject<PymOperandBundle, LLVMOperandBundleRef>>
      (m, "OperandBundle", "OperandBundle");
  auto BuilderClass =
    nb::class_<PymBuilder, PymLLVMObject<PymBuilder, LLVMBuilderRef>>
      (m, "Builder", "Builder");
  auto ModuleProviderClass =
    nb::class_<PymModuleProvider, PymLLVMObject<PymModuleProvider, LLVMModuleProviderRef>>
      (m, "ModuleProvider", "ModuleProvider");
  auto MemoryBufferClass =
    nb::class_<PymMemoryBuffer, PymLLVMObject<PymMemoryBuffer, LLVMMemoryBufferRef>>
      (m, "MemoryBuffer", "MemoryBuffer");
  
  // no need to create PymPassManagerBase binding
  auto PassManagerClass =
    nb::class_<PymPassManager, PymLLVMObject<PymPassManagerBase, LLVMPassManagerRef>>
      (m, "PassManager", "PassManager");
  auto FunctionPassManagerClass = nb::class_<PymFunctionPassManager>
                                    (m, "FunctionPassManager", "FunctionPassManager");

  MetadataClass
      .def("__repr__",
           [](PymMetadata &self) {
             return "<Metadata>";
           })
      .def_static("get_md_kind_id",
                  [](const std::string &name) {
                    return LLVMGetMDKindID(name.c_str(), name.size());
                  },
                  "name"_a = "")
      .def("as_value",
           [](PymMetadata &self, PymContext &c) {
             return PymMetadataAsValue(LLVMMetadataAsValue(c.get(), self.get()));
           },
           "context"_a);


  MDStringClass
      .def("__repr__",
           [](PymMDString &self) {
             return "<MDString>";
           })
      .def("__init__",
           [](PymMDString *mds, PymContext &context, std::string &name) {
             new (mds) PymMDString(LLVMMDStringInContext2
                                    (context.get(), name.c_str(), name.size()));
           },
           "context"_a, "name"_a = "",
           "Create an MDString value from a given string value.")
      .def("as_value",
           [](PymMDString &self, PymContext &c) {
             return PymMDStringValue(LLVMMetadataAsValue(c.get(), self.get()));
           },
           "context"_a);

  MDNodeClass
      .def("__repr__",
           [](PymMDNode &self) {
             return "<MDNode>";
           })
      .def("__init__",
           [](PymMDNode *mdd, PymContext &c, std::vector<PymMetadata> mds) {
             size_t num = mds.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMMetadataRef, mds, raw, num);
             new (mdd) PymMDNode(LLVMMDNodeInContext2(c.get(), raw.data(), num));
           },
           "context"_a, "metadata"_a)
      .def("as_value",
           [](PymMDNode &self, PymContext &c) {
             return PymMDNodeValue(LLVMMetadataAsValue(c.get(), self.get()));
           },
           "context"_a);


  PassManagerClass
      .def("__repr__",
           [](PymPassManager &self) {
             return "<PassManager>";
           })
      .def("__init__",
           [](PymPassManager *pm) {
             new (pm) PymPassManager(LLVMCreatePassManager());
           },
           "Constructs a new whole-module pass pipeline. This type of pipeline is"
           "suitable for link-time optimization and whole-module transformations.")
      .def("run",
           [](PymPassManager &self, PymModule &module) {
             return LLVMRunPassManager(self.get(), module.get()) != 0;
           },
           "module"_a,
           "Initializes, executes on the provided module, and finalizes all of the"
           "passes scheduled in the pass manager. Returns true if any of the passes"
           "modified the module, false otherwise.");

  FunctionPassManagerClass
      .def("__repr__",
           [](PymFunctionPassManager &self) {
             return "<FunctionPassManager>";
           })
      .def("__init__",
           [](PymFunctionPassManager *fpm, PymModule &module) {
             new (fpm) PymFunctionPassManager(LLVMCreateFunctionPassManagerForModule
                                               (module.get()));
           },
           "module"_a,
           "Constructs a new function-by-function pass pipeline over the module"
           "provider. It does not take ownership of the module provider. This type of"
           "pipeline is suitable for code generation and JIT compilation tasks.")
      .def("initialize",
           [](PymFunctionPassManager &self) {
             return LLVMInitializeFunctionPassManager(self.get()) != 0;
           },
           "Initializes all of the function passes scheduled in the function pass"
           "manager. Returns true if any of the passes modified the module, false "
           "otherwise.")
      .def("run",
           [](PymFunctionPassManager &self, PymFunction f) {
             return LLVMRunFunctionPassManager(self.get(), f.get()) != 0;
           },
           "f"_a,
           "Executes all of the function passes scheduled in the function pass manager"
           "on the provided function. Returns true if any of the passes modified the"
           "function, false otherwise.")
      .def("finalize",
           [](PymFunctionPassManager &self) {
             return LLVMFinalizeFunctionPassManager(self.get()) != 0;
           },
           "Finalizes all of the function passes scheduled in the function pass"
           "manager. Returns 1 if any of the passes modified the module, 0 otherwise.");
  

  ModuleProviderClass
      .def("__repr__",
           [](PymModuleProvider &self) {
             return "<ModuleProvider>";
           })
      .def("create_function_pass_manager",
           [](PymModuleProvider &self) {
             return PymFunctionPassManager(LLVMCreateFunctionPassManager(self.get()));
           },
           "Deprecated: Use :func:`Module.create_function_pass_manager` instead.");

  MemoryBufferClass
      .def("__repr__",
           [](PymMemoryBuffer &self) {
             return "<MemoryBuffer>";
           })
      .def_static("from_file",
                  [](const char *Path) {
                    LLVMMemoryBufferRef OutMemBuf;
                    char *OutMessage;
                    auto success = LLVMCreateMemoryBufferWithContentsOfFile
                                     (Path, &OutMemBuf, &OutMessage);
                    if (!success) {
                      throw std::runtime_error(OutMessage);
                    }

                    return PymMemoryBuffer(OutMemBuf);
                  },
                  "path"_a,
                  ":raises RuntimeError")
      .def_static("from_stdin",
                  []() {
                    LLVMMemoryBufferRef OutMemBuf;
                    char *OutMessage;
                    auto success = LLVMCreateMemoryBufferWithSTDIN
                                     (&OutMemBuf, &OutMessage);
                    if (!success) {
                      throw std::runtime_error(OutMessage);
                    }
                    return PymMemoryBuffer(OutMemBuf);
                  },
                  ":raises RuntimeError")
      .def_static("from_str",
                  [](std::string &inputData, bool RequiresNullTerminator, const char *BufferName) {
                    return PymMemoryBuffer(LLVMCreateMemoryBufferWithMemoryRange
                                            (inputData.c_str(), inputData.size(),
                                             BufferName, RequiresNullTerminator));
                  },
                  "input_data"_a, "requires_null_terminator"_a, "buffer_name"_a = "")
      .def_static("from_str",
                  [](const std::string &inputData, const char *BufferName) ->
                   optional<PymMemoryBuffer>{
                    auto res = LLVMCreateMemoryBufferWithMemoryRangeCopy
                                 (inputData.c_str(), inputData.size(), BufferName);
                    WRAP_OPTIONAL_RETURN(res, PymMemoryBuffer);
                  },
                  "input_data"_a, "buffer_name"_a = "")
      .def_prop_ro("buffer_start",
                   [](PymMemoryBuffer &self) {
                     return LLVMGetBufferStart(self.get());
                   })
      .def_prop_ro("buffer_size",
                   [](PymMemoryBuffer &self) {
                     return LLVMGetBufferSize(self.get());
                   });
  

  BuilderClass
      .def("__repr__",
           [](PymBuilder &self) {
             return "<Builder>";
           })
      .def("__init__",
           [](PymBuilder *b, PymContext &c) {
             new (b) PymBuilder(LLVMCreateBuilderInContext(c.get()));
           })
      .def("__init__",
           [](PymBuilder *b) {
             new (b) PymBuilder(LLVMCreateBuilder());
           })
      .def_prop_ro("insert_block",
                   [](PymBuilder &self) {
                     return PymBasicBlock(LLVMGetInsertBlock(self.get()));
                   })
      // note LLVMSetCurrentDebugLocation is deprecated in favor of
      // LLVMSetCurrentDebugLocation2. Also the LLVMGetCurrentDebugLocation
      .def_prop_rw("current_debug_location",
                   [](PymBuilder &self) {
                     return PymMetadata(LLVMGetCurrentDebugLocation2(self.get()));
                   },
                   [](PymBuilder &self, PymMetadata &loc) {
                     return LLVMSetCurrentDebugLocation2(self.get(), loc.get());
                   })
      .def_prop_rw("default_fp_math_tag",
                   [](PymBuilder &self) {
                     return PymMetadata(LLVMBuilderGetDefaultFPMathTag(self.get()));
                   },
                   [](PymBuilder &self, PymMetadata &fpMathTag) {
                     return LLVMBuilderSetDefaultFPMathTag(self.get(), fpMathTag.get());
                   },
                   nb::for_getter("Get the dafult floating-point math metadata."))
      .def("position",
           [](PymBuilder &self, PymBasicBlock bb, PymInstruction &inst) {
             return LLVMPositionBuilder(self.get(), bb.get(), inst.get());
           },
           "Original Function: LLVMPositionBuilder.")
      .def("position_before",
           [](PymBuilder &self, PymInstruction &inst) {
             return LLVMPositionBuilderBefore(self.get(), inst.get());
           },
           "instruction"_a,
           "Original function: LLVMPositionBuilderBefore.")
      .def("position_at_end",
           [](PymBuilder &self, PymBasicBlock bb) {
             return LLVMPositionBuilderAtEnd(self.get(), bb.get());
           },
           "basicblock"_a,
           "Original function: LLVMPositionBuilderAtEnd.")
      .def("clear_insert_position",
           [](PymBuilder &self) {
             return LLVMClearInsertionPosition(self.get());
           })
      .def("insert",
           [](PymBuilder &self, PymBasicBlock &bb) {
             return LLVMInsertExistingBasicBlockAfterInsertBlock(self.get(), bb.get());
           },
           "basic_block"_a,
           "Insert the given basic block after the insertion point of the given "
           "builder.\n\n"
           "The insertion point must be valid.")
      .def("insert",
           [](PymBuilder &self, PymInstruction &inst) {
             return LLVMInsertIntoBuilder(self.get(), inst.get());
           },
           "instruction"_a)
      .def("insert_with_name",
           [](PymBuilder &self, PymInstruction &inst, const char *name) {
             return LLVMInsertIntoBuilderWithName(self.get(), inst.get(), name);
           },
           "instruction"_a, "name"_a = "")
      .def("destory",
           [](PymBuilder &self) {
             return LLVMDisposeBuilder(self.get());
           },
           "Original Function: LLVMDisposeBuilder.")
      .def("set_instruction_debug_location",
           [](PymBuilder &self, PymInstruction &inst) {
             return LLVMSetInstDebugLocation(self.get(), inst.get());
           },
           "instruction"_a,
           "Attempts to set the debug location for the given instruction using the"
           "current debug location for the given builder.  If the builder has no current"
           "debug location, this function is a no-op.")
      .def("add_metadata_to_instruction",
           [](PymBuilder &self, PymInstruction &inst) {
             return LLVMAddMetadataToInst(self.get(), inst.get());
           },
           "instruction"_a,
           "Adds the metadata registered with the given builder to the given"
           "instruction.")
       /*
        * The following methods are used to build instructions
        * the returned instructions are as specific are possible
        * But if the instruction type doesn't meant to be used in other API functions,
        * then we make it as PymInstruction Type to reduce the Instruction sub-classes number
        */
      .def("ret_void",
           [](PymBuilder &self) {
             return PymReturnInst(LLVMBuildRetVoid(self.get()));
           })
      .def("ret",
           [](PymBuilder &self, PymValue &v) {
             return PymReturnInst(LLVMBuildRet(self.get(), v.get()));
           },
           "value"_a)
      .def("aggregate_ret",
           [](PymBuilder &self, std::vector<PymValue> &values) {
             unsigned num = values.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, values, raw, num);
             return PymReturnInst(LLVMBuildAggregateRet(self.get(), raw.data(), num));
           },
           "values"_a)
      .def("br",
           [](PymBuilder &self, PymBasicBlock &dest) {
             return PymBranchInst(LLVMBuildBr(self.get(), dest.get()));
           },
           "dest"_a)
      .def("cond_br",
           [](PymBuilder &self, PymValue &If, PymBasicBlock &Then,
              PymBasicBlock &Else) {
             return PymBranchInst(LLVMBuildCondBr(self.get(), If.get(),
                                                 Then.get(), Else.get()));
           },
           "If"_a, "Then"_a, "Else"_a)
      .def("switch",
           [](PymBuilder &self, PymValue &value, PymBasicBlock &Else,
              unsigned numCases) {
             return PymSwitchInst(LLVMBuildSwitch(self.get(), value.get(), Else.get(), numCases));
           },
           "value"_a, "Else"_a, "num_cases"_a)
      .def("indirect_br",
           [](PymBuilder &self, PymValue &addr, unsigned numDests) {
             return PymIndirectBrInst(LLVMBuildIndirectBr(self.get(), addr.get(), numDests));
           },
           "addr"_a, "num_dests"_a)
      .def("invoke",
           [](PymBuilder &self, PymType &type, PymFunction &fn, std::vector<PymValue> args,
              PymBasicBlock Then, PymBasicBlock Catch, const char *name) {
             unsigned args_num  = args.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);
             auto res = LLVMBuildInvoke2(self.get(), type.get(), fn.get(),
                                         rawArgs.data(), args_num, Then.get(),
                                         Catch.get(), name);
             return PymInvokeInst(res);
           },
           "type"_a, "fn"_a, "args"_a, "Then"_a, "Catch"_a, "name"_a = "",
           "Original Function: LLVMBuildInvoke2.")
      .def("invoke_with_operand_bundles",
           [](PymBuilder &self, PymType &type, PymFunction &fn, std::vector<PymValue> args,
              PymBasicBlock Then, PymBasicBlock Catch,
              std::vector<PymOperandBundle> bundles, const char *name) {
             unsigned args_num  = args.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);

             unsigned bundles_num = bundles.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMOperandBundleRef, bundles, rawBundles,
                                         bundles_num);
             
             auto res = LLVMBuildInvokeWithOperandBundles
                          (self.get(), type.get(), fn.get(),
                           rawArgs.data(), args_num, Then.get(),
                           Catch.get(), rawBundles.data(), bundles_num,
                           name);
             return PymInvokeInst(res);
           },
           "type"_a, "fn"_a, "args"_a, "Then"_a, "Catch"_a, "bundles"_a,
           "name"_a = "")
      .def("unreachable",
           [](PymBuilder &self) {
             return PymInstruction(LLVMBuildUnreachable(self.get()));
           })
      .def("resume",
           [](PymBuilder &self, PymValue &exn) {
             return PymInstruction(LLVMBuildResume(self.get(), exn.get()));
           },
           "exn"_a)
      .def("landing_pad",
           [](PymBuilder &self, PymType &type, PymValue &PersFn, unsigned numClauses,
              const char *name) {
             return PymLandingPadInst(LLVMBuildLandingPad
                                       (self.get(), type.get(), PersFn.get(), numClauses,
                                        name));
           },
           "type"_a, "pers_fn"_a, "num_clauses"_a, "name"_a = "")
      .def("cleanup_ret",
           [](PymBuilder &self, PymValue &catchPad, PymBasicBlock bb) {
             return PymCleanupReturnInst(LLVMBuildCleanupRet(self.get(), catchPad.get(),
                                                            bb.get()));
           },
           "catch_pad"_a, "bb"_a)
      .def("catch_pad",
           [](PymBuilder &self, PymValue &parentPad, std::vector<PymValue> args,
              const char *name) {
             unsigned args_num  = args.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);

             auto res = LLVMBuildCatchPad(self.get(), parentPad.get(), rawArgs.data(),
                                          args_num, name);
             return PymCatchPadInst(res);
           },
           "parent_pad"_a, "args"_a, "name"_a = "")
      .def("cleanup_pad",
           [](PymBuilder &self, PymValue &parentPad, std::vector<PymValue> args,
              const char *name) {
             unsigned args_num  = args.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);

             auto res = LLVMBuildCleanupPad(self.get(), parentPad.get(), rawArgs.data(),
                                            args_num, name);
             return PymInstruction(res);
           },
           "parent_pad"_a, "args"_a, "name"_a = "")
      .def("catch_switch",
           [](PymBuilder &self, PymValue &parentPad, PymBasicBlock &unwindBB,
              unsigned numHandlers, const char *name) {
             auto res = LLVMBuildCatchSwitch(self.get(), parentPad.get(),
                                             unwindBB.get(), numHandlers,
                                             name);
             return PymCatchSwitchInst(res);
           },
           "parent_pad"_a, "unwind_bb"_a, "num_handlers"_a, "name"_a = "")
       /*
         Arithmetic
        */
  BUILDER_BIND_BINARY_OPS
      .def("binop",
           [](PymBuilder &self, LLVMOpcode op, PymValue &lhs, PymValue &rhs,
              const char *name) {
             return PymValueAuto(LLVMBuildBinOp(self.get(), op, lhs.get(), rhs.get(), name));
           })
      .def("neg",
           [](PymBuilder &self, PymValue &v, const char *name) {
             return PymValueAuto(LLVMBuildNeg(self.get(), v.get(), name));
           })
      .def("neg_nsw",
           [](PymBuilder &self, PymValue &v, const char *name) {
             return PymValueAuto(LLVMBuildNSWNeg(self.get(), v.get(), name));
           })
      .def("neg_nuw",
           [](PymBuilder &self, PymValue &v, const char *name) {
             return PymValueAuto(LLVMBuildNUWNeg(self.get(), v.get(), name));
           })
      .def("fneg",
           [](PymBuilder &self, PymValue &v, const char *name) {
             return PymValueAuto(LLVMBuildFNeg(self.get(), v.get(), name));
           })
      .def("not_",
           [](PymBuilder &self, PymValue &v, const char *name) {
             return PymValueAuto(LLVMBuildNot(self.get(), v.get(), name));
           })
      .def("malloc",
           [](PymBuilder &self, PymType &type, const char *name) {
             return PymCallInst(LLVMBuildMalloc(self.get(), type.get(), name));
           },
           "type"_a, "name"_a = "")
      .def("array_malloc",
           [](PymBuilder &self, PymType &type, PymValue &val, const char *name) {
             return PymCallInst(LLVMBuildArrayAlloca(self.get(), type.get(), val.get(), name));
           },
           "type"_a, "value"_a, "name"_a = "")
      .def("memset",
           [](PymBuilder &self, PymValue &ptr, PymValue &val, PymValue &len, unsigned align) {
             auto res = LLVMBuildMemSet(self.get(), ptr.get(), val.get(), len.get(),
                                        align);
             return PymCallInst(res);
           },
           "ptr"_a, "val"_a, "len"_a, "align"_a)
      .def("memcpy",
           [](PymBuilder &self, PymValue &dest, unsigned dstAlign, PymValue &src,
              unsigned srcAlign, PymValue size) {
             auto res = LLVMBuildMemCpy(self.get(), dest.get(), dstAlign, src.get(),
                                        srcAlign, size.get());
             return PymCallInst(res);
           },
           "dest"_a, "dest_align"_a, "src"_a, "src_align"_a, "size"_a,
           "Creates and inserts a memcpy between the specified pointers.")
      .def("mem_move",
           [](PymBuilder &self, PymValue &dest, unsigned dstAlign, PymValue &src,
              unsigned srcAlign, PymValue size) {
             auto res = LLVMBuildMemMove(self.get(), dest.get(), dstAlign, src.get(),
                                         srcAlign, size.get());
             return PymCallInst(res);
           },
           "dest"_a, "dest_align"_a, "src"_a, "src_align"_a, "size"_a,
           "Creates and inserts a memmove between the specified pointers.")
      .def("alloca",
           [](PymBuilder &self, PymType &type, const char *name) {
             return PymAllocaInst(LLVMBuildAlloca(self.get(), type.get(), name));
           },
           "type"_a, "name"_a = "")
      .def("array_alloca",
           [](PymBuilder &self, PymType &type, PymValue &val, const char *name) {
             return PymAllocaInst(LLVMBuildArrayAlloca(self.get(), type.get(),
                                                      val.get(), name));
           },
           "type"_a, "value"_a, "name"_a = "")
      .def("free",
           [](PymBuilder &self, PymValue pointer) {
             return PymCallInst(LLVMBuildFree(self.get(), pointer.get()));
           },
           "pointer"_a)
      .def("load2",
           [](PymBuilder &self, PymType &type, PymValue &pointer, const char *name) {
             return PymLoadInst(LLVMBuildLoad2(self.get(), type.get(), pointer.get(),
                                              name));
           },
           "type"_a, "ptr"_a, "name"_a = "")
      .def("store",
           [](PymBuilder &self, PymValue &val, PymValue &ptr) {
             return PymStoreInst(LLVMBuildStore(self.get(), val.get(), ptr.get()));
           },
           "value"_a, "ptr"_a)
      .def("gep2",
           [](PymBuilder &self, PymType &type, PymValue &ptr, std::vector<PymValue> indices,
              const char *name) {
             unsigned num_indices = indices.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, indices, rawIndices, num_indices);
             return PymValueAuto(LLVMBuildGEP2(self.get(), type.get(), ptr.get(),
                                              rawIndices.data(), num_indices, name));
           },
           "type"_a,  "ptr"_a, "indices"_a, "name"_a = "")
     .def("in_bounds_gep2",
          [](PymBuilder &self, PymType &type, PymValue &ptr, std::vector<PymValue> indices,
             const char *name) {
            unsigned num_indices = indices.size();
            UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, indices, rawIndices, num_indices);
            return PymValueAuto(LLVMBuildGEP2(self.get(), type.get(), ptr.get(),
                                             rawIndices.data(), num_indices, name));
          },
          "type"_a,  "ptr"_a, "indices"_a, "name"_a = "")
     .def("struct_gep2",
          [](PymBuilder &self, PymType &type, PymValue &ptr, unsigned index, const char *name) {
            auto res = LLVMBuildStructGEP2(self.get(), type.get(), ptr.get(),
                                           index, name);
            return PymValueAuto(res);
          },
          "type"_a, "ptr"_a, "index"_a, "name"_a = "")
     .def("global_string",
          [](PymBuilder &self, const char *str, const char *name) {
            return PymValueAuto(LLVMBuildGlobalString(self.get(), str, name));
          })
     .def("global_string_ptr",
          [](PymBuilder &self, const char *str, const char *name) {
            return PymValueAuto(LLVMBuildGlobalStringPtr(self.get(), str, name));
          })
       /* cast start  */
  BUILDER_BIND_CAST_OPS
     .def("cast",
          [](PymBuilder &self, LLVMOpcode opcode, PymValue &value,  PymType &destType,
             const char *name) {
            return PymValueAuto(LLVMBuildCast(self.get(), opcode, value.get(),
                                             destType.get(), name));
          },
          "opcode"_a, "value"_a, "dest_type"_a, "name"_a = "")
     .def("int_cast_2",
          [](PymBuilder &self, PymValue &value, PymType &destType, const char *name){
            return PymValueAuto(LLVMBuildFPCast
                                 (self.get(), value.get(), destType.get(), name));
          },
          "value"_a, "dest_type"_a, "name"_a = "")
     .def("int_cast",
          [](PymBuilder &self, PymValue &value, PymType &destType, const char *name){
            return PymValueAuto(LLVMBuildIntCast
                                 (self.get(), value.get(), destType.get(), name));
          },
          "value"_a, "dest_type"_a, "name"_a = "",
          "Deprecated: This cast is always signed. Use LLVMBuildIntCast2 instead.")
     .def_static("get_cast_opcode",
                 [](PymValue &src, bool srcIsSigned, PymType &destType, bool destIsSigned) {
                   return LLVMGetCastOpcode(src.get(), srcIsSigned, destType.get(),
                                            destIsSigned);
                 },
                 "src"_a, "src_is_signed"_a, "dest_type"_a, "dest_is_signed"_a)
     /* end */
     .def("icmp",
          [](PymBuilder &self, LLVMIntPredicate op, PymValue &lhs, PymValue &rhs,
             const char *name) {
            return PymValueAuto(LLVMBuildICmp(self.get(), op, lhs.get(), rhs.get(),
                                             name));
          },
          "op"_a, "lhs"_a, "rhs"_a, "name"_a = "")
     .def("phi",
          [](PymBuilder &self, PymType &type, const char *name) {
            return PymPHINode(LLVMBuildPhi(self.get(), type.get(), name));
          },
          "type"_a, "name"_a = "")
     .def("call_2",
          [](PymBuilder &self, PymTypeFunction &type, PymFunction &fn, std::vector<PymValue> args,
             const char *name) {
            unsigned args_num = args.size();
            UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);
            return PymCallInst(LLVMBuildCall2
                                (self.get(), type.get(), fn.get(), rawArgs.data(),
                                 args_num, name));
          },
          "fn_type"_a, "fn"_a, "args"_a, "name"_a = "")
     .def("call_with_operand_bundles",
          [](PymBuilder &self, PymTypeFunction type, PymFunction fn,
             std::vector<PymValue> args,
             std::vector<PymOperandBundle> bundles,
             const char *name) {
            unsigned args_num = args.size();
            UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);

            unsigned bundles_num = bundles.size();
            UNWRAP_VECTOR_WRAPPER_CLASS(LLVMOperandBundleRef, bundles, rawBundles,
                                        bundles_num);
            
            return PymCallInst(LLVMBuildCallWithOperandBundles
                                (self.get(), type.get(), fn.get(), rawArgs.data(),
                                 args_num, rawBundles.data(), bundles_num, name));
          })
     .def("select",
          [](PymBuilder &self, PymValue &If, PymValue &Then, PymValue &Else,
             const char *name) {
            return PymCallInst(LLVMBuildSelect
                                (self.get(), If.get(), Then.get(), Else.get(),
                                 name));
          },
          "If"_a, "Then"_a, "Else"_a, "name"_a = "")
     .def("vaarg",
          [](PymBuilder &self, PymValue &list, PymType &type, const char *name) {
            return PymValue(LLVMBuildVAArg(self.get(), list.get(), type.get(),
                                          name));
          },
          "list"_a, "type"_a, "name"_a = "")
     .def("extract_element",
          [](PymBuilder &self, PymValue &vecVal, PymValue &index, const char *name) {
            return PymValueAuto(LLVMBuildExtractElement
                                 (self.get(), vecVal.get(), index.get(), name));
          },
          "vec"_a, "index"_a, "name"_a = "")
     .def("insert_element",
          [](PymBuilder &self, PymValue &vecVal, PymValue &eltVal,
             PymValue &index, const char *name) {
            return PymValueAuto(LLVMBuildInsertElement
                                 (self.get(), vecVal.get(), eltVal.get(),
                                  index.get(), name));
          },
          "vec"_a, "element"_a, "index"_a, "name"_a = "")
    .def("shuffle_vector",
         [](PymBuilder &self, PymValue &v1, PymValue &v2, PymValue &mask, const char *name) {
           return PymShuffleVectorInst(LLVMBuildShuffleVector
                                        (self.get(), v1.get(), v2.get(), mask.get(), name));
         },
         "v1"_a, "v2"_a, "mask"_a, "name"_a = "")
    .def("extract_value",
         [](PymBuilder &self, PymValue &aggVal, unsigned index, const char *name) {
           return PymValueAuto(LLVMBuildExtractValue
                                (self.get(), aggVal.get(), index, name));
         },
         "agg"_a, "index"_a, "name"_a = "")
    .def("insert_value",
         [](PymBuilder &self, PymValue &aggVal, PymValue &eltVal, unsigned index,
            const char *name) {
           return PymValueAuto(LLVMBuildInsertValue
                                (self.get(), aggVal.get(), eltVal.get(), index,
                                 name));
         },
         "agg"_a, "elt"_a, "index"_a, "name"_a = "")
    .def("freeze",
         [](PymBuilder &self, PymValue &val, const char *name) {
           return PymValueAuto(LLVMBuildFreeze(self.get(), val.get(), name));
         },
         "val"_a, "name"_a = "")
    .def("is_null",
         [](PymBuilder &self, PymValue &val, const char *name) {
           return PymValueAuto(LLVMBuildIsNull(self.get(), val.get(), name));
         },
         "value"_a, "name"_a = "")
    .def("is_not_null",
         [](PymBuilder &self, PymValue &val, const char *name) {
           return PymValueAuto(LLVMBuildIsNotNull(self.get(), val.get(), name));
         },
         "value"_a, "name"_a = "")
    .def("ptr_diff_2",
         [](PymBuilder &self, PymType &elemType, PymValue &lhs, PymValue &rhs,
            const char *name) {
           return PymValueAuto(LLVMBuildPtrDiff2
                                (self.get(), elemType.get(), lhs.get(),
                                 rhs.get(), name));
         },
         "elem_type"_a, "lhs"_a, "rhs"_a, "name"_a = "")
    .def("fence",
         [](PymBuilder &self, LLVMAtomicOrdering ordering, bool singleThread,
            const char *name) {
           return PymFenceInst(LLVMBuildFence(self.get(), ordering, singleThread, name));
         },
         "ordering"_a, "singleThread"_a, "name"_a = "")
    .def("atomic_rmw",
         [](PymBuilder &self, LLVMAtomicRMWBinOp op, PymValue &ptr, PymValue val,
            LLVMAtomicOrdering ordering, bool singleThread) {
           auto res = LLVMBuildAtomicRMW(self.get(), op, ptr.get(), val.get(), ordering,
                                         singleThread);
           return PymAtomicRMWInst(res);
         },
         "op"_a, "ptr"_a, "val"_a, "ordering"_a, "singleThread"_a)
    .def("atomic_cmp_xchg",
         [](PymBuilder &self, PymValue &ptr, PymValue &cmp, PymValue &New,
            LLVMAtomicOrdering successOrdering, LLVMAtomicOrdering failureOrdering,
            bool singleThread) {
           auto res = LLVMBuildAtomicCmpXchg(self.get(), ptr.get(), cmp.get(),
                                             New.get(), successOrdering,
                                             failureOrdering, singleThread);
           return PymAtomicCmpXchgInst(res);
         });

  

  BasicBlockClass
      .def("__repr__",
           [](PymBasicBlock &self) {
             return "<BasicBlock>";
           })
      .def("__str__",
           [](PymBasicBlock &self) {
             auto value = LLVMBasicBlockAsValue(self.get());
             return get_value_str(value);
           })
      .def("__init__",
           [](PymBasicBlock *bb, PymContext &c, const char *name) {
             new (bb) PymBasicBlock(LLVMCreateBasicBlockInContext(c.get(), name));
           },
           "context"_a, "name"_a = "",
           "Create a new basic block without inserting it into a function.")
      .def("__init__",
           [](PymBasicBlock *bb, PymContext &c, PymFunction &f, const char *name) {
             new (bb) PymBasicBlock(LLVMAppendBasicBlockInContext
                                     (c.get(), f.get(), name));
           },
           "context"_a, "function"_a, "name"_a = "",
           "Create a new basic block without inserting it into a function.")
      .def("__init__",
           [](PymBasicBlock *bb, PymContext &c, PymBasicBlock &BB, const char *name) {
             new (bb) PymBasicBlock(LLVMInsertBasicBlockInContext
                                     (c.get(), BB.get(), name));
           },
           "context"_a, "bb"_a, "name"_a = "",
           "Insert a basic block in a function before another basic block.\n\n"
           "The function to add to is determined by the function of the"
           "passed basic block.")
      .def("__init__",
           [](PymBasicBlock *bb, PymBasicBlock &BB, const char *name) {
             new (bb) PymBasicBlock(LLVMInsertBasicBlock(BB.get(), name));
           },
           "insert_before_bb"_a, "name"_a = "",
           "Insert a basic block in a function using the global context.")
      .def_prop_ro("name",
                   [](PymBasicBlock &self) {
                     return LLVMGetBasicBlockName(self.get());
                   })
      .def_prop_ro("parent",
                   [](PymBasicBlock &self) {
                     return PymFunction(LLVMGetBasicBlockParent(self.get()));
                   },
                   "Obtain the function to which a basic block belongs.")
      .def_prop_ro("terminator",
                   [](PymBasicBlock &self) -> optional<PymInstruction> {
                     auto res = LLVMGetBasicBlockTerminator(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymInstruction);
                   },
                   "Obtain the terminator instruction for a basic block.\n\n"
                   "If the basic block does not have a terminator (it is not well-formed"
                   "if it doesn't), then NULL is returned.")
      .def_prop_ro("value",
                   [](PymBasicBlock &self) {
                     return PymBasicBlockValue(LLVMBasicBlockAsValue(self.get()));
                   })
      .def_prop_ro("next",
                   [](PymBasicBlock &self) -> optional<PymBasicBlock> {
                     auto res = LLVMGetNextBasicBlock(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymBasicBlock);
                   })
      .def_prop_ro("prev",
                   [](PymBasicBlock &self) -> optional<PymBasicBlock> {
                     auto res = LLVMGetPreviousBasicBlock(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymBasicBlock);
                   })
      .def_prop_ro("first_instruction",
                   [](PymBasicBlock &self) -> optional<PymInstruction> {
                     auto res = LLVMGetFirstInstruction(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymInstruction);
                   })
      .def_prop_ro("last_instruction",
                   [](PymBasicBlock &self) -> optional<PymInstruction> {
                     auto res = LLVMGetLastInstruction(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymInstruction);
                   })
      .def_prop_ro("instructions",
                   [](PymBasicBlock &self) {
                     auto res = LLVMGetFirstInstruction(self.get());
                     return PymInstructionIterator(PymInstruction(res));
                   })
      .def("create_and_insert_before",
           [](PymBasicBlock &self, const char *name) {
             return PymBasicBlock(LLVMInsertBasicBlock(self.get(), name));
           },
           "Insert a basic block in a function using the global context.")
      .def("destroy", // TODO test
           [](PymBasicBlock &self) {
             return LLVMDeleteBasicBlock(self.get());
           },
           "Remove a basic block from a function and delete it.\n\n"
           "This deletes the basic block from its containing function and deletes"
           "the basic block itself.")
      .def("remove_from_parent",
           [](PymBasicBlock &self) {
             return LLVMRemoveBasicBlockFromParent(self.get());
           },
           "Remove a basic block from a function.\n\n"
           "This deletes the basic block from its containing function but keep"
           "the basic block alive.")
      .def("move_before",
           [](PymBasicBlock &self, PymBasicBlock posBB) {
             return LLVMMoveBasicBlockBefore(self.get(), posBB.get());
           },
           "pos"_a,
           "Move a basic block to before another one.")
      .def("move_after",
           [](PymBasicBlock &self, PymBasicBlock posBB) {
             return LLVMMoveBasicBlockAfter(self.get(), posBB.get());
           },
           "pos",
           "Move a basic block to after another one.");


  OperandBundleClass
      .def("__repr__",
           [](PymOperandBundle &self) {
             return "<OperandBundle>";
           })
      .def("__init__",
           [](PymOperandBundle *ob, std::string &tag, std::vector<PymValue> args) {
             unsigned arg_num = args.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, raw_args, arg_num);
             new (ob) PymOperandBundle(LLVMCreateOperandBundle
                                        (tag.c_str(), tag.size(), raw_args.data(),
                                         arg_num));
           })
      .def_prop_ro("tag",
                   [](PymOperandBundle &self) {
                     size_t len;
                     auto tag = LLVMGetOperandBundleTag(self.get(), &len);
                     return std::string(tag, len);
                   })
      .def_prop_ro("operands_num",
                   [](PymOperandBundle &self) {
                     return LLVMGetNumOperandBundleArgs(self.get());
                   })
      .def("get_operand_at",
           [](PymOperandBundle &self, unsigned index) {
             return PymValueAuto(LLVMGetOperandBundleArgAtIndex(self.get(), index));
           });

    

  IntrinsicClass
      .def("__repr__",
           [](PymIntrinsic &self) {
             return "<Intrinsic>";
           })
       .def("__repr__",
            [](PymIntrinsic &self) {
              size_t len;
              auto raw_name = LLVMIntrinsicGetName(self.get(), &len);
              auto name = std::string(raw_name, len);
              return fmt::format("<Instrinsic id={} name={}>", self.get(), name);
            })
       .def("__bool__", // override default behavior
            [](PymIntrinsic &self) { return true; })
       .def_static("lookup",
                   [](std::string &name) {
                     return PymIntrinsic(LLVMLookupIntrinsicID(name.c_str(), name.size()));
                   },
                   "name"_a = "",
                   "Obtain the intrinsic ID number which matches the given function name.")
      .def("get_type",
           [](PymIntrinsic &self, PymContext &context, std::vector<PymType> paramTypes) {
             size_t paramCnt = paramTypes.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, paramTypes, params, paramCnt);
             return PymTypeFunction(LLVMIntrinsicGetType
                                     (context.get(), self.get(), params.data(),
                                      paramCnt));

           },
           "context"_a, "param_types"_a,
           "Retrieves the type of an intrinsic.  For overloaded intrinsics,"
           " parameter types must be provided to uniquely identify an overload.")
      .def_prop_ro("id",
                   [](PymIntrinsic &self) {
                     return self.get();
                   })
      .def_prop_ro("name",
                   [](PymIntrinsic &self) {
                     size_t len;
                     auto name = LLVMIntrinsicGetName(self.get(), &len);
                     return std::string(name, len);
                   },
                   "Retrieves the name of an intrinsic.")
      .def_prop_ro("is_overloaded",
                   [](PymIntrinsic &self) {
                     return LLVMIntrinsicIsOverloaded(self.get()) != 0;
                   })
      // note LLVMIntrinsicCopyOverloadedName is deprecated
      .def("copy_overloaded_name",
           [](PymIntrinsic &self, std::vector<PymType> paramTypes) {
             size_t paramCnt = paramTypes.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, paramTypes, params, paramCnt);
             size_t nameLen;
             auto name = LLVMIntrinsicCopyOverloadedName
                           (self.get(), params.data(), paramCnt, &nameLen);
             return std::string(name, nameLen);
           },
           "param_types"_a,
           "Deprecated: Use :func:`copy_overloaded_name2` instead.")
      .def("copy_overloaded_name2",
           [](PymIntrinsic &self, PymModule &module, std::vector<PymType> paramTypes) {
             size_t nameLen;
             size_t paramCnt = paramTypes.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, paramTypes, params, paramCnt);
             auto name = LLVMIntrinsicCopyOverloadedName2
                           (module.get(), self.get(), params.data(), paramCnt, &nameLen);
             return std::string(name, nameLen);
           },
           "module"_a, "param_types"_a,
           "Copies the name of an overloaded intrinsic identified by a given"
           " list of parameter types.\n\n"
           "This version also supports unnamed types.");
  


  MetadataEntriesClass
      .def("__repr__",
           [](PymMetadataEntries &self) {
             return "<MetadataEntries>";
           })
      .def("get_kind",
           [](PymMetadataEntries &self, unsigned index) {
             return LLVMValueMetadataEntriesGetKind(self.get(), index);
           },
           "index"_a,
           "Returns the kind of a value metadata entry at a specific index.")
      .def("get_metadata",
           [](PymMetadataEntries &self, unsigned index) {
             return PymMetadata(LLVMValueMetadataEntriesGetMetadata(self.get(), index));
           },
           "index"_a,
           "Returns the underlying metadata node of a value metadata entry at a"
           "specific index.");

  UseClass
      .def("__repr__",
           [](PymUse &self) {
             return "<Use>";
           })
      .def_prop_ro("next",
                   [](PymUse &u) -> optional<PymUse> {
                     auto res = LLVMGetNextUse(u.get());
                     WRAP_OPTIONAL_RETURN(res, PymUse);
                   },
                   "Obtain the next use of a value.\n\n"
                   "This effectively advances the iterator. It returns NULL if you are on"
                   "the final use and no more are available.")
      .def_prop_ro("user",
                   [](PymUse &u) { return PymUser(LLVMGetUser(u.get())); },
                   "Obtain the user value for a user.\n",
                   "The returned value corresponds to a llvm::User type.")
      .def_prop_ro("used_value",
                   [](PymUse &u) {
                     return PymValueAuto(LLVMGetUsedValue(u.get()));
                   });
  
  
  AttributeClass
      .def("__repr__",
           [](PymAttribute &self) {
             return "<Attribute>";
           })
      .def_prop_ro("is_enum",
                   [](PymAttribute &attr) {
                     return LLVMIsEnumAttribute(attr.get()) != 0;
                   })
      .def_prop_ro("is_string",
                   [](PymAttribute &attr) {
                     return LLVMIsStringAttribute(attr.get()) != 0;
                   })
      .def_prop_ro("is_type",
                   [](PymAttribute &attr) {
                     return LLVMIsTypeAttribute(attr.get()) != 0;
                   });
  
  EnumAttributeClass
      .def("__repr__",
           [](PymEnumAttribute &self) {
             return "<EnumAttribute>";
           })
      .def("__init__",
           [](PymEnumAttribute *t, PymContext &c, unsigned kindID, uint64_t val) {
             new (t) PymEnumAttribute(LLVMCreateEnumAttribute(c.get(), kindID, val));
           },
           "context"_a, "kind_id"_a, "val"_a)
      .def("__repr__",
           [](PymEnumAttribute &self) {
             auto kind = LLVMGetEnumAttributeKind(self.get());
             auto value = LLVMGetEnumAttributeValue(self.get());
             return fmt::format("<EnumAttribute kind={} value={}>", kind, value);
           })
      .def_prop_ro("kind",
                   [](PymEnumAttribute &attr) {
                     return LLVMGetEnumAttributeKind(attr.get());
                   }, "attr"_a)
      .def_prop_ro("value",
                   [](PymEnumAttribute &attr) {
                     return LLVMGetEnumAttributeValue(attr.get());
                   },
                   "Get the enum attribute's value. 0 is returned if none exists.")
      .def_static("get_enum_attribute_kind_for_name",
                  [](const std::string &name){
                    return LLVMGetEnumAttributeKindForName(name.c_str(), name.size());
                  },
                  "name"_a = "",
                  "Return an unique id given the name of a enum attribute,"
                  "or 0 if no attribute by that name exists.\n\n"
                  "See http://llvm.org/docs/LangRef.html#parameter-attributes"
                  "and http://llvm.org/docs/LangRef.html#function-attributes"
                  "for the list of available attributes.\n\n"
                  "NB: Attribute names and/or id are subject to change without"
                  "going through the C API deprecation cycle.")
      .def_static("get_last_enum_attribute_kind", &LLVMGetLastEnumAttributeKind);

  TypeAttributeClass
      .def("__repr__",
           [](PymTypeAttribute &self) {
             return "<TypeAttribute>";
           })
      .def("__init__",
           [](PymTypeAttribute *t, PymContext &context, unsigned kind_id, PymType &type) {
             new (t) PymTypeAttribute(LLVMCreateTypeAttribute
                                       (context.get(), kind_id, type.get()));
           },
           "context"_a, "kind_id"_a, "type"_a)
      .def("__repr__",
           [](PymTypeAttribute &self) {
             auto value = LLVMGetTypeAttributeValue(self.get());
             auto type_kind = LLVMGetTypeKind(value);
             return fmt::format("<TypeAttribute value={}>", get_repr_str(type_kind));
           })
      .def_prop_ro("value",
                   [](PymTypeAttribute &ta){
                     return PymTypeAuto(LLVMGetTypeAttributeValue(ta.get()));
                   },
                   "Get the type attribute's value.");

  StringAttributeClass
      .def("__repr__",
           [](PymStringAttribute &self) {
             return "<StringAttribute>";
           })
      .def("__init__",
           [](PymStringAttribute *t, PymContext &c, const std::string &kind, const std::string &value) {
             auto raw = LLVMCreateStringAttribute(c.get(),
                                                  kind.c_str(), kind.size(),
                                                  value.c_str(), value.size());
             new (t) PymStringAttribute(raw);
           },
           "context"_a, "kind"_a, "value"_a)
      .def("__repr__",
           [](PymStringAttribute &self) {
             unsigned kind_length;
             const char *raw_kind = LLVMGetStringAttributeKind(self.get(), &kind_length);
             auto kind = std::string(raw_kind, kind_length);
             
             unsigned value_length;
             const char *raw_value = LLVMGetStringAttributeValue(self.get(), &value_length);
             auto value = std::string(raw_value, value_length);

             return fmt::format("<TypeAttribute kind={} value={}>", kind, value);
           })
      .def_prop_ro("kind",
                   [](PymStringAttribute &ta) {
                     unsigned length;
                     const char *kind = LLVMGetStringAttributeKind(ta.get(), &length);
                     return std::string(kind, length);
                   })
      .def_prop_ro("value",
                   [](PymStringAttribute &ta) {
                     unsigned length;
                     const char *value = LLVMGetStringAttributeValue(ta.get(), &length);
                     return std::string(value, length);
                   },
                   "Get the type attribute's value.");
  

  DiagnosticInfoClass
      .def("__repr__",
           [](PymDiagnosticInfo &self) {
             return "<DiagnosticInfo>";
           })
      // .def(nb::init<>()) // NOTE currently no constructor function for python, we'll see
      .def_prop_ro("description",
                   [](PymDiagnosticInfo &d) {
                     char *diagInfoDesc = LLVMGetDiagInfoDescription(d.get());
                     std::string diagInfoDescCopy(diagInfoDesc);
                     LLVMDisposeMessage(diagInfoDesc);
                     return diagInfoDescCopy;
                   },
                   "Return a string representation of the DiagnosticInfo.")
      .def_prop_ro("severity",
                   [](PymDiagnosticInfo &d) {
                     return LLVMGetDiagInfoSeverity(d.get());
                   },
                   "Return an enum LLVMDiagnosticSeverity.");

  
  NamedMDNodeClass
      .def("__repr__",
           [](PymNamedMDNode &self) {
             return "<NamedMDNode>";
           })
      .def_prop_ro("next",
                   [](PymNamedMDNode &nmdn) -> optional<PymNamedMDNode>{
                     auto res = LLVMGetNextNamedMetadata(nmdn.get());
                     WRAP_OPTIONAL_RETURN(res, PymNamedMDNode);
                   },
                   "Advance a NamedMDNode iterator to the next NamedMDNode.\n\n"
                   "Returns NULL if the iterator was already at the end and there"
                   " are no more named metadata nodes.")
      .def_prop_ro("prev",
                   [](PymNamedMDNode &nmdn) -> optional<PymNamedMDNode> {
                     auto res = LLVMGetPreviousNamedMetadata(nmdn.get());
                     WRAP_OPTIONAL_RETURN(res, PymNamedMDNode);
                   },
                   "Decrement a NamedMDNode iterator to the previous NamedMDNode.\n\n"
                   "Returns NULL if the iterator was already at the beginning and there are"
                   "no previous named metadata nodes.")
      .def_prop_ro("name",
                   [](PymNamedMDNode &nmdn) {
                     size_t len;
                     const char *name =  LLVMGetNamedMetadataName(nmdn.get(), &len);
                     return std::string(name, len);
                   },
                   "Retrieve the name of a NamedMDNode.");

  
  ContextClass
      .def("__repr__",
           [](PymContext &self) {
             return "<Context>";
           })
      .def(nb::init<>(), "Create a new context.")
      .def("__enter__",
           [](PymContext &self) {
             return &self;
           })
      .def("__exit__",
           [](PymContext &self, nb::args args, nb::kwargs kwargs) {})
      .def_static("get_global_context", &PymContext::getGlobalContext,
                  "Obtain the global context instance.")
      .def_prop_ro("diagnostic_context",
                   [](PymContext &c) {
                     // FIXME The function cannot work correctly (always None) since
                     // `LLVMContextSetDiagnosticHandler` cannot, which set
                     // the diagnostic context
                     return LLVMContextGetDiagnosticContext(c.get());
                   },
                   "Get the diagnostic context of this context.")
      .def_prop_rw("should_discard_value_names", // TODO convert LLVMBool to bool
                   [](PymContext &c) -> bool { return LLVMContextShouldDiscardValueNames(c.get()) != 0; },
                   [](PymContext &c, bool discard) {
                     return LLVMContextSetDiscardValueNames(c.get(), discard);
                   },
                   nb::for_getter
                     ("Retrieve whether the given context is set to"
                      "discard all value names.\n\n"
                      "Return true if the Context runtime configuration "
                      "is set to discard all value names. When true, "
                      "only GlobalValue names will be available in the IR."),
                   nb::for_setter
                     ("Set whether the given context discards all value names.\n\n"
                      "If true, only the names of GlobalValue objects"
                      "will be available in the IR.\n"
                      "This can be used to save memory and runtime, "
                      "especially in release mode."))
      .def("set_diagnostic_handler", // FIXME
           [](PymContext &c, LLVMDiagnosticHandler handler, void * diagnosticContext){
             return LLVMContextSetDiagnosticHandler(c.get(), handler, diagnosticContext);
           },
           "handler"_a, "diagnostic_context"_a,
           "Set the diagnostic handler for this context.")
      // .def("get_diagnostic_handler", FIXME
      //      [](PymContext &c)  { return LLVMContextGetDiagnosticHandler(c.get()); },
      //      "Get the diagnostic handler of this context.")
      .def("set_yield_callback", // FIXME
           [](PymContext &c, LLVMYieldCallback callback, void *opaqueHandle){
             return LLVMContextSetYieldCallback(c.get(), callback, opaqueHandle);
           },
           "callback"_a, "opaque_handle"_a,
           "Set the yield callback function for this context.")
      .def("parse_ir",
           [](PymContext &self, PymMemoryBuffer &memBuf) {
             auto res = parseIR(self.get(), memBuf.get());
             memBuf.reset(); // We Cannot reuse the memory buffer again
             return res;
           },
           "memory_buffer"_a,
           "Read LLVM IR from a memory buffer and convert it into an in-memory Module"
           "object.\n\n"
           ":raises RuntimeError\n"
           "NOTE that you cannot use passed-in memory_buffer after this operation.")
      .def("create_builder",
           [](PymContext &self) {
             return PymBuilder(LLVMCreateBuilderInContext(self.get()));
           })
       // LLVMParseBitcodeInContext is deprecated in favor of LLVMParseBitcodeInContext2
      .def("parse_bitcode",
           [](PymContext &self, PymMemoryBuffer &memBuf) {
             LLVMModuleRef module;
             auto res = LLVMParseBitcodeInContext2
                          (self.get(), memBuf.get(), &module) == 0;
             if (!res) {
               throw std::runtime_error("Error!");
             }
             // TODO check memBuf availability
             return PymModule(module);
           },
           "mem_buf"_a,
           "Builds a module from the bitcode in the specified memory buffer, "
           "returning a reference to the module")
      // LLVMGetBitcodeModuleInContext is deprecated in favor of
      //  LLVMGetBitcodeModuleInContext2
      .def("get_bitcode_module",
           [](PymContext &self, PymMemoryBuffer &memBuf) {
             LLVMModuleRef module;
             auto res = LLVMGetBitcodeModuleInContext2
                          (self.get(), memBuf.get(), &module) == 0;
             if (!res) {
               throw std::runtime_error("Error!");
             }
             memBuf.reset();
             return PymModule(module);
           },
           "mem_buf"_a,
           "Reads a module from the given memory buffer.\n"
           "Takes ownership of MemBuf if (and only if) the module was read "
           "successfully")
      .def("create_basic_block",
           [](PymContext &self, const char *name) {
             return PymBasicBlock(LLVMCreateBasicBlockInContext
                                   (self.get(), name));
           },
           "name"_a = "",
           "Create a new basic block without inserting it into a function.")
      .def("append_basic_block",
           [](PymContext &self, PymFunction fn, const char *name) {
             return PymBasicBlock(LLVMAppendBasicBlockInContext
                                   (self.get(), fn.get(), name));
           },
           "fn"_a, "name"_a = "",
           "Append a basic block to the end of a function.")
      .def("insert_basic_block",
           [](PymContext &self, PymBasicBlock bb, const char *name) {
             return PymBasicBlock(LLVMInsertBasicBlockInContext
                                   (self.get(), bb.get(), name));
           },
           "bb"_a, "name"_a = "",
           "Insert a basic block in a function before another basic block.\n\n"
           "The function to add to is determined by the function of the"
           "passed basic block.")
      .def("get_md_kind_id",
           [](PymContext &c, const std::string &name) {
             return LLVMGetMDKindIDInContext(c.get(), name.c_str(), name.size());
           },
           "name"_a = "")
      .def("create_enum_attribute",
           [](PymContext &c, unsigned kindID, uint64_t val) {
             return PymEnumAttribute(LLVMCreateEnumAttribute(c.get(), kindID, val));
           },
           "kind_id"_a, "val"_a,
           "Create an enum attribute.")
      .def("create_type_attribute",
           [](PymContext &context, unsigned kind_id, PymType &type) {
             return PymTypeAttribute(LLVMCreateTypeAttribute
                                      (context.get(), kind_id, type.get()));
           },
           "kind_id"_a, "type"_a,
           "Create a type attribute")
      .def("create_string_attribute",
           [](PymContext &c, const std::string &kind, const std::string &value) {
             auto raw = LLVMCreateStringAttribute(c.get(),
                                                  kind.c_str(), kind.size(),
                                                  value.c_str(), value.size());
             return PymStringAttribute(raw);
           })
      .def("get_type_by_name_2", // TODO also create one in PymType static method
           [](PymContext &c, const char *name) -> optional<PymType*> {
             auto res = LLVMGetTypeByName2(c.get(), name);
             WRAP_OPTIONAL_RETURN(res, PymTypeAuto);
           },
           "name"_a = "")
      .def("create_md_string_2",
           [](PymContext &self, std::string &name) {
             return PymMDString(LLVMMDStringInContext2(self.get(), name.c_str(), name.size()));
           },
           "name"_a = "",
           "Create an MDString value from a given string value.")
      .def("create_md_node_2",
           [](PymContext &self, std::vector<PymMetadata> mds) {
             size_t num = mds.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMMetadataRef, mds, raw, num);
             return PymMDNode(LLVMMDNodeInContext2(self.get(), raw.data(), num));
           },
           "metadata"_a)
      .def("get_metadata_as_value",
           [](PymContext &self, PymMetadata &md) {
             return getMoreSpcMetadataAsValue(LLVMMetadataAsValue
                                                (self.get(), md.get()));
           });

  

  ModuleClass
      .def(nb::init<const std::string &>(), "name"_a = "")
      .def("__repr__",
           [](PymModule &self) {
             size_t len;
             const char *raw_identifier = LLVMGetModuleIdentifier(self.get(), &len);
             auto id = std::string(raw_identifier, len);
             return fmt::format("<Module id={}>", id);
           })
      .def("__str__",
           [](PymModule &m) {
             char *str = LLVMPrintModuleToString(m.get());
             std::string strCopy(str);
             LLVMDisposeMessage(str);
             return strCopy;
           },
           "Return a string representation of the module")
      .def("__enter__",
           [](PymModule &self) {
             return &self;
           })
      .def("__exit__", [](PymModule &self, nb::args args, nb::kwargs kwargs) {})
      .def_prop_ro("first_global_variable",
                   [](PymModule &m) -> optional<PymGlobalVariable> {
                     auto res = LLVMGetFirstGlobal(m.get());
                     WRAP_OPTIONAL_RETURN(res, PymGlobalVariable);
                   })
      .def_prop_ro("last_global_variable",
                   [](PymModule &self) -> optional<PymGlobalVariable> {
                     auto res = LLVMGetLastGlobal(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymGlobalVariable);
                   })
      .def_prop_ro("global_variables",
                   [](PymModule &m) {
                     auto res = LLVMGetFirstGlobal(m.get());
                     return PymGlobalVariableIterator(PymGlobalVariable(res));
                   })
      .def_prop_ro("first_global_ifunc",
                   [](PymModule &self) -> optional<PymGlobalIFunc> {
                     auto res = LLVMGetFirstGlobalIFunc(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymGlobalIFunc);
                   })
      .def_prop_ro("last_global_ifunc",
                   [](PymModule &self) -> optional<PymGlobalIFunc> {
                     auto res = LLVMGetLastGlobalIFunc(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymGlobalIFunc);
                   })
      .def_prop_ro("global_ifuncs",
                   [](PymModule &self) {
                     auto res = LLVMGetFirstGlobalIFunc(self.get());
                     return PymGlobalIFuncIterator(PymGlobalIFunc(res));
                   })
      .def_prop_ro("first_global_alias",
                   [](PymModule &self) -> optional<PymGlobalAlias> {
                     auto res = LLVMGetFirstGlobalAlias(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymGlobalAlias);
                   })
      .def_prop_ro("last_global_alias",
                   [](PymModule &self) -> optional<PymGlobalAlias> {
                     auto res = LLVMGetLastGlobalAlias(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymGlobalAlias);
                   })
      .def_prop_ro("global_aliases",
                   [](PymModule &self) {
                     auto res = LLVMGetFirstGlobalAlias(self.get());
                     return PymGlobalAliasIterator(PymGlobalAlias(res));
                   })
      .def_prop_ro("first_named_metadata",
                   [](PymModule &m) -> optional<PymNamedMDNode> {
                     auto res = LLVMGetFirstNamedMetadata(m.get());
                     WRAP_OPTIONAL_RETURN(res, PymNamedMDNode);
                   },
                   "Obtain an iterator to the first NamedMDNode in a Module.")
      .def_prop_ro("last_named_metadata",
                   [](PymModule &m) -> optional<PymNamedMDNode> {
                     auto res = LLVMGetLastNamedMetadata(m.get());
                     WRAP_OPTIONAL_RETURN(res, PymNamedMDNode);
                   },
                   "Obtain an iterator to the last NamedMDNode in a Module.")
      .def_prop_ro("named_metadatas",
                   [](PymModule &m) {
                     auto res = LLVMGetFirstNamedMetadata(m.get());
                     return PymNamedMDNodeIterator(PymNamedMDNode(res));
                   },
                   "Obtain an iterator to the first NamedMDNode in a Module.")
      .def_prop_ro("context",
                   [](PymModule &m) {
                     return PymContext(LLVMGetModuleContext(m.get()));
                   },
                   "Obtain the context to which this module is associated.")
      .def_prop_rw("id",
                   [](PymModule &m) {
                     size_t len;
                     const char *identifier = LLVMGetModuleIdentifier(m.get(), &len);
                     return std::string(identifier, len);
                   },
                   [](PymModule &m, const std::string &id) {
                     return LLVMSetModuleIdentifier(m.get(), id.c_str(),
                                                    id.size());
                   },
                   nb::for_getter("Get the module identifier.\n"
                                  "Origin Function: LLVMSetModuleIdentifier."),
                   nb::for_setter("Set the module identifier.\n"
                                  "Origin Function: LLVMGetModuleIdentifier."))
      .def_prop_rw("source_file_name",
                   [](PymModule &m) {
                     size_t len;
                     const char* sfn = LLVMGetSourceFileName(m.get(), &len);
                     return std::string(sfn, len);
                   },
                   [](PymModule &m, const std::string &name) {
                     return LLVMSetSourceFileName(m.get(), name.c_str(), name.size());
                   },
                   "name"_a = "")
      .def_prop_rw("data_layout",
                   [](PymModule &m) {
                     return LLVMGetDataLayoutStr(m.get());
                   },
                   [](PymModule &m, const std::string &dlstr) {
                     return LLVMSetDataLayout(m.get(), dlstr.c_str());
                   },
                   nb::for_getter("Obtain the data layout for a module."),
                   nb::for_setter("Set the data layout for a module."))
      .def_prop_rw("target",
                   [](PymModule &m) {
                     return LLVMGetTarget(m.get());
                   },
                   [](PymModule &m, const std::string &triple) {
                     return LLVMSetTarget(m.get(), triple.c_str());
                   },
                   nb::for_getter("Obtain the target triple for a module."),
                   nb::for_setter("Set the target triple for a module."))
      .def_prop_rw("inline_asm",
                   [](PymModule &m) {
                     size_t len;
                     const char *iasm = LLVMGetModuleInlineAsm(m.get(), &len);
                     return std::string(iasm, len);
                   },
                   [](PymModule &m, std::string &iasm) {
                     // NOTE LLVMSetModuleInlineAsm is deprecated
                     return LLVMSetModuleInlineAsm2(m.get(), iasm.c_str(), iasm.size());
                   })
      .def_prop_ro("first_function",
                   [](PymModule &m) {
                     return PymFunction(LLVMGetFirstFunction(m.get()));
                   },
                   "Obtain an iterator to the first Function in a Module.")
      .def_prop_ro("last_function",
                   [](PymModule &m) {
                     return PymFunction(LLVMGetLastFunction(m.get()));
                   },
                   "Obtain an iterator to the last Function in a Module.")
      .def_prop_ro("functions",
                   [](PymModule &m) {
                     return PymFunctionIterator(PymFunction(LLVMGetFirstFunction(m.get())));
                   })
      .def("create_function_pass_manager",
           [](PymModule &self) {
             return PymFunctionPassManager(LLVMCreateFunctionPassManagerForModule(self.get()));
           },
           "Constructs a new function-by-function pass pipeline over the module"
           "provider. It does not take ownership of the module provider. This type of"
           "pipeline is suitable for code generation and JIT compilation tasks.")
      .def("create_module_provider",
           [](PymModule &self) {
             return PymModuleProvider(LLVMCreateModuleProviderForExistingModule
                                       (self.get()));
           })
      .def("write_bitcode_to_file",
           [](PymModule &self, const char *path) {
             return LLVMWriteBitcodeToFile(self.get(), path);
           },
           "path"_a)
      // TODO LLVMWriteBitcodeToFD and LLVMWriteBitcodeToFileHandle is currently not
      //  implemented
      .def("write_bitcode_to_memory_buffer",
           [](PymModule &self) {
             return PymMemoryBuffer(LLVMWriteBitcodeToMemoryBuffer(self.get()));
           })
      .def("get_intrinsic_declaration",
           [](PymModule &module, unsigned ID, std::vector<PymType> paramTypes) {
             size_t paramCnt = paramTypes.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, paramTypes, params, paramCnt);
             return PymFunction(LLVMGetIntrinsicDeclaration(module.get(), ID, params.data(),
                                                           paramCnt));
           },
           "id"_a, "param_types"_a,
           "Create or insert the declaration of an intrinsic.  For overloaded intrinsics,"
           "parameter types must be provided to uniquely identify an overload.")
      .def("verify",
           [](PymModule &self, LLVMVerifierFailureAction action) -> optional<std::string> {
             char *outMessage;
             auto res = LLVMVerifyModule(self.get(), action, &outMessage) == 0;
             if (!res) {
               if (outMessage) {
                 std::string errMsg(outMessage);
                 LLVMDisposeMessage(outMessage);
                 return errMsg;
               }
               return "";
             }
             return std::nullopt;
           },
           // NOTE currently no default argument since the stub file generation will emit
           // error
           // "action"_a = LLVMVerifierFailureAction::LLVMAbortProcessAction,
           "action"_a,
           "Verifies that a module is valid, taking the specified action if not.\n"
           "Returns:\n"
           "\tIf success, return None. Otherwise, optionally(based on action) return "
           "a human-readable description if any invalid constructs.")
      .def("add_alias",
           [](PymModule &self, PymType &valueType, unsigned addrSpace, PymValue aliasee,
              const char *name) {
             return PymGlobalAlias(LLVMAddAlias2
                                    (self.get(), valueType.get(), addrSpace,
                                     aliasee.get(), name));
           },
           "value_type"_a, "addr_space"_a, "aliasee"_a, "name"_a = "",
           "Add a GlobalAlias with the given value type, address space and aliasee.")
      .def("get_named_global_alias",
           [](PymModule &self, std::string &name) {
             return PymGlobalAlias(LLVMGetNamedGlobalAlias
                                    (self.get(), name.c_str(), name.size()));
           },
           "name"_a = "",
           "Obtain a GlobalAlias value from by its name.")
      .def("add_global",
           [](PymModule &self, PymType &type, const char *name) {
             return PymGlobalVariable(LLVMAddGlobal(self.get(), type.get(), name));
           },
           "type"_a, "name"_a = "")
      .def("add_global_in_address_space",
           [](PymModule &self, PymType &type, const char *name, unsigned addressSpace) {
             return PymGlobalVariable(LLVMAddGlobalInAddressSpace
                                       (self.get(), type.get(), name, addressSpace));
           },
           "type"_a, "name"_a = "", "address_space"_a = 0)
      .def("get_named_global",
           [](PymModule &self, const char *name) {
             return PymGlobalVariable(LLVMGetNamedGlobal(self.get(), name));
           })
      .def("add_global_indirect_func",
           [](PymModule &self, PymType &type, unsigned addrSpace, PymConstant resolver,
              std::string &name) {
             return PymGlobalIFunc(LLVMAddGlobalIFunc
                                    (self.get(), name.c_str(), name.size(), type.get(),
                                     addrSpace, resolver.get()));
           },
           "type"_a, "addr_space"_a, "resolver"_a, "name"_a = "")
      .def("get_named_global_ifunc",
           [](PymModule &self, std::string &name) -> std::optional<PymGlobalIFunc> {
             auto res = LLVMGetNamedGlobalIFunc(self.get(), name.c_str(), name.size());
             WRAP_OPTIONAL_RETURN(res, PymGlobalIFunc);
           })
      .def("add_function",
           [](PymModule &m, PymTypeFunction &functionTy, std::string &name) {
             return PymFunction(LLVMAddFunction(m.get(), name.c_str(), functionTy.get()));
           },
           "function_type"_a, "name"_a = "",
           "Add a function to a module under a specified name.")
      .def("get_named_function",
           [](PymModule &m, std::string &name) {
             return PymFunction(LLVMGetNamedFunction(m.get(), name.c_str()));
           }, "name"_a = "",
           "Obtain a Function value from a Module by its name.")
      .def("get_named_metadata",
           [](PymModule &m, std::string &name) -> optional<PymNamedMDNode> {
             auto res = LLVMGetNamedMetadata(m.get(), name.c_str(), name.size());
             WRAP_OPTIONAL_RETURN(res, PymNamedMDNode);
           },
           "name"_a = "",
           "Retrieve a NamedMDNode with the given name, returning NULL if no such"
           "node exists.")
      .def("get_or_insert_named_metadata",
           [](PymModule &m, std::string &name) {
             return PymNamedMDNode
                      (LLVMGetOrInsertNamedMetadata
                         (m.get(), name.c_str(), name.size()));
           },
           "name"_a = "",
           "Retrieve a NamedMDNode with the given name, creating a new node if no such"
           "node exists.")
      .def("get_named_metadata_operands_num",
           [](PymModule &m, std::string &name) {
             return LLVMGetNamedMetadataNumOperands(m.get(), name.c_str());
           }, "name"_a = "",
           "Obtain the number of operands for named metadata in a module.")
      .def("get_named_metadata_operands",
           [](PymModule &m, std::string &name) {
             int num = LLVMGetNamedMetadataNumOperands(m.get(), name.c_str());
             std::vector<LLVMValueRef> dest(num);
             LLVMGetNamedMetadataOperands(m.get(), name.c_str(), dest.data());
             WRAP_VECTOR_FROM_DEST_AUTO(PymValue, num, res, dest);
             return res;
           },
           "name"_a = "",
           "Obtain the named metadata operands for a module.\n\n"
           "The passed LLVMValueRef pointer should refer to an array of"
           "LLVMValueRef at least LLVMGetNamedMetadataNumOperands long. This"
           "array will be populated with the LLVMValueRef instances. Each"
           "instance corresponds to a llvm::MDNode.")
      .def("add_named_metadata_operand",
           [](PymModule &m, std::string &name, PymValue &val) {
             return LLVMAddNamedMetadataOperand(m.get(), name.c_str(), val.get());
           },
           "Add an operand to named metadata.")
      .def("clone",
           [](PymModule &m) {
             return PymModule(LLVMCloneModule(m.get()));
           },
           "Return an exact copy of the specified module.")
      .def("copy_module_flags_metadata",
           [](PymModule &m) {
             size_t Len;
             LLVMModuleFlagEntry *flags = LLVMCopyModuleFlagsMetadata(m.get(), &Len);
             auto res = PymModuleFlagEntries(flags, Len);
             return res;
           },
           "Returns the module flags as an array of flag-key-value triples.")
      .def("get_flag",
           [](PymModule &m, const std::string &key) -> optional<PymMetadata> {
             auto res = LLVMGetModuleFlag(m.get(), key.c_str(), key.size());
             WRAP_OPTIONAL_RETURN(res, PymMetadata);
           },
           "key"_a,
           "Return the corresponding value if Key appears in module flags, otherwise"
           "return null.")
      .def("add_flag",
           [](PymModule &m, LLVMModuleFlagBehavior behavior,
              const std::string key, PymMetadata &val) {
             return LLVMAddModuleFlag(m.get(), behavior, key.c_str(), key.size(),
                                      val.get());
           },
           "behavior"_a, "key"_a, "val"_a,
           "Add a module-level flag to the module-level flags metadata if it doesn't"
           "already exist.")
      .def("dump",
           [](PymModule &m) {
             return LLVMDumpModule(m.get());
           },
           "Dump a representation of a module to stderr.")
      .def("print_to_file",
           [](PymModule &m, const std::string &filename) {
             char *errorMessage = nullptr;
             LLVMBool res = LLVMPrintModuleToFile(m.get(), filename.c_str(),
                                                  &errorMessage);
             bool success = res == 0;
             
             if (!success) {
               std::string errorStr(errorMessage);
               LLVMDisposeMessage(errorMessage);
               throw std::runtime_error(errorStr);
             }
           },
           "filename"_a,
           "Print a representation of a module to a file.\n"
           ":raises RuntimeError")
      .def("append_inline_asm",
           [](PymModule &m, std::string &iasm) {
             return LLVMAppendModuleInlineAsm(m.get(), iasm.c_str(), iasm.size());
           })
      .def("get_type_by_name",
           [](PymModule &m, std::string &name) {
             return PymTypeAuto(LLVMGetTypeByName(m.get(), name.c_str()));
           }, "name"_a = "",
           "Deprecated: Use LLVMGetTypeByName2 instead.");

  ModuleFlagEntriesClass
      .def("__repr__",
           [](PymModuleFlagEntries &self) {
             return "<ModuleFlagEntries>";
           })
      .def("get_behavior",
           [](PymModuleFlagEntries &self, unsigned index) {
             // TODO test whether LLVMModuleFlagEntriesGetFlagBehavior will
             // check index is in bound
             return LLVMModuleFlagEntriesGetFlagBehavior(self.get(), index);
           },
           "index"_a,
           "Returns the flag behavior for a module flag entry at a specific index.")
      .def("get_key",
           [](PymModuleFlagEntries &self, unsigned index) {
             size_t len;
             const char *str = LLVMModuleFlagEntriesGetKey(self.get(), index, &len);
             return std::string(str, len);
           },
           "index"_a,
           "Returns the key for a module flag entry at a specific index.")
      .def("get_metadata",
           [](PymModuleFlagEntries &self, unsigned index) {
             // TODO can this return null?
             return PymMetadata(LLVMModuleFlagEntriesGetMetadata(self.get(), index));
           },
           "index"_a,
           "Returns the metadata for a module flag entry at a specific index.");
}
