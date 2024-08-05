#include "value.h"

#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/tuple.h>
#include <fmt/core.h>
#include <optional>
#include "../types_priv.h"
#include "../utils_priv.h"
#include "utils.h"
#include <llvm-c/Analysis.h>

namespace nb = nanobind;
using namespace nb::literals;

template <typename T>
using optional = std::optional<T>;


void bindValueClasses(nb::module_ &m) {
  auto ValueClass = nb::class_<PymValue, PymLLVMObject<PymValue, LLVMValueRef>>
                      (m, "Value", "Value");
  nb::class_<PymMetadataAsValue, PymValue>(m, "MetadataAsValue", "MetadataAsValue");
  auto MDNodeValueClass = nb::class_<PymMDNodeValue, PymMetadataAsValue>
                            (m, "MDNodeValue", "MDNodeValue");
  auto ValueAsMetadataValueClass = nb::class_<PymValueAsMetadataValue, PymMetadataAsValue>
                                     (m, "ValueAsMetadataValue", "ValueAsMetadataValue");
  auto MDStringValueClass = nb::class_<PymMDStringValue, PymMetadataAsValue>
                              (m, "MDStringValue", "MDStringValue");
    
  auto ArgumentClass = nb::class_<PymArgument, PymValue>(m, "Argument", "Argument");
  auto BasicBlockValueClass = nb::class_<PymBasicBlockValue, PymValue>(m, "BasicBlockValue", "BasicBlockValue");
  auto InlineAsmClass = nb::class_<PymInlineAsm, PymValue>(m, "InlineAsm", "InlineAsm");
  auto UserClass = nb::class_<PymUser, PymValue>(m, "User", "User");
  auto ConstantClass = nb::class_<PymConstant, PymUser>(m, "Constant", "Constant");
  auto ConstantArrayClass = nb::class_<PymConstantArray, PymConstant>(m, "ConstantArray", "ConstantArray");
  auto ConstantDataSequentialClass = nb::class_<PymConstantDataSequential, PymConstant>
                                       (m, "ConstantDataSequential", "ConstantDataSequential");
  auto ConstantDataArrayClass = nb::class_<PymConstantDataArray, PymConstantDataSequential>
                                  (m, "ConstantDataArray", "ConstantDataArray");
  auto ConstantDataVectorClass = nb::class_<PymConstantDataVector, PymConstantDataSequential>
                                   (m, "ConstantDataVector", "ConstantDataVector");
  auto ConstantExprClass = nb::class_<PymConstantExpr, PymConstant>(m, "ConstantExpr", "ConstantExpr");
  auto ConstantFPClass = nb::class_<PymConstantFP, PymConstant>(m, "ConstantFP", "ConstantFP");
  auto ConstantIntClass = nb::class_<PymConstantInt, PymConstant>(m, "ConstantInt", "ConstantInt");
  auto ConstantStructClass = nb::class_<PymConstantStruct, PymConstant>(m, "ConstantStruct", "ConstantStruct");
  auto ConstantVectorClass = nb::class_<PymConstantVector, PymConstant>(m, "ConstantVector", "ConstantVector");
  auto GlobalValueClass = nb::class_<PymGlobalValue, PymConstant>(m, "GlobalValue", "GlobalValue");
  auto GlobalAliasClass = nb::class_<PymGlobalAlias, PymGlobalValue>(m, "GlobalAlias", "GlobalAlias");
  auto GlobalObjectClass = nb::class_<PymGlobalObject, PymGlobalValue>(m, "GlobalObject", "GlobalObject");
  auto FunctionClass = nb::class_<PymFunction, PymGlobalObject>(m, "Function", "Function");
  auto GlobalVariableClass = nb::class_<PymGlobalVariable, PymGlobalObject>(m, "GlobalVariable", "GlobalVariable");
  auto GlobalIFuncClass = nb::class_<PymGlobalIFunc, PymGlobalObject>(m, "GlobalIFunc", "GlobalIFunc");
  auto UndefValueClass = nb::class_<PymUndefValue, PymConstant>(m, "UndefValue", "UndefValue");
  auto PoisonValueClass = nb::class_<PymPoisonValue, PymConstant>(m, "PoisonValue", "PoisonValue");
  auto InstructionClass = nb::class_<PymInstruction, PymUser>(m, "Instruction", "Instruction");
  auto CallBaseClass = nb::class_<PymCallBase, PymInstruction>(m, "CallBase", "CallBase");
  auto CallInstClass = nb::class_<PymCallInst, PymCallBase>(m, "CallInst", "CallInst");
  auto InvokeInstClass = nb::class_<PymInvokeInst, PymCallBase>(m, "InvokeInst", "InvokeInst");
  auto FCmpInstClass = nb::class_<PymFCmpInst, PymInstruction>(m, "FCmpInst", "FCmpInst");
  auto ICmpInstClass = nb::class_<PymICmpInst, PymInstruction>(m, "ICmpInst", "ICmpInst");
  auto GetElementPtrInstClass = nb::class_<PymGetElementPtrInst, PymInstruction>(m, "GetElementPtrInst", "GetElementPtrInst");
  auto PHINodeClass = nb::class_<PymPHINode, PymInstruction>(m, "PHINode", "PHINode");
  auto ShuffleVectorInstClass = nb::class_<PymShuffleVectorInst, PymInstruction>(m, "ShuffleVectorInst", "ShuffleVectorInst");
  auto StoreInstClass = nb::class_<PymStoreInst, PymInstruction>(m, "StoreInst", "StoreInst");
  auto BranchInstClass = nb::class_<PymBranchInst, PymInstruction>(m, "BranchInst", "BranchInst");
  auto ReturnInstClass = nb::class_<PymReturnInst, PymInstruction>(m, "ReturnInst", "ReturnInst");
  auto SwitchInstClass = nb::class_<PymSwitchInst, PymInstruction>(m, "SwitchInst", "SwitchInst");
  auto CatchSwitchInstClass = nb::class_<PymCatchSwitchInst, PymInstruction>
                                (m, "CatchSwitchInst", "CatchSwitchInst");
  auto CleanupReturnInstClass = nb::class_<PymCleanupReturnInst, PymInstruction>
                                  (m, "CleanupReturnInst", "CleanupReturnInst");
  auto FuncletPadInstClass = nb::class_<PymFuncletPadInst, PymInstruction>(m, "FuncletPadInst", "FuncletPadInst");
  auto CatchPadInstClass = nb::class_<PymCatchPadInst, PymFuncletPadInst>(m, "CatchPadInst", "CatchPadInst");
  auto AllocaInstClass = nb::class_<PymAllocaInst, PymInstruction>(m, "AllocaInst", "AllocaInst");
  auto LoadInstClass = nb::class_<PymLoadInst, PymInstruction>(m, "LoadInst", "LoadInst");
  auto AtomicCmpXchgInstClass = nb::class_<PymAtomicCmpXchgInst, PymInstruction>(m, "AtomicCmpXchgInst", "AtomicCmpXchgInst");
  auto AtomicRMWInstClass = nb::class_<PymAtomicRMWInst, PymInstruction>
                              (m, "AtomicRMWInst", "AtomicRMWInst");
  auto IEValueInstBaseClass = nb::class_<PymIEValueInstBase, PymInstruction>
                                (m, "IEValueInstBase", "IEValueInstBase");
  auto InsertValueInstClass = nb::class_<PymInsertValueInst, PymIEValueInstBase>
                                (m, "InsertValueInst", "InsertValueInst");
  auto ExtractValueInstClass = nb::class_<PymExtractValueInst, PymIEValueInstBase>
                                 (m, "ExtractValueInst", "ExtractValueInst");
  auto LandingPadInstClass = nb::class_<PymLandingPadInst, PymInstruction>
                               (m, "LandingPadInst", "LandingPadInst");
  auto IndirectBrInstClass = nb::class_<PymIndirectBrInst, PymInstruction>
                               (m, "IndirectBrInst", "IndirectBrInst");
  auto FenceInstClass = nb::class_<PymFenceInst, PymInstruction>
                          (m, "FenceInst", "FenceInst");

  MDNodeValueClass
      .def("__repr__",
           [](PymMDNodeValue &self) {
             return gen_value_repr("MDNodeValue", self);
           })
      // deprecated functions not binded:
      // LLVMMDNodeInContext, LLVMMDNode
      .def("as_metadata",
           [](PymMDStringValue &self) {
             return PymMDNode(LLVMValueAsMetadata(self.get()));
           })
      .def_prop_ro("num_operands",
                   [](PymMDNodeValue &self) {
                     return LLVMGetMDNodeNumOperands(self.get());
                   })
      .def_prop_ro("operands",
                   [](PymMDNodeValue &self) {
                     unsigned num = LLVMGetMDNodeNumOperands(self.get());
                     std::vector<LLVMValueRef> dest(num);
                     LLVMGetMDNodeOperands(self.get(), dest.data());
                     WRAP_VECTOR_FROM_DEST_AUTO(PymValue, num, res, dest);
                     return res;
                   },
                   "Obtain the given MDNode's operands.")
      .def_prop_ro("replace_operand_with",
                   [](PymMDNodeValue &self, unsigned index, PymMetadata &replacement) {
                     return LLVMReplaceMDNodeOperandWith
                              (self.get(), index, replacement.get());
                   },
                   "Replace an operand at a specific index in a llvm::MDNode value.");
  
  MDStringValueClass
      .def("__repr__",
           [](PymMDStringValue &self) {
             return gen_value_repr("MDStringValue", self);
           })
      // deprecated functions not binded:
      // LLVMMDStringInContext, LLVMMDString
      .def("as_metadata",
           [](PymMDStringValue &self) {
             return PymMDString(LLVMValueAsMetadata(self.get()));
           })
      .def_prop_ro("raw_string",
                   [](PymMDStringValue &v) {
                     unsigned len;
                     auto str = LLVMGetMDString(v.get(), &len);
                     return std::string(str, len);
                   },
                   "Obtain the underlying string from a MDString value.");

  ValueClass
      // TODO note there are many functions here that belongs `UserClass`
      .def("__repr__",
           [](PymValue &self) {
             auto raw = self.get();
             auto name = get_value_name(raw);
             auto kind = get_repr_str(LLVMGetValueKind(raw));
             return fmt::format("<Value name={} kind={} str='{}'>",
                                name, kind, get_value_str(raw));
           })
      .def("__str__",
           [](PymValue &self) {
             return get_value_str(self.get());
           })
      .def_prop_ro("type",
                   [](PymValue &v) { return PymTypeAuto(LLVMTypeOf(v.get())); })
      .def_prop_ro("kind",
                   [](PymValue &v) { return LLVMGetValueKind(v.get()); })
      // NOTE LLVMSetValueName and LLVMGetValueName are depreciated
      .def_prop_rw("name",
                   [](PymValue &v) {
                     return get_value_name(v.get());
                   },
                   [](PymValue &v, std::string &name) {
                     return LLVMSetValueName2(v.get(), name.c_str(), name.size());
                   })
      .def_prop_ro("is_constant",
                   [](PymValue &v) { return LLVMIsConstant(v.get()) != 0; })
      .def_prop_ro("is_undef",
                   [](PymValue &v) { return LLVMIsUndef(v.get()) != 0; })
      .def_prop_ro("is_poisonous",
                   [](PymValue &v) { return LLVMIsPoison(v.get()) != 0;})
      .def_prop_ro("first_use",
                   [](PymValue &v) -> optional<PymUse> {
                     auto res = LLVMGetFirstUse(v.get());
                     WRAP_OPTIONAL_RETURN(res, PymUse);
                   })
      .def_prop_ro("uses",
                   [](PymValue &self) {
                     return PymUseIterator(PymUse(LLVMGetFirstUse(self.get())));
                   })
      .def("as_metadata",
           [](PymValue &self) {
             return PymValueAsMetadata(LLVMValueAsMetadata(self.get()));
           })
      .def("dump",
           [](PymValue &v) { return LLVMDumpValue(v.get()); },
           "Dump a representation of a value to stderr.")
      .def_static("replace_all_uses_with",
                  [](PymValue &oldVal, PymValue &newVal) {
                    return LLVMReplaceAllUsesWith(oldVal.get(), newVal.get());
                  },
                  "Replace all uses of a value with another one.")
  PY_FOR_EACH_VALUE_SUBCLASS(PY_DECLARE_VALUE_CAST)
      .def("to_BasicBlockValue",
           [](PymValue &self) -> optional<PymBasicBlockValue> {
             auto res = LLVMIsABasicBlock(self.get());
             WRAP_OPTIONAL_RETURN(res, PymBasicBlockValue);
           })
      .def("to_MDNodeValue",
           [](PymValue &self) -> optional<PymMDNodeValue> {
             auto res = LLVMIsAMDNode(self.get());
             WRAP_OPTIONAL_RETURN(res, PymMDNodeValue);
           })
      .def("to_ValueAsMetadataValue",
           [](PymValue &self) -> optional<PymValueAsMetadataValue> {
             auto res = LLVMIsAValueAsMetadata(self.get());
             WRAP_OPTIONAL_RETURN(res, PymValueAsMetadataValue);
           })
      .def("to_MDStringValue",
           [](PymValue &self) -> optional<PymMDStringValue> {
             auto res = LLVMIsAMDString(self.get());
             WRAP_OPTIONAL_RETURN(res, PymMDStringValue);
           })
      // TODO test whether these property are only true to BasicBlockValue
      .def_prop_ro("is_basic_block",
                   [](PymValue &self) {
                     return LLVMValueIsBasicBlock(self.get()) != 0;
                   })
      .def("as_basic_block",
           [](PymValue &self) {
             return PymBasicBlock(LLVMValueAsBasicBlock(self.get()));
           });

  PoisonValueClass
      .def("__repr__",
           [](PymPoisonValue &self) {
             return gen_value_repr("PoisonValue", self);
           })
      .def("__init__",
           [](PymPoisonValue *p, PymType &t) {
             new (p) PymPoisonValue(LLVMGetPoison(t.get()));
           },
           "type"_a,
           "Obtain a constant value referring to a poison value of a type.");

  
  UndefValueClass
      .def("__repr__",
           [](PymUndefValue &self) {
             return gen_value_repr("UndefValue", self);
           })

      .def("__init__",
           [](PymUndefValue *uv, PymType &t) {
             new (uv) PymUndefValue(LLVMGetUndef(t.get()));
           },
           "type"_a,
           "Obtain a constant value referring to an undefined value of a type.");
  

  ShuffleVectorInstClass
      .def("__repr__",
           [](PymShuffleVectorInst &self) {
             return "<ShuffleVectorInst>";
           })
      .def_prop_ro("mask_elems_num",
                   [](PymShuffleVectorInst &self) {
                     return LLVMGetNumMaskElements(self.get());
                   })
      .def_static("get_undef_mask_elem",
                  [](PymShuffleVectorInst &self) {
                    return LLVMGetUndefMaskElem();
                  },
                  "returns a constant that specifies that the result of a "
                  "ShuffleVectorInst is undefined.")
      .def("get_mask_value",
           [](PymValue &self, unsigned index) {
             return LLVMGetMaskValue(self.get(), index);
           },
           "index"_a,
           "Get the mask value at position index in the mask of the ShuffleVector"
           "instruction.\n\n"
           "Returns the result of LLVMGetUndefMaskElem() if the mask value is"
           "poison at that position.");

  
  CatchPadInstClass
      .def("__repr__",
           [](PymCatchPadInst &self) {
             return "<CatchPadInst>";
           })
      .def_prop_rw("parent",
                   [](PymCatchPadInst &self) {
                     return PymCatchSwitchInst(LLVMGetParentCatchSwitch(self.get()));
                   },
                   [](PymCatchPadInst &self, PymCatchSwitchInst catchSwitch) {
                     return LLVMSetParentCatchSwitch(self.get(), catchSwitch.get());
                   });
  

  

  GlobalIFuncClass
      .def("__repr__",
           [](PymGlobalIFunc &self) {
             return "<GlobalIFunc>";
           })
      .def_prop_ro("next",
                   [](PymGlobalIFunc &self) -> optional<PymGlobalIFunc> {
                     auto res = LLVMGetNextGlobalIFunc(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymGlobalIFunc);
                   })
      .def_prop_ro("prev",
                   [](PymGlobalIFunc &self) -> optional<PymGlobalIFunc> {
                     auto res = LLVMGetPreviousGlobalIFunc(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymGlobalIFunc);
                   })
      .def_prop_rw("resolver",
                   [](PymGlobalIFunc &self) -> optional<PymConstant> {
                     auto res = LLVMGetGlobalIFuncResolver(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymConstant);
                   },
                   [](PymGlobalIFunc &self, PymConstant resolver) {
                     return LLVMSetGlobalIFuncResolver(self.get(), resolver.get());
                   })
      .def("destory",
           [](PymGlobalIFunc &self) {
             return LLVMEraseGlobalIFunc(self.get());
           },
           "Remove a global indirect function from its parent module and delete it.\n\n"
           "You shouldn't use it anymore after removal.")
      .def("remove_from_parent",
           [](PymGlobalIFunc &self) {
             return LLVMRemoveGlobalIFunc(self.get());
           },
           "Remove a global indirect function from its parent module.\n\n"
           "This unlinks the global indirect function from its containing module but"
           "keeps it alive.");

  InlineAsmClass
      .def("__repr__",
           [](PymInlineAsm &self) {
             return "<InlineAsm>";
           })
      .def("get_inline_asm",
           [](PymInlineAsm *iasm, PymType &ty, std::string asmString, std::string constraints,
              bool hasSideEffects, bool isAlignStack, LLVMInlineAsmDialect dialect,
              bool canThrow){
             new (iasm) PymInlineAsm(LLVMGetInlineAsm
                                      (ty.get(), asmString.c_str(), asmString.size(),
                                       constraints.c_str(), constraints.size(),
                                       hasSideEffects, isAlignStack, dialect, canThrow));
           },
           "type"_a, "asm"_a, "constraints"_a, "has_side_effects"_a,
           "is_align_stack"_a, "dialect"_a, "can_throw"_a,
           "Create the specified unique inline asm string.")
      .def_prop_ro("str",
                   [](PymInlineAsm &iasm) {
                     size_t len;
                     const char *str = LLVMGetInlineAsmAsmString(iasm.get(), &len);
                     return std::string(str, len);
                   },
                   "Get the template string used for an inline assembly snippet.")
      .def_prop_ro("constraint_str",
                   [](PymInlineAsm &iasm) {
                     size_t len;
                     const char *str = LLVMGetInlineAsmConstraintString(iasm.get(), &len);
                     return std::string(str, len);
                   },
                   "Get the raw constraint string for an inline assembly snippet.")
      .def_prop_ro("dialect",
                   [](PymInlineAsm &iasm) {
                     return LLVMGetInlineAsmDialect(iasm.get());
                   },
                   "Get the dialect used by the inline asm snippet.")
      .def_prop_ro("function_type",
                   [](PymInlineAsm &iasm) {
                     return PymTypeFunction(LLVMGetInlineAsmFunctionType(iasm.get()));
                   },
                   "Get the function type of the inline assembly snippet. "
                   "The same type that was passed into :func:`get_inline_asm` originally.")
      .def_prop_ro("has_side_effects",
                   [](PymInlineAsm &iasm) {
                     return LLVMGetInlineAsmHasSideEffects(iasm.get()) != 0;
                   },
                   "Get if the inline asm snippet has side effects.")
      .def_prop_ro("needs_aligned_stack",
                   [](PymInlineAsm &iasm) {
                     return LLVMGetInlineAsmNeedsAlignedStack(iasm.get()) != 0;
                   })
      .def_prop_ro("can_unwind",
                   [](PymInlineAsm &iasm) {
                     return LLVMGetInlineAsmCanUnwind(iasm.get()) != 0;
                   });

  InstructionClass
      .def("__repr__",
           [](PymInstruction &self) {
             auto opcode = LLVMGetInstructionOpcode(self.get());
             return fmt::format("<Instruction opcode={}>", get_repr_str(opcode));
           })
      .def_prop_ro("can_use_fast_math_flags",
                   [](PymInstruction &self) {
                     return LLVMCanValueUseFastMathFlags(self.get()) != 0;
                   },
                   "Check if a given value can potentially have fast math flags.\n"
                   "Will return true for floating point arithmetic instructions, and "
                   "for select, phi, and call instructions whose type is a floating "
                   "point type, or a vector or array thereof.\n"
                   "See https://llvm.org/docs/LangRef.html#fast-math-flags")
      .def_prop_ro("next",
                   [](PymInstruction &self) -> optional<PymInstruction> {
                     auto res = LLVMGetNextInstruction(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymInstruction);
                   },
                   "Obtain the instruction that occurs after the one specified.\n"
                   "The next instruction will be from the same basic block.\n\n"
                   "If this is the last instruction in a basic block, None will "
                   "be returned.")
      .def_prop_ro("prev",
                   [](PymInstruction &self) -> optional<PymInstruction> {
                     auto res = LLVMGetPreviousInstruction(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymInstruction);
                   },
                   "Obtain the instruction that occurred before this one.\n\n"
                   "If the instruction is the first instruction in a basic block, NULL"
                   "will be returned.")
      .def_prop_ro("has_metadata",
                   [](PymInstruction &self) {
                     return LLVMHasMetadata(self.get()) != 0;
                   })
      .def_prop_ro("debug_loc_directory",
                   [](PymInstruction &i) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocDirectory(i.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the directory of the debug location for this value")
      .def_prop_ro("debug_loc_filename",
                   [](PymInstruction &i) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocFilename(i.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the filename of the debug location for this value.")
      .def_prop_ro("debug_loc_line",
                   [](PymInstruction &i) { return LLVMGetDebugLocLine(i.get()); },
                   "Return the line number of the debug location for this value")
      .def_prop_ro("debug_loc_column",
                   [](PymInstruction &i) { return LLVMGetDebugLocColumn(i.get()); },
                   "Return the column number of the debug location for this value")
      .def_prop_ro("parent",
                   [](PymInstruction &self) -> optional<PymBasicBlock> {
                     auto res = LLVMGetInstructionParent(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymBasicBlock);
                   },
                   "Obtain the basic block to which an instruction belongs.")
      .def_prop_ro("opcode",
                   [](PymInstruction &self) {
                     return LLVMGetInstructionOpcode(self.get());
                   })
      .def_prop_ro("is_terminator",
                   [](PymInstruction &self) {
                     return LLVMIsATerminatorInst(self.get()) != nullptr;
                   })
      .def("clone",
           [](PymInstruction &self) -> optional<PymInstruction*> {
             auto res = LLVMInstructionClone(self.get());
             WRAP_OPTIONAL_RETURN(res, PymInstructionAuto);
           },
           "Create a copy of 'this' instruction that is identical in all ways"
           "except the following:\n"
           "  - The instruction has no parent"
           "  - The instruction has no name")
      .def("remove_from_parent",
           [](PymInstruction &self) {
             return LLVMInstructionRemoveFromParent(self.get());
           },
           "The instruction specified is removed from its containing building"
           "block but is kept alive.")
      .def("destory",
           [](PymInstruction &self) {
             return LLVMInstructionEraseFromParent(self.get());
           },
           "Remove and delete an instruction.\n\n"
           "The instruction specified is removed from its containing building"
           "block and then deleted.")
      .def("delete",
           [](PymInstruction &self) {
             return LLVMDeleteInstruction(self.get());
           },
           "Delete an instruction.\n\n"
           "The instruction specified is deleted. It must have previously been"
           "removed from its containing building block.")
      .def("get_metadata",
           [](PymInstruction &self, unsigned kindID) -> optional<PymValue*> {
             auto res = LLVMGetMetadata(self.get(), kindID);
             WRAP_OPTIONAL_RETURN(res, PymValueAuto);
           },
           "kind_id"_a)
      .def("set_metadata",
           [](PymInstruction &self, unsigned kindID, PymMetadataAsValue node) {
             return LLVMSetMetadata(self.get(), kindID, node.get());
           },
           "kind_id"_a, "value"_a)
      .def("get_all_metadata_no_debug_loc",
           [](PymInstruction &self) {
             size_t num;
             LLVMValueMetadataEntries entries =
               LLVMInstructionGetAllMetadataOtherThanDebugLoc(self.get(), &num);
             return PymMetadataEntries(entries, num);
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
                   [](PymInstruction &self) {
                     return LLVMGetNumSuccessors(self.get());
                   })
      .def("get_successor",
           [](PymInstruction &self, unsigned index) {
             return PymBasicBlock(LLVMGetSuccessor(self.get(), index));
           })
      .def("set_successor",
           [](PymInstruction &self, unsigned index, PymBasicBlock bb) {
             return LLVMSetSuccessor(self.get(), index, bb.get());
           })
       // TODO
      .def_static("get_nuw", 
                  [](PymInstruction &arithInst) {
                    return LLVMGetNUW(arithInst.get()) != 0;
                  },
                  "arith_inst"_a)
      .def_static("set_nuw",
                  [](PymInstruction &arithInst, bool hasNUW) {
                    return LLVMSetNUW(arithInst.get(), hasNUW);
                  },
                  "arith_inst"_a, "hasNUW"_a)
      .def_static("get_nsw",
                  [](PymInstruction &arithInst) {
                    return LLVMGetNSW(arithInst.get()) != 0;
                  },
                  "arithInst"_a)
      .def_static("set_nsw",
                  [](PymInstruction &arithInst, bool hasNSW) {
                    return LLVMSetNSW(arithInst.get(), hasNSW);
                  },
                  "arith_inst"_a, "hasNSW"_a)
      .def_static("get_exact",
                  [](PymInstruction &DivOrShrInst) {
                    return LLVMGetExact(DivOrShrInst.get()) != 0;
                  },
                  "div_or_shr_inst"_a)
      .def_static("set_exact",
                  [](PymInstruction &DivOrShrInst, bool isExact) {
                    return LLVMSetExact(DivOrShrInst.get(), isExact);
                  },
                  "div_or_shr_inst"_a, "is_exact"_a)
      .def_static("get_nneg",
                  [](PymInstruction &NonNegInst) {
                    return LLVMGetNNeg(NonNegInst.get()) != 0;
                  },
                  "non_neg_inst"_a,
                  "Gets if the instruction has the non-negative flag set.\n",
                  "Only valid for zext instructions.")
      .def_static("set_nned",
                  [](PymInstruction &NonNegInst, bool isNonNeg) {
                    return LLVMSetNNeg(NonNegInst.get(), isNonNeg);
                  },
                  "non_neg_inst"_a, "is_non_neg"_a,
                  "Sets the non-negative flag for the instruction.\n"
                  "Only valid for zext instructions.")
      .def_static("get_fast_math_flags",
                  [](PymInstruction &FPMathInst) {
                    return LLVMGetFastMathFlags(FPMathInst.get());
                  },
                  "fp_math_inst"_a,
                  "Get the flags for which fast-math-style optimizations are allowed "
                  "for this value.\n\n"
                  "Only valid on floating point instructions."
                  "See `can_use_fast_math_flags`.")
      .def_static("set_fast_math_flags",
                  [](PymInstruction &FPMathInst, LLVMFastMathFlags FMF) {
                    return LLVMSetFastMathFlags(FPMathInst.get(), FMF);
                  },
                  "fp_math_inst"_a, "fmf"_a,
                  "Sets the flags for which fast-math-style optimizations are allowed "
                  "for this value.\n\n"
                  "Only valid on floating point instructions.\n"
                  "See `can_use_fast_math_flags`.")
      .def_static("get_is_disjoint",
                  [](PymInstruction &inst) {
                    return LLVMGetIsDisjoint(inst.get()) != 0;
                  },
                  "inst"_a,
                  "Gets whether the instruction has the disjoint flag set.\n"
                  "Only valid for or instructions.")
      .def_static("set_is_disjoint",
                  [](PymInstruction &inst, bool isDisjoint) {
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
                   [](PymBranchInst &self) {
                     return LLVMIsConditional(self.get());
                   })
      .def_prop_rw("condition", // TODO test whether return NULL
                   [](PymBranchInst &self) {
                     return PymValueAuto(LLVMGetCondition(self.get()));
                   },
                   [](PymBranchInst &self, PymValue &cond) {
                     return LLVMSetCondition(self.get(), cond.get());
                   });

  SwitchInstClass
      .def("__repr__",
           [](PymSwitchInst &self) {
             return "<SwitchInst>";
           })
      .def_prop_ro("default_dest",
                   [](PymSwitchInst &self) {
                     return PymBasicBlock(LLVMGetSwitchDefaultDest(self.get()));
                   })
      .def_prop_ro("add_case",
                   [](PymSwitchInst &self, PymConstantInt &onVal, PymBasicBlock dest) {
                     return LLVMAddCase(self.get(), onVal.get(), dest.get());
                   });

  IEValueInstBaseClass
      .def_prop_ro("indices_num",
                   [](PymIEValueInstBase &self) {
                     return LLVMGetNumIndices(self.get());
                   })
      .def("indices",
           [](PymIEValueInstBase &self) {
             return LLVMGetIndices(self.get());
           });


  CallInstClass
      .def_prop_rw("is_tail_call",
                   [](PymCallInst &self) {
                     return LLVMIsTailCall(self.get()) != 0;
                   },
                   [](PymCallInst &self, bool isTailCall) {
                     return LLVMSetTailCall(self.get(), isTailCall);
                   })
      .def_prop_rw("tail_call_kind",
                   [](PymCallInst &self) {
                     return LLVMGetTailCallKind(self.get());
                   },
                   [](PymCallInst &self, LLVMTailCallKind kind) {
                     return LLVMSetTailCallKind(self.get(), kind);
                   });

  
  CallBaseClass
      .def_prop_ro("arg_num",
                   [](PymCallBase &self) {
                     return LLVMGetNumArgOperands(self.get());
                   })
      .def_prop_rw("call_conv",
                   [](PymCallBase &self) {
                     auto res = LLVMGetInstructionCallConv(self.get());
                     return static_cast<LLVMCallConv>(res);
                   },
                   [](PymCallBase &self, LLVMCallConv CC) {
                     return LLVMSetInstructionCallConv(self.get(), CC);
                   })
      .def_prop_ro("called_fn_type",
                   [](PymCallBase &self) {
                     return PymTypeFunction(LLVMGetCalledFunctionType(self.get()));
                   },
                   "Obtain the function type called by this instruction.")
      .def_prop_ro("called_value",
                   [](PymCallBase &self) {
                     return PymFunction(LLVMGetCalledValue(self.get()));
                   })
      .def_prop_ro("operand_bundles_num",
                   [](PymCallBase &self) {
                     return LLVMGetNumOperandBundles(self.get());
                   })
      .def("get_operand_bundle_at",
           [](PymCallBase &self, unsigned index) {
             return PymOperandBundle(LLVMGetOperandBundleAtIndex(self.get(), index));
           },
           "index"_a,
           "Obtain the operand bundle attached to this instruction at the given index.")
      .def("set_arg_alignment",
           [](PymCallBase &self, LLVMAttributeIndex idx, unsigned align) {
             return LLVMSetInstrParamAlignment(self.get(), idx, align);
           },
           "index"_a, "align"_a)
      .def("add_call_site_attribute",
           [](PymCallBase &self, LLVMAttributeIndex idx, PymAttribute &attr) {
             return LLVMAddCallSiteAttribute(self.get(), idx, attr.get());
           },
           "index"_a, "attr"_a)
      .def("get_call_site_attribute_count",
           [](PymCallBase &self, LLVMAttributeIndex idx) {
             return LLVMGetCallSiteAttributeCount(self.get(), idx);
           })
      .def("get_call_site_attributes",
           [](PymCallBase &self, LLVMAttributeIndex idx) {
             unsigned num = LLVMGetCallSiteAttributeCount(self.get(), idx);
             std::vector<LLVMAttributeRef> attrs(num);
             LLVMGetCallSiteAttributes(self.get(), idx, attrs.data());
             WRAP_VECTOR_FROM_DEST_AUTO(PymAttribute, num, res, attrs);
             return res;
           })
      .def("get_call_site_enum_attribute",
           [](PymCallBase &self, LLVMAttributeIndex idx, unsigned kindID) {
             return PymEnumAttribute(LLVMGetCallSiteEnumAttribute(self.get(), idx,
                                                                 kindID));
           })
      .def("get_call_site_string_attribute",
           [](PymCallBase &self, LLVMAttributeIndex idx, std::string &kind) {
             return PymStringAttribute(LLVMGetCallSiteStringAttribute
                                        (self.get(), idx, kind.c_str(), kind.size()));
           })
      .def("remove_call_site_enum_attribute",
           [](PymCallBase &self, LLVMAttributeIndex idx, unsigned kindID) {
             return LLVMRemoveCallSiteEnumAttribute(self.get(), idx, kindID);
           })
      .def("remove_call_site_string_attribute",
           [](PymCallBase &self, LLVMAttributeIndex idx, std::string &kind) {
             return LLVMRemoveCallSiteStringAttribute
                      (self.get(), idx, kind.c_str(), kind.size());
           });

  InvokeInstClass
      .def("__repr__",
           [](PymInvokeInst &self) {
             return "<InvokeInst>";
           })
      .def_prop_rw("normal_dest",
                   [](PymInvokeInst &self) {
                     return PymBasicBlock(LLVMGetNormalDest(self.get()));
                   },
                   [](PymInvokeInst &self, PymBasicBlock bb) {
                     return LLVMSetNormalDest(self.get(), bb.get());
                   })
      .def_prop_rw("unwind_dest",
                   [](PymInvokeInst &self) {
                     return PymBasicBlock(LLVMGetUnwindDest(self.get()));
                   },
                   [](PymInvokeInst &self, PymBasicBlock bb) {
                     return LLVMSetUnwindDest(self.get(), bb.get());
                   });

  CleanupReturnInstClass
      .def("__repr__",
           [](PymCleanupReturnInst &self) {
             return "<CleanupReturnInst>";
           })
      .def_prop_rw("unwind_dest",
                   [](PymCleanupReturnInst &self) {
                     return PymBasicBlock(LLVMGetUnwindDest(self.get()));
                   },
                   [](PymCleanupReturnInst &self, PymBasicBlock bb) {
                     return LLVMSetUnwindDest(self.get(), bb.get());
                   });

  CatchSwitchInstClass
      .def("__repr__",
           [](PymCatchSwitchInst &self) {
             return "<CatchSwitchInst>";
           })
      .def_prop_rw("unwind_dest",
                   [](PymCatchSwitchInst &self) {
                     return PymBasicBlock(LLVMGetUnwindDest(self.get()));
                   },
                   [](PymCatchSwitchInst &self, PymBasicBlock bb) {
                     return LLVMSetUnwindDest(self.get(), bb.get());
                   })
      .def_prop_ro("handlers_num",
                   [](PymCatchSwitchInst &self) {
                     return LLVMGetNumHandlers(self.get());
                   })
      .def_prop_ro("handlers",
                   [](PymCatchSwitchInst &self) {
                     unsigned num = LLVMGetNumHandlers(self.get());
                     std::vector<LLVMBasicBlockRef> handlers;
                     LLVMGetHandlers(self.get(), handlers.data());
                     WRAP_VECTOR_FROM_DEST(PymBasicBlock, num, res, handlers);
                     return res;
                   },
                   "Obtain the basic blocks acting as handlers for a catchswitch "
                   "instruction.")
      .def("add_handler",
           [](PymCatchSwitchInst &self, PymBasicBlock dest) {
             return LLVMAddHandler(self.get(), dest.get());
           });


  FuncletPadInstClass
      .def("__repr__",
           [](PymFuncletPadInst &self) {
             return "<FuncletPadInst>";
           })
      .def_prop_ro("arg_num",
                   [](PymCallBase &self) {
                     // TODO change it into enum type
                     return LLVMGetNumArgOperands(self.get());
                   })
      .def("get_arg_operand",
           [](PymFuncletPadInst &self, unsigned index) {
             return PymValueAuto(LLVMGetArgOperand(self.get(), index));
           },
           "index"_a)
      .def("set_arg_operand",
           [](PymFuncletPadInst &self, unsigned index, PymValue &value) {
             return LLVMSetArgOperand(self.get(), index, value.get());
           });

  ICmpInstClass
      .def("__repr__",
           [](PymICmpInst &self) {
             return "<ICmpInst>";
           })
      .def_prop_ro("predicate",
                   [](PymInstruction &self) {
                     return LLVMGetICmpPredicate(self.get());
                   },
                   "Obtain the predicate of an instruction.");

  FCmpInstClass
      .def("__repr__",
           [](PymFCmpInst &self) {
             return "<FCmpInst>";
           })
      .def_prop_ro("predicate",
                   [](PymInstruction &self) {
                     return LLVMGetFCmpPredicate(self.get());
                   },
                   "Obtain the float predicate of an instruction.");

  GlobalVariableClass
      .def("__repr__",
           [](PymGlobalVariable &self) {
             return gen_value_repr("GlobalVariable", self);
           })
      .def("__init__",
           [](PymGlobalVariable *g, PymModule &m, PymType &type, const char *name) {
             new (g) PymGlobalVariable(LLVMAddGlobal(m.get(), type.get(), name));
           },
           "module"_a, "type"_a, "name"_a = "")
      .def_prop_rw("initializer",
                   [](PymGlobalVariable &self) {
                     return PymValueAuto(LLVMGetInitializer(self.get()));
                   },
                   [](PymGlobalVariable &self, PymConstant &c) {
                     return LLVMSetInitializer(self.get(), c.get());
                   },
                   nb::for_setter(nb::sig("def initializer(self, value: Constant) -> None")))
      .def_prop_rw("is_thread_local",
                   [](PymGlobalVariable &self) {
                     return LLVMIsThreadLocal(self.get()) != 0;
                   },
                   [](PymGlobalVariable &self, bool isThreadLocal) {
                     return LLVMSetThreadLocal(self.get(), isThreadLocal);
                   })
      .def_prop_rw("is_global_constant",
                   [](PymGlobalVariable &self) {
                     return LLVMIsGlobalConstant(self.get()) != 0;
                   },
                   [](PymGlobalVariable &self, bool isConstant) {
                     return LLVMSetGlobalConstant(self.get(), isConstant);
                   },
                   nb::for_setter
                     (nb::sig
                        ("def is_global_constant(self, is_constant: bool, /) -> None")))
      .def_prop_rw("thread_local_mode",
                   [](PymGlobalVariable &self) {
                     return LLVMGetThreadLocalMode(self.get());
                   },
                   [](PymGlobalVariable &self, LLVMThreadLocalMode mode) {
                     return LLVMSetThreadLocalMode(self.get(), mode);
                   },
                   nb::for_setter
                     (nb::sig("def thread_local_mode(self, mode: ThreadLocalMode, /) "
                              "-> None")))
      .def_prop_rw("is_externally_initialized",
                   [](PymGlobalVariable &self) {
                     return LLVMIsExternallyInitialized(self.get()) != 0;
                   },
                   [](PymGlobalVariable &self, bool isExtInit) {
                     return LLVMSetExternallyInitialized(self.get(), isExtInit);
                   },
                   nb::for_setter
                     (nb::sig
                        ("def is_externally_initialized(self, is_ext_init: bool, /)"
                         " -> None")))
      .def_prop_ro("next",
                   [](PymGlobalVariable &v) -> std::optional<PymGlobalVariable> {
                     auto res = LLVMGetNextGlobal(v.get());
                     WRAP_OPTIONAL_RETURN(res, PymGlobalVariable);
                   })
      .def_prop_ro("prev",
                   [](PymGlobalVariable &v) -> std::optional<PymGlobalVariable> {
                     auto res = LLVMGetPreviousGlobal(v.get());
                     WRAP_OPTIONAL_RETURN(res, PymGlobalVariable);
                   })
      .def_prop_ro("debug_loc_directory",
                   [](PymGlobalVariable &v) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocDirectory(v.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the directory of the debug location for this value")
      .def_prop_ro("debug_loc_filename",
                   [](PymGlobalVariable &v) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocFilename(v.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the filename of the debug location for this value")
      .def_prop_ro("debug_loc_line",
                   [](PymGlobalVariable &v) { return LLVMGetDebugLocLine(v.get()); },
                   "Return the line number of the debug location for this value")
      // TODO test + more suitable way? (python side also do `del v`?)
      // but python pass variable by value...
      .def("destory", 
           [](PymGlobalVariable &self) {
             return LLVMDeleteGlobal(self.get());
           },
           "Delete this variable. You are not supposed to use this variable later.");


  
  GlobalAliasClass
      .def("__repr__",
           [](PymGlobalAlias &self) {
             return gen_value_repr("GlobalAlias", self);
           })
      .def("__init__",
           [](PymGlobalAlias *g, PymModule &self, PymType &valueType, unsigned addrSpace,
              PymValue aliasee, const char *name) {
             new (g) PymGlobalAlias(LLVMAddAlias2
                                     (self.get(), valueType.get(), addrSpace,
                                      aliasee.get(), name));
           },
           "module"_a, "value_type"_a, "addr_space"_a, "aliasee"_a, "name"_a = "",
           "Add a GlobalAlias with the given value type, address space and aliasee.")
      .def_prop_ro("next",
                   [](PymGlobalAlias &self) -> std::optional<PymGlobalAlias> {
                     auto res = LLVMGetNextGlobalAlias(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymGlobalAlias);
                   },
                   "Advance a GlobalAlias iterator to the next GlobalAlias.\n\n"
                   "Returns NULL if the iterator was already at the beginning and there are"
                   "no previous global aliases.")
       .def_prop_ro("prev",
                    [](PymGlobalAlias &self) -> std::optional<PymGlobalAlias> {
                      auto res = LLVMGetPreviousGlobalAlias(self.get());
                      WRAP_OPTIONAL_RETURN(res, PymGlobalAlias);
                    },
                    "Decrement a GlobalAlias iterator to the previous GlobalAlias.\n\n"
                    "Returns NULL if the iterator was already at the beginning and there are"
                    "no previous global aliases.")
       .def_prop_rw("aliasee",
                    [](PymGlobalAlias &self) {
                      return PymValueAuto(LLVMAliasGetAliasee(self.get()));
                    },
                    [](PymGlobalAlias &self, PymValue &aliasee) {
                      return LLVMAliasSetAliasee(self.get(), aliasee.get());
                    });
  

  FunctionClass
      .def("__repr__",
           [](PymFunction &self) {
             return gen_value_repr("Function", self);
           })
      .def("__init__",
           [](PymFunction *f, PymModule &m, PymTypeFunction &functionTy, std::string &name) {
             new (f) PymFunction(LLVMAddFunction(m.get(), name.c_str(), functionTy.get()));
           },
           "module"_a, "function_type"_a, "name"_a = "",
           "Add a function to a module under a specified name.")
      .def_prop_rw("call_conv",
                   [](PymFunction &self){
                     auto res = LLVMGetFunctionCallConv(self.get());
                     return static_cast<LLVMCallConv>(res);
                   },
                   [](PymFunction &self, LLVMCallConv CC) {
                     return LLVMSetFunctionCallConv(self.get(), CC);
                   })
      .def_prop_rw("gc",
                   [](PymFunction &self) {
                     return LLVMGetGC(self.get());
                   },
                   [](PymFunction &self, const char *name) {
                     return LLVMSetGC(self.get(), name);
                   },
                   nb::for_getter
                     ("Obtain the name of the garbage collector to use "
                      "during code generation."))
      .def_prop_ro("basic_blocks_num",
                   [](PymFunction &self) {
                     return LLVMCountBasicBlocks(self.get());
                   })
      .def_prop_ro("basic_blocks",
                   [](PymFunction &self) {
                     unsigned num = LLVMCountBasicBlocks(self.get());
                     std::vector<LLVMBasicBlockRef> BasicBlocks(num);
                     LLVMGetBasicBlocks(self.get(), BasicBlocks.data());
                     WRAP_VECTOR_FROM_DEST(PymBasicBlock, num,
                                           res, BasicBlocks);
                     return res;
                   })
      .def_prop_ro("first_basic_block",
                   [](PymFunction &self) -> optional<PymBasicBlock> {
                     auto res =  LLVMGetFirstBasicBlock(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymBasicBlock);
                   })
      .def_prop_ro("last_basic_block",
                   [](PymFunction &self) -> optional<PymBasicBlock> {
                     auto res = LLVMGetLastBasicBlock(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymBasicBlock);
                   })
      .def_prop_ro("entry_basic_block",
                   [](PymFunction &self) {
                     return PymBasicBlock(LLVMGetEntryBasicBlock(self.get()));
                   })
      .def_prop_ro("has_personality_fn",
                   [](PymFunction &self) {
                     return LLVMHasPersonalityFn(self.get()) != 0;
                   })
      .def_prop_rw("personality_fn",
                   [](PymFunction &self) {
                     return PymFunction(LLVMGetPersonalityFn(self.get()));
                   },
                   [](PymFunction &self, PymFunction fn) {
                     return LLVMSetPersonalityFn(self.get(), fn.get());
                   })
      .def_prop_ro("intrinsic_id",
                   [](PymFunction &self) {
                     return LLVMGetIntrinsicID(self.get());
                   })
      .def_prop_ro("next",
                   [](PymFunction &f) -> std::optional<PymFunction> {
                     auto res = LLVMGetNextFunction(f.get());
                     WRAP_OPTIONAL_RETURN(res, PymFunction);
                   },
                   "Advance a Function iterator to the next Function.\n\n"
                   "Returns NULL if the iterator was already at the end and there are no more"
                   "functions.")
      .def_prop_ro("previous",
                   [](PymFunction &f) -> std::optional<PymFunction> {
                     auto res = LLVMGetPreviousFunction(f.get());
                     WRAP_OPTIONAL_RETURN(res, PymFunction);
                   },
                   "Decrement a Function iterator to the previous Function.\n\n"
                   "Returns NULL if the iterator was already at the beginning and there are"
                   "no previous functions.")
      .def_prop_ro("debug_loc_directory",
                   [](PymFunction &f) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocDirectory(f.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the directory of the debug location for this value")
      .def_prop_ro("debug_loc_filename",
                   [](PymFunction &f) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocFilename(f.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the filename of the debug location for this value")
      .def_prop_ro("debug_loc_line",
                   [](PymFunction &f) { return LLVMGetDebugLocLine(f.get()); },
                   "Return the line number of the debug location for this value")
      .def_prop_ro("arg_num",
                   [](PymFunction &self) {
                     return LLVMCountParams(self.get());
                   })
      .def_prop_ro("first_arg",
                   [](PymFunction &self) -> optional<PymArgument> {
                     auto res = LLVMGetFirstParam(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymArgument);
                   })
      .def_prop_ro("last_arg",
                   [](PymFunction &self) -> optional<PymArgument> {
                     auto res = LLVMGetLastParam(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymArgument);
                   })
      .def_prop_ro("args",
                   [](PymFunction &self) {
                     unsigned param_num = LLVMCountParams(self.get());
                     std::vector<LLVMValueRef> params(param_num);
                     LLVMGetParams(self.get(), params.data());
                     WRAP_VECTOR_FROM_DEST(PymArgument, param_num, res, params);
                     return res;
                   })
      .def("verify",
           [](PymFunction &self, LLVMVerifierFailureAction action) -> optional<std::string>{
             char *outMsg;
             auto res = LLVMVerifyFunction(self.get(), action) == 0;
             if (!res) {
               if (outMsg) {
                 std::string errMsg(outMsg);
                 LLVMDisposeMessage(outMsg);
                 return errMsg;
               }
               return "";
             }
             return std::nullopt;
           },
           "action"_a,
           "Verifies that a single function is valid, taking the specified action. Usefu"
           "for debugging.")
      .def("get_arg",
           [](PymFunction &self, unsigned index) {
             return PymArgument(LLVMGetParam(self.get(), index));
           },
           "index"_a,
           "Obtain the parameter at the specified index.\n\n"
           "Parameters are indexed from 0.")
      .def("destory", // TODO test
           [](PymFunction &self) {
             return LLVMDeleteFunction(self.get());
           },
           "Remove a function from its containing module and deletes it.\n\n"
           "Note you shouldn't use the the variable afterwards!")
      .def("add_attribute_at_index",
           [](PymFunction &self, LLVMAttributeIndex idx, PymAttribute attr) {
             return LLVMAddAttributeAtIndex(self.get(), idx, attr.get());
           },
           "index"_a, "attr"_a)
      .def("append_existing_basic_block",
           [](PymFunction &self, PymBasicBlock bb) {
             return LLVMAppendExistingBasicBlock(self.get(), bb.get());
           },
           "basic_block"_a,
           "Append the given basic block to the basic block list of the given function.")
      .def("append_basic_block",
           [](PymFunction &self, const char *name) {
             return PymBasicBlock(LLVMAppendBasicBlock(self.get(), name));
           },
           "name"_a = "",
           "Append a basic block to the end of a function using the global context.")
      .def("get_attribute_count_at_index",
           [](PymFunction &self, LLVMAttributeIndex idx) {
             return LLVMGetAttributeCountAtIndex(self.get(), idx);
           },
           "index"_a)
      .def("get_attributes_at_index",
           [](PymFunction &self, LLVMAttributeIndex idx) {
             unsigned cnt = LLVMGetAttributeCountAtIndex(self.get(), idx);
             std::vector<LLVMAttributeRef> attrs(cnt);
             LLVMGetAttributesAtIndex(self.get(), idx, attrs.data());
             WRAP_VECTOR_FROM_DEST_AUTO(PymAttribute, cnt, res, attrs);
             return res;
           },
           "index"_a)
      .def("get_enum_attribute_at_index",
           [](PymFunction &self, LLVMAttributeIndex idx, unsigned kindID) {
             return PymEnumAttribute(LLVMGetEnumAttributeAtIndex(self.get(), idx, kindID));
           },
           "index"_a, "kind"_a)
      .def("get_string_attribute_at_index",
           [](PymFunction &self, LLVMAttributeIndex idx, std::string &kind) {
             return PymStringAttribute(LLVMGetStringAttributeAtIndex
                                        (self.get(), idx, kind.c_str(), kind.size()));
           },
           "index"_a, "kind"_a)
      .def("remove_enum_attribute_at_index",
           [](PymFunction &self, LLVMAttributeIndex idx, unsigned kindID) {
             return LLVMRemoveEnumAttributeAtIndex(self.get(), idx, kindID);
           },
           "index"_a, "kind"_a)
      .def("remove_string_attribute_at_index",
           [](PymFunction &self, LLVMAttributeIndex idx, std::string &kind) {
             return LLVMRemoveStringAttributeAtIndex
                      (self.get(), idx, kind.c_str(), kind.size());
           },
           "index"_a, "kind"_a)
      .def("add_target_dependent_attr",
           [](PymFunction &self, const char *A, const char *V) {
             return LLVMAddTargetDependentFunctionAttr(self.get(), A, V);
           },
           "A"_a, "V"_a,
           "Add a target-dependent attribute to a function");

  ArgumentClass
      .def("__repr__",
           [](PymArgument &self) {
             return gen_value_repr("Argument", self);
           })
      .def_prop_ro("parent",
                   [](PymArgument &self) {
                     return PymFunction(LLVMGetParamParent(self.get()));
                   },
                   "Obtain the function to which this argument belongs.")
      .def_prop_ro("next",
                   [](PymArgument &self) -> std::optional<PymArgument> {
                     auto res = LLVMGetNextParam(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymArgument);
                   })
      .def_prop_ro("prev",
                   [](PymArgument &self) -> std::optional<PymArgument> {
                     auto res = LLVMGetPreviousParam(self.get());
                     WRAP_OPTIONAL_RETURN(res, PymArgument);
                   })
      .def("set_alignment",
           [](PymArgument &self, unsigned Align) {
             return LLVMSetParamAlignment(self.get(), Align);
           });


  UserClass
      .def("__repr__",
           [](PymUser &self) {
             return gen_value_repr("User", self);
           })
      .def("get_operand",
           [](PymUser &u, unsigned index) {
             return PymValueAuto(LLVMGetOperand(u.get(), index));
           },
           "index"_a,
           "Obtain an operand at a specific index.")
      .def("get_operand_use",
           [](PymUser &u, unsigned index) {
             return PymUse(LLVMGetOperandUse(u.get(), index));
           },
           "Obtain the use of an operand at a specific index")
      .def("set_operand",
           [](PymUser &u, unsigned index, PymValue &v) {
             return LLVMSetOperand(u.get(), index, v.get());
           },
           "index"_a, "value"_a,
           "Set an operand at a specific index")
       .def_prop_ro("operands_num",
                    [](PymUser &u) {
                      return LLVMGetNumOperands(u.get());
                    });

  ConstantClass
      .def("__repr__",
           [](PymConstant &self) {
             return gen_value_repr("Constant", self);
           })
      // note these constructors are also available in TypeClass
      .def_static("null",
                  [](PymType &t) {
                    return PymValueAuto(LLVMConstNull(t.get()));
                  }, "type"_a,
                  "Obtain a constant value referring to the null instance of the type.")
      .def_static("pointer_null",
                  [](PymType &t) {
                    return PymValueAuto(LLVMConstPointerNull(t.get()));
                  }, "type"_a,
                  "Obtain a constant that is a constant pointer pointing to NULL for the"
                  "type.")
      .def_static("all_ones",
                  [](PymTypeInt &t) {
                    return PymValueAuto(LLVMConstAllOnes(t.get()));
                  }, "type"_a,
                  "Obtain a constant value referring to the instance of the type"
                  "consisting of all ones.")
      .def_static("undef",
                  [](PymType &t) {
                    return PymUndefValue(LLVMGetUndef(t.get()));
                  }, "type"_a,
                  "Obtain a constant value referring to an undefined value of a type.")
      .def_static("poison",
                  [](PymType &t) {
                    return PymPoisonValue(LLVMGetPoison(t.get()));
                  }, "type"_a,
                  "Obtain a constant value referring to a poison value of a type.")
      .def_prop_ro("is_null",
                   [](PymConstant &c) { return LLVMIsNull(c.get()) != 0; });

  ConstantIntClass
      .def("__repr__",
           [](PymConstantInt &self) {
             return gen_value_repr("ConstantInt", self);
           })
      .def("__init__",
           [](PymConstantInt *c, PymTypeInt &t, unsigned long long N, bool SignExtend) {
             new (c) PymConstantInt(LLVMConstInt(t.get(), N, SignExtend));
           },
           "int_type"_a, "value"_a, "sign_extend"_a,
           "Original Function: LLVMConstInt.\n"
           "Obtain a constant value for an integer type.\n\n"
           "Parameters:\n--------"
           "int_type: IntTy Integer type to obtain value of.\n"
           "value: The value the returned instance should refer to.\n"
           "sign_extend: Whether to sign extend the produced value.")
      .def("__init__",
           [](PymConstantInt *c, PymTypeInt &t, unsigned NumWords, const uint64_t Words[]) {
             auto res = LLVMConstIntOfArbitraryPrecision
                          (t.get(), NumWords, Words);
             new (c) PymConstantInt(res);
           },
           "int_type"_a, "num_words"_a, "words"_a,
           "Origin Function: LLVMConstIntOfArbitraryPrecision.\n"
           "Obtain a constant value for an integer of arbitrary precision.")
      // LLVMConstIntOfString is discarded in favor of LLVMConstIntOfStringAndSize 
      .def("__init__", 
           [](PymConstantInt *c, PymTypeInt &t, std::string &text, uint8_t Radix) {
             new (c) PymConstantInt(LLVMConstIntOfStringAndSize
                                     (t.get(), text.c_str(), text.size(),
                                      Radix));
           },
           "int_type"_a, "text"_a, "radix"_a,
           "Original Function: LLVMConstIntOfStringAndSize\n"
           "Obtain a constant value for an integer parsed from a string.")
      .def_prop_ro("zext",
                   [](PymConstantInt &c) {
                     return LLVMConstIntGetZExtValue(c.get());
                   },
                   "Obtain the zero extended value.")
      .def_prop_ro("sext",
                   [](PymConstantInt &c) {
                     return LLVMConstIntGetSExtValue(c.get());
                   },
                   "Obtain the sign extended value.");

  ConstantFPClass
      .def("__repr__",
           [](PymConstantFP &self) {
             return gen_value_repr("ConstantFP", self);
           })
      .def("__init__",
           [](PymConstantFP *c, PymTypeReal &t, double value) {
             new (c) PymConstantFP(LLVMConstReal(t.get(), value));
           },
           "real_type"_a, "value"_a,
           "Original Function: LLVMConstReal.\n"
           "Obtain a constant value referring to a double floating point value.\n\n")
      // LLVMConstRealOfString is discarded in favor of LLVMConstRealOfStringAndSize
      .def("__init__",
           [](PymConstantFP *c, PymTypeReal &t, std::string text) {
             new (c) PymConstantFP(LLVMConstRealOfStringAndSize
                                    (t.get(), text.c_str(), text.size()));
           },
           "type"_a, "text"_a,
           "Original Function: LLVMConstRealOfStringAndSize\n"
           "Obtain a constant for a floating point value parsed from a string.")
      .def_prop_ro("double",
                   [](PymConstantFP &c) {
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
           [](PymConstantDataArray &self) {
             return gen_value_repr("ConstantDataArray", self);
           })
       .def("__str__",
            [](PymConstantDataArray &c) {
              size_t len;
              auto str = LLVMGetAsString(c.get(), &len);
              return std::string(str, len);
            },
            "Get the given constant data sequential as a string.")
      .def_static("create_string_in_context",
                  [](PymContext &cxt, std::string &str, bool DontNullTerminate) {
                    return PymConstantDataArray
                             (LLVMConstStringInContext
                                (cxt.get(), str.c_str(), str.size(),
                                 DontNullTerminate));
                  },
                  "context"_a, "str"_a, "dont_null_terminate"_a,
                  "Create a ConstantDataSequential and initialize it"
                  "with a string.")
      .def_static("create_string",
                  [](std::string &str, bool DontNullTerminate) {
                    return PymConstantDataArray
                             (LLVMConstString
                                (str.c_str(), str.size(), DontNullTerminate));
                  },
                  "str"_a, "dont_null_terminate"_a,
                  "Create a ConstantDataSequential with string content"
                  "in the global context.")
       .def_prop_ro("is_string",
                    [](PymConstantDataArray &c) {
                      return LLVMIsConstantString(c.get()) != 0;
                    },
                    "Returns true if the specified constant is an array of i8.")
       .def("as_string",
            [](PymConstantDataArray &c) {
              size_t len;
              auto str = LLVMGetAsString(c.get(), &len);
              return std::string(str, len);
            },
            "Get the given constant data sequential as a string.");

  ConstantStructClass
      .def("__repr__",
           [](PymConstantStruct &self) {
             return gen_value_repr("ConstantStruct", self);
           })
      .def("__init__",
           [](PymConstantStruct *c, PymContext &cxt,
              std::vector<PymConstant> &vc, bool packed) {
             unsigned cnt = vc.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, vc, raw, cnt);
             new (c) PymConstantStruct(LLVMConstStructInContext
                                        (cxt.get(), raw.data(), cnt, packed));
           },
           "context"_a, "constant_values"_a, "packed"_a,
           "Create an anonymous ConstantStruct with the specified"
           "values.")
      .def("__init__",
           [](PymConstantStruct *c, PymType &structTy,
              std::vector<PymConstant> &constantVals) {
             unsigned cnt = constantVals.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, constantVals,
                                         raw, cnt);
             new (c) PymConstantStruct(LLVMConstNamedStruct(structTy.get(),
                                                           raw.data(), cnt));
           },
           "struct_type"_a, "constant_values"_a,
           "Create a non-anonymous ConstantStruct from values.")
      .def("get_element_at",
           [](PymConstantStruct &c, unsigned idx) -> std::optional<PymValue*>{
             auto res = LLVMGetAggregateElement(c.get(), idx);
             WRAP_OPTIONAL_RETURN(res, PymValueAuto);
           },
           "index"_a,
           "Returns null if the index is out of range, or it's not possible to "
           "determine the element (e.g., because the constant is a constant "
           "expression.)");
  

  ConstantArrayClass
      .def("__repr__",
           [](PymConstantArray &self) {
             return gen_value_repr("ConstantArray", self);
           })
       // LLVMConstArray is deprecated in favor of LLVMConstArray2
       .def("__init__",
            [](PymConstantArray *c, PymType &elemType,
               std::vector<PymConstant> ConstantVals) {
              uint64_t cnt = ConstantVals.size();
              UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, ConstantVals, raw, cnt);
              new (c) PymConstantArray(LLVMConstArray2(elemType.get(),
                                                      raw.data(), cnt));
            },
            "elem_type"_a, "constant_vals"_a,
            "Create a ConstantArray from values.")
       .def("get_element_at",
            [](PymConstantArray &c, unsigned idx) -> std::optional<PymValue*>{
              auto res = LLVMGetAggregateElement(c.get(), idx);
              WRAP_OPTIONAL_RETURN(res, PymValueAuto);
            },
            "index"_a,
            "Returns null if the index is out of range, or it's not possible to "
            "determine the element (e.g., because the constant is a constant "
            "expression.)");

  ConstantVectorClass
      .def("__repr__",
           [](PymConstantVector &self) {
             return gen_value_repr("ConstantVector", self);
           })
      .def("__init__",
           [](PymConstantVector *c, std::vector<PymConstant> values) {
             auto cnt = values.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, values, raw, cnt);
             new (c) PymConstantVector(LLVMConstVector(raw.data(), cnt));
           }, "values"_a)
      // LLVMGetElementAsConstant is deprecated in favor of  LLVMGetAggregateElement
      .def("get_element_at",
           [](PymConstantVector &c, unsigned idx) -> std::optional<PymValue*>{
             auto res = LLVMGetAggregateElement(c.get(), idx);
             WRAP_OPTIONAL_RETURN(res, PymValueAuto);
           },
           "index"_a,
           "Returns null if the index is out of range, or it's not possible to "
           "determine the element (e.g., because the constant is a constant "
           "expression.)");


  ConstantExprClass
      .def("__repr__",
           [](PymConstantExpr &self) {
             return gen_value_repr("ConstantExpr", self);
           })
      .def_prop_ro("opcode",
                   [](PymConstantExpr &c) {
                     return LLVMGetConstOpcode(c.get());
                   },
                   "value"_a)
      .def("get_icmp_predicate", // TODO test
           [](PymConstantExpr &self) {
             return LLVMGetICmpPredicate(self.get());
           },
           "Obtain the predicate of an instruction."
           "The opcode needs to be llvm::Instruction::ICmp.")
       .def("get_fcmp_predicate", // TODO test
            [](PymConstantExpr &self) {
              return LLVMGetFCmpPredicate(self.get());
            },
            "Obtain the float predicate of an instruction."
            "The opcode needs to be llvm::Instruction::FCmp.")
      .def_static("get_align_of",
                  [](PymType &t) {
                    return PymConstantExpr(LLVMAlignOf(t.get()));
                  },
                  "type"_a)
      .def_static("get_size_of",
                  [](PymType &t) {
                    return PymConstantExpr(LLVMSizeOf(t.get()));
                  },
                  "type"_a)
      .def_static("neg",
                  [](PymConstant &c) {
                    return PymValueAuto(LLVMConstNeg(c.get()));
                  },
                  "value"_a)
      .def_static("neg_nsw", // no signed wrap
                  [](PymConstant &c) {
                    return PymValueAuto(LLVMConstNSWNeg(c.get()));
                  },
                  "value"_a,
                  "LLVMConstNSWNeg")
      .def_static("neg_nuw", // no unsigned wrap
                  [](PymConstant &c) {
                    return PymValueAuto(LLVMConstNUWNeg(c.get()));
                  },
                  "value"_a,
                  "LLVMConstNUWNeg")
      .def_static("not_",
                  [](PymConstant &c) {
                    return PymValueAuto(LLVMConstNot(c.get()));
                  },
                  "value"_a)
  CONSTANT_EXPR_BIND_BINARY_OPS
      .def_static("icmp", // test passing ConstantFP type values
                  [](LLVMIntPredicate predicate, PymConstant &lhs, PymConstant &rhs) {
                    return PymValueAuto(LLVMConstICmp(predicate, lhs.get(), rhs.get()));
                  },
                  "predicate"_a, "lhs"_a, "rhs"_a)
      .def_static("fcmp", // test passing ConstantFP type values
                  [](LLVMRealPredicate predicate, PymConstant &lhs, PymConstant &rhs) {
                    return PymValueAuto(LLVMConstFCmp(predicate, lhs.get(), rhs.get()));
                  },
                  "predicate"_a, "lhs"_a, "rhs"_a)

      .def_static("gep2",
                  [](PymType &type, PymConstant &value, std::vector<PymValue> indices) {
                    unsigned num = indices.size();
                    UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, indices, raw, num);
                    return PymValueAuto(LLVMConstGEP2(type.get(), value.get(), raw.data(),
                                                     num));
                  },
                  "type"_a, "value"_a, "indices"_a)
      .def_static("gep2_in_bounds",
                  [](PymType &type, PymConstant &value, std::vector<PymValue> indices) {
                    unsigned num = indices.size();
                    UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, indices, raw, num);
                    return PymValueAuto(LLVMConstInBoundsGEP2(type.get(), value.get(),
                                                             raw.data(), num));
                  },
                  "type"_a, "value"_a, "indices"_a)

      .def_static("trunc",
                  [](PymConstant &value, PymType &toType) {
                    return PymValueAuto(LLVMConstTrunc(value.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("ptr2int",
                  [](PymConstant &c, PymType &toType) {
                    return PymValueAuto(LLVMConstIntToPtr(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("int2ptr",
                  [](PymConstant &c, PymType &toType) {
                    return PymValueAuto(LLVMConstPtrToInt(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("bit_cast",
                  [](PymConstant &c, PymType &toType) {
                    return PymValueAuto(LLVMConstBitCast(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("addr_space_cast",
                  [](PymConstant &c, PymType &toType) {
                    return PymValueAuto(LLVMConstAddrSpaceCast(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("trunc_or_bit_cast",
                  [](PymConstant &c, PymType &toType) {
                    return PymValueAuto(LLVMConstTruncOrBitCast(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("pointer_cast",
                  [](PymConstant &c, PymType &toType) {
                    return PymValueAuto(LLVMConstPointerCast(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("extract_element", // TODO test ConstantArray
                  [](PymConstantVector &vector, PymConstant &index) {
                    return PymValueAuto(LLVMConstExtractElement(vector.get(), index.get()));
                  },
                  "vector"_a, "index"_a)
      .def_static("insert_element",
                  [](PymConstantVector &vector, PymConstant &index, PymConstant &value) {
                    return PymValueAuto(LLVMConstInsertElement(vector.get(), value.get(),
                                                              index.get()));
                  },
                  "vector"_a, "index"_a, "value"_a)
      .def_static("shuffle_vector",
                  [](PymConstantVector &vectorA, PymConstantVector &vectorB,
                     PymConstant &mask) {
                    return PymValueAuto(LLVMConstShuffleVector
                                         (vectorA.get(), vectorB.get(), mask.get()));
                  },
                  "vector_a"_a, "vector_b"_a, "mask"_a)
      .def_static("block_address",
                  [](PymConstant &value, PymBasicBlock &bb) {
                    // This instruction returns an BlockAddress kind value, which
                    // isn't used elsewhere in LLVM C API
                    return PymValue(LLVMBlockAddress(value.get(), bb.get()));
                  });

  
  // LLVMConstInlineAsm is deprecated in favor of LLVMGetInlineAsm

  GlobalValueClass
      .def("__repr__",
           [](PymGlobalValue &self) {
             return gen_value_repr("GlobalValue", self);
           })
      .def_prop_rw("alignment",
                   [](PymGlobalValue &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PymGlobalValue &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_ro("parent",
                   [](PymGlobalValue &v) {
                     auto res = LLVMGetGlobalParent(v.get());
                     return PymModule(res);
                   })
      .def_prop_ro("is_declaration",
                   [](PymGlobalValue &v) {
                     return LLVMIsDeclaration(v.get()) != 0;
                   })
      .def_prop_rw("linkage",
                   [](PymGlobalValue &v) {
                     return LLVMGetLinkage(v.get());
                   },
                   [](PymGlobalValue &v, LLVMLinkage linkage) {
                     return LLVMSetLinkage(v.get(), linkage);
                   },
                   nb::for_setter
                     (nb::sig("def linkage(self, linkage: Linkage, /) -> None")))
      .def_prop_rw("section",
                   [](PymGlobalValue &v) {
                     return LLVMGetSection(v.get());
                   },
                   [](PymGlobalValue &v, const char *Section) {
                     return LLVMSetSection(v.get(), Section);
                   },
                   nb::for_setter
                     (nb::sig("def section(self, section: str, /) -> None")))
      .def_prop_rw("visibility",
                   [](PymGlobalValue &v) {
                     return LLVMGetVisibility(v.get());
                   },
                   [](PymGlobalValue &v, LLVMVisibility visibility) {
                     return LLVMSetVisibility(v.get(), visibility);
                   },
                   nb::for_setter
                     (nb::sig
                        ("def visibility(self, visibility: Visibility, /) -> None")))
      .def_prop_rw("dll_storage_class",
                   [](PymGlobalValue &v) {
                     return LLVMGetDLLStorageClass(v.get());
                   },
                   [](PymGlobalValue &v, LLVMDLLStorageClass cls) {
                     return LLVMSetDLLStorageClass(v.get(), cls);
                   })
      // LLVMHasUnnamedAddr is deprecated in favor of LLVMGetUnnamedAddress
      // LLVMSetUnnamedAddr is deprecated in favor of LLVMSetUnnamedAddress
      .def_prop_rw("unnamed_address",
                   [](PymGlobalValue &v) {
                     return LLVMGetUnnamedAddress(v.get());
                   },
                   [](PymGlobalValue &v, LLVMUnnamedAddr unnamedAddr) {
                     return LLVMSetUnnamedAddress(v.get(), unnamedAddr);
                   },
                   nb::for_setter
                     (nb::sig("def unnamed_address(self, addr: UnnamedAddr) -> None")))
      .def_prop_ro("value_type",
                   [](PymGlobalValue &v) {
                     return PymTypeAuto(LLVMGlobalGetValueType(v.get()));
                   },
                   "Returns the \"value type\" of a global value.  This differs "
                   "from the formal type of a global value which is always a pointer"
                   " type.");


  AllocaInstClass
      .def("__repr__",
           [](PymAllocaInst &self) {
             return "<AllocaInst>";
           })
      .def_prop_rw("alignment",
                   [](PymAllocaInst &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PymAllocaInst &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_ro("type",
                   [](PymAllocaInst &self) {
                     return PymTypeAuto(LLVMGetAllocatedType(self.get()));
                   });

  GetElementPtrInstClass
      .def("__repr__",
           [](PymGetElementPtrInst &self) {
             return "<GetElementPtrInst>";
           })
      .def_prop_ro("indices_num",
                   [](PymGetElementPtrInst &self) {
                     return LLVMGetNumIndices(self.get());
                   })
      .def_prop_rw("is_bounds",
                   [](PymGetElementPtrInst &self) {
                     return LLVMIsInBounds(self.get()) != 0;
                   },
                   [](PymGetElementPtrInst &self, bool inBounds) {
                     return LLVMSetIsInBounds(self.get(), inBounds);
                   })
      .def_prop_ro("source_element_type",
                   [](PymGetElementPtrInst &self) {
                     return PymTypeAuto(LLVMGetGEPSourceElementType(self.get()));
                   });

  PHINodeClass
      .def("__repr__",
           [](PymPHINode &self) {
             return "<PHINode>";
           })
      .def_prop_ro("incoming_num",
                   [](PymPHINode &self) {
                     return LLVMCountIncoming(self.get());
                   },
                   "Obtain the number of incoming basic blocks to a PHI node.")
      .def("add_incoming",
           [](PymPHINode &self, std::vector<PymValue> incomingValues,
              std::vector<PymBasicBlock> incomingBlocks, unsigned Count) {
             unsigned cnt = incomingValues.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, incomingValues,
                                         rawValues, cnt);
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMBasicBlockRef, incomingBlocks,
                                         rawBlocks, cnt);
             return LLVMAddIncoming(self.get(), rawValues.data(), rawBlocks.data(),
                                    cnt);
           })
      .def("get_incoming_value",
           [](PymPHINode &self, unsigned index) {
             return PymValueAuto(LLVMGetIncomingValue(self.get(), index));
           })
      .def("get_incoming_block",
           [](PymPHINode &self, unsigned index) {
             return PymBasicBlock(LLVMGetIncomingBlock(self.get(), index));
           });

  FenceInstClass
      .def("__repr__",
           [](PymFenceInst &self) {
             return "<FenceInst>";
           })
      .def_prop_rw("ordering",
                   [](PymFenceInst &self) {
                     return LLVMGetOrdering(self.get());
                   },
                   [](PymFenceInst &self, LLVMAtomicOrdering ordering) {
                     return LLVMSetOrdering(self.get(), ordering);
                   })
      .def_prop_rw("is_single_thread",
                   [](PymFenceInst &self) {
                     return LLVMIsAtomicSingleThread(self.get()) != 0;
                   },
                   [](PymFenceInst &self, bool isSingleThread) {
                     return LLVMSetAtomicSingleThread(self.get(), isSingleThread);
                   });

  LoadInstClass
      .def("__repr__",
           [](PymLoadInst &self) {
             return "<LoadInst>";
           })
      .def_prop_rw("alignment",
                   [](PymLoadInst &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PymLoadInst &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_rw("is_volatile",
                   [](PymLoadInst &self) {
                     return LLVMGetVolatile(self.get()) != 0;
                   },
                   [](PymLoadInst &self, bool isVolatile) {
                     return LLVMSetVolatile(self.get(), isVolatile);
                   })
      .def_prop_rw("ordering",
                   [](PymLoadInst &self) {
                     return LLVMGetOrdering(self.get());
                   },
                   [](PymLoadInst &self, LLVMAtomicOrdering ordering) {
                     return LLVMSetOrdering(self.get(), ordering);
                   })
      .def_prop_rw("is_single_thread",
                   [](PymLoadInst &self) {
                     return LLVMIsAtomicSingleThread(self.get()) != 0;
                   },
                   [](PymLoadInst &self, bool isSingleThread) {
                     return LLVMSetAtomicSingleThread(self.get(), isSingleThread);
                   });

  StoreInstClass
      .def("__repr__",
           [](PymStoreInst &self) {
             return "<StoreInst>";
           })
      .def_prop_rw("alignment",
                   [](PymStoreInst &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PymStoreInst &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_rw("is_volatile",
                   [](PymStoreInst &self) {
                     return LLVMGetVolatile(self.get()) != 0;
                   },
                   [](PymStoreInst &self, bool isVolatile) {
                     return LLVMSetVolatile(self.get(), isVolatile);
                   })
      .def_prop_rw("ordering",
                   [](PymStoreInst &self) {
                     return LLVMGetOrdering(self.get());
                   },
                   [](PymStoreInst &self, LLVMAtomicOrdering ordering) {
                     return LLVMSetOrdering(self.get(), ordering);
                   })
      .def_prop_rw("is_single_thread",
                   [](PymStoreInst &self) {
                     return LLVMIsAtomicSingleThread(self.get()) != 0;
                   },
                   [](PymStoreInst &self, bool isSingleThread) {
                     return LLVMSetAtomicSingleThread(self.get(), isSingleThread);
                   });
  
  AtomicRMWInstClass
      .def("__repr__",
           [](PymAtomicRMWInst &self) {
             return "<AtomicRMWInst>";
           })
      .def_prop_rw("alignment",
                   [](PymAtomicRMWInst &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PymAtomicRMWInst &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_rw("is_volatile",
                   [](PymAtomicRMWInst &self) {
                     return LLVMGetVolatile(self.get()) != 0;
                   },
                   [](PymAtomicRMWInst &self, bool isVolatile) {
                     return LLVMSetVolatile(self.get(), isVolatile);
                   })
      .def_prop_rw("ordering",
                   [](PymAtomicRMWInst &self) {
                     return LLVMGetOrdering(self.get());
                   },
                   [](PymAtomicRMWInst &self, LLVMAtomicOrdering ordering) {
                     return LLVMSetOrdering(self.get(), ordering);
                   })
      .def_prop_rw("bin_op",
                   [](PymAtomicRMWInst &self) {
                     return LLVMGetAtomicRMWBinOp(self.get());
                   },
                   [](PymAtomicRMWInst &self, LLVMAtomicRMWBinOp binOp) {
                     return LLVMSetAtomicRMWBinOp(self.get(), binOp);
                   })
      .def_prop_rw("is_single_thread",
                   [](PymAtomicRMWInst &self) {
                     return LLVMIsAtomicSingleThread(self.get()) != 0;
                   },
                   [](PymAtomicRMWInst &self, bool isSingleThread) {
                     return LLVMSetAtomicSingleThread(self.get(), isSingleThread);
                   });
  
  AtomicCmpXchgInstClass
      .def("__repr__",
           [](PymAtomicCmpXchgInst &self) {
             return "<AtomicCmpXchgInst>";
           })
      .def_prop_rw("alignment",
                   [](PymAtomicCmpXchgInst &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PymAtomicCmpXchgInst &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_rw("is_volatile",
                   [](PymLoadInst &self) {
                     return LLVMGetVolatile(self.get()) != 0;
                   },
                   [](PymLoadInst &self, bool isVolatile) {
                     return LLVMSetVolatile(self.get(), isVolatile);
                   })
      .def_prop_rw("is_weak",
                   [](PymAtomicCmpXchgInst &self) {
                     return LLVMGetWeak(self.get());
                   },
                   [](PymAtomicCmpXchgInst &self, bool isWeak) {
                     return LLVMSetWeak(self.get(), isWeak);
                   })
      .def_prop_rw("is_single_thread",
                   [](PymAtomicCmpXchgInst &self) {
                     return LLVMIsAtomicSingleThread(self.get()) != 0;
                   },
                   [](PymAtomicCmpXchgInst &self, bool isSingleThread) {
                     return LLVMSetAtomicSingleThread(self.get(), isSingleThread);
                   })
      .def_prop_rw("success_ordering",
                   [](PymAtomicCmpXchgInst &self) {
                     return LLVMGetCmpXchgSuccessOrdering(self.get());
                   },
                   [](PymAtomicCmpXchgInst &self, LLVMAtomicOrdering ordering) { 
                     return LLVMSetCmpXchgSuccessOrdering(self.get(), ordering);
                   })
      .def_prop_rw("failure_ordering",
                   [](PymAtomicCmpXchgInst &self) {
                     return LLVMGetCmpXchgFailureOrdering(self.get());
                   },
                   [](PymAtomicCmpXchgInst &self, LLVMAtomicOrdering ordering) {
                     return LLVMSetCmpXchgFailureOrdering(self.get(), ordering);
                   });

  GlobalObjectClass
      .def("__repr__",
           [](PymGlobalObject &self) {
             return gen_value_repr("GlobalObject", self);
           })
      .def("set_metadata",
           [](PymGlobalObject &g, unsigned kind, PymMetadata md) {
             return LLVMGlobalSetMetadata(g.get(), kind, md.get());
           },
           "Sets a metadata attachment, erasing the existing metadata attachment if"
           "it already exists for the given kind.")
      .def("erase_metadata",
           [](PymGlobalObject &g, unsigned kind) {
             return LLVMGlobalEraseMetadata(g.get(), kind);
           },
           "Erases a metadata attachment of the given kind if it exists.")
      .def("clear_metadata",
           [](PymGlobalObject &g) {
             return LLVMGlobalClearMetadata(g.get());
           },
           "Removes all metadata attachments from this value.")
      .def("copy_all_metadata",
           [](PymGlobalObject &g) {
             size_t num;
             LLVMValueMetadataEntries entries = LLVMGlobalCopyAllMetadata
                                                  (g.get(), &num);
             return PymMetadataEntries(entries, num);
           },
           "Retrieves an array of metadata entries representing the metadata attached to"
           "this value.");
  
  IndirectBrInstClass
      .def("__repr__",
           [](PymIndirectBrInst &self) {
             return "<IndirectBrInst>";
           })
      .def("add_destination",
           [](PymIndirectBrInst &self, PymBasicBlock &dest) {
             return LLVMAddDestination(self.get(), dest.get());
           },
           "dest"_a,
           "Add a destination to the indirectbr instruction.");

  LandingPadInstClass
      .def("__repr__",
           [](PymLandingPadInst &self) {
             return "<LandingPadInst>";
           })
      .def_prop_ro("num_clauses",
                   [](PymIndirectBrInst &self) {
                     return LLVMGetNumClauses(self.get());
                   })
      .def_prop_rw("is_cleanup",
                   [](PymLandingPadInst &self) {
                     return LLVMIsCleanup(self.get()) != 0;
                   },
                   [](PymLandingPadInst &self, bool val) {
                     return LLVMSetCleanup(self.get(), val);
                   })
      .def("get_clause",
           [](PymIndirectBrInst &self, unsigned index) {
             return PymValueAuto(LLVMGetClause(self.get(), index));
           },
           "index"_a)
      .def("add_clause",
           [](PymIndirectBrInst &self, PymConstant clause) {
             return LLVMAddClause(self.get(), clause.get());
           });

}
