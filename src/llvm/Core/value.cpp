#include "value.h"

#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/tuple.h>
#include <fmt/core.h>
#include <optional>
#include "../_types.h"
#include "../_utils.h"
#include "utils.h"

namespace nb = nanobind;
using namespace nb::literals;

template <typename T>
using optional = std::optional<T>;


void bindValueClasses(nb::module_ &m) {
  auto ValueClass = nb::class_<PyValue, PyLLVMObject<PyValue, LLVMValueRef>>
                      (m, "Value", "Value");
  nb::class_<PyMetadataAsValue, PyValue>(m, "MetadataAsValue", "MetadataAsValue");
  auto MDNodeValueClass = nb::class_<PyMDNodeValue, PyMetadataAsValue>
                            (m, "MDNodeValue", "MDNodeValue");
  auto ValueAsMetadataValueClass = nb::class_<PyValueAsMetadataValue, PyMetadataAsValue>
                                     (m, "ValueAsMetadataValue", "ValueAsMetadataValue");
  auto MDStringValueClass = nb::class_<PyMDStringValue, PyMetadataAsValue>
                              (m, "MDStringValue", "MDStringValue");
    
  auto ArgumentClass = nb::class_<PyArgument, PyValue>(m, "Argument", "Argument");
  auto BasicBlockValueClass = nb::class_<PyBasicBlockValue, PyValue>(m, "BasicBlockValue", "BasicBlockValue");
  auto InlineAsmClass = nb::class_<PyInlineAsm, PyValue>(m, "InlineAsm", "InlineAsm");
  auto UserClass = nb::class_<PyUser, PyValue>(m, "User", "User");
  auto ConstantClass = nb::class_<PyConstant, PyUser>(m, "Constant", "Constant");
  auto ConstantArrayClass = nb::class_<PyConstantArray, PyConstant>(m, "ConstantArray", "ConstantArray");
  auto ConstantDataSequentialClass = nb::class_<PyConstantDataSequential, PyConstant>
                                       (m, "ConstantDataSequential", "ConstantDataSequential");
  auto ConstantDataArrayClass = nb::class_<PyConstantDataArray, PyConstantDataSequential>
                                  (m, "ConstantDataArray", "ConstantDataArray");
  auto ConstantDataVectorClass = nb::class_<PyConstantDataVector, PyConstantDataSequential>
                                   (m, "ConstantDataVector", "ConstantDataVector");
  auto ConstantExprClass = nb::class_<PyConstantExpr, PyConstant>(m, "ConstantExpr", "ConstantExpr");
  auto ConstantFPClass = nb::class_<PyConstantFP, PyConstant>(m, "ConstantFP", "ConstantFP");
  auto ConstantIntClass = nb::class_<PyConstantInt, PyConstant>(m, "ConstantInt", "ConstantInt");
  auto ConstantStructClass = nb::class_<PyConstantStruct, PyConstant>(m, "ConstantStruct", "ConstantStruct");
  auto ConstantVectorClass = nb::class_<PyConstantVector, PyConstant>(m, "ConstantVector", "ConstantVector");
  auto GlobalValueClass = nb::class_<PyGlobalValue, PyConstant>(m, "GlobalValue", "GlobalValue");
  auto GlobalAliasClass = nb::class_<PyGlobalAlias, PyGlobalValue>(m, "GlobalAlias", "GlobalAlias");
  auto GlobalObjectClass = nb::class_<PyGlobalObject, PyGlobalValue>(m, "GlobalObject", "GlobalObject");
  auto FunctionClass = nb::class_<PyFunction, PyGlobalObject>(m, "Function", "Function");
  auto GlobalVariableClass = nb::class_<PyGlobalVariable, PyGlobalObject>(m, "GlobalVariable", "GlobalVariable");
  auto GlobalIFuncClass = nb::class_<PyGlobalIFunc, PyGlobalObject>(m, "GlobalIFunc", "GlobalIFunc");
  auto UndefValueClass = nb::class_<PyUndefValue, PyConstant>(m, "UndefValue", "UndefValue");
  auto PoisonValueClass = nb::class_<PyPoisonValue, PyConstant>(m, "PoisonValue", "PoisonValue");
  auto InstructionClass = nb::class_<PyInstruction, PyUser>(m, "Instruction", "Instruction");
  auto CallBaseClass = nb::class_<PyCallBase, PyInstruction>(m, "CallBase", "CallBase");
  auto CallInstClass = nb::class_<PyCallInst, PyCallBase>(m, "CallInst", "CallInst");
  auto InvokeInstClass = nb::class_<PyInvokeInst, PyCallBase>(m, "InvokeInst", "InvokeInst");
  auto FCmpInstClass = nb::class_<PyFCmpInst, PyInstruction>(m, "FCmpInst", "FCmpInst");
  auto ICmpInstClass = nb::class_<PyICmpInst, PyInstruction>(m, "ICmpInst", "ICmpInst");
  auto GetElementPtrInstClass = nb::class_<PyGetElementPtrInst, PyInstruction>(m, "GetElementPtrInst", "GetElementPtrInst");
  auto PHINodeClass = nb::class_<PyPHINode, PyInstruction>(m, "PHINode", "PHINode");
  auto ShuffleVectorInstClass = nb::class_<PyShuffleVectorInst, PyInstruction>(m, "ShuffleVectorInst", "ShuffleVectorInst");
  auto StoreInstClass = nb::class_<PyStoreInst, PyInstruction>(m, "StoreInst", "StoreInst");
  auto BranchInstClass = nb::class_<PyBranchInst, PyInstruction>(m, "BranchInst", "BranchInst");
  auto ReturnInstClass = nb::class_<PyReturnInst, PyInstruction>(m, "ReturnInst", "ReturnInst");
  auto SwitchInstClass = nb::class_<PySwitchInst, PyInstruction>(m, "SwitchInst", "SwitchInst");
  auto CatchSwitchInstClass = nb::class_<PyCatchSwitchInst, PyInstruction>
                                (m, "CatchSwitchInst", "CatchSwitchInst");
  auto CleanupReturnInstClass = nb::class_<PyCleanupReturnInst, PyInstruction>
                                  (m, "CleanupReturnInst", "CleanupReturnInst");
  auto FuncletPadInstClass = nb::class_<PyFuncletPadInst, PyInstruction>(m, "FuncletPadInst", "FuncletPadInst");
  auto CatchPadInstClass = nb::class_<PyCatchPadInst, PyFuncletPadInst>(m, "CatchPadInst", "CatchPadInst");
  auto AllocaInstClass = nb::class_<PyAllocaInst, PyInstruction>(m, "AllocaInst", "AllocaInst");
  auto LoadInstClass = nb::class_<PyLoadInst, PyInstruction>(m, "LoadInst", "LoadInst");
  auto AtomicCmpXchgInstClass = nb::class_<PyAtomicCmpXchgInst, PyInstruction>(m, "AtomicCmpXchgInst", "AtomicCmpXchgInst");
  auto AtomicRMWInstClass = nb::class_<PyAtomicRMWInst, PyInstruction>
                              (m, "AtomicRMWInst", "AtomicRMWInst");
  auto IEValueInstBaseClass = nb::class_<PyIEValueInstBase, PyInstruction>
                                (m, "IEValueInstBase", "IEValueInstBase");
  auto InsertValueInstClass = nb::class_<PyInsertValueInst, PyIEValueInstBase>
                                (m, "InsertValueInst", "InsertValueInst");
  auto ExtractValueInstClass = nb::class_<PyExtractValueInst, PyIEValueInstBase>
                                 (m, "ExtractValueInst", "ExtractValueInst");
  auto LandingPadInstClass = nb::class_<PyLandingPadInst, PyInstruction>
                               (m, "LandingPadInst", "LandingPadInst");
  auto IndirectBrInstClass = nb::class_<PyIndirectBrInst, PyInstruction>
                               (m, "IndirectBrInst", "IndirectBrInst");
  auto FenceInstClass = nb::class_<PyFenceInst, PyInstruction>
                          (m, "FenceInst", "FenceInst");

  MDNodeValueClass
      .def("__repr__",
           [](PyMDNodeValue &self) {
             return gen_value_repr("MDNodeValue", self);
           })
      // deprecated functions not binded:
      // LLVMMDNodeInContext, LLVMMDNode
      .def("as_metadata",
           [](PyMDStringValue &self) {
             return PyMDNode(LLVMValueAsMetadata(self.get()));
           })
      .def_prop_ro("num_operands",
                   [](PyMDNodeValue &self) {
                     return LLVMGetMDNodeNumOperands(self.get());
                   })
      .def_prop_ro("operands",
                   [](PyMDNodeValue &self) {
                     unsigned num = LLVMGetMDNodeNumOperands(self.get());
                     std::vector<LLVMValueRef> dest(num);
                     LLVMGetMDNodeOperands(self.get(), dest.data());
                     WRAP_VECTOR_FROM_DEST_AUTO(PyValue, num, res, dest);
                     return res;
                   },
                   "Obtain the given MDNode's operands.")
      .def_prop_ro("replace_operand_with",
                   [](PyMDNodeValue &self, unsigned index, PyMetadata &replacement) {
                     return LLVMReplaceMDNodeOperandWith
                              (self.get(), index, replacement.get());
                   },
                   "Replace an operand at a specific index in a llvm::MDNode value.");
  
  MDStringValueClass
      .def("__repr__",
           [](PyMDStringValue &self) {
             return gen_value_repr("MDStringValue", self);
           })
      // deprecated functions not binded:
      // LLVMMDStringInContext, LLVMMDString
      .def("as_metadata",
           [](PyMDStringValue &self) {
             return PyMDString(LLVMValueAsMetadata(self.get()));
           })
      .def_prop_ro("raw_string",
                   [](PyMDStringValue &v) {
                     unsigned len;
                     auto str = LLVMGetMDString(v.get(), &len);
                     return std::string(str, len);
                   },
                   "Obtain the underlying string from a MDString value.");

  ValueClass
      // TODO note there are many functions here that belongs `UserClass`
      .def("__repr__",
           [](PyValue &self) {
             auto raw = self.get();
             auto name = get_value_name(raw);
             auto kind = get_repr_str(LLVMGetValueKind(raw));
             return fmt::format("<Value name={} kind={} str='{}'>",
                                name, kind, get_value_str(raw));
           })
      .def("__str__",
           [](PyValue &self) {
             return get_value_str(self.get());
           })
      .def_prop_ro("type",
                   [](PyValue &v) { return PyTypeAuto(LLVMTypeOf(v.get())); })
      .def_prop_ro("kind",
                   [](PyValue &v) { return LLVMGetValueKind(v.get()); })
      // NOTE LLVMSetValueName and LLVMGetValueName are depreciated
      .def_prop_rw("name",
                   [](PyValue &v) {
                     return get_value_name(v.get());
                   },
                   [](PyValue &v, std::string &name) {
                     return LLVMSetValueName2(v.get(), name.c_str(), name.size());
                   })
      .def_prop_ro("is_constant",
                   [](PyValue &v) { return LLVMIsConstant(v.get()) != 0; })
      .def_prop_ro("is_undef",
                   [](PyValue &v) { return LLVMIsUndef(v.get()) != 0; })
      .def_prop_ro("is_poisonous",
                   [](PyValue &v) { return LLVMIsPoison(v.get()) != 0;})
      .def_prop_ro("first_use",
                   [](PyValue &v) -> optional<PyUse> {
                     auto res = LLVMGetFirstUse(v.get());
                     WRAP_OPTIONAL_RETURN(res, PyUse);
                   })
      .def_prop_ro("uses",
                   [](PyValue &self) {
                     return PyUseIterator(PyUse(LLVMGetFirstUse(self.get())));
                   })
      .def("as_metadata",
           [](PyValue &self) {
             return PyValueAsMetadata(LLVMValueAsMetadata(self.get()));
           })
      .def("dump",
           [](PyValue &v) { return LLVMDumpValue(v.get()); },
           "Dump a representation of a value to stderr.")
      .def_static("replace_all_uses_with",
                  [](PyValue &oldVal, PyValue &newVal) {
                    return LLVMReplaceAllUsesWith(oldVal.get(), newVal.get());
                  },
                  "Replace all uses of a value with another one.")
  PY_FOR_EACH_VALUE_SUBCLASS(PY_DECLARE_VALUE_CAST)
      .def("to_BasicBlockValue",
           [](PyValue &self) -> optional<PyBasicBlockValue> {
             auto res = LLVMIsABasicBlock(self.get());
             WRAP_OPTIONAL_RETURN(res, PyBasicBlockValue);
           })
      .def("to_MDNodeValue",
           [](PyValue &self) -> optional<PyMDNodeValue> {
             auto res = LLVMIsAMDNode(self.get());
             WRAP_OPTIONAL_RETURN(res, PyMDNodeValue);
           })
      .def("to_ValueAsMetadataValue",
           [](PyValue &self) -> optional<PyValueAsMetadataValue> {
             auto res = LLVMIsAValueAsMetadata(self.get());
             WRAP_OPTIONAL_RETURN(res, PyValueAsMetadataValue);
           })
      .def("to_MDStringValue",
           [](PyValue &self) -> optional<PyMDStringValue> {
             auto res = LLVMIsAMDString(self.get());
             WRAP_OPTIONAL_RETURN(res, PyMDStringValue);
           })
      // TODO test whether these property are only true to BasicBlockValue
      .def_prop_ro("is_basic_block",
                   [](PyValue &self) {
                     return LLVMValueIsBasicBlock(self.get()) != 0;
                   })
      .def("as_basic_block",
           [](PyValue &self) {
             return PyBasicBlock(LLVMValueAsBasicBlock(self.get()));
           });

  PoisonValueClass
      .def("__repr__",
           [](PyPoisonValue &self) {
             return gen_value_repr("PoisonValue", self);
           })
      .def("__init__",
           [](PyPoisonValue *p, PyType &t) {
             new (p) PyPoisonValue(LLVMGetPoison(t.get()));
           },
           "type"_a,
           "Obtain a constant value referring to a poison value of a type.");

  
  UndefValueClass
      .def("__repr__",
           [](PyUndefValue &self) {
             return gen_value_repr("UndefValue", self);
           })

      .def("__init__",
           [](PyUndefValue *uv, PyType &t) {
             new (uv) PyUndefValue(LLVMGetUndef(t.get()));
           },
           "type"_a,
           "Obtain a constant value referring to an undefined value of a type.");
  

  ShuffleVectorInstClass
      .def("__repr__",
           [](PyShuffleVectorInst &self) {
             return "<ShuffleVectorInst>";
           })
      .def_prop_ro("mask_elems_num",
                   [](PyShuffleVectorInst &self) {
                     return LLVMGetNumMaskElements(self.get());
                   })
      .def_static("get_undef_mask_elem",
                  [](PyShuffleVectorInst &self) {
                    return LLVMGetUndefMaskElem();
                  },
                  "returns a constant that specifies that the result of a "
                  "ShuffleVectorInst is undefined.")
      .def("get_mask_value",
           [](PyValue &self, unsigned index) {
             return LLVMGetMaskValue(self.get(), index);
           },
           "index"_a,
           "Get the mask value at position index in the mask of the ShuffleVector"
           "instruction.\n\n"
           "Returns the result of LLVMGetUndefMaskElem() if the mask value is"
           "poison at that position.");

  
  CatchPadInstClass
      .def("__repr__",
           [](PyCatchPadInst &self) {
             return "<CatchPadInst>";
           })
      .def_prop_rw("parent",
                   [](PyCatchPadInst &self) {
                     return PyCatchSwitchInst(LLVMGetParentCatchSwitch(self.get()));
                   },
                   [](PyCatchPadInst &self, PyCatchSwitchInst catchSwitch) {
                     return LLVMSetParentCatchSwitch(self.get(), catchSwitch.get());
                   });
  

  

  GlobalIFuncClass
      .def("__repr__",
           [](PyGlobalIFunc &self) {
             return "<GlobalIFunc>";
           })
      .def_prop_ro("next",
                   [](PyGlobalIFunc &self) -> optional<PyGlobalIFunc> {
                     auto res = LLVMGetNextGlobalIFunc(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalIFunc);
                   })
      .def_prop_ro("prev",
                   [](PyGlobalIFunc &self) -> optional<PyGlobalIFunc> {
                     auto res = LLVMGetPreviousGlobalIFunc(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalIFunc);
                   })
      .def_prop_rw("resolver",
                   [](PyGlobalIFunc &self) -> optional<PyConstant> {
                     auto res = LLVMGetGlobalIFuncResolver(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyConstant);
                   },
                   [](PyGlobalIFunc &self, PyConstant resolver) {
                     return LLVMSetGlobalIFuncResolver(self.get(), resolver.get());
                   })
      .def("destory",
           [](PyGlobalIFunc &self) {
             return LLVMEraseGlobalIFunc(self.get());
           },
           "Remove a global indirect function from its parent module and delete it.\n\n"
           "You shouldn't use it anymore after removal.")
      .def("remove_from_parent",
           [](PyGlobalIFunc &self) {
             return LLVMRemoveGlobalIFunc(self.get());
           },
           "Remove a global indirect function from its parent module.\n\n"
           "This unlinks the global indirect function from its containing module but"
           "keeps it alive.");

  InlineAsmClass
      .def("__repr__",
           [](PyInlineAsm &self) {
             return "<InlineAsm>";
           })
      .def("get_inline_asm",
           [](PyInlineAsm *iasm, PyType &ty, std::string asmString, std::string constraints,
              bool hasSideEffects, bool isAlignStack, LLVMInlineAsmDialect dialect,
              bool canThrow){
             new (iasm) PyInlineAsm(LLVMGetInlineAsm
                                      (ty.get(), asmString.c_str(), asmString.size(),
                                       constraints.c_str(), constraints.size(),
                                       hasSideEffects, isAlignStack, dialect, canThrow));
           },
           "type"_a, "asm"_a, "constraints"_a, "has_side_effects"_a,
           "is_align_stack"_a, "dialect"_a, "can_throw"_a,
           "Create the specified unique inline asm string.")
      .def_prop_ro("str",
                   [](PyInlineAsm &iasm) {
                     size_t len;
                     const char *str = LLVMGetInlineAsmAsmString(iasm.get(), &len);
                     return std::string(str, len);
                   },
                   "Get the template string used for an inline assembly snippet.")
      .def_prop_ro("constraint_str",
                   [](PyInlineAsm &iasm) {
                     size_t len;
                     const char *str = LLVMGetInlineAsmConstraintString(iasm.get(), &len);
                     return std::string(str, len);
                   },
                   "Get the raw constraint string for an inline assembly snippet.")
      .def_prop_ro("dialect",
                   [](PyInlineAsm &iasm) {
                     return LLVMGetInlineAsmDialect(iasm.get());
                   },
                   "Get the dialect used by the inline asm snippet.")
      .def_prop_ro("function_type",
                   [](PyInlineAsm &iasm) {
                     return PyTypeFunction(LLVMGetInlineAsmFunctionType(iasm.get()));
                   },
                   "Get the function type of the inline assembly snippet. "
                   "The same type that was passed into :func:`get_inline_asm` originally.")
      .def_prop_ro("has_side_effects",
                   [](PyInlineAsm &iasm) {
                     return LLVMGetInlineAsmHasSideEffects(iasm.get()) != 0;
                   },
                   "Get if the inline asm snippet has side effects.")
      .def_prop_ro("needs_aligned_stack",
                   [](PyInlineAsm &iasm) {
                     return LLVMGetInlineAsmNeedsAlignedStack(iasm.get()) != 0;
                   })
      .def_prop_ro("can_unwind",
                   [](PyInlineAsm &iasm) {
                     return LLVMGetInlineAsmCanUnwind(iasm.get()) != 0;
                   });

  InstructionClass
      .def("__repr__",
           [](PyInstruction &self) {
             auto opcode = LLVMGetInstructionOpcode(self.get());
             return fmt::format("<Instruction opcode={}>", get_repr_str(opcode));
           })
      .def_prop_ro("can_use_fast_math_flags",
                   [](PyInstruction &self) {
                     return LLVMCanValueUseFastMathFlags(self.get()) != 0;
                   },
                   "Check if a given value can potentially have fast math flags.\n"
                   "Will return true for floating point arithmetic instructions, and "
                   "for select, phi, and call instructions whose type is a floating "
                   "point type, or a vector or array thereof.\n"
                   "See https://llvm.org/docs/LangRef.html#fast-math-flags")
      .def_prop_ro("next",
                   [](PyInstruction &self) -> optional<PyInstruction> {
                     auto res = LLVMGetNextInstruction(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyInstruction);
                   },
                   "Obtain the instruction that occurs after the one specified.\n"
                   "The next instruction will be from the same basic block.\n\n"
                   "If this is the last instruction in a basic block, None will "
                   "be returned.")
      .def_prop_ro("prev",
                   [](PyInstruction &self) -> optional<PyInstruction> {
                     auto res = LLVMGetPreviousInstruction(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyInstruction);
                   },
                   "Obtain the instruction that occurred before this one.\n\n"
                   "If the instruction is the first instruction in a basic block, NULL"
                   "will be returned.")
      .def_prop_ro("has_metadata",
                   [](PyInstruction &self) {
                     return LLVMHasMetadata(self.get()) != 0;
                   })
      .def_prop_ro("debug_loc_directory",
                   [](PyInstruction &i) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocDirectory(i.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the directory of the debug location for this value")
      .def_prop_ro("debug_loc_filename",
                   [](PyInstruction &i) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocFilename(i.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the filename of the debug location for this value.")
      .def_prop_ro("debug_loc_line",
                   [](PyInstruction &i) { return LLVMGetDebugLocLine(i.get()); },
                   "Return the line number of the debug location for this value")
      .def_prop_ro("debug_loc_column",
                   [](PyInstruction &i) { return LLVMGetDebugLocColumn(i.get()); },
                   "Return the column number of the debug location for this value")
      .def_prop_ro("parent",
                   [](PyInstruction &self) -> optional<PyBasicBlock> {
                     auto res = LLVMGetInstructionParent(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyBasicBlock);
                   },
                   "Obtain the basic block to which an instruction belongs.")
      .def_prop_ro("opcode",
                   [](PyInstruction &self) {
                     return LLVMGetInstructionOpcode(self.get());
                   })
      .def_prop_ro("is_terminator",
                   [](PyInstruction &self) {
                     return LLVMIsATerminatorInst(self.get()) != nullptr;
                   })
      .def("clone",
           [](PyInstruction &self) -> optional<PyInstruction*> {
             auto res = LLVMInstructionClone(self.get());
             WRAP_OPTIONAL_RETURN(res, PyInstructionAuto);
           },
           "Create a copy of 'this' instruction that is identical in all ways"
           "except the following:\n"
           "  - The instruction has no parent"
           "  - The instruction has no name")
      .def("remove_from_parent",
           [](PyInstruction &self) {
             return LLVMInstructionRemoveFromParent(self.get());
           },
           "The instruction specified is removed from its containing building"
           "block but is kept alive.")
      .def("destory",
           [](PyInstruction &self) {
             return LLVMInstructionEraseFromParent(self.get());
           },
           "Remove and delete an instruction.\n\n"
           "The instruction specified is removed from its containing building"
           "block and then deleted.")
      .def("delete",
           [](PyInstruction &self) {
             return LLVMDeleteInstruction(self.get());
           },
           "Delete an instruction.\n\n"
           "The instruction specified is deleted. It must have previously been"
           "removed from its containing building block.")
      .def("get_metadata",
           [](PyInstruction &self, unsigned kindID) -> optional<PyValue*> {
             auto res = LLVMGetMetadata(self.get(), kindID);
             WRAP_OPTIONAL_RETURN(res, PyValueAuto);
           },
           "kind_id"_a)
      .def("set_metadata",
           [](PyInstruction &self, unsigned kindID, PyMetadataAsValue node) {
             return LLVMSetMetadata(self.get(), kindID, node.get());
           },
           "kind_id"_a, "value"_a)
      .def("get_all_metadata_no_debug_loc",
           [](PyInstruction &self) {
             size_t num;
             LLVMValueMetadataEntries entries =
               LLVMInstructionGetAllMetadataOtherThanDebugLoc(self.get(), &num);
             return PyMetadataEntries(entries, num);
           })
      /*
        The following methods only applys to instructions for which LLVMIsATerminatorInst
        returns true.
       
        NOTE the reason why we don't create a base class for them to inherit is
        because by inspecting the llvm c++ api (by following the call trace,
        the generated Instruction.def file) groups different opcode to be terminator than
        LLVMOpcode in C api, so possible C api is outdated?
        
        TODO we'll give it a test first.   
       */

      .def_prop_ro("successors_num",
                   [](PyInstruction &self) {
                     return LLVMGetNumSuccessors(self.get());
                   })
      .def("get_successor",
           [](PyInstruction &self, unsigned index) {
             return PyBasicBlock(LLVMGetSuccessor(self.get(), index));
           })
      .def("set_successor",
           [](PyInstruction &self, unsigned index, PyBasicBlock bb) {
             return LLVMSetSuccessor(self.get(), index, bb.get());
           })
       // TODO
      .def_static("get_nuw", 
                  [](PyInstruction &arithInst) {
                    return LLVMGetNUW(arithInst.get()) != 0;
                  },
                  "arith_inst"_a)
      .def_static("set_nuw",
                  [](PyInstruction &arithInst, bool hasNUW) {
                    return LLVMSetNUW(arithInst.get(), hasNUW);
                  },
                  "arith_inst"_a, "hasNUW"_a)
      .def_static("get_nsw",
                  [](PyInstruction &arithInst) {
                    return LLVMGetNSW(arithInst.get()) != 0;
                  },
                  "arithInst"_a)
      .def_static("set_nsw",
                  [](PyInstruction &arithInst, bool hasNSW) {
                    return LLVMSetNSW(arithInst.get(), hasNSW);
                  },
                  "arith_inst"_a, "hasNSW"_a)
      .def_static("get_exact",
                  [](PyInstruction &DivOrShrInst) {
                    return LLVMGetExact(DivOrShrInst.get()) != 0;
                  },
                  "div_or_shr_inst"_a)
      .def_static("set_exact",
                  [](PyInstruction &DivOrShrInst, bool isExact) {
                    return LLVMSetExact(DivOrShrInst.get(), isExact);
                  },
                  "div_or_shr_inst"_a, "is_exact"_a)
      .def_static("get_nneg",
                  [](PyInstruction &NonNegInst) {
                    return LLVMGetNNeg(NonNegInst.get()) != 0;
                  },
                  "non_neg_inst"_a,
                  "Gets if the instruction has the non-negative flag set.\n",
                  "Only valid for zext instructions.")
      .def_static("set_nned",
                  [](PyInstruction &NonNegInst, bool isNonNeg) {
                    return LLVMSetNNeg(NonNegInst.get(), isNonNeg);
                  },
                  "non_neg_inst"_a, "is_non_neg"_a,
                  "Sets the non-negative flag for the instruction.\n"
                  "Only valid for zext instructions.")
      .def_static("get_fast_math_flags",
                  [](PyInstruction &FPMathInst) {
                    return LLVMGetFastMathFlags(FPMathInst.get());
                  },
                  "fp_math_inst"_a,
                  "Get the flags for which fast-math-style optimizations are allowed "
                  "for this value.\n\n"
                  "Only valid on floating point instructions."
                  "See `can_use_fast_math_flags`.")
      .def_static("set_fast_math_flags",
                  [](PyInstruction &FPMathInst, LLVMFastMathFlags FMF) {
                    return LLVMSetFastMathFlags(FPMathInst.get(), FMF);
                  },
                  "fp_math_inst"_a, "fmf"_a,
                  "Sets the flags for which fast-math-style optimizations are allowed "
                  "for this value.\n\n"
                  "Only valid on floating point instructions.\n"
                  "See `can_use_fast_math_flags`.")
      .def_static("get_is_disjoint",
                  [](PyInstruction &inst) {
                    return LLVMGetIsDisjoint(inst.get()) != 0;
                  },
                  "inst"_a,
                  "Gets whether the instruction has the disjoint flag set.\n"
                  "Only valid for or instructions.")
      .def_static("set_is_disjoint",
                  [](PyInstruction &inst, bool isDisjoint) {
                    return LLVMSetIsDisjoint(inst.get(), isDisjoint);
                  },
                  "inst"_a, "is_disjoint"_a,
                  "Sets the disjoint flag for the instruction.\n"
                  "Only valid for or instructions.");

  /*
    Terminator only methods needs ===================================
   */

  BranchInstClass
      .def_prop_ro("is_conditional",
                   [](PyBranchInst &self) {
                     return LLVMIsConditional(self.get());
                   })
      .def_prop_rw("condition", // TODO test whether return NULL
                   [](PyBranchInst &self) {
                     return PyValueAuto(LLVMGetCondition(self.get()));
                   },
                   [](PyBranchInst &self, PyValue &cond) {
                     return LLVMSetCondition(self.get(), cond.get());
                   });

  SwitchInstClass
      .def("__repr__",
           [](PySwitchInst &self) {
             return "<SwitchInst>";
           })
      .def_prop_ro("default_dest",
                   [](PySwitchInst &self) {
                     return PyBasicBlock(LLVMGetSwitchDefaultDest(self.get()));
                   })
      .def_prop_ro("add_case",
                   [](PySwitchInst &self, PyConstantInt &onVal, PyBasicBlock dest) {
                     return LLVMAddCase(self.get(), onVal.get(), dest.get());
                   });

  IEValueInstBaseClass
      .def_prop_ro("indices_num",
                   [](PyIEValueInstBase &self) {
                     return LLVMGetNumIndices(self.get());
                   })
      .def("indices",
           [](PyIEValueInstBase &self) {
             return LLVMGetIndices(self.get());
           });


  CallInstClass
      .def_prop_rw("is_tail_call",
                   [](PyCallInst &self) {
                     return LLVMIsTailCall(self.get()) != 0;
                   },
                   [](PyCallInst &self, bool isTailCall) {
                     return LLVMSetTailCall(self.get(), isTailCall);
                   })
      .def_prop_rw("tail_call_kind",
                   [](PyCallInst &self) {
                     return LLVMGetTailCallKind(self.get());
                   },
                   [](PyCallInst &self, LLVMTailCallKind kind) {
                     return LLVMSetTailCallKind(self.get(), kind);
                   });

  
  CallBaseClass
      .def_prop_ro("arg_num",
                   [](PyCallBase &self) {
                     return LLVMGetNumArgOperands(self.get());
                   })
      .def_prop_rw("call_conv",
                   [](PyCallBase &self) {
                     auto res = LLVMGetInstructionCallConv(self.get());
                     return static_cast<LLVMCallConv>(res);
                   },
                   [](PyCallBase &self, LLVMCallConv CC) {
                     return LLVMSetInstructionCallConv(self.get(), CC);
                   })
      .def_prop_ro("called_fn_type",
                   [](PyCallBase &self) {
                     return PyTypeFunction(LLVMGetCalledFunctionType(self.get()));
                   },
                   "Obtain the function type called by this instruction.")
      .def_prop_ro("called_value",
                   [](PyCallBase &self) {
                     return PyFunction(LLVMGetCalledValue(self.get()));
                   })
      .def_prop_ro("operand_bundles_num",
                   [](PyCallBase &self) {
                     return LLVMGetNumOperandBundles(self.get());
                   })
      .def("get_operand_bundle_at",
           [](PyCallBase &self, unsigned index) {
             return PyOperandBundle(LLVMGetOperandBundleAtIndex(self.get(), index));
           },
           "index"_a,
           "Obtain the operand bundle attached to this instruction at the given index.")
      .def("set_arg_alignment",
           [](PyCallBase &self, LLVMAttributeIndex idx, unsigned align) {
             return LLVMSetInstrParamAlignment(self.get(), idx, align);
           },
           "index"_a, "align"_a)
      .def("add_call_site_attribute",
           [](PyCallBase &self, LLVMAttributeIndex idx, PyAttribute &attr) {
             return LLVMAddCallSiteAttribute(self.get(), idx, attr.get());
           },
           "index"_a, "attr"_a)
      .def("get_call_site_attribute_count",
           [](PyCallBase &self, LLVMAttributeIndex idx) {
             return LLVMGetCallSiteAttributeCount(self.get(), idx);
           })
      .def("get_call_site_attributes",
           [](PyCallBase &self, LLVMAttributeIndex idx) {
             unsigned num = LLVMGetCallSiteAttributeCount(self.get(), idx);
             std::vector<LLVMAttributeRef> attrs(num);
             LLVMGetCallSiteAttributes(self.get(), idx, attrs.data());
             WRAP_VECTOR_FROM_DEST_AUTO(PyAttribute, num, res, attrs);
             return res;
           })
      .def("get_call_site_enum_attribute",
           [](PyCallBase &self, LLVMAttributeIndex idx, unsigned kindID) {
             return PyEnumAttribute(LLVMGetCallSiteEnumAttribute(self.get(), idx,
                                                                 kindID));
           })
      .def("get_call_site_string_attribute",
           [](PyCallBase &self, LLVMAttributeIndex idx, std::string &kind) {
             return PyStringAttribute(LLVMGetCallSiteStringAttribute
                                        (self.get(), idx, kind.c_str(), kind.size()));
           })
      .def("remove_call_site_enum_attribute",
           [](PyCallBase &self, LLVMAttributeIndex idx, unsigned kindID) {
             return LLVMRemoveCallSiteEnumAttribute(self.get(), idx, kindID);
           })
      .def("remove_call_site_string_attribute",
           [](PyCallBase &self, LLVMAttributeIndex idx, std::string &kind) {
             return LLVMRemoveCallSiteStringAttribute
                      (self.get(), idx, kind.c_str(), kind.size());
           });

  InvokeInstClass
      .def("__repr__",
           [](PyInvokeInst &self) {
             return "<InvokeInst>";
           })
      .def_prop_rw("normal_dest",
                   [](PyInvokeInst &self) {
                     return PyBasicBlock(LLVMGetNormalDest(self.get()));
                   },
                   [](PyInvokeInst &self, PyBasicBlock bb) {
                     return LLVMSetNormalDest(self.get(), bb.get());
                   })
      .def_prop_rw("unwind_dest",
                   [](PyInvokeInst &self) {
                     return PyBasicBlock(LLVMGetUnwindDest(self.get()));
                   },
                   [](PyInvokeInst &self, PyBasicBlock bb) {
                     return LLVMSetUnwindDest(self.get(), bb.get());
                   });

  CleanupReturnInstClass
      .def("__repr__",
           [](PyCleanupReturnInst &self) {
             return "<CleanupReturnInst>";
           })
      .def_prop_rw("unwind_dest",
                   [](PyCleanupReturnInst &self) {
                     return PyBasicBlock(LLVMGetUnwindDest(self.get()));
                   },
                   [](PyCleanupReturnInst &self, PyBasicBlock bb) {
                     return LLVMSetUnwindDest(self.get(), bb.get());
                   });

  CatchSwitchInstClass
      .def("__repr__",
           [](PyCatchSwitchInst &self) {
             return "<CatchSwitchInst>";
           })
      .def_prop_rw("unwind_dest",
                   [](PyCatchSwitchInst &self) {
                     return PyBasicBlock(LLVMGetUnwindDest(self.get()));
                   },
                   [](PyCatchSwitchInst &self, PyBasicBlock bb) {
                     return LLVMSetUnwindDest(self.get(), bb.get());
                   })
      .def_prop_ro("handlers_num",
                   [](PyCatchSwitchInst &self) {
                     return LLVMGetNumHandlers(self.get());
                   })
      .def_prop_ro("handlers",
                   [](PyCatchSwitchInst &self) {
                     unsigned num = LLVMGetNumHandlers(self.get());
                     std::vector<LLVMBasicBlockRef> handlers;
                     LLVMGetHandlers(self.get(), handlers.data());
                     WRAP_VECTOR_FROM_DEST(PyBasicBlock, num, res, handlers);
                     return res;
                   },
                   "Obtain the basic blocks acting as handlers for a catchswitch "
                   "instruction.")
      .def("add_handler",
           [](PyCatchSwitchInst &self, PyBasicBlock dest) {
             return LLVMAddHandler(self.get(), dest.get());
           });


  FuncletPadInstClass
      .def("__repr__",
           [](PyFuncletPadInst &self) {
             return "<FuncletPadInst>";
           })
      .def_prop_ro("arg_num",
                   [](PyCallBase &self) {
                     // TODO change it into enum type
                     return LLVMGetNumArgOperands(self.get());
                   })
      .def("get_arg_operand",
           [](PyFuncletPadInst &self, unsigned index) {
             return PyValueAuto(LLVMGetArgOperand(self.get(), index));
           },
           "index"_a)
      .def("set_arg_operand",
           [](PyFuncletPadInst &self, unsigned index, PyValue &value) {
             return LLVMSetArgOperand(self.get(), index, value.get());
           });

  ICmpInstClass
      .def("__repr__",
           [](PyICmpInst &self) {
             return "<ICmpInst>";
           })
      .def_prop_ro("predicate",
                   [](PyInstruction &self) {
                     return LLVMGetICmpPredicate(self.get());
                   },
                   "Obtain the predicate of an instruction.");

  FCmpInstClass
      .def("__repr__",
           [](PyFCmpInst &self) {
             return "<FCmpInst>";
           })
      .def_prop_ro("predicate",
                   [](PyInstruction &self) {
                     return LLVMGetFCmpPredicate(self.get());
                   },
                   "Obtain the float predicate of an instruction.");

  GlobalVariableClass
      .def("__repr__",
           [](PyGlobalVariable &self) {
             return gen_value_repr("GlobalVariable", self);
           })
      .def("__init__",
           [](PyGlobalVariable *g, PyModule &m, PyType &type, const char *name) {
             new (g) PyGlobalVariable(LLVMAddGlobal(m.get(), type.get(), name));
           },
           "module"_a, "type"_a, "name"_a = "")
      .def_prop_rw("initializer",
                   [](PyGlobalVariable &self) {
                     return PyValueAuto(LLVMGetInitializer(self.get()));
                   },
                   [](PyGlobalVariable &self, PyConstant &c) {
                     return LLVMSetInitializer(self.get(), c.get());
                   },
                   nb::for_setter(nb::sig("def initializer(self, value: Constant) -> None")))
      .def_prop_rw("is_thread_local",
                   [](PyGlobalVariable &self) {
                     return LLVMIsThreadLocal(self.get()) != 0;
                   },
                   [](PyGlobalVariable &self, bool isThreadLocal) {
                     return LLVMSetThreadLocal(self.get(), isThreadLocal);
                   })
      .def_prop_rw("is_global_constant",
                   [](PyGlobalVariable &self) {
                     return LLVMIsGlobalConstant(self.get()) != 0;
                   },
                   [](PyGlobalVariable &self, bool isConstant) {
                     return LLVMSetGlobalConstant(self.get(), isConstant);
                   },
                   nb::for_setter
                     (nb::sig
                        ("def is_global_constant(self, is_constant: bool, /) -> None")))
      .def_prop_rw("thread_local_mode",
                   [](PyGlobalVariable &self) {
                     return LLVMGetThreadLocalMode(self.get());
                   },
                   [](PyGlobalVariable &self, LLVMThreadLocalMode mode) {
                     return LLVMSetThreadLocalMode(self.get(), mode);
                   },
                   nb::for_setter
                     (nb::sig("def thread_local_mode(self, mode: ThreadLocalMode, /) "
                              "-> None")))
      .def_prop_rw("is_externally_initialized",
                   [](PyGlobalVariable &self) {
                     return LLVMIsExternallyInitialized(self.get()) != 0;
                   },
                   [](PyGlobalVariable &self, bool isExtInit) {
                     return LLVMSetExternallyInitialized(self.get(), isExtInit);
                   },
                   nb::for_setter
                     (nb::sig
                        ("def is_externally_initialized(self, is_ext_init: bool, /)"
                         " -> None")))
      .def_prop_ro("next",
                   [](PyGlobalVariable &v) -> std::optional<PyGlobalVariable> {
                     auto res = LLVMGetNextGlobal(v.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalVariable);
                   })
      .def_prop_ro("prev",
                   [](PyGlobalVariable &v) -> std::optional<PyGlobalVariable> {
                     auto res = LLVMGetPreviousGlobal(v.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalVariable);
                   })
      .def_prop_ro("debug_loc_directory",
                   [](PyGlobalVariable &v) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocDirectory(v.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the directory of the debug location for this value")
      .def_prop_ro("debug_loc_filename",
                   [](PyGlobalVariable &v) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocFilename(v.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the filename of the debug location for this value")
      .def_prop_ro("debug_loc_line",
                   [](PyGlobalVariable &v) { return LLVMGetDebugLocLine(v.get()); },
                   "Return the line number of the debug location for this value")
      // TODO test + more suitable way? (python side also do `del v`?)
      // but python pass variable by value...
      .def("destory", 
           [](PyGlobalVariable &self) {
             return LLVMDeleteGlobal(self.get());
           },
           "Delete this variable. You are not supposed to use this variable later.");


  
  GlobalAliasClass
      .def("__repr__",
           [](PyGlobalAlias &self) {
             return gen_value_repr("GlobalAlias", self);
           })
      .def("__init__",
           [](PyGlobalAlias *g, PyModule &self, PyType &valueType, unsigned addrSpace,
              PyValue aliasee, const char *name) {
             new (g) PyGlobalAlias(LLVMAddAlias2
                                     (self.get(), valueType.get(), addrSpace,
                                      aliasee.get(), name));
           },
           "module"_a, "value_type"_a, "addr_space"_a, "aliasee"_a, "name"_a = "",
           "Add a GlobalAlias with the given value type, address space and aliasee.")
      .def_prop_ro("next",
                   [](PyGlobalAlias &self) -> std::optional<PyGlobalAlias> {
                     auto res = LLVMGetNextGlobalAlias(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalAlias);
                   },
                   "Advance a GlobalAlias iterator to the next GlobalAlias.\n\n"
                   "Returns NULL if the iterator was already at the beginning and there are"
                   "no previous global aliases.")
       .def_prop_ro("prev",
                    [](PyGlobalAlias &self) -> std::optional<PyGlobalAlias> {
                      auto res = LLVMGetPreviousGlobalAlias(self.get());
                      WRAP_OPTIONAL_RETURN(res, PyGlobalAlias);
                    },
                    "Decrement a GlobalAlias iterator to the previous GlobalAlias.\n\n"
                    "Returns NULL if the iterator was already at the beginning and there are"
                    "no previous global aliases.")
       .def_prop_rw("aliasee",
                    [](PyGlobalAlias &self) {
                      return PyValueAuto(LLVMAliasGetAliasee(self.get()));
                    },
                    [](PyGlobalAlias &self, PyValue &aliasee) {
                      return LLVMAliasSetAliasee(self.get(), aliasee.get());
                    });
  

  FunctionClass
      .def("__repr__",
           [](PyFunction &self) {
             return gen_value_repr("Function", self);
           })
      .def("__init__",
           [](PyFunction *f, PyModule &m, PyTypeFunction &functionTy, std::string &name) {
             new (f) PyFunction(LLVMAddFunction(m.get(), name.c_str(), functionTy.get()));
           },
           "module"_a, "function_type"_a, "name"_a = "",
           "Add a function to a module under a specified name.")
      .def_prop_rw("call_conv",
                   [](PyFunction &self){
                     auto res = LLVMGetFunctionCallConv(self.get());
                     return static_cast<LLVMCallConv>(res);
                   },
                   [](PyFunction &self, LLVMCallConv CC) {
                     return LLVMSetFunctionCallConv(self.get(), CC);
                   })
      .def_prop_rw("gc",
                   [](PyFunction &self) {
                     return LLVMGetGC(self.get());
                   },
                   [](PyFunction &self, const char *name) {
                     return LLVMSetGC(self.get(), name);
                   },
                   nb::for_getter
                     ("Obtain the name of the garbage collector to use "
                      "during code generation."))
      .def_prop_ro("basic_blocks_num",
                   [](PyFunction &self) {
                     return LLVMCountBasicBlocks(self.get());
                   })
      .def_prop_ro("basic_blocks",
                   [](PyFunction &self) {
                     unsigned num = LLVMCountBasicBlocks(self.get());
                     std::vector<LLVMBasicBlockRef> BasicBlocks(num);
                     LLVMGetBasicBlocks(self.get(), BasicBlocks.data());
                     WRAP_VECTOR_FROM_DEST(PyBasicBlock, num,
                                           res, BasicBlocks);
                     return res;
                   })
      .def_prop_ro("first_basic_block",
                   [](PyFunction &self) -> optional<PyBasicBlock> {
                     auto res =  LLVMGetFirstBasicBlock(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyBasicBlock);
                   })
      .def_prop_ro("last_basic_block",
                   [](PyFunction &self) -> optional<PyBasicBlock> {
                     auto res = LLVMGetLastBasicBlock(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyBasicBlock);
                   })
      .def_prop_ro("entry_basic_block",
                   [](PyFunction &self) {
                     return PyBasicBlock(LLVMGetEntryBasicBlock(self.get()));
                   })
      .def_prop_ro("has_personality_fn",
                   [](PyFunction &self) {
                     return LLVMHasPersonalityFn(self.get()) != 0;
                   })
      .def_prop_rw("personality_fn",
                   [](PyFunction &self) {
                     return PyFunction(LLVMGetPersonalityFn(self.get()));
                   },
                   [](PyFunction &self, PyFunction fn) {
                     return LLVMSetPersonalityFn(self.get(), fn.get());
                   })
      .def_prop_ro("intrinsic_id",
                   [](PyFunction &self) {
                     return LLVMGetIntrinsicID(self.get());
                   })
      .def_prop_ro("next",
                   [](PyFunction &f) -> std::optional<PyFunction> {
                     auto res = LLVMGetNextFunction(f.get());
                     WRAP_OPTIONAL_RETURN(res, PyFunction);
                   },
                   "Advance a Function iterator to the next Function.\n\n"
                   "Returns NULL if the iterator was already at the end and there are no more"
                   "functions.")
      .def_prop_ro("previous",
                   [](PyFunction &f) -> std::optional<PyFunction> {
                     auto res = LLVMGetPreviousFunction(f.get());
                     WRAP_OPTIONAL_RETURN(res, PyFunction);
                   },
                   "Decrement a Function iterator to the previous Function.\n\n"
                   "Returns NULL if the iterator was already at the beginning and there are"
                   "no previous functions.")
      .def_prop_ro("debug_loc_directory",
                   [](PyFunction &f) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocDirectory(f.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the directory of the debug location for this value")
      .def_prop_ro("debug_loc_filename",
                   [](PyFunction &f) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocFilename(f.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the filename of the debug location for this value")
      .def_prop_ro("debug_loc_line",
                   [](PyFunction &f) { return LLVMGetDebugLocLine(f.get()); },
                   "Return the line number of the debug location for this value")
      .def_prop_ro("arg_num",
                   [](PyFunction &self) {
                     return LLVMCountParams(self.get());
                   })
      .def_prop_ro("first_arg",
                   [](PyFunction &self) -> optional<PyArgument> {
                     auto res = LLVMGetFirstParam(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyArgument);
                   })
      .def_prop_ro("last_arg",
                   [](PyFunction &self) -> optional<PyArgument> {
                     auto res = LLVMGetLastParam(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyArgument);
                   })
      .def_prop_ro("args",
                   [](PyFunction &self) {
                     unsigned param_num = LLVMCountParams(self.get());
                     std::vector<LLVMValueRef> params(param_num);
                     LLVMGetParams(self.get(), params.data());
                     WRAP_VECTOR_FROM_DEST(PyArgument, param_num, res, params);
                     return res;
                   })
      .def("get_arg",
           [](PyFunction &self, unsigned index) {
             return PyArgument(LLVMGetParam(self.get(), index));
           },
           "index"_a,
           "Obtain the parameter at the specified index.\n\n"
           "Parameters are indexed from 0.")
      .def("destory", // TODO test
           [](PyFunction &self) {
             return LLVMDeleteFunction(self.get());
           },
           "Remove a function from its containing module and deletes it.\n\n"
           "Note you shouldn't use the the variable afterwards!")
      .def("add_attribute_at_index",
           [](PyFunction &self, LLVMAttributeIndex idx, PyAttribute attr) {
             return LLVMAddAttributeAtIndex(self.get(), idx, attr.get());
           },
           "index"_a, "attr"_a)
      .def("append_existing_basic_block",
           [](PyFunction &self, PyBasicBlock bb) {
             return LLVMAppendExistingBasicBlock(self.get(), bb.get());
           },
           "basic_block"_a,
           "Append the given basic block to the basic block list of the given function.")
      .def("append_basic_block",
           [](PyFunction &self, const char *name) {
             return PyBasicBlock(LLVMAppendBasicBlock(self.get(), name));
           },
           "name"_a = "",
           "Append a basic block to the end of a function using the global context.")
      .def("get_attribute_count_at_index",
           [](PyFunction &self, LLVMAttributeIndex idx) {
             return LLVMGetAttributeCountAtIndex(self.get(), idx);
           },
           "index"_a)
      .def("get_attributes_at_index",
           [](PyFunction &self, LLVMAttributeIndex idx) {
             unsigned cnt = LLVMGetAttributeCountAtIndex(self.get(), idx);
             std::vector<LLVMAttributeRef> attrs(cnt);
             LLVMGetAttributesAtIndex(self.get(), idx, attrs.data());
             WRAP_VECTOR_FROM_DEST_AUTO(PyAttribute, cnt, res, attrs);
             return res;
           },
           "index"_a)
      .def("get_enum_attribute_at_index",
           [](PyFunction &self, LLVMAttributeIndex idx, unsigned kindID) {
             return PyEnumAttribute(LLVMGetEnumAttributeAtIndex(self.get(), idx, kindID));
           },
           "index"_a, "kind"_a)
      .def("get_string_attribute_at_index",
           [](PyFunction &self, LLVMAttributeIndex idx, std::string &kind) {
             return PyStringAttribute(LLVMGetStringAttributeAtIndex
                                        (self.get(), idx, kind.c_str(), kind.size()));
           },
           "index"_a, "kind"_a)
      .def("remove_enum_attribute_at_index",
           [](PyFunction &self, LLVMAttributeIndex idx, unsigned kindID) {
             return LLVMRemoveEnumAttributeAtIndex(self.get(), idx, kindID);
           },
           "index"_a, "kind"_a)
      .def("remove_string_attribute_at_index",
           [](PyFunction &self, LLVMAttributeIndex idx, std::string &kind) {
             return LLVMRemoveStringAttributeAtIndex
                      (self.get(), idx, kind.c_str(), kind.size());
           },
           "index"_a, "kind"_a)
      .def("add_target_dependent_attr",
           [](PyFunction &self, const char *A, const char *V) {
             return LLVMAddTargetDependentFunctionAttr(self.get(), A, V);
           },
           "A"_a, "V"_a,
           "Add a target-dependent attribute to a function");

  ArgumentClass
      .def("__repr__",
           [](PyArgument &self) {
             return gen_value_repr("Argument", self);
           })
      .def_prop_ro("parent",
                   [](PyArgument &self) {
                     return PyFunction(LLVMGetParamParent(self.get()));
                   },
                   "Obtain the function to which this argument belongs.")
      .def_prop_ro("next",
                   [](PyArgument &self) -> std::optional<PyArgument> {
                     auto res = LLVMGetNextParam(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyArgument);
                   })
      .def_prop_ro("prev",
                   [](PyArgument &self) -> std::optional<PyArgument> {
                     auto res = LLVMGetPreviousParam(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyArgument);
                   })
      .def("set_alignment",
           [](PyArgument &self, unsigned Align) {
             return LLVMSetParamAlignment(self.get(), Align);
           });


  UserClass
      .def("__repr__",
           [](PyUser &self) {
             return gen_value_repr("User", self);
           })
      .def("get_operand",
           [](PyUser &u, unsigned index) {
             return PyValueAuto(LLVMGetOperand(u.get(), index));
           },
           "index"_a,
           "Obtain an operand at a specific index.")
      .def("get_operand_use",
           [](PyUser &u, unsigned index) {
             return PyUse(LLVMGetOperandUse(u.get(), index));
           },
           "Obtain the use of an operand at a specific index")
      .def("set_operand",
           [](PyUser &u, unsigned index, PyValue &v) {
             return LLVMSetOperand(u.get(), index, v.get());
           },
           "index"_a, "value"_a,
           "Set an operand at a specific index")
       .def_prop_ro("operands_num",
                    [](PyUser &u) {
                      return LLVMGetNumOperands(u.get());
                    });

  ConstantClass
      .def("__repr__",
           [](PyConstant &self) {
             return gen_value_repr("Constant", self);
           })
      // note these constructors are also available in TypeClass
      .def_static("null",
                  [](PyType &t) {
                    return PyValueAuto(LLVMConstNull(t.get()));
                  }, "type"_a,
                  "Obtain a constant value referring to the null instance of the type.")
      .def_static("pointer_null",
                  [](PyType &t) {
                    return PyValueAuto(LLVMConstPointerNull(t.get()));
                  }, "type"_a,
                  "Obtain a constant that is a constant pointer pointing to NULL for the"
                  "type.")
      .def_static("all_ones",
                  [](PyTypeInt &t) {
                    return PyValueAuto(LLVMConstAllOnes(t.get()));
                  }, "type"_a,
                  "Obtain a constant value referring to the instance of the type"
                  "consisting of all ones.")
      .def_static("undef",
                  [](PyType &t) {
                    return PyUndefValue(LLVMGetUndef(t.get()));
                  }, "type"_a,
                  "Obtain a constant value referring to an undefined value of a type.")
      .def_static("poison",
                  [](PyType &t) {
                    return PyPoisonValue(LLVMGetPoison(t.get()));
                  }, "type"_a,
                  "Obtain a constant value referring to a poison value of a type.")
      .def_prop_ro("is_null",
                   [](PyConstant &c) { return LLVMIsNull(c.get()) != 0; });

  ConstantIntClass
      .def("__repr__",
           [](PyConstantInt &self) {
             return gen_value_repr("ConstantInt", self);
           })
      .def("__init__",
           [](PyConstantInt *c, PyTypeInt &t, unsigned long long N, bool SignExtend) {
             new (c) PyConstantInt(LLVMConstInt(t.get(), N, SignExtend));
           },
           "int_type"_a, "value"_a, "sign_extend"_a,
           "Original Function: LLVMConstInt.\n"
           "Obtain a constant value for an integer type.\n\n"
           "Parameters:\n--------"
           "int_type: IntTy Integer type to obtain value of.\n"
           "value: The value the returned instance should refer to.\n"
           "sign_extend: Whether to sign extend the produced value.")
      .def("__init__",
           [](PyConstantInt *c, PyTypeInt &t, unsigned NumWords, const uint64_t Words[]) {
             auto res = LLVMConstIntOfArbitraryPrecision
                          (t.get(), NumWords, Words);
             new (c) PyConstantInt(res);
           },
           "int_type"_a, "num_words"_a, "words"_a,
           "Origin Function: LLVMConstIntOfArbitraryPrecision.\n"
           "Obtain a constant value for an integer of arbitrary precision.")
      // LLVMConstIntOfString is discarded in favor of LLVMConstIntOfStringAndSize 
      .def("__init__", 
           [](PyConstantInt *c, PyTypeInt &t, std::string &text, uint8_t Radix) {
             new (c) PyConstantInt(LLVMConstIntOfStringAndSize
                                     (t.get(), text.c_str(), text.size(),
                                      Radix));
           },
           "int_type"_a, "text"_a, "radix"_a,
           "Original Function: LLVMConstIntOfStringAndSize\n"
           "Obtain a constant value for an integer parsed from a string.")
      .def_prop_ro("zext",
                   [](PyConstantInt &c) {
                     return LLVMConstIntGetZExtValue(c.get());
                   },
                   "Obtain the zero extended value.")
      .def_prop_ro("sext",
                   [](PyConstantInt &c) {
                     return LLVMConstIntGetSExtValue(c.get());
                   },
                   "Obtain the sign extended value.");

  ConstantFPClass
      .def("__repr__",
           [](PyConstantFP &self) {
             return gen_value_repr("ConstantFP", self);
           })
      .def("__init__",
           [](PyConstantFP *c, PyTypeReal &t, double value) {
             new (c) PyConstantFP(LLVMConstReal(t.get(), value));
           },
           "real_type"_a, "value"_a,
           "Original Function: LLVMConstReal.\n"
           "Obtain a constant value referring to a double floating point value.\n\n")
      // LLVMConstRealOfString is discarded in favor of LLVMConstRealOfStringAndSize
      .def("__init__",
           [](PyConstantFP *c, PyTypeReal &t, std::string text) {
             new (c) PyConstantFP(LLVMConstRealOfStringAndSize
                                    (t.get(), text.c_str(), text.size()));
           },
           "type"_a, "text"_a,
           "Original Function: LLVMConstRealOfStringAndSize\n"
           "Obtain a constant for a floating point value parsed from a string.")
      .def_prop_ro("double",
                   [](PyConstantFP &c) {
                     LLVMBool losesInfo;
                     double res = LLVMConstRealGetDouble(c.get(), &losesInfo);
                     return std::make_tuple(res, losesInfo);
                   },
                   "Obtain the double value for an floating point constant value.\n\n"
                   "The returned value is a tuple, with first one as the double"
                   "value and the second one indicating whether the some precision"
                   "was lost in the conversion.");

  ConstantDataArrayClass
      .def("__repr__",
           [](PyConstantDataArray &self) {
             return gen_value_repr("ConstantDataArray", self);
           })
       .def("__str__",
            [](PyConstantDataArray &c) {
              size_t len;
              auto str = LLVMGetAsString(c.get(), &len);
              return std::string(str, len);
            },
            "Get the given constant data sequential as a string.")
      .def_static("create_string_in_context",
                  [](PyContext &cxt, std::string &str, bool DontNullTerminate) {
                    return PyConstantDataArray
                             (LLVMConstStringInContext
                                (cxt.get(), str.c_str(), str.size(),
                                 DontNullTerminate));
                  },
                  "context"_a, "str"_a, "dont_null_terminate"_a,
                  "Create a ConstantDataSequential and initialize it"
                  "with a string.")
      .def_static("create_string",
                  [](std::string &str, bool DontNullTerminate) {
                    return PyConstantDataArray
                             (LLVMConstString
                                (str.c_str(), str.size(), DontNullTerminate));
                  },
                  "str"_a, "dont_null_terminate"_a,
                  "Create a ConstantDataSequential with string content"
                  "in the global context.")
       .def_prop_ro("is_string",
                    [](PyConstantDataArray &c) {
                      return LLVMIsConstantString(c.get()) != 0;
                    },
                    "Returns true if the specified constant is an array of i8.")
       .def("as_string",
            [](PyConstantDataArray &c) {
              size_t len;
              auto str = LLVMGetAsString(c.get(), &len);
              return std::string(str, len);
            },
            "Get the given constant data sequential as a string.");

  ConstantStructClass
      .def("__repr__",
           [](PyConstantStruct &self) {
             return gen_value_repr("ConstantStruct", self);
           })
      .def("__init__",
           [](PyConstantStruct *c, PyContext &cxt,
              std::vector<PyConstant> &vc, bool packed) {
             unsigned cnt = vc.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, vc, raw, cnt);
             new (c) PyConstantStruct(LLVMConstStructInContext
                                        (cxt.get(), raw.data(), cnt, packed));
           },
           "context"_a, "constant_values"_a, "packed"_a,
           "Create an anonymous ConstantStruct with the specified"
           "values.")
      .def("__init__",
           [](PyConstantStruct *c, PyType &structTy,
              std::vector<PyConstant> &constantVals) {
             unsigned cnt = constantVals.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, constantVals,
                                         raw, cnt);
             new (c) PyConstantStruct(LLVMConstNamedStruct(structTy.get(),
                                                           raw.data(), cnt));
           },
           "struct_type"_a, "constant_values"_a,
           "Create a non-anonymous ConstantStruct from values.")
      .def("get_element_at",
           [](PyConstantStruct &c, unsigned idx) -> std::optional<PyValue*>{
             auto res = LLVMGetAggregateElement(c.get(), idx);
             WRAP_OPTIONAL_RETURN(res, PyValueAuto);
           },
           "index"_a,
           "Returns null if the index is out of range, or it's not possible to "
           "determine the element (e.g., because the constant is a constant "
           "expression.)");
  

  ConstantArrayClass
      .def("__repr__",
           [](PyConstantArray &self) {
             return gen_value_repr("ConstantArray", self);
           })
       // LLVMConstArray is deprecated in favor of LLVMConstArray2
       .def("__init__",
            [](PyConstantArray *c, PyType &elemType,
               std::vector<PyConstant> ConstantVals) {
              uint64_t cnt = ConstantVals.size();
              UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, ConstantVals, raw, cnt);
              new (c) PyConstantArray(LLVMConstArray2(elemType.get(),
                                                      raw.data(), cnt));
            },
            "elem_type"_a, "constant_vals"_a,
            "Create a ConstantArray from values.")
       .def("get_element_at",
            [](PyConstantArray &c, unsigned idx) -> std::optional<PyValue*>{
              auto res = LLVMGetAggregateElement(c.get(), idx);
              WRAP_OPTIONAL_RETURN(res, PyValueAuto);
            },
            "index"_a,
            "Returns null if the index is out of range, or it's not possible to "
            "determine the element (e.g., because the constant is a constant "
            "expression.)");

  ConstantVectorClass
      .def("__repr__",
           [](PyConstantVector &self) {
             return gen_value_repr("ConstantVector", self);
           })
      .def("__init__",
           [](PyConstantVector *c, std::vector<PyConstant> values) {
             auto cnt = values.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, values, raw, cnt);
             new (c) PyConstantVector(LLVMConstVector(raw.data(), cnt));
           }, "values"_a)
      // LLVMGetElementAsConstant is deprecated in favor of  LLVMGetAggregateElement
      .def("get_element_at",
           [](PyConstantVector &c, unsigned idx) -> std::optional<PyValue*>{
             auto res = LLVMGetAggregateElement(c.get(), idx);
             WRAP_OPTIONAL_RETURN(res, PyValueAuto);
           },
           "index"_a,
           "Returns null if the index is out of range, or it's not possible to "
           "determine the element (e.g., because the constant is a constant "
           "expression.)");


  ConstantExprClass
      .def("__repr__",
           [](PyConstantExpr &self) {
             return gen_value_repr("ConstantExpr", self);
           })
      .def_prop_ro("opcode",
                   [](PyConstantExpr &c) {
                     return LLVMGetConstOpcode(c.get());
                   },
                   "value"_a)
      .def("get_icmp_predicate", // TODO test
           [](PyConstantExpr &self) {
             return LLVMGetICmpPredicate(self.get());
           },
           "Obtain the predicate of an instruction."
           "The opcode needs to be llvm::Instruction::ICmp.")
       .def("get_fcmp_predicate", // TODO test
            [](PyConstantExpr &self) {
              return LLVMGetFCmpPredicate(self.get());
            },
            "Obtain the float predicate of an instruction."
            "The opcode needs to be llvm::Instruction::FCmp.")
      .def_static("get_align_of",
                  [](PyType &t) {
                    return PyConstantExpr(LLVMAlignOf(t.get()));
                  },
                  "type"_a)
      .def_static("get_size_of",
                  [](PyType &t) {
                    return PyConstantExpr(LLVMSizeOf(t.get()));
                  },
                  "type"_a)
      .def_static("neg",
                  [](PyConstant &c) {
                    return PyValueAuto(LLVMConstNeg(c.get()));
                  },
                  "value"_a)
      .def_static("neg_nsw", // no signed wrap
                  [](PyConstant &c) {
                    return PyValueAuto(LLVMConstNSWNeg(c.get()));
                  },
                  "value"_a,
                  "LLVMConstNSWNeg")
      .def_static("neg_nuw", // no unsigned wrap
                  [](PyConstant &c) {
                    return PyValueAuto(LLVMConstNUWNeg(c.get()));
                  },
                  "value"_a,
                  "LLVMConstNUWNeg")
      .def_static("not_",
                  [](PyConstant &c) {
                    return PyValueAuto(LLVMConstNot(c.get()));
                  },
                  "value"_a)
  CONSTANT_EXPR_BIND_BINARY_OPS
      .def_static("icmp", // test passing ConstantFP type values
                  [](LLVMIntPredicate predicate, PyConstant &lhs, PyConstant &rhs) {
                    return PyValueAuto(LLVMConstICmp(predicate, lhs.get(), rhs.get()));
                  },
                  "predicate"_a, "lhs"_a, "rhs"_a)
      .def_static("fcmp", // test passing ConstantFP type values
                  [](LLVMRealPredicate predicate, PyConstant &lhs, PyConstant &rhs) {
                    return PyValueAuto(LLVMConstFCmp(predicate, lhs.get(), rhs.get()));
                  },
                  "predicate"_a, "lhs"_a, "rhs"_a)

      .def_static("gep2",
                  [](PyType &type, PyConstant &value, std::vector<PyValue> indices) {
                    unsigned num = indices.size();
                    UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, indices, raw, num);
                    return PyValueAuto(LLVMConstGEP2(type.get(), value.get(), raw.data(),
                                                     num));
                  },
                  "type"_a, "value"_a, "indices"_a)
      .def_static("gep2_in_bounds",
                  [](PyType &type, PyConstant &value, std::vector<PyValue> indices) {
                    unsigned num = indices.size();
                    UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, indices, raw, num);
                    return PyValueAuto(LLVMConstInBoundsGEP2(type.get(), value.get(),
                                                             raw.data(), num));
                  },
                  "type"_a, "value"_a, "indices"_a)

      .def_static("trunc",
                  [](PyConstant &value, PyType &toType) {
                    return PyValueAuto(LLVMConstTrunc(value.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("ptr2int",
                  [](PyConstant &c, PyType &toType) {
                    return PyValueAuto(LLVMConstIntToPtr(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("int2ptr",
                  [](PyConstant &c, PyType &toType) {
                    return PyValueAuto(LLVMConstPtrToInt(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("bit_cast",
                  [](PyConstant &c, PyType &toType) {
                    return PyValueAuto(LLVMConstBitCast(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("addr_space_cast",
                  [](PyConstant &c, PyType &toType) {
                    return PyValueAuto(LLVMConstAddrSpaceCast(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("trunc_or_bit_cast",
                  [](PyConstant &c, PyType &toType) {
                    return PyValueAuto(LLVMConstTruncOrBitCast(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("pointer_cast",
                  [](PyConstant &c, PyType &toType) {
                    return PyValueAuto(LLVMConstPointerCast(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("extract_element", // TODO test ConstantArray
                  [](PyConstantVector &vector, PyConstant &index) {
                    return PyValueAuto(LLVMConstExtractElement(vector.get(), index.get()));
                  },
                  "vector"_a, "index"_a)
      .def_static("insert_element",
                  [](PyConstantVector &vector, PyConstant &index, PyConstant &value) {
                    return PyValueAuto(LLVMConstInsertElement(vector.get(), value.get(),
                                                              index.get()));
                  },
                  "vector"_a, "index"_a, "value"_a)
      .def_static("shuffle_vector",
                  [](PyConstantVector &vectorA, PyConstantVector &vectorB,
                     PyConstant &mask) {
                    return PyValueAuto(LLVMConstShuffleVector
                                         (vectorA.get(), vectorB.get(), mask.get()));
                  },
                  "vector_a"_a, "vector_b"_a, "mask"_a)
      .def_static("block_address",
                  [](PyConstant &value, PyBasicBlock &bb) {
                    // This instruction returns an BlockAddress kind value, which
                    // isn't used elsewhere in LLVM C API
                    return PyValue(LLVMBlockAddress(value.get(), bb.get()));
                  });

  
  // LLVMConstInlineAsm is deprecated in favor of LLVMGetInlineAsm

  GlobalValueClass
      .def("__repr__",
           [](PyGlobalValue &self) {
             return gen_value_repr("GlobalValue", self);
           })
      .def_prop_rw("alignment",
                   [](PyGlobalValue &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PyGlobalValue &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_ro("parent",
                   [](PyGlobalValue &v) {
                     auto res = LLVMGetGlobalParent(v.get());
                     return PyModule(res);
                   })
      .def_prop_ro("is_declaration",
                   [](PyGlobalValue &v) {
                     return LLVMIsDeclaration(v.get()) != 0;
                   })
      .def_prop_rw("linkage",
                   [](PyGlobalValue &v) {
                     return LLVMGetLinkage(v.get());
                   },
                   [](PyGlobalValue &v, LLVMLinkage linkage) {
                     return LLVMSetLinkage(v.get(), linkage);
                   },
                   nb::for_setter
                     (nb::sig("def linkage(self, linkage: Linkage, /) -> None")))
      .def_prop_rw("section",
                   [](PyGlobalValue &v) {
                     return LLVMGetSection(v.get());
                   },
                   [](PyGlobalValue &v, const char *Section) {
                     return LLVMSetSection(v.get(), Section);
                   },
                   nb::for_setter
                     (nb::sig("def section(self, section: str, /) -> None")))
      .def_prop_rw("visibility",
                   [](PyGlobalValue &v) {
                     return LLVMGetVisibility(v.get());
                   },
                   [](PyGlobalValue &v, LLVMVisibility visibility) {
                     return LLVMSetVisibility(v.get(), visibility);
                   },
                   nb::for_setter
                     (nb::sig
                        ("def visibility(self, visibility: Visibility, /) -> None")))
      .def_prop_rw("dll_storage_class",
                   [](PyGlobalValue &v) {
                     return LLVMGetDLLStorageClass(v.get());
                   },
                   [](PyGlobalValue &v, LLVMDLLStorageClass cls) {
                     return LLVMSetDLLStorageClass(v.get(), cls);
                   })
      // LLVMHasUnnamedAddr is deprecated in favor of LLVMGetUnnamedAddress
      // LLVMSetUnnamedAddr is deprecated in favor of LLVMSetUnnamedAddress
      .def_prop_rw("unnamed_address",
                   [](PyGlobalValue &v) {
                     return LLVMGetUnnamedAddress(v.get());
                   },
                   [](PyGlobalValue &v, LLVMUnnamedAddr unnamedAddr) {
                     return LLVMSetUnnamedAddress(v.get(), unnamedAddr);
                   },
                   nb::for_setter
                     (nb::sig("def unnamed_address(self, addr: UnnamedAddr) -> None")))
      .def_prop_ro("value_type",
                   [](PyGlobalValue &v) {
                     return PyTypeAuto(LLVMGlobalGetValueType(v.get()));
                   },
                   "Returns the \"value type\" of a global value.  This differs "
                   "from the formal type of a global value which is always a pointer"
                   " type.");


  AllocaInstClass
      .def("__repr__",
           [](PyAllocaInst &self) {
             return "<AllocaInst>";
           })
      .def_prop_rw("alignment",
                   [](PyAllocaInst &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PyAllocaInst &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_ro("type",
                   [](PyAllocaInst &self) {
                     return PyTypeAuto(LLVMGetAllocatedType(self.get()));
                   });

  GetElementPtrInstClass
      .def("__repr__",
           [](PyGetElementPtrInst &self) {
             return "<GetElementPtrInst>";
           })
      .def_prop_ro("indices_num",
                   [](PyGetElementPtrInst &self) {
                     return LLVMGetNumIndices(self.get());
                   })
      .def_prop_rw("is_bounds",
                   [](PyGetElementPtrInst &self) {
                     return LLVMIsInBounds(self.get()) != 0;
                   },
                   [](PyGetElementPtrInst &self, bool inBounds) {
                     return LLVMSetIsInBounds(self.get(), inBounds);
                   })
      .def_prop_ro("source_element_type",
                   [](PyGetElementPtrInst &self) {
                     return PyTypeAuto(LLVMGetGEPSourceElementType(self.get()));
                   });

  PHINodeClass
      .def("__repr__",
           [](PyPHINode &self) {
             return "<PHINode>";
           })
      .def_prop_ro("incoming_num",
                   [](PyPHINode &self) {
                     return LLVMCountIncoming(self.get());
                   },
                   "Obtain the number of incoming basic blocks to a PHI node.")
      .def("add_incoming",
           [](PyPHINode &self, std::vector<PyValue> incomingValues,
              std::vector<PyBasicBlock> incomingBlocks, unsigned Count) {
             unsigned cnt = incomingValues.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, incomingValues,
                                         rawValues, cnt);
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMBasicBlockRef, incomingBlocks,
                                         rawBlocks, cnt);
             return LLVMAddIncoming(self.get(), rawValues.data(), rawBlocks.data(),
                                    cnt);
           })
      .def("get_incoming_value",
           [](PyPHINode &self, unsigned index) {
             return PyValueAuto(LLVMGetIncomingValue(self.get(), index));
           })
      .def("get_incoming_block",
           [](PyPHINode &self, unsigned index) {
             return PyBasicBlock(LLVMGetIncomingBlock(self.get(), index));
           });

  FenceInstClass
      .def("__repr__",
           [](PyFenceInst &self) {
             return "<FenceInst>";
           })
      .def_prop_rw("ordering",
                   [](PyFenceInst &self) {
                     return LLVMGetOrdering(self.get());
                   },
                   [](PyFenceInst &self, LLVMAtomicOrdering ordering) {
                     return LLVMSetOrdering(self.get(), ordering);
                   })
      .def_prop_rw("is_single_thread",
                   [](PyFenceInst &self) {
                     return LLVMIsAtomicSingleThread(self.get()) != 0;
                   },
                   [](PyFenceInst &self, bool isSingleThread) {
                     return LLVMSetAtomicSingleThread(self.get(), isSingleThread);
                   });

  LoadInstClass
      .def("__repr__",
           [](PyLoadInst &self) {
             return "<LoadInst>";
           })
      .def_prop_rw("alignment",
                   [](PyLoadInst &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PyLoadInst &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_rw("is_volatile",
                   [](PyLoadInst &self) {
                     return LLVMGetVolatile(self.get()) != 0;
                   },
                   [](PyLoadInst &self, bool isVolatile) {
                     return LLVMSetVolatile(self.get(), isVolatile);
                   })
      .def_prop_rw("ordering",
                   [](PyLoadInst &self) {
                     return LLVMGetOrdering(self.get());
                   },
                   [](PyLoadInst &self, LLVMAtomicOrdering ordering) {
                     return LLVMSetOrdering(self.get(), ordering);
                   })
      .def_prop_rw("is_single_thread",
                   [](PyLoadInst &self) {
                     return LLVMIsAtomicSingleThread(self.get()) != 0;
                   },
                   [](PyLoadInst &self, bool isSingleThread) {
                     return LLVMSetAtomicSingleThread(self.get(), isSingleThread);
                   });

  StoreInstClass
      .def("__repr__",
           [](PyStoreInst &self) {
             return "<StoreInst>";
           })
      .def_prop_rw("alignment",
                   [](PyStoreInst &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PyStoreInst &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_rw("is_volatile",
                   [](PyStoreInst &self) {
                     return LLVMGetVolatile(self.get()) != 0;
                   },
                   [](PyStoreInst &self, bool isVolatile) {
                     return LLVMSetVolatile(self.get(), isVolatile);
                   })
      .def_prop_rw("ordering",
                   [](PyStoreInst &self) {
                     return LLVMGetOrdering(self.get());
                   },
                   [](PyStoreInst &self, LLVMAtomicOrdering ordering) {
                     return LLVMSetOrdering(self.get(), ordering);
                   })
      .def_prop_rw("is_single_thread",
                   [](PyStoreInst &self) {
                     return LLVMIsAtomicSingleThread(self.get()) != 0;
                   },
                   [](PyStoreInst &self, bool isSingleThread) {
                     return LLVMSetAtomicSingleThread(self.get(), isSingleThread);
                   });
  
  AtomicRMWInstClass
      .def("__repr__",
           [](PyAtomicRMWInst &self) {
             return "<AtomicRMWInst>";
           })
      .def_prop_rw("alignment",
                   [](PyAtomicRMWInst &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PyAtomicRMWInst &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_rw("is_volatile",
                   [](PyAtomicRMWInst &self) {
                     return LLVMGetVolatile(self.get()) != 0;
                   },
                   [](PyAtomicRMWInst &self, bool isVolatile) {
                     return LLVMSetVolatile(self.get(), isVolatile);
                   })
      .def_prop_rw("ordering",
                   [](PyAtomicRMWInst &self) {
                     return LLVMGetOrdering(self.get());
                   },
                   [](PyAtomicRMWInst &self, LLVMAtomicOrdering ordering) {
                     return LLVMSetOrdering(self.get(), ordering);
                   })
      .def_prop_rw("bin_op",
                   [](PyAtomicRMWInst &self) {
                     return LLVMGetAtomicRMWBinOp(self.get());
                   },
                   [](PyAtomicRMWInst &self, LLVMAtomicRMWBinOp binOp) {
                     return LLVMSetAtomicRMWBinOp(self.get(), binOp);
                   })
      .def_prop_rw("is_single_thread",
                   [](PyAtomicRMWInst &self) {
                     return LLVMIsAtomicSingleThread(self.get()) != 0;
                   },
                   [](PyAtomicRMWInst &self, bool isSingleThread) {
                     return LLVMSetAtomicSingleThread(self.get(), isSingleThread);
                   });
  
  AtomicCmpXchgInstClass
      .def("__repr__",
           [](PyAtomicCmpXchgInst &self) {
             return "<AtomicCmpXchgInst>";
           })
      .def_prop_rw("alignment",
                   [](PyAtomicCmpXchgInst &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PyAtomicCmpXchgInst &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_rw("is_volatile",
                   [](PyLoadInst &self) {
                     return LLVMGetVolatile(self.get()) != 0;
                   },
                   [](PyLoadInst &self, bool isVolatile) {
                     return LLVMSetVolatile(self.get(), isVolatile);
                   })
      .def_prop_rw("is_weak",
                   [](PyAtomicCmpXchgInst &self) {
                     return LLVMGetWeak(self.get());
                   },
                   [](PyAtomicCmpXchgInst &self, bool isWeak) {
                     return LLVMSetWeak(self.get(), isWeak);
                   })
      .def_prop_rw("is_single_thread",
                   [](PyAtomicCmpXchgInst &self) {
                     return LLVMIsAtomicSingleThread(self.get()) != 0;
                   },
                   [](PyAtomicCmpXchgInst &self, bool isSingleThread) {
                     return LLVMSetAtomicSingleThread(self.get(), isSingleThread);
                   })
      .def_prop_rw("success_ordering",
                   [](PyAtomicCmpXchgInst &self) {
                     return LLVMGetCmpXchgSuccessOrdering(self.get());
                   },
                   [](PyAtomicCmpXchgInst &self, LLVMAtomicOrdering ordering) { 
                     return LLVMSetCmpXchgSuccessOrdering(self.get(), ordering);
                   })
      .def_prop_rw("failure_ordering",
                   [](PyAtomicCmpXchgInst &self) {
                     return LLVMGetCmpXchgFailureOrdering(self.get());
                   },
                   [](PyAtomicCmpXchgInst &self, LLVMAtomicOrdering ordering) {
                     return LLVMSetCmpXchgFailureOrdering(self.get(), ordering);
                   });

  GlobalObjectClass
      .def("__repr__",
           [](PyGlobalObject &self) {
             return gen_value_repr("GlobalObject", self);
           })
      .def("set_metadata",
           [](PyGlobalObject &g, unsigned kind, PyMetadata md) {
             return LLVMGlobalSetMetadata(g.get(), kind, md.get());
           },
           "Sets a metadata attachment, erasing the existing metadata attachment if"
           "it already exists for the given kind.")
      .def("erase_metadata",
           [](PyGlobalObject &g, unsigned kind) {
             return LLVMGlobalEraseMetadata(g.get(), kind);
           },
           "Erases a metadata attachment of the given kind if it exists.")
      .def("clear_metadata",
           [](PyGlobalObject &g) {
             return LLVMGlobalClearMetadata(g.get());
           },
           "Removes all metadata attachments from this value.")
      .def("copy_all_metadata",
           [](PyGlobalObject &g) {
             size_t num;
             LLVMValueMetadataEntries entries = LLVMGlobalCopyAllMetadata
                                                  (g.get(), &num);
             return PyMetadataEntries(entries, num);
           },
           "Retrieves an array of metadata entries representing the metadata attached to"
           "this value.");
  
  IndirectBrInstClass
      .def("__repr__",
           [](PyIndirectBrInst &self) {
             return "<IndirectBrInst>";
           })
      .def("add_destination",
           [](PyIndirectBrInst &self, PyBasicBlock &dest) {
             return LLVMAddDestination(self.get(), dest.get());
           },
           "dest"_a,
           "Add a destination to the indirectbr instruction.");

  LandingPadInstClass
      .def("__repr__",
           [](PyLandingPadInst &self) {
             return "<LandingPadInst>";
           })
      .def_prop_ro("num_clauses",
                   [](PyIndirectBrInst &self) {
                     return LLVMGetNumClauses(self.get());
                   })
      .def_prop_rw("is_cleanup",
                   [](PyLandingPadInst &self) {
                     return LLVMIsCleanup(self.get()) != 0;
                   },
                   [](PyLandingPadInst &self, bool val) {
                     return LLVMSetCleanup(self.get(), val);
                   })
      .def("get_clause",
           [](PyIndirectBrInst &self, unsigned index) {
             return PyValueAuto(LLVMGetClause(self.get(), index));
           },
           "index"_a)
      .def("add_clause",
           [](PyIndirectBrInst &self, PyConstant clause) {
             return LLVMAddClause(self.get(), clause.get());
           });

}
