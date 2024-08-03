#include "miscClasses.h"

#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/optional.h>
#include <fmt/core.h>
#include <optional>
#include <stdexcept>
#include "../_types.h"
#include "../_utils.h"
#include "utils.h"

namespace nb = nanobind;
using namespace nb::literals;

template <typename T>
using optional = std::optional<T>;


void bindOtherClasses(nb::module_ &m) {
  auto ContextClass =
    nb::class_<PyContext, PyLLVMObject<PyContext, LLVMContextRef>>
      (m, "Context",
       "Contexts are execution states for the core LLVM IR system.\n\n"
       "Most types are tied to a context instance. Multiple contexts can"
       "exist simultaneously. A single context is not thread safe. However,"
       "different contexts can execute on different threads simultaneously.");

  auto AttributeClass =
    nb::class_<PyAttribute, PyLLVMObject<PyAttribute, LLVMAttributeRef>>
      (m, "Attribute", "Attribute");
  auto EnumAttributeClass = nb::class_<PyEnumAttribute, PyAttribute>
                              (m, "EnumAttribute", "EnumAttribute");
  auto TypeAttributeClass = nb::class_<PyTypeAttribute, PyAttribute>
                              (m, "TypeAttribute", "TypeAttribute");
  auto StringAttributeClass = nb::class_<PyStringAttribute, PyAttribute>
                                (m, "StringAttribute", "StringAttribute");
  
  
  auto BasicBlockClass =
    nb::class_<PyBasicBlock, PyLLVMObject<PyBasicBlock, LLVMBasicBlockRef>>
      (m, "BasicBlock", "BasicBlock");
  auto DiagnosticInfoClass =
    nb::class_<PyDiagnosticInfo, PyLLVMObject<PyDiagnosticInfo, LLVMDiagnosticInfoRef>>
      (m, "DiagnosticInfo", "DiagnosticInfo");

  auto NamedMDNodeClass =
    nb::class_<PyNamedMDNode, PyLLVMObject<PyNamedMDNode, LLVMNamedMDNodeRef>>
      (m, "NamedMDNode", "NamedMDNode");
  
  auto ModuleClass =
    nb::class_<PyModule, PyLLVMObject<PyModule, LLVMModuleRef>>
      (m, "Module",
       "Modules represent the top-level structure in an LLVM program. An LLVM"
       "module is effectively a translation unit or a collection of translation "
       "units merged together.");

  auto ModuleFlagEntriesClass =
    nb::class_<PyModuleFlagEntries, PyLLVMObject<PyModuleFlagEntries, LLVMModuleFlagEntries>>
      (m, "ModuleFlagEntry", "ModuleFlagEntry");

  
  auto MetadataClass =
    nb::class_<PyMetadata, PyLLVMObject<PyMetadata, LLVMMetadataRef>>
      (m, "Metadata", "Metadata");
  auto MDNodeClass = nb::class_<PyMDNode, PyMetadata>(m, "MDNode", "MDNode");
  auto MDStringClass = nb::class_<PyMDString, PyMetadata>(m, "MDString", "MDString");
  auto ValueAsMetadata = nb::class_<PyValueAsMetadata, PyMetadata>
                           (m, "ValueAsMetadata", "ValueAsMetadata");

  
  auto MetadataEntriesClass =
    nb::class_<PyMetadataEntries, PyLLVMObject<PyMetadataEntries, LLVMValueMetadataEntries>>
      (m, "MetadataEntry", "MetadataEntry");
  
  auto UseClass =
    nb::class_<PyUse, PyLLVMObject<PyUse, LLVMUseRef>>
      (m, "Use", "Use");

  auto IntrinsicClass =
    // nb::class_<PyIntrinsic, PyLLVMObject<PyIntrinsic, unsigned>>
    nb::class_<PyIntrinsic, PyLLVMObject<PyIntrinsic, unsigned>>
      (m, "Intrinsic", "Intrinsic");
  auto OperandBundleClass =
    nb::class_<PyOperandBundle, PyLLVMObject<PyOperandBundle, LLVMOperandBundleRef>>
      (m, "OperandBundle", "OperandBundle");
  auto BuilderClass =
    nb::class_<PyBuilder, PyLLVMObject<PyBuilder, LLVMBuilderRef>>
      (m, "Builder", "Builder");
  auto ModuleProviderClass =
    nb::class_<PyModuleProvider, PyLLVMObject<PyModuleProvider, LLVMModuleProviderRef>>
      (m, "ModuleProvider", "ModuleProvider");
  auto MemoryBufferClass =
    nb::class_<PyMemoryBuffer, PyLLVMObject<PyMemoryBuffer, LLVMMemoryBufferRef>>
      (m, "MemoryBuffer", "MemoryBuffer");
  
  // no need to create PyPassManagerBase binding
  auto PassManagerClass =
    nb::class_<PyPassManager, PyLLVMObject<PyPassManagerBase, LLVMPassManagerRef>>
      (m, "PassManager", "PassManager");
  auto FunctionPassManagerClass = nb::class_<PyFunctionPassManager>
                                    (m, "FunctionPassManager", "FunctionPassManager");

  MetadataClass
      .def("__repr__",
           [](PyMetadata &self) {
             return "<Metadata>";
           })
      .def_static("get_md_kind_id",
                  [](const std::string &name) {
                    return LLVMGetMDKindID(name.c_str(), name.size());
                  },
                  "name"_a = "")
      .def("as_value",
           [](PyMetadata &self, PyContext &c) {
             return PyMetadataAsValue(LLVMMetadataAsValue(c.get(), self.get()));
           },
           "context"_a);


  MDStringClass
      .def("__repr__",
           [](PyMDString &self) {
             return "<MDString>";
           })
      .def("__init__",
           [](PyMDString *mds, PyContext &context, std::string &name) {
             new (mds) PyMDString(LLVMMDStringInContext2
                                    (context.get(), name.c_str(), name.size()));
           },
           "context"_a, "name"_a = "",
           "Create an MDString value from a given string value.")
      .def("as_value",
           [](PyMDString &self, PyContext &c) {
             return PyMDStringValue(LLVMMetadataAsValue(c.get(), self.get()));
           },
           "context"_a);

  MDNodeClass
      .def("__repr__",
           [](PyMDNode &self) {
             return "<MDNode>";
           })
      .def("__init__",
           [](PyMDNode *mdd, PyContext &c, std::vector<PyMetadata> mds) {
             size_t num = mds.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMMetadataRef, mds, raw, num);
             new (mdd) PyMDNode(LLVMMDNodeInContext2(c.get(), raw.data(), num));
           },
           "context"_a, "metadata"_a)
      .def("as_value",
           [](PyMDNode &self, PyContext &c) {
             return PyMDNodeValue(LLVMMetadataAsValue(c.get(), self.get()));
           },
           "context"_a);


  PassManagerClass
      .def("__repr__",
           [](PyPassManager &self) {
             return "<PassManager>";
           })
      .def("__init__",
           [](PyPassManager *pm) {
             new (pm) PyPassManager(LLVMCreatePassManager());
           },
           "Constructs a new whole-module pass pipeline. This type of pipeline is"
           "suitable for link-time optimization and whole-module transformations.")
      .def("run",
           [](PyPassManager &self, PyModule &module) {
             return LLVMRunPassManager(self.get(), module.get()) != 0;
           },
           "module"_a,
           "Initializes, executes on the provided module, and finalizes all of the"
           "passes scheduled in the pass manager. Returns true if any of the passes"
           "modified the module, false otherwise.");

  FunctionPassManagerClass
      .def("__repr__",
           [](PyFunctionPassManager &self) {
             return "<FunctionPassManager>";
           })
      .def("__init__",
           [](PyFunctionPassManager *fpm, PyModule &module) {
             new (fpm) PyFunctionPassManager(LLVMCreateFunctionPassManagerForModule
                                               (module.get()));
           },
           "module"_a,
           "Constructs a new function-by-function pass pipeline over the module"
           "provider. It does not take ownership of the module provider. This type of"
           "pipeline is suitable for code generation and JIT compilation tasks.")
      .def("initialize",
           [](PyFunctionPassManager &self) {
             return LLVMInitializeFunctionPassManager(self.get()) != 0;
           },
           "Initializes all of the function passes scheduled in the function pass"
           "manager. Returns true if any of the passes modified the module, false "
           "otherwise.")
      .def("run",
           [](PyFunctionPassManager &self, PyFunction f) {
             return LLVMRunFunctionPassManager(self.get(), f.get()) != 0;
           },
           "f"_a,
           "Executes all of the function passes scheduled in the function pass manager"
           "on the provided function. Returns true if any of the passes modified the"
           "function, false otherwise.")
      .def("finalize",
           [](PyFunctionPassManager &self) {
             return LLVMFinalizeFunctionPassManager(self.get()) != 0;
           },
           "Finalizes all of the function passes scheduled in the function pass"
           "manager. Returns 1 if any of the passes modified the module, 0 otherwise.");
  

  ModuleProviderClass
      .def("__repr__",
           [](PyModuleProvider &self) {
             return "<ModuleProvider>";
           })
      .def("create_function_pass_manager",
           [](PyModuleProvider &self) {
             return PyFunctionPassManager(LLVMCreateFunctionPassManager(self.get()));
           },
           "Deprecated: Use :func:`Module.create_function_pass_manager` instead.");

  MemoryBufferClass
      .def("__repr__",
           [](PyMemoryBuffer &self) {
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

                    return PyMemoryBuffer(OutMemBuf);
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
                    return PyMemoryBuffer(OutMemBuf);
                  },
                  ":raises RuntimeError")
      .def_static("from_str",
                  [](std::string &inputData, bool RequiresNullTerminator, const char *BufferName) {
                    return PyMemoryBuffer(LLVMCreateMemoryBufferWithMemoryRange
                                            (inputData.c_str(), inputData.size(),
                                             BufferName, RequiresNullTerminator));
                  },
                  "input_data"_a, "requires_null_terminator"_a, "buffer_name"_a = "")
      .def_static("from_str",
                  [](const std::string &inputData, const char *BufferName) ->
                   optional<PyMemoryBuffer>{
                    auto res = LLVMCreateMemoryBufferWithMemoryRangeCopy
                                 (inputData.c_str(), inputData.size(), BufferName);
                    WRAP_OPTIONAL_RETURN(res, PyMemoryBuffer);
                  },
                  "input_data"_a, "buffer_name"_a = "")
      .def_prop_ro("buffer_start",
                   [](PyMemoryBuffer &self) {
                     return LLVMGetBufferStart(self.get());
                   })
      .def_prop_ro("buffer_size",
                   [](PyMemoryBuffer &self) {
                     return LLVMGetBufferSize(self.get());
                   });
  

  BuilderClass
      .def("__repr__",
           [](PyBuilder &self) {
             return "<Builder>";
           })
      .def("__init__",
           [](PyBuilder *b, PyContext &c) {
             new (b) PyBuilder(LLVMCreateBuilderInContext(c.get()));
           })
      .def("__init__",
           [](PyBuilder *b) {
             new (b) PyBuilder(LLVMCreateBuilder());
           })
      .def_prop_ro("insert_block",
                   [](PyBuilder &self) {
                     return PyBasicBlock(LLVMGetInsertBlock(self.get()));
                   })
      // note LLVMSetCurrentDebugLocation is deprecated in favor of
      // LLVMSetCurrentDebugLocation2. Also the LLVMGetCurrentDebugLocation
      .def_prop_rw("current_debug_location",
                   [](PyBuilder &self) {
                     return PyMetadata(LLVMGetCurrentDebugLocation2(self.get()));
                   },
                   [](PyBuilder &self, PyMetadata &loc) {
                     return LLVMSetCurrentDebugLocation2(self.get(), loc.get());
                   })
      .def_prop_rw("default_fp_math_tag",
                   [](PyBuilder &self) {
                     return PyMetadata(LLVMBuilderGetDefaultFPMathTag(self.get()));
                   },
                   [](PyBuilder &self, PyMetadata &fpMathTag) {
                     return LLVMBuilderSetDefaultFPMathTag(self.get(), fpMathTag.get());
                   },
                   nb::for_getter("Get the dafult floating-point math metadata."))
      .def("position",
           [](PyBuilder &self, PyBasicBlock bb, PyInstruction &inst) {
             return LLVMPositionBuilder(self.get(), bb.get(), inst.get());
           },
           "Original Function: LLVMPositionBuilder.")
      .def("position_before",
           [](PyBuilder &self, PyInstruction &inst) {
             return LLVMPositionBuilderBefore(self.get(), inst.get());
           },
           "instruction"_a,
           "Original function: LLVMPositionBuilderBefore.")
      .def("position_at_end",
           [](PyBuilder &self, PyBasicBlock bb) {
             return LLVMPositionBuilderAtEnd(self.get(), bb.get());
           },
           "basicblock"_a,
           "Original function: LLVMPositionBuilderAtEnd.")
      .def("clear_insert_position",
           [](PyBuilder &self) {
             return LLVMClearInsertionPosition(self.get());
           })
      .def("insert",
           [](PyBuilder &self, PyBasicBlock &bb) {
             return LLVMInsertExistingBasicBlockAfterInsertBlock(self.get(), bb.get());
           },
           "basic_block"_a,
           "Insert the given basic block after the insertion point of the given "
           "builder.\n\n"
           "The insertion point must be valid.")
      .def("insert",
           [](PyBuilder &self, PyInstruction &inst) {
             return LLVMInsertIntoBuilder(self.get(), inst.get());
           },
           "instruction"_a)
      .def("insert_with_name",
           [](PyBuilder &self, PyInstruction &inst, const char *name) {
             return LLVMInsertIntoBuilderWithName(self.get(), inst.get(), name);
           },
           "instruction"_a, "name"_a = "")
      .def("destory",
           [](PyBuilder &self) {
             return LLVMDisposeBuilder(self.get());
           },
           "Original Function: LLVMDisposeBuilder.")
      .def("set_instruction_debug_location",
           [](PyBuilder &self, PyInstruction &inst) {
             return LLVMSetInstDebugLocation(self.get(), inst.get());
           },
           "instruction"_a,
           "Attempts to set the debug location for the given instruction using the"
           "current debug location for the given builder.  If the builder has no current"
           "debug location, this function is a no-op.")
      .def("add_metadata_to_instruction",
           [](PyBuilder &self, PyInstruction &inst) {
             return LLVMAddMetadataToInst(self.get(), inst.get());
           },
           "instruction"_a,
           "Adds the metadata registered with the given builder to the given"
           "instruction.")
       /*
        * The following methods are used to build instructions
        * the returned instructions are as specific are possible
        * But if the instruction type doesn't meant to be used in other API functions,
        * then we make it as PyInstruction Type to reduce the Instruction sub-classes number
        */
      .def("ret_void",
           [](PyBuilder &self) {
             return PyReturnInst(LLVMBuildRetVoid(self.get()));
           })
      .def("ret",
           [](PyBuilder &self, PyValue &v) {
             return PyReturnInst(LLVMBuildRet(self.get(), v.get()));
           },
           "value"_a)
      .def("aggregate_ret",
           [](PyBuilder &self, std::vector<PyValue> &values) {
             unsigned num = values.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, values, raw, num);
             return PyReturnInst(LLVMBuildAggregateRet(self.get(), raw.data(), num));
           },
           "values"_a)
      .def("br",
           [](PyBuilder &self, PyBasicBlock &dest) {
             return PyBranchInst(LLVMBuildBr(self.get(), dest.get()));
           },
           "dest"_a)
      .def("cond_br",
           [](PyBuilder &self, PyValue &If, PyBasicBlock &Then,
              PyBasicBlock &Else) {
             return PyBranchInst(LLVMBuildCondBr(self.get(), If.get(),
                                                 Then.get(), Else.get()));
           },
           "If"_a, "Then"_a, "Else"_a)
      .def("switch",
           [](PyBuilder &self, PyValue &value, PyBasicBlock &Else,
              unsigned numCases) {
             return PySwitchInst(LLVMBuildSwitch(self.get(), value.get(), Else.get(), numCases));
           },
           "value"_a, "Else"_a, "num_cases"_a)
      .def("indirect_br",
           [](PyBuilder &self, PyValue &addr, unsigned numDests) {
             return PyIndirectBrInst(LLVMBuildIndirectBr(self.get(), addr.get(), numDests));
           },
           "addr"_a, "num_dests"_a)
      .def("invoke",
           [](PyBuilder &self, PyType &type, PyFunction &fn, std::vector<PyValue> args,
              PyBasicBlock Then, PyBasicBlock Catch, const char *name) {
             unsigned args_num  = args.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);
             auto res = LLVMBuildInvoke2(self.get(), type.get(), fn.get(),
                                         rawArgs.data(), args_num, Then.get(),
                                         Catch.get(), name);
             return PyInvokeInst(res);
           },
           "type"_a, "fn"_a, "args"_a, "Then"_a, "Catch"_a, "name"_a = "",
           "Original Function: LLVMBuildInvoke2.")
      .def("invoke_with_operand_bundles",
           [](PyBuilder &self, PyType &type, PyFunction &fn, std::vector<PyValue> args,
              PyBasicBlock Then, PyBasicBlock Catch,
              std::vector<PyOperandBundle> bundles, const char *name) {
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
             return PyInvokeInst(res);
           },
           "type"_a, "fn"_a, "args"_a, "Then"_a, "Catch"_a, "bundles"_a,
           "name"_a = "")
      .def("unreachable",
           [](PyBuilder &self) {
             return PyInstruction(LLVMBuildUnreachable(self.get()));
           })
      .def("resume",
           [](PyBuilder &self, PyValue &exn) {
             return PyInstruction(LLVMBuildResume(self.get(), exn.get()));
           },
           "exn"_a)
      .def("landing_pad",
           [](PyBuilder &self, PyType &type, PyValue &PersFn, unsigned numClauses,
              const char *name) {
             return PyLandingPadInst(LLVMBuildLandingPad
                                       (self.get(), type.get(), PersFn.get(), numClauses,
                                        name));
           },
           "type"_a, "pers_fn"_a, "num_clauses"_a, "name"_a = "")
      .def("cleanup_ret",
           [](PyBuilder &self, PyValue &catchPad, PyBasicBlock bb) {
             return PyCleanupReturnInst(LLVMBuildCleanupRet(self.get(), catchPad.get(),
                                                            bb.get()));
           },
           "catch_pad"_a, "bb"_a)
      .def("catch_pad",
           [](PyBuilder &self, PyValue &parentPad, std::vector<PyValue> args,
              const char *name) {
             unsigned args_num  = args.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);

             auto res = LLVMBuildCatchPad(self.get(), parentPad.get(), rawArgs.data(),
                                          args_num, name);
             return PyCatchPadInst(res);
           },
           "parent_pad"_a, "args"_a, "name"_a = "")
      .def("cleanup_pad",
           [](PyBuilder &self, PyValue &parentPad, std::vector<PyValue> args,
              const char *name) {
             unsigned args_num  = args.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);

             auto res = LLVMBuildCleanupPad(self.get(), parentPad.get(), rawArgs.data(),
                                            args_num, name);
             return PyInstruction(res);
           },
           "parent_pad"_a, "args"_a, "name"_a = "")
      .def("catch_switch",
           [](PyBuilder &self, PyValue &parentPad, PyBasicBlock &unwindBB,
              unsigned numHandlers, const char *name) {
             auto res = LLVMBuildCatchSwitch(self.get(), parentPad.get(),
                                             unwindBB.get(), numHandlers,
                                             name);
             return PyCatchSwitchInst(res);
           },
           "parent_pad"_a, "unwind_bb"_a, "num_handlers"_a, "name"_a = "")
       /*
         Arithmetic
        */
  BUILDER_BIND_BINARY_OPS
      .def("binop",
           [](PyBuilder &self, LLVMOpcode op, PyValue &lhs, PyValue &rhs,
              const char *name) {
             return PyValueAuto(LLVMBuildBinOp(self.get(), op, lhs.get(), rhs.get(), name));
           })
      .def("neg",
           [](PyBuilder &self, PyValue &v, const char *name) {
             return PyValueAuto(LLVMBuildNeg(self.get(), v.get(), name));
           })
      .def("neg_nsw",
           [](PyBuilder &self, PyValue &v, const char *name) {
             return PyValueAuto(LLVMBuildNSWNeg(self.get(), v.get(), name));
           })
      .def("neg_nuw",
           [](PyBuilder &self, PyValue &v, const char *name) {
             return PyValueAuto(LLVMBuildNUWNeg(self.get(), v.get(), name));
           })
      .def("fneg",
           [](PyBuilder &self, PyValue &v, const char *name) {
             return PyValueAuto(LLVMBuildFNeg(self.get(), v.get(), name));
           })
      .def("not_",
           [](PyBuilder &self, PyValue &v, const char *name) {
             return PyValueAuto(LLVMBuildNot(self.get(), v.get(), name));
           })
      .def("malloc",
           [](PyBuilder &self, PyType &type, const char *name) {
             return PyCallInst(LLVMBuildMalloc(self.get(), type.get(), name));
           },
           "type"_a, "name"_a = "")
      .def("array_malloc",
           [](PyBuilder &self, PyType &type, PyValue &val, const char *name) {
             return PyCallInst(LLVMBuildArrayAlloca(self.get(), type.get(), val.get(), name));
           },
           "type"_a, "value"_a, "name"_a = "")
      .def("memset",
           [](PyBuilder &self, PyValue &ptr, PyValue &val, PyValue &len, unsigned align) {
             auto res = LLVMBuildMemSet(self.get(), ptr.get(), val.get(), len.get(),
                                        align);
             return PyCallInst(res);
           },
           "ptr"_a, "val"_a, "len"_a, "align"_a)
      .def("memcpy",
           [](PyBuilder &self, PyValue &dest, unsigned dstAlign, PyValue &src,
              unsigned srcAlign, PyValue size) {
             auto res = LLVMBuildMemCpy(self.get(), dest.get(), dstAlign, src.get(),
                                        srcAlign, size.get());
             return PyCallInst(res);
           },
           "dest"_a, "dest_align"_a, "src"_a, "src_align"_a, "size"_a,
           "Creates and inserts a memcpy between the specified pointers.")
      .def("mem_move",
           [](PyBuilder &self, PyValue &dest, unsigned dstAlign, PyValue &src,
              unsigned srcAlign, PyValue size) {
             auto res = LLVMBuildMemMove(self.get(), dest.get(), dstAlign, src.get(),
                                         srcAlign, size.get());
             return PyCallInst(res);
           },
           "dest"_a, "dest_align"_a, "src"_a, "src_align"_a, "size"_a,
           "Creates and inserts a memmove between the specified pointers.")
      .def("alloca",
           [](PyBuilder &self, PyType &type, const char *name) {
             return PyAllocaInst(LLVMBuildAlloca(self.get(), type.get(), name));
           },
           "type"_a, "name"_a = "")
      .def("array_alloca",
           [](PyBuilder &self, PyType &type, PyValue &val, const char *name) {
             return PyAllocaInst(LLVMBuildArrayAlloca(self.get(), type.get(),
                                                      val.get(), name));
           },
           "type"_a, "value"_a, "name"_a = "")
      .def("free",
           [](PyBuilder &self, PyValue pointer) {
             return PyCallInst(LLVMBuildFree(self.get(), pointer.get()));
           },
           "pointer"_a)
      .def("load2",
           [](PyBuilder &self, PyType &type, PyValue &pointer, const char *name) {
             return PyLoadInst(LLVMBuildLoad2(self.get(), type.get(), pointer.get(),
                                              name));
           },
           "type"_a, "ptr"_a, "name"_a = "")
      .def("store",
           [](PyBuilder &self, PyValue &val, PyValue &ptr) {
             return PyStoreInst(LLVMBuildStore(self.get(), val.get(), ptr.get()));
           },
           "value"_a, "ptr"_a)
      .def("gep2",
           [](PyBuilder &self, PyType &type, PyValue &ptr, std::vector<PyValue> indices,
              const char *name) {
             unsigned num_indices = indices.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, indices, rawIndices, num_indices);
             return PyValueAuto(LLVMBuildGEP2(self.get(), type.get(), ptr.get(),
                                              rawIndices.data(), num_indices, name));
           },
           "type"_a,  "ptr"_a, "indices"_a, "name"_a = "")
     .def("in_bounds_gep2",
          [](PyBuilder &self, PyType &type, PyValue &ptr, std::vector<PyValue> indices,
             const char *name) {
            unsigned num_indices = indices.size();
            UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, indices, rawIndices, num_indices);
            return PyValueAuto(LLVMBuildGEP2(self.get(), type.get(), ptr.get(),
                                             rawIndices.data(), num_indices, name));
          },
          "type"_a,  "ptr"_a, "indices"_a, "name"_a = "")
     .def("struct_gep2",
          [](PyBuilder &self, PyType &type, PyValue &ptr, unsigned index, const char *name) {
            auto res = LLVMBuildStructGEP2(self.get(), type.get(), ptr.get(),
                                           index, name);
            return PyValueAuto(res);
          },
          "type"_a, "ptr"_a, "index"_a, "name"_a = "")
     .def("global_string",
          [](PyBuilder &self, const char *str, const char *name) {
            return PyValueAuto(LLVMBuildGlobalString(self.get(), str, name));
          })
     .def("global_string_ptr",
          [](PyBuilder &self, const char *str, const char *name) {
            return PyValueAuto(LLVMBuildGlobalStringPtr(self.get(), str, name));
          })
       /* cast start  */
  BUILDER_BIND_CAST_OPS
     .def("cast",
          [](PyBuilder &self, LLVMOpcode opcode, PyValue &value,  PyType &destType,
             const char *name) {
            return PyValueAuto(LLVMBuildCast(self.get(), opcode, value.get(),
                                             destType.get(), name));
          },
          "opcode"_a, "value"_a, "dest_type"_a, "name"_a = "")
     .def("int_cast_2",
          [](PyBuilder &self, PyValue &value, PyType &destType, const char *name){
            return PyValueAuto(LLVMBuildFPCast
                                 (self.get(), value.get(), destType.get(), name));
          },
          "value"_a, "dest_type"_a, "name"_a = "")
     .def("int_cast",
          [](PyBuilder &self, PyValue &value, PyType &destType, const char *name){
            return PyValueAuto(LLVMBuildIntCast
                                 (self.get(), value.get(), destType.get(), name));
          },
          "value"_a, "dest_type"_a, "name"_a = "",
          "Deprecated: This cast is always signed. Use LLVMBuildIntCast2 instead.")
     .def_static("get_cast_opcode",
                 [](PyValue &src, bool srcIsSigned, PyType &destType, bool destIsSigned) {
                   return LLVMGetCastOpcode(src.get(), srcIsSigned, destType.get(),
                                            destIsSigned);
                 },
                 "src"_a, "src_is_signed"_a, "dest_type"_a, "dest_is_signed"_a)
     /* end */
     .def("icmp",
          [](PyBuilder &self, LLVMIntPredicate op, PyValue &lhs, PyValue &rhs,
             const char *name) {
            return PyValueAuto(LLVMBuildICmp(self.get(), op, lhs.get(), rhs.get(),
                                             name));
          },
          "op"_a, "lhs"_a, "rhs"_a, "name"_a = "")
     .def("phi",
          [](PyBuilder &self, PyType &type, const char *name) {
            return PyPHINode(LLVMBuildPhi(self.get(), type.get(), name));
          },
          "type"_a, "name"_a = "")
     .def("call_2",
          [](PyBuilder &self, PyTypeFunction &type, PyFunction &fn, std::vector<PyValue> args,
             const char *name) {
            unsigned args_num = args.size();
            UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);
            return PyCallInst(LLVMBuildCall2
                                (self.get(), type.get(), fn.get(), rawArgs.data(),
                                 args_num, name));
          },
          "fn_type"_a, "fn"_a, "args"_a, "name"_a = "")
     .def("call_with_operand_bundles",
          [](PyBuilder &self, PyTypeFunction type, PyFunction fn,
             std::vector<PyValue> args,
             std::vector<PyOperandBundle> bundles,
             const char *name) {
            unsigned args_num = args.size();
            UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);

            unsigned bundles_num = bundles.size();
            UNWRAP_VECTOR_WRAPPER_CLASS(LLVMOperandBundleRef, bundles, rawBundles,
                                        bundles_num);
            
            return PyCallInst(LLVMBuildCallWithOperandBundles
                                (self.get(), type.get(), fn.get(), rawArgs.data(),
                                 args_num, rawBundles.data(), bundles_num, name));
          })
     .def("select",
          [](PyBuilder &self, PyValue &If, PyValue &Then, PyValue &Else,
             const char *name) {
            return PyCallInst(LLVMBuildSelect
                                (self.get(), If.get(), Then.get(), Else.get(),
                                 name));
          },
          "If"_a, "Then"_a, "Else"_a, "name"_a = "")
     .def("vaarg",
          [](PyBuilder &self, PyValue &list, PyType &type, const char *name) {
            return PyValue(LLVMBuildVAArg(self.get(), list.get(), type.get(),
                                          name));
          },
          "list"_a, "type"_a, "name"_a = "")
     .def("extract_element",
          [](PyBuilder &self, PyValue &vecVal, PyValue &index, const char *name) {
            return PyValueAuto(LLVMBuildExtractElement
                                 (self.get(), vecVal.get(), index.get(), name));
          },
          "vec"_a, "index"_a, "name"_a = "")
     .def("insert_element",
          [](PyBuilder &self, PyValue &vecVal, PyValue &eltVal,
             PyValue &index, const char *name) {
            return PyValueAuto(LLVMBuildInsertElement
                                 (self.get(), vecVal.get(), eltVal.get(),
                                  index.get(), name));
          },
          "vec"_a, "element"_a, "index"_a, "name"_a = "")
    .def("shuffle_vector",
         [](PyBuilder &self, PyValue &v1, PyValue &v2, PyValue &mask, const char *name) {
           return PyShuffleVectorInst(LLVMBuildShuffleVector
                                        (self.get(), v1.get(), v2.get(), mask.get(), name));
         },
         "v1"_a, "v2"_a, "mask"_a, "name"_a = "")
    .def("extract_value",
         [](PyBuilder &self, PyValue &aggVal, unsigned index, const char *name) {
           return PyValueAuto(LLVMBuildExtractValue
                                (self.get(), aggVal.get(), index, name));
         },
         "agg"_a, "index"_a, "name"_a = "")
    .def("insert_value",
         [](PyBuilder &self, PyValue &aggVal, PyValue &eltVal, unsigned index,
            const char *name) {
           return PyValueAuto(LLVMBuildInsertValue
                                (self.get(), aggVal.get(), eltVal.get(), index,
                                 name));
         },
         "agg"_a, "elt"_a, "index"_a, "name"_a = "")
    .def("freeze",
         [](PyBuilder &self, PyValue &val, const char *name) {
           return PyValueAuto(LLVMBuildFreeze(self.get(), val.get(), name));
         },
         "val"_a, "name"_a = "")
    .def("is_null",
         [](PyBuilder &self, PyValue &val, const char *name) {
           return PyValueAuto(LLVMBuildIsNull(self.get(), val.get(), name));
         },
         "value"_a, "name"_a = "")
    .def("is_not_null",
         [](PyBuilder &self, PyValue &val, const char *name) {
           return PyValueAuto(LLVMBuildIsNotNull(self.get(), val.get(), name));
         },
         "value"_a, "name"_a = "")
    .def("ptr_diff_2",
         [](PyBuilder &self, PyType &elemType, PyValue &lhs, PyValue &rhs,
            const char *name) {
           return PyValueAuto(LLVMBuildPtrDiff2
                                (self.get(), elemType.get(), lhs.get(),
                                 rhs.get(), name));
         },
         "elem_type"_a, "lhs"_a, "rhs"_a, "name"_a = "")
    .def("fence",
         [](PyBuilder &self, LLVMAtomicOrdering ordering, bool singleThread,
            const char *name) {
           return PyFenceInst(LLVMBuildFence(self.get(), ordering, singleThread, name));
         },
         "ordering"_a, "singleThread"_a, "name"_a = "")
    .def("atomic_rmw",
         [](PyBuilder &self, LLVMAtomicRMWBinOp op, PyValue &ptr, PyValue val,
            LLVMAtomicOrdering ordering, bool singleThread) {
           auto res = LLVMBuildAtomicRMW(self.get(), op, ptr.get(), val.get(), ordering,
                                         singleThread);
           return PyAtomicRMWInst(res);
         },
         "op"_a, "ptr"_a, "val"_a, "ordering"_a, "singleThread"_a)
    .def("atomic_cmp_xchg",
         [](PyBuilder &self, PyValue &ptr, PyValue &cmp, PyValue &New,
            LLVMAtomicOrdering successOrdering, LLVMAtomicOrdering failureOrdering,
            bool singleThread) {
           auto res = LLVMBuildAtomicCmpXchg(self.get(), ptr.get(), cmp.get(),
                                             New.get(), successOrdering,
                                             failureOrdering, singleThread);
           return PyAtomicCmpXchgInst(res);
         });

  

  BasicBlockClass
      .def("__repr__",
           [](PyBasicBlock &self) {
             return "<BasicBlock>";
           })
      .def("__str__",
           [](PyBasicBlock &self) {
             auto value = LLVMBasicBlockAsValue(self.get());
             return get_value_str(value);
           })
      .def("__init__",
           [](PyBasicBlock *bb, PyContext &c, const char *name) {
             new (bb) PyBasicBlock(LLVMCreateBasicBlockInContext(c.get(), name));
           },
           "context"_a, "name"_a = "",
           "Create a new basic block without inserting it into a function.")
      .def("__init__",
           [](PyBasicBlock *bb, PyContext &c, PyFunction &f, const char *name) {
             new (bb) PyBasicBlock(LLVMAppendBasicBlockInContext
                                     (c.get(), f.get(), name));
           },
           "context"_a, "function"_a, "name"_a = "",
           "Create a new basic block without inserting it into a function.")
      .def("__init__",
           [](PyBasicBlock *bb, PyContext &c, PyBasicBlock &BB, const char *name) {
             new (bb) PyBasicBlock(LLVMInsertBasicBlockInContext
                                     (c.get(), BB.get(), name));
           },
           "context"_a, "bb"_a, "name"_a = "",
           "Insert a basic block in a function before another basic block.\n\n"
           "The function to add to is determined by the function of the"
           "passed basic block.")
      .def("__init__",
           [](PyBasicBlock *bb, PyBasicBlock &BB, const char *name) {
             new (bb) PyBasicBlock(LLVMInsertBasicBlock(BB.get(), name));
           },
           "insert_before_bb"_a, "name"_a = "",
           "Insert a basic block in a function using the global context.")
      .def_prop_ro("name",
                   [](PyBasicBlock &self) {
                     return LLVMGetBasicBlockName(self.get());
                   })
      .def_prop_ro("parent",
                   [](PyBasicBlock &self) {
                     return PyFunction(LLVMGetBasicBlockParent(self.get()));
                   },
                   "Obtain the function to which a basic block belongs.")
      .def_prop_ro("terminator",
                   [](PyBasicBlock &self) -> optional<PyInstruction> {
                     auto res = LLVMGetBasicBlockTerminator(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyInstruction);
                   },
                   "Obtain the terminator instruction for a basic block.\n\n"
                   "If the basic block does not have a terminator (it is not well-formed"
                   "if it doesn't), then NULL is returned.")
      .def_prop_ro("value",
                   [](PyBasicBlock &self) {
                     return PyBasicBlockValue(LLVMBasicBlockAsValue(self.get()));
                   })
      .def_prop_ro("next",
                   [](PyBasicBlock &self) -> optional<PyBasicBlock> {
                     auto res = LLVMGetNextBasicBlock(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyBasicBlock);
                   })
      .def_prop_ro("prev",
                   [](PyBasicBlock &self) -> optional<PyBasicBlock> {
                     auto res = LLVMGetPreviousBasicBlock(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyBasicBlock);
                   })
      .def_prop_ro("first_instruction",
                   [](PyBasicBlock &self) -> optional<PyInstruction> {
                     auto res = LLVMGetFirstInstruction(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyInstruction);
                   })
      .def_prop_ro("last_instruction",
                   [](PyBasicBlock &self) -> optional<PyInstruction> {
                     auto res = LLVMGetLastInstruction(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyInstruction);
                   })
      .def_prop_ro("instructions",
                   [](PyBasicBlock &self) {
                     auto res = LLVMGetFirstInstruction(self.get());
                     return PyInstructionIterator(PyInstruction(res));
                   })
      .def("create_and_insert_before",
           [](PyBasicBlock &self, const char *name) {
             return PyBasicBlock(LLVMInsertBasicBlock(self.get(), name));
           },
           "Insert a basic block in a function using the global context.")
      .def("destroy", // TODO test
           [](PyBasicBlock &self) {
             return LLVMDeleteBasicBlock(self.get());
           },
           "Remove a basic block from a function and delete it.\n\n"
           "This deletes the basic block from its containing function and deletes"
           "the basic block itself.")
      .def("remove_from_parent",
           [](PyBasicBlock &self) {
             return LLVMRemoveBasicBlockFromParent(self.get());
           },
           "Remove a basic block from a function.\n\n"
           "This deletes the basic block from its containing function but keep"
           "the basic block alive.")
      .def("move_before",
           [](PyBasicBlock &self, PyBasicBlock posBB) {
             return LLVMMoveBasicBlockBefore(self.get(), posBB.get());
           },
           "pos"_a,
           "Move a basic block to before another one.")
      .def("move_after",
           [](PyBasicBlock &self, PyBasicBlock posBB) {
             return LLVMMoveBasicBlockAfter(self.get(), posBB.get());
           },
           "pos",
           "Move a basic block to after another one.");


  OperandBundleClass
      .def("__repr__",
           [](PyOperandBundle &self) {
             return "<OperandBundle>";
           })
      .def("__init__",
           [](PyOperandBundle *ob, std::string &tag, std::vector<PyValue> args) {
             unsigned arg_num = args.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, raw_args, arg_num);
             new (ob) PyOperandBundle(LLVMCreateOperandBundle
                                        (tag.c_str(), tag.size(), raw_args.data(),
                                         arg_num));
           })
      .def_prop_ro("tag",
                   [](PyOperandBundle &self) {
                     size_t len;
                     auto tag = LLVMGetOperandBundleTag(self.get(), &len);
                     return std::string(tag, len);
                   })
      .def_prop_ro("operands_num",
                   [](PyOperandBundle &self) {
                     return LLVMGetNumOperandBundleArgs(self.get());
                   })
      .def("get_operand_at",
           [](PyOperandBundle &self, unsigned index) {
             return PyValueAuto(LLVMGetOperandBundleArgAtIndex(self.get(), index));
           });

    

  IntrinsicClass
      .def("__repr__",
           [](PyIntrinsic &self) {
             return "<Intrinsic>";
           })
       .def("__repr__",
            [](PyIntrinsic &self) {
              size_t len;
              auto raw_name = LLVMIntrinsicGetName(self.get(), &len);
              auto name = std::string(raw_name, len);
              return fmt::format("<Instrinsic id={} name={}>", self.get(), name);
            })
       .def("__bool__", // override default behavior
            [](PyIntrinsic &self) { return true; })
       .def_static("lookup",
                   [](std::string &name) {
                     return PyIntrinsic(LLVMLookupIntrinsicID(name.c_str(), name.size()));
                   },
                   "name"_a = "",
                   "Obtain the intrinsic ID number which matches the given function name.")
      .def("get_type",
           [](PyIntrinsic &self, PyContext &context, std::vector<PyType> paramTypes) {
             size_t paramCnt = paramTypes.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, paramTypes, params, paramCnt);
             return PyTypeFunction(LLVMIntrinsicGetType
                                     (context.get(), self.get(), params.data(),
                                      paramCnt));

           },
           "context"_a, "param_types"_a,
           "Retrieves the type of an intrinsic.  For overloaded intrinsics,"
           " parameter types must be provided to uniquely identify an overload.")
      .def_prop_ro("id",
                   [](PyIntrinsic &self) {
                     return self.get();
                   })
      .def_prop_ro("name",
                   [](PyIntrinsic &self) {
                     size_t len;
                     auto name = LLVMIntrinsicGetName(self.get(), &len);
                     return std::string(name, len);
                   },
                   "Retrieves the name of an intrinsic.")
      .def_prop_ro("is_overloaded",
                   [](PyIntrinsic &self) {
                     return LLVMIntrinsicIsOverloaded(self.get()) != 0;
                   })
      // note LLVMIntrinsicCopyOverloadedName is deprecated
      .def("copy_overloaded_name",
           [](PyIntrinsic &self, std::vector<PyType> paramTypes) {
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
           [](PyIntrinsic &self, PyModule &module, std::vector<PyType> paramTypes) {
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
           [](PyMetadataEntries &self) {
             return "<MetadataEntries>";
           })
      .def("get_kind",
           [](PyMetadataEntries &self, unsigned index) {
             return LLVMValueMetadataEntriesGetKind(self.get(), index);
           },
           "index"_a,
           "Returns the kind of a value metadata entry at a specific index.")
      .def("get_metadata",
           [](PyMetadataEntries &self, unsigned index) {
             return PyMetadata(LLVMValueMetadataEntriesGetMetadata(self.get(), index));
           },
           "index"_a,
           "Returns the underlying metadata node of a value metadata entry at a"
           "specific index.");

  UseClass
      .def("__repr__",
           [](PyUse &self) {
             return "<Use>";
           })
      .def_prop_ro("next",
                   [](PyUse &u) -> optional<PyUse> {
                     auto res = LLVMGetNextUse(u.get());
                     WRAP_OPTIONAL_RETURN(res, PyUse);
                   },
                   "Obtain the next use of a value.\n\n"
                   "This effectively advances the iterator. It returns NULL if you are on"
                   "the final use and no more are available.")
      .def_prop_ro("user",
                   [](PyUse &u) { return PyUser(LLVMGetUser(u.get())); },
                   "Obtain the user value for a user.\n",
                   "The returned value corresponds to a llvm::User type.")
      .def_prop_ro("used_value",
                   [](PyUse &u) {
                     return PyValueAuto(LLVMGetUsedValue(u.get()));
                   });
  
  
  AttributeClass
      .def("__repr__",
           [](PyAttribute &self) {
             return "<Attribute>";
           })
      .def_prop_ro("is_enum",
                   [](PyAttribute &attr) {
                     return LLVMIsEnumAttribute(attr.get()) != 0;
                   })
      .def_prop_ro("is_string",
                   [](PyAttribute &attr) {
                     return LLVMIsStringAttribute(attr.get()) != 0;
                   })
      .def_prop_ro("is_type",
                   [](PyAttribute &attr) {
                     return LLVMIsTypeAttribute(attr.get()) != 0;
                   });
  
  EnumAttributeClass
      .def("__repr__",
           [](PyEnumAttribute &self) {
             return "<EnumAttribute>";
           })
      .def("__init__",
           [](PyEnumAttribute *t, PyContext &c, unsigned kindID, uint64_t val) {
             new (t) PyEnumAttribute(LLVMCreateEnumAttribute(c.get(), kindID, val));
           },
           "context"_a, "kind_id"_a, "val"_a)
      .def("__repr__",
           [](PyEnumAttribute &self) {
             auto kind = LLVMGetEnumAttributeKind(self.get());
             auto value = LLVMGetEnumAttributeValue(self.get());
             return fmt::format("<EnumAttribute kind={} value={}>", kind, value);
           })
      .def_prop_ro("kind",
                   [](PyEnumAttribute &attr) {
                     return LLVMGetEnumAttributeKind(attr.get());
                   }, "attr"_a)
      .def_prop_ro("value",
                   [](PyEnumAttribute &attr) {
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
           [](PyTypeAttribute &self) {
             return "<TypeAttribute>";
           })
      .def("__init__",
           [](PyTypeAttribute *t, PyContext &context, unsigned kind_id, PyType &type) {
             new (t) PyTypeAttribute(LLVMCreateTypeAttribute
                                       (context.get(), kind_id, type.get()));
           },
           "context"_a, "kind_id"_a, "type"_a)
      .def("__repr__",
           [](PyTypeAttribute &self) {
             auto value = LLVMGetTypeAttributeValue(self.get());
             auto type_kind = LLVMGetTypeKind(value);
             return fmt::format("<TypeAttribute value={}>", get_repr_str(type_kind));
           })
      .def_prop_ro("value",
                   [](PyTypeAttribute &ta){
                     return PyTypeAuto(LLVMGetTypeAttributeValue(ta.get()));
                   },
                   "Get the type attribute's value.");

  StringAttributeClass
      .def("__repr__",
           [](PyStringAttribute &self) {
             return "<StringAttribute>";
           })
      .def("__init__",
           [](PyStringAttribute *t, PyContext &c, const std::string &kind, const std::string &value) {
             auto raw = LLVMCreateStringAttribute(c.get(),
                                                  kind.c_str(), kind.size(),
                                                  value.c_str(), value.size());
             new (t) PyStringAttribute(raw);
           },
           "context"_a, "kind"_a, "value"_a)
      .def("__repr__",
           [](PyStringAttribute &self) {
             unsigned kind_length;
             const char *raw_kind = LLVMGetStringAttributeKind(self.get(), &kind_length);
             auto kind = std::string(raw_kind, kind_length);
             
             unsigned value_length;
             const char *raw_value = LLVMGetStringAttributeValue(self.get(), &value_length);
             auto value = std::string(raw_value, value_length);

             return fmt::format("<TypeAttribute kind={} value={}>", kind, value);
           })
      .def_prop_ro("kind",
                   [](PyStringAttribute &ta) {
                     unsigned length;
                     const char *kind = LLVMGetStringAttributeKind(ta.get(), &length);
                     return std::string(kind, length);
                   })
      .def_prop_ro("value",
                   [](PyStringAttribute &ta) {
                     unsigned length;
                     const char *value = LLVMGetStringAttributeValue(ta.get(), &length);
                     return std::string(value, length);
                   },
                   "Get the type attribute's value.");
  

  DiagnosticInfoClass
      .def("__repr__",
           [](PyDiagnosticInfo &self) {
             return "<DiagnosticInfo>";
           })
      // .def(nb::init<>()) // NOTE currently no constructor function for python, we'll see
      .def_prop_ro("description",
                   [](PyDiagnosticInfo &d) {
                     char *diagInfoDesc = LLVMGetDiagInfoDescription(d.get());
                     std::string diagInfoDescCopy(diagInfoDesc);
                     LLVMDisposeMessage(diagInfoDesc);
                     return diagInfoDescCopy;
                   },
                   "Return a string representation of the DiagnosticInfo.")
      .def_prop_ro("severity",
                   [](PyDiagnosticInfo &d) {
                     return LLVMGetDiagInfoSeverity(d.get());
                   },
                   "Return an enum LLVMDiagnosticSeverity.");

  
  NamedMDNodeClass
      .def("__repr__",
           [](PyNamedMDNode &self) {
             return "<NamedMDNode>";
           })
      .def_prop_ro("next",
                   [](PyNamedMDNode &nmdn) -> optional<PyNamedMDNode>{
                     auto res = LLVMGetNextNamedMetadata(nmdn.get());
                     WRAP_OPTIONAL_RETURN(res, PyNamedMDNode);
                   },
                   "Advance a NamedMDNode iterator to the next NamedMDNode.\n\n"
                   "Returns NULL if the iterator was already at the end and there"
                   " are no more named metadata nodes.")
      .def_prop_ro("prev",
                   [](PyNamedMDNode &nmdn) -> optional<PyNamedMDNode> {
                     auto res = LLVMGetPreviousNamedMetadata(nmdn.get());
                     WRAP_OPTIONAL_RETURN(res, PyNamedMDNode);
                   },
                   "Decrement a NamedMDNode iterator to the previous NamedMDNode.\n\n"
                   "Returns NULL if the iterator was already at the beginning and there are"
                   "no previous named metadata nodes.")
      .def_prop_ro("name",
                   [](PyNamedMDNode &nmdn) {
                     size_t len;
                     const char *name =  LLVMGetNamedMetadataName(nmdn.get(), &len);
                     return std::string(name, len);
                   },
                   "Retrieve the name of a NamedMDNode.");

  
  ContextClass
      .def("__repr__",
           [](PyContext &self) {
             return "<Context>";
           })
      .def(nb::init<>(), "Create a new context.")
      .def("__enter__",
           [](PyContext &self) {
             return &self;
           })
      .def("__exit__",
           [](PyContext &self, nb::args args, nb::kwargs kwargs) {})
      .def_static("get_global_context", &PyContext::getGlobalContext,
                  "Obtain the global context instance.")
      .def_prop_ro("diagnostic_context",
                   [](PyContext &c) {
                     // FIXME The function cannot work correctly (always None) since
                     // `LLVMContextSetDiagnosticHandler` cannot, which set
                     // the diagnostic context
                     return LLVMContextGetDiagnosticContext(c.get());
                   },
                   "Get the diagnostic context of this context.")
      .def_prop_rw("should_discard_value_names", // TODO convert LLVMBool to bool
                   [](PyContext &c) -> bool { return LLVMContextShouldDiscardValueNames(c.get()) != 0; },
                   [](PyContext &c, bool discard) {
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
           [](PyContext &c, LLVMDiagnosticHandler handler, void * diagnosticContext){
             return LLVMContextSetDiagnosticHandler(c.get(), handler, diagnosticContext);
           },
           "handler"_a, "diagnostic_context"_a,
           "Set the diagnostic handler for this context.")
      // .def("get_diagnostic_handler", FIXME
      //      [](PyContext &c)  { return LLVMContextGetDiagnosticHandler(c.get()); },
      //      "Get the diagnostic handler of this context.")
      .def("set_yield_callback", // FIXME
           [](PyContext &c, LLVMYieldCallback callback, void *opaqueHandle){
             return LLVMContextSetYieldCallback(c.get(), callback, opaqueHandle);
           },
           "callback"_a, "opaque_handle"_a,
           "Set the yield callback function for this context.")
      .def("parse_ir",
           [](PyContext &self, PyMemoryBuffer &memBuf) {
             try {
               auto res = parseIR(self.get(), memBuf.get());
               memBuf.reset(); // We Cannot reuse the memory buffer again
               return res;
             } catch (const std::exception& ex) {
               memBuf.reset();
               throw ex;
             }
           },
           "memory_buffer"_a,
           "Read LLVM IR from a memory buffer and convert it into an in-memory Module"
           "object.\n\n"
           ":raises RuntimeError\n"
           "NOTE that you cannot use passed-in memory_buffer after this operation.")
      .def("create_builder",
           [](PyContext &self) {
             return PyBuilder(LLVMCreateBuilderInContext(self.get()));
           })
      .def("create_basic_block",
           [](PyContext &self, const char *name) {
             return PyBasicBlock(LLVMCreateBasicBlockInContext
                                   (self.get(), name));
           },
           "name"_a = "",
           "Create a new basic block without inserting it into a function.")
      .def("append_basic_block",
           [](PyContext &self, PyFunction fn, const char *name) {
             return PyBasicBlock(LLVMAppendBasicBlockInContext
                                   (self.get(), fn.get(), name));
           },
           "fn"_a, "name"_a = "",
           "Append a basic block to the end of a function.")
      .def("insert_basic_block",
           [](PyContext &self, PyBasicBlock bb, const char *name) {
             return PyBasicBlock(LLVMInsertBasicBlockInContext
                                   (self.get(), bb.get(), name));
           },
           "bb"_a, "name"_a = "",
           "Insert a basic block in a function before another basic block.\n\n"
           "The function to add to is determined by the function of the"
           "passed basic block.")
      .def("get_md_kind_id",
           [](PyContext &c, const std::string &name) {
             return LLVMGetMDKindIDInContext(c.get(), name.c_str(), name.size());
           },
           "name"_a = "")
      .def("create_enum_attribute",
           [](PyContext &c, unsigned kindID, uint64_t val) {
             return PyEnumAttribute(LLVMCreateEnumAttribute(c.get(), kindID, val));
           },
           "kind_id"_a, "val"_a,
           "Create an enum attribute.")
      .def("create_type_attribute",
           [](PyContext &context, unsigned kind_id, PyType &type) {
             return PyTypeAttribute(LLVMCreateTypeAttribute
                                      (context.get(), kind_id, type.get()));
           },
           "kind_id"_a, "type"_a,
           "Create a type attribute")
      .def("create_string_attribute",
           [](PyContext &c, const std::string &kind, const std::string &value) {
             auto raw = LLVMCreateStringAttribute(c.get(),
                                                  kind.c_str(), kind.size(),
                                                  value.c_str(), value.size());
             return PyStringAttribute(raw);
           })
      .def("get_type_by_name_2", // TODO also create one in PyType static method
           [](PyContext &c, const char *name) -> optional<PyType*> {
             auto res = LLVMGetTypeByName2(c.get(), name);
             WRAP_OPTIONAL_RETURN(res, PyTypeAuto);
           },
           "name"_a = "")
      .def("create_md_string_2",
           [](PyContext &self, std::string &name) {
             return PyMDString(LLVMMDStringInContext2(self.get(), name.c_str(), name.size()));
           },
           "name"_a = "",
           "Create an MDString value from a given string value.")
      .def("create_md_node_2",
           [](PyContext &self, std::vector<PyMetadata> mds) {
             size_t num = mds.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMMetadataRef, mds, raw, num);
             return PyMDNode(LLVMMDNodeInContext2(self.get(), raw.data(), num));
           },
           "metadata"_a)
      .def("get_metadata_as_value",
           [](PyContext &self, PyMetadata &md) {
             return getMoreSpcMetadataAsValue(LLVMMetadataAsValue
                                                (self.get(), md.get()));
           });

  

  ModuleClass
      .def(nb::init<const std::string &>(), "name"_a = "")
      .def("__repr__",
           [](PyModule &self) {
             size_t len;
             const char *raw_identifier = LLVMGetModuleIdentifier(self.get(), &len);
             auto id = std::string(raw_identifier, len);
             return fmt::format("<Module id={}>", id);
           })
      .def("__str__",
           [](PyModule &m) {
             char *str = LLVMPrintModuleToString(m.get());
             std::string strCopy(str);
             LLVMDisposeMessage(str);
             return strCopy;
           },
           "Return a string representation of the module")
      .def("__enter__",
           [](PyModule &self) {
             return &self;
           })
      .def("__exit__", [](PyModule &self, nb::args args, nb::kwargs kwargs) {})
      .def_prop_ro("first_global_variable",
                   [](PyModule &m) -> optional<PyGlobalVariable> {
                     auto res = LLVMGetFirstGlobal(m.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalVariable);
                   })
      .def_prop_ro("last_global_variable",
                   [](PyModule &self) -> optional<PyGlobalVariable> {
                     auto res = LLVMGetLastGlobal(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalVariable);
                   })
      .def_prop_ro("global_variables",
                   [](PyModule &m) {
                     auto res = LLVMGetFirstGlobal(m.get());
                     return PyGlobalVariableIterator(PyGlobalVariable(res));
                   })
      .def_prop_ro("first_global_ifunc",
                   [](PyModule &self) -> optional<PyGlobalIFunc> {
                     auto res = LLVMGetFirstGlobalIFunc(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalIFunc);
                   })
      .def_prop_ro("last_global_ifunc",
                   [](PyModule &self) -> optional<PyGlobalIFunc> {
                     auto res = LLVMGetLastGlobalIFunc(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalIFunc);
                   })
      .def_prop_ro("global_ifuncs",
                   [](PyModule &self) {
                     auto res = LLVMGetFirstGlobalIFunc(self.get());
                     return PyGlobalIFuncIterator(PyGlobalIFunc(res));
                   })
      .def_prop_ro("first_global_alias",
                   [](PyModule &self) -> optional<PyGlobalAlias> {
                     auto res = LLVMGetFirstGlobalAlias(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalAlias);
                   })
      .def_prop_ro("last_global_alias",
                   [](PyModule &self) -> optional<PyGlobalAlias> {
                     auto res = LLVMGetLastGlobalAlias(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalAlias);
                   })
      .def_prop_ro("global_aliases",
                   [](PyModule &self) {
                     auto res = LLVMGetFirstGlobalAlias(self.get());
                     return PyGlobalAliasIterator(PyGlobalAlias(res));
                   })
      .def_prop_ro("first_named_metadata",
                   [](PyModule &m) -> optional<PyNamedMDNode> {
                     auto res = LLVMGetFirstNamedMetadata(m.get());
                     WRAP_OPTIONAL_RETURN(res, PyNamedMDNode);
                   },
                   "Obtain an iterator to the first NamedMDNode in a Module.")
      .def_prop_ro("last_named_metadata",
                   [](PyModule &m) -> optional<PyNamedMDNode> {
                     auto res = LLVMGetLastNamedMetadata(m.get());
                     WRAP_OPTIONAL_RETURN(res, PyNamedMDNode);
                   },
                   "Obtain an iterator to the last NamedMDNode in a Module.")
      .def_prop_ro("named_metadatas",
                   [](PyModule &m) {
                     auto res = LLVMGetFirstNamedMetadata(m.get());
                     return PyNamedMDNodeIterator(PyNamedMDNode(res));
                   },
                   "Obtain an iterator to the first NamedMDNode in a Module.")
      .def_prop_ro("context",
                   [](PyModule &m) {
                     return PyContext(LLVMGetModuleContext(m.get()));
                   },
                   "Obtain the context to which this module is associated.")
      .def_prop_rw("id",
                   [](PyModule &m) {
                     size_t len;
                     const char *identifier = LLVMGetModuleIdentifier(m.get(), &len);
                     return std::string(identifier, len);
                   },
                   [](PyModule &m, const std::string &id) {
                     return LLVMSetModuleIdentifier(m.get(), id.c_str(),
                                                    id.size());
                   },
                   nb::for_getter("Get the module identifier.\n"
                                  "Origin Function: LLVMSetModuleIdentifier."),
                   nb::for_setter("Set the module identifier.\n"
                                  "Origin Function: LLVMGetModuleIdentifier."))
      .def_prop_rw("source_file_name",
                   [](PyModule &m) {
                     size_t len;
                     const char* sfn = LLVMGetSourceFileName(m.get(), &len);
                     return std::string(sfn, len);
                   },
                   [](PyModule &m, const std::string &name) {
                     return LLVMSetSourceFileName(m.get(), name.c_str(), name.size());
                   },
                   "name"_a = "")
      .def_prop_rw("data_layout",
                   [](PyModule &m) {
                     return LLVMGetDataLayoutStr(m.get());
                   },
                   [](PyModule &m, const std::string &dlstr) {
                     return LLVMSetDataLayout(m.get(), dlstr.c_str());
                   },
                   nb::for_getter("Obtain the data layout for a module."),
                   nb::for_setter("Set the data layout for a module."))
      .def_prop_rw("target",
                   [](PyModule &m) {
                     return LLVMGetTarget(m.get());
                   },
                   [](PyModule &m, const std::string &triple) {
                     return LLVMSetTarget(m.get(), triple.c_str());
                   },
                   nb::for_getter("Obtain the target triple for a module."),
                   nb::for_setter("Set the target triple for a module."))
      .def_prop_rw("inline_asm",
                   [](PyModule &m) {
                     size_t len;
                     const char *iasm = LLVMGetModuleInlineAsm(m.get(), &len);
                     return std::string(iasm, len);
                   },
                   [](PyModule &m, std::string &iasm) {
                     // NOTE LLVMSetModuleInlineAsm is deprecated
                     return LLVMSetModuleInlineAsm2(m.get(), iasm.c_str(), iasm.size());
                   })
      .def_prop_ro("first_function",
                   [](PyModule &m) {
                     return PyFunction(LLVMGetFirstFunction(m.get()));
                   },
                   "Obtain an iterator to the first Function in a Module.")
      .def_prop_ro("last_function",
                   [](PyModule &m) {
                     return PyFunction(LLVMGetLastFunction(m.get()));
                   },
                   "Obtain an iterator to the last Function in a Module.")
      .def_prop_ro("functions",
                   [](PyModule &m) {
                     return PyFunctionIterator(PyFunction(LLVMGetFirstFunction(m.get())));
                   })
      .def("create_function_pass_manager",
           [](PyModule &self) {
             return PyFunctionPassManager(LLVMCreateFunctionPassManagerForModule(self.get()));
           },
           "Constructs a new function-by-function pass pipeline over the module"
           "provider. It does not take ownership of the module provider. This type of"
           "pipeline is suitable for code generation and JIT compilation tasks.")
      .def("create_module_provider",
           [](PyModule &self) {
             return PyModuleProvider(LLVMCreateModuleProviderForExistingModule
                                       (self.get()));
           })
      .def("get_intrinsic_declaration",
           [](PyModule &module, unsigned ID, std::vector<PyType> paramTypes) {
             size_t paramCnt = paramTypes.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, paramTypes, params, paramCnt);
             return PyFunction(LLVMGetIntrinsicDeclaration(module.get(), ID, params.data(),
                                                           paramCnt));
           },
           "id"_a, "param_types"_a,
           "Create or insert the declaration of an intrinsic.  For overloaded intrinsics,"
           "parameter types must be provided to uniquely identify an overload.")
      .def("add_alias",
           [](PyModule &self, PyType &valueType, unsigned addrSpace, PyValue aliasee,
              const char *name) {
             return PyGlobalAlias(LLVMAddAlias2
                                    (self.get(), valueType.get(), addrSpace,
                                     aliasee.get(), name));
           },
           "value_type"_a, "addr_space"_a, "aliasee"_a, "name"_a = "",
           "Add a GlobalAlias with the given value type, address space and aliasee.")
      .def("get_named_global_alias",
           [](PyModule &self, std::string &name) {
             return PyGlobalAlias(LLVMGetNamedGlobalAlias
                                    (self.get(), name.c_str(), name.size()));
           },
           "name"_a = "",
           "Obtain a GlobalAlias value from by its name.")
      .def("add_global",
           [](PyModule &self, PyType &type, const char *name) {
             return PyGlobalVariable(LLVMAddGlobal(self.get(), type.get(), name));
           },
           "type"_a, "name"_a = "")
      .def("add_global_in_address_space",
           [](PyModule &self, PyType &type, const char *name, unsigned addressSpace) {
             return PyGlobalVariable(LLVMAddGlobalInAddressSpace
                                       (self.get(), type.get(), name, addressSpace));
           },
           "type"_a, "name"_a = "", "address_space"_a = 0)
      .def("get_named_global",
           [](PyModule &self, const char *name) {
             return PyGlobalVariable(LLVMGetNamedGlobal(self.get(), name));
           })
      .def("add_global_indirect_func",
           [](PyModule &self, PyType &type, unsigned addrSpace, PyConstant resolver,
              std::string &name) {
             return PyGlobalIFunc(LLVMAddGlobalIFunc
                                    (self.get(), name.c_str(), name.size(), type.get(),
                                     addrSpace, resolver.get()));
           },
           "type"_a, "addr_space"_a, "resolver"_a, "name"_a = "")
      .def("get_named_global_ifunc",
           [](PyModule &self, std::string &name) -> std::optional<PyGlobalIFunc> {
             auto res = LLVMGetNamedGlobalIFunc(self.get(), name.c_str(), name.size());
             WRAP_OPTIONAL_RETURN(res, PyGlobalIFunc);
           })
      .def("add_function",
           [](PyModule &m, PyTypeFunction &functionTy, std::string &name) {
             return PyFunction(LLVMAddFunction(m.get(), name.c_str(), functionTy.get()));
           },
           "function_type"_a, "name"_a = "",
           "Add a function to a module under a specified name.")
      .def("get_named_function",
           [](PyModule &m, std::string &name) {
             return PyFunction(LLVMGetNamedFunction(m.get(), name.c_str()));
           }, "name"_a = "",
           "Obtain a Function value from a Module by its name.")
      .def("get_named_metadata",
           [](PyModule &m, std::string &name) -> optional<PyNamedMDNode> {
             auto res = LLVMGetNamedMetadata(m.get(), name.c_str(), name.size());
             WRAP_OPTIONAL_RETURN(res, PyNamedMDNode);
           },
           "name"_a = "",
           "Retrieve a NamedMDNode with the given name, returning NULL if no such"
           "node exists.")
      .def("get_or_insert_named_metadata",
           [](PyModule &m, std::string &name) {
             return PyNamedMDNode
                      (LLVMGetOrInsertNamedMetadata
                         (m.get(), name.c_str(), name.size()));
           },
           "name"_a = "",
           "Retrieve a NamedMDNode with the given name, creating a new node if no such"
           "node exists.")
      .def("get_named_metadata_operands_num",
           [](PyModule &m, std::string &name) {
             return LLVMGetNamedMetadataNumOperands(m.get(), name.c_str());
           }, "name"_a = "",
           "Obtain the number of operands for named metadata in a module.")
      .def("get_named_metadata_operands",
           [](PyModule &m, std::string &name) {
             int num = LLVMGetNamedMetadataNumOperands(m.get(), name.c_str());
             std::vector<LLVMValueRef> dest(num);
             LLVMGetNamedMetadataOperands(m.get(), name.c_str(), dest.data());
             WRAP_VECTOR_FROM_DEST_AUTO(PyValue, num, res, dest);
             return res;
           },
           "name"_a = "",
           "Obtain the named metadata operands for a module.\n\n"
           "The passed LLVMValueRef pointer should refer to an array of"
           "LLVMValueRef at least LLVMGetNamedMetadataNumOperands long. This"
           "array will be populated with the LLVMValueRef instances. Each"
           "instance corresponds to a llvm::MDNode.")
      .def("add_named_metadata_operand",
           [](PyModule &m, std::string &name, PyValue &val) {
             return LLVMAddNamedMetadataOperand(m.get(), name.c_str(), val.get());
           },
           "Add an operand to named metadata.")
      .def("clone",
           [](PyModule &m) {
             return PyModule(LLVMCloneModule(m.get()));
           },
           "Return an exact copy of the specified module.")
      .def("copy_module_flags_metadata",
           [](PyModule &m) {
             size_t Len;
             LLVMModuleFlagEntry *flags = LLVMCopyModuleFlagsMetadata(m.get(), &Len);
             auto res = PyModuleFlagEntries(flags, Len);
             return res;
           },
           "Returns the module flags as an array of flag-key-value triples.")
      .def("get_flag",
           [](PyModule &m, const std::string &key) -> optional<PyMetadata> {
             auto res = LLVMGetModuleFlag(m.get(), key.c_str(), key.size());
             WRAP_OPTIONAL_RETURN(res, PyMetadata);
           },
           "key"_a,
           "Return the corresponding value if Key appears in module flags, otherwise"
           "return null.")
      .def("add_flag",
           [](PyModule &m, LLVMModuleFlagBehavior behavior,
              const std::string key, PyMetadata &val) {
             return LLVMAddModuleFlag(m.get(), behavior, key.c_str(), key.size(),
                                      val.get());
           },
           "behavior"_a, "key"_a, "val"_a,
           "Add a module-level flag to the module-level flags metadata if it doesn't"
           "already exist.")
      .def("dump",
           [](PyModule &m) {
             return LLVMDumpModule(m.get());
           },
           "Dump a representation of a module to stderr.")
      .def("print_to_file",
           [](PyModule &m, const std::string &filename) {
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
           [](PyModule &m, std::string &iasm) {
             return LLVMAppendModuleInlineAsm(m.get(), iasm.c_str(), iasm.size());
           })
      .def("get_type_by_name",
           [](PyModule &m, std::string &name) {
             return PyTypeAuto(LLVMGetTypeByName(m.get(), name.c_str()));
           }, "name"_a = "",
           "Deprecated: Use LLVMGetTypeByName2 instead.");

  ModuleFlagEntriesClass
      .def("__repr__",
           [](PyModuleFlagEntries &self) {
             return "<ModuleFlagEntries>";
           })
      .def("get_behavior",
           [](PyModuleFlagEntries &self, unsigned index) {
             // TODO test whether LLVMModuleFlagEntriesGetFlagBehavior will
             // check index is in bound
             return LLVMModuleFlagEntriesGetFlagBehavior(self.get(), index);
           },
           "index"_a,
           "Returns the flag behavior for a module flag entry at a specific index.")
      .def("get_key",
           [](PyModuleFlagEntries &self, unsigned index) {
             size_t len;
             const char *str = LLVMModuleFlagEntriesGetKey(self.get(), index, &len);
             return std::string(str, len);
           },
           "index"_a,
           "Returns the key for a module flag entry at a specific index.")
      .def("get_metadata",
           [](PyModuleFlagEntries &self, unsigned index) {
             // TODO can this return null?
             return PyMetadata(LLVMModuleFlagEntriesGetMetadata(self.get(), index));
           },
           "index"_a,
           "Returns the metadata for a module flag entry at a specific index.");
}
