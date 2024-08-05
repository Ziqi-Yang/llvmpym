#include "utils.h"

#include <llvm-c/IRReader.h>
#include <stdexcept>

PyMetadataAsValue* getMoreSpcMetadataAsValue(LLVMValueRef raw) {
  if (auto v = LLVMIsAMDNode(raw)) {
    return new PyMDNodeValue(v);
  } else if (auto v = LLVMIsAMDString(raw)) {
    return new PyMDStringValue(v);
  } 
  return new PyMetadataAsValue(raw);
}


/**
 * NOTE only mapping needed specific instruction sub-classes
 * add the order here is according to `PY_FOR_EACH_VALUE_CLASS_RELATIONSHIP` macro
 * in Types.h
 * 
 * @param Inst: should be of LLVMInstructionValueKind
 */
PyInstruction* PyInstructionAuto(LLVMValueRef inst) {
  LLVMOpcode opcode = LLVMGetInstructionOpcode(inst);
  switch (opcode) {
  case LLVMCall:
    return new PyCallInst(inst);
  case LLVMInvoke:
    return new PyInvokeInst(inst);

  case LLVMFCmp:
    return new PyFCmpInst(inst);
  case LLVMICmp:
    return new PyICmpInst(inst);
  case LLVMGetElementPtr:
    return new PyGetElementPtrInst(inst);
  case LLVMPHI:
    return new PyPHINode(inst);
  case LLVMShuffleVector:
    return new PyShuffleVectorInst(inst);
  case LLVMRet:
    return new PyReturnInst(inst);
  case LLVMSwitch:
    return new PySwitchInst(inst);
  case LLVMCatchSwitch:
    return new PyCatchSwitchInst(inst);
  case LLVMCleanupRet:
    return new PyCleanupReturnInst(inst);
  // no kind corresponding to PyFuncletPadInst
  case LLVMCatchPad:
    return new PyCatchPadInst(inst);
  case LLVMAlloca:
    return new PyAllocaInst(inst);
  case LLVMInsertValue:
    return new PyInsertValueInst(inst);
  case LLVMExtractValue:
    return new PyExtractValueInst(inst);
  case LLVMBr:
    return new PyBranchInst(inst);
  case LLVMIndirectBr:
    return new PyIndirectBrInst(inst);
  case LLVMLandingPad:
    return new PyLandingPadInst(inst);
  case LLVMLoad:
    return new PyLoadInst(inst);
  case LLVMStore:
    return new PyStoreInst(inst);
  case LLVMAtomicRMW:
    return new PyAtomicRMWInst(inst);
  case LLVMAtomicCmpXchg:
    return new PyAtomicCmpXchgInst(inst);
  case LLVMFence:
    return new PyFenceInst(inst);

  default:
    return new PyInstruction(inst);
  }
}



PyType* PyTypeAuto(LLVMTypeRef rawType) {
  LLVMTypeKind kind = LLVMGetTypeKind(rawType);
  switch (kind) {
  case LLVMVoidTypeKind:
    return new PyTypeVoid(rawType);
  case LLVMHalfTypeKind:
  case LLVMFloatTypeKind:
  case LLVMDoubleTypeKind:
  case LLVMX86_FP80TypeKind:
  case LLVMFP128TypeKind:
  case LLVMPPC_FP128TypeKind:
  case LLVMBFloatTypeKind:
    return new PyTypeReal(rawType);
  case LLVMLabelTypeKind:
    return new PyTypeLabel(rawType);
  case LLVMIntegerTypeKind:
    return new PyTypeInt(rawType);
  case LLVMFunctionTypeKind:
    return new PyTypeFunction(rawType);
  case LLVMStructTypeKind:
    return new PyTypeStruct(rawType);
  case LLVMArrayTypeKind:
    return new PyTypeArray(rawType);
  case LLVMPointerTypeKind:
    return new PyTypePointer(rawType);
  case LLVMVectorTypeKind:
    return new PyTypeVector(rawType);
  case LLVMMetadataTypeKind:
    return new PyTypeMetadata(rawType);
  case LLVMX86_MMXTypeKind:
    return new PyTypeX86MMX(rawType);
  case LLVMTokenTypeKind:
    return new PyTypeToken(rawType);
  case LLVMScalableVectorTypeKind:
    return new PyTypeVector(rawType);
  case LLVMX86_AMXTypeKind:
    return new PyTypeX86AMX(rawType);
  case LLVMTargetExtTypeKind:
    return new PyTypeTargetExt(rawType);
  default:
    return new PyType(rawType);
  }
}

/**
 * It seems like the enum type doesn't cover all the sub-classes,
 * so user may still need to do a manual cast using `to_XXX` method
 */
PyValue* PyValueAuto(LLVMValueRef rawValue) {
  LLVMValueKind kind = LLVMGetValueKind(rawValue);
  switch (kind) {
  case LLVMArgumentValueKind:
    return new PyArgument(rawValue);
  case LLVMBasicBlockValueKind:
    return new PyBasicBlockValue(rawValue);
  // case LLVMMemoryUseValueKind:
  // case LLVMMemoryDefValueKind:
  // case LLVMMemoryPhiValueKind:

  case LLVMFunctionValueKind:
    return new PyFunction(rawValue);
  case LLVMGlobalAliasValueKind:
    return new PyGlobalAlias(rawValue);
  case LLVMGlobalIFuncValueKind:
    return new PyGlobalIFunc(rawValue);
  case LLVMGlobalVariableValueKind:
    return new PyGlobalVariable(rawValue);
  // case LLVMBlockAddressValueKind:
  case LLVMConstantExprValueKind:
    return new PyConstantExpr(rawValue);
  case LLVMConstantArrayValueKind:
    return new PyConstantArray(rawValue);
  case LLVMConstantStructValueKind:
    return new PyConstantStruct(rawValue);
  case LLVMConstantVectorValueKind:
    return new PyConstantVector(rawValue);

  case LLVMUndefValueValueKind:
    return new PyUndefValue(rawValue);
  case LLVMConstantDataArrayValueKind:
    return new PyConstantDataArray(rawValue);
  case LLVMConstantDataVectorValueKind:
    return new PyConstantDataVector(rawValue);
  case LLVMConstantIntValueKind:
    return new PyConstantInt(rawValue);
  case LLVMConstantFPValueKind:
    return new PyConstantFP(rawValue);
  case LLVMConstantTokenNoneValueKind:
  case LLVMConstantAggregateZeroValueKind:
  case LLVMConstantPointerNullValueKind:
    return new PyConstant(rawValue);

  case LLVMMetadataAsValueValueKind:
    return getMoreSpcMetadataAsValue(rawValue);
  case LLVMInlineAsmValueKind:
    return new PyInlineAsm(rawValue);

  case LLVMInstructionValueKind:
    return PyInstructionAuto(rawValue);
  case LLVMPoisonValueValueKind:
    return new PyPoisonValue(rawValue);
  // case LLVMConstantTargetNoneValueKind: // 
  default:
    return new PyValue(rawValue);
  }
}

PyAttribute* PyAttributeAuto(LLVMAttributeRef rawValue) {
  if (LLVMIsEnumAttribute(rawValue)) {
    return new PyEnumAttribute(rawValue);
  } else if (LLVMIsStringAttribute(rawValue)) {
    return new PyStringAttribute(rawValue);
  } else if (LLVMIsTypeAttribute(rawValue)) {
    return new PyTypeAttribute(rawValue);
  } else {
    return new PyAttribute(rawValue);
  }
}

PyModule parseIR(LLVMContextRef ctx, LLVMMemoryBufferRef memBuf) {
  LLVMModuleRef m = nullptr;
  char *errMsg = nullptr;
  bool success = LLVMParseIRInContext(ctx, memBuf, &m, &errMsg) == 0;

  if (!success) {
    std::string errorMessage;
    if (m) LLVMDisposeModule(m);
    if (errMsg) errorMessage = std::string(errMsg);
    LLVMDisposeMessage(errMsg);
    throw std::runtime_error(errorMessage);
  }

  return PyModule(m);
}
