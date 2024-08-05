#include "utils.h"

#include <llvm-c/IRReader.h>
#include <stdexcept>

PymMetadataAsValue* getMoreSpcMetadataAsValue(LLVMValueRef raw) {
  if (auto v = LLVMIsAMDNode(raw)) {
    return new PymMDNodeValue(v);
  } else if (auto v = LLVMIsAMDString(raw)) {
    return new PymMDStringValue(v);
  } 
  return new PymMetadataAsValue(raw);
}


/**
 * NOTE only mapping needed specific instruction sub-classes
 * add the order here is according to `PY_FOR_EACH_VALUE_CLASS_RELATIONSHIP` macro
 * in Types.h
 * 
 * @param Inst: should be of LLVMInstructionValueKind
 */
PymInstruction* PymInstructionAuto(LLVMValueRef inst) {
  LLVMOpcode opcode = LLVMGetInstructionOpcode(inst);
  switch (opcode) {
  case LLVMCall:
    return new PymCallInst(inst);
  case LLVMInvoke:
    return new PymInvokeInst(inst);

  case LLVMFCmp:
    return new PymFCmpInst(inst);
  case LLVMICmp:
    return new PymICmpInst(inst);
  case LLVMGetElementPtr:
    return new PymGetElementPtrInst(inst);
  case LLVMPHI:
    return new PymPHINode(inst);
  case LLVMShuffleVector:
    return new PymShuffleVectorInst(inst);
  case LLVMRet:
    return new PymReturnInst(inst);
  case LLVMSwitch:
    return new PymSwitchInst(inst);
  case LLVMCatchSwitch:
    return new PymCatchSwitchInst(inst);
  case LLVMCleanupRet:
    return new PymCleanupReturnInst(inst);
  // no kind corresponding to PymFuncletPadInst
  case LLVMCatchPad:
    return new PymCatchPadInst(inst);
  case LLVMAlloca:
    return new PymAllocaInst(inst);
  case LLVMInsertValue:
    return new PymInsertValueInst(inst);
  case LLVMExtractValue:
    return new PymExtractValueInst(inst);
  case LLVMBr:
    return new PymBranchInst(inst);
  case LLVMIndirectBr:
    return new PymIndirectBrInst(inst);
  case LLVMLandingPad:
    return new PymLandingPadInst(inst);
  case LLVMLoad:
    return new PymLoadInst(inst);
  case LLVMStore:
    return new PymStoreInst(inst);
  case LLVMAtomicRMW:
    return new PymAtomicRMWInst(inst);
  case LLVMAtomicCmpXchg:
    return new PymAtomicCmpXchgInst(inst);
  case LLVMFence:
    return new PymFenceInst(inst);

  default:
    return new PymInstruction(inst);
  }
}



PymType* PymTypeAuto(LLVMTypeRef rawType) {
  LLVMTypeKind kind = LLVMGetTypeKind(rawType);
  switch (kind) {
  case LLVMVoidTypeKind:
    return new PymTypeVoid(rawType);
  case LLVMHalfTypeKind:
  case LLVMFloatTypeKind:
  case LLVMDoubleTypeKind:
  case LLVMX86_FP80TypeKind:
  case LLVMFP128TypeKind:
  case LLVMPPC_FP128TypeKind:
  case LLVMBFloatTypeKind:
    return new PymTypeReal(rawType);
  case LLVMLabelTypeKind:
    return new PymTypeLabel(rawType);
  case LLVMIntegerTypeKind:
    return new PymTypeInt(rawType);
  case LLVMFunctionTypeKind:
    return new PymTypeFunction(rawType);
  case LLVMStructTypeKind:
    return new PymTypeStruct(rawType);
  case LLVMArrayTypeKind:
    return new PymTypeArray(rawType);
  case LLVMPointerTypeKind:
    return new PymTypePointer(rawType);
  case LLVMVectorTypeKind:
    return new PymTypeVector(rawType);
  case LLVMMetadataTypeKind:
    return new PymTypeMetadata(rawType);
  case LLVMX86_MMXTypeKind:
    return new PymTypeX86MMX(rawType);
  case LLVMTokenTypeKind:
    return new PymTypeToken(rawType);
  case LLVMScalableVectorTypeKind:
    return new PymTypeVector(rawType);
  case LLVMX86_AMXTypeKind:
    return new PymTypeX86AMX(rawType);
  case LLVMTargetExtTypeKind:
    return new PymTypeTargetExt(rawType);
  default:
    return new PymType(rawType);
  }
}

/**
 * It seems like the enum type doesn't cover all the sub-classes,
 * so user may still need to do a manual cast using `to_XXX` method
 */
PymValue* PymValueAuto(LLVMValueRef rawValue) {
  LLVMValueKind kind = LLVMGetValueKind(rawValue);
  switch (kind) {
  case LLVMArgumentValueKind:
    return new PymArgument(rawValue);
  case LLVMBasicBlockValueKind:
    return new PymBasicBlockValue(rawValue);
  // case LLVMMemoryUseValueKind:
  // case LLVMMemoryDefValueKind:
  // case LLVMMemoryPhiValueKind:

  case LLVMFunctionValueKind:
    return new PymFunction(rawValue);
  case LLVMGlobalAliasValueKind:
    return new PymGlobalAlias(rawValue);
  case LLVMGlobalIFuncValueKind:
    return new PymGlobalIFunc(rawValue);
  case LLVMGlobalVariableValueKind:
    return new PymGlobalVariable(rawValue);
  // case LLVMBlockAddressValueKind:
  case LLVMConstantExprValueKind:
    return new PymConstantExpr(rawValue);
  case LLVMConstantArrayValueKind:
    return new PymConstantArray(rawValue);
  case LLVMConstantStructValueKind:
    return new PymConstantStruct(rawValue);
  case LLVMConstantVectorValueKind:
    return new PymConstantVector(rawValue);

  case LLVMUndefValueValueKind:
    return new PymUndefValue(rawValue);
  case LLVMConstantDataArrayValueKind:
    return new PymConstantDataArray(rawValue);
  case LLVMConstantDataVectorValueKind:
    return new PymConstantDataVector(rawValue);
  case LLVMConstantIntValueKind:
    return new PymConstantInt(rawValue);
  case LLVMConstantFPValueKind:
    return new PymConstantFP(rawValue);
  case LLVMConstantTokenNoneValueKind:
  case LLVMConstantAggregateZeroValueKind:
  case LLVMConstantPointerNullValueKind:
    return new PymConstant(rawValue);

  case LLVMMetadataAsValueValueKind:
    return getMoreSpcMetadataAsValue(rawValue);
  case LLVMInlineAsmValueKind:
    return new PymInlineAsm(rawValue);

  case LLVMInstructionValueKind:
    return PymInstructionAuto(rawValue);
  case LLVMPoisonValueValueKind:
    return new PymPoisonValue(rawValue);
  // case LLVMConstantTargetNoneValueKind: // 
  default:
    return new PymValue(rawValue);
  }
}

PymAttribute* PymAttributeAuto(LLVMAttributeRef rawValue) {
  if (LLVMIsEnumAttribute(rawValue)) {
    return new PymEnumAttribute(rawValue);
  } else if (LLVMIsStringAttribute(rawValue)) {
    return new PymStringAttribute(rawValue);
  } else if (LLVMIsTypeAttribute(rawValue)) {
    return new PymTypeAttribute(rawValue);
  } else {
    return new PymAttribute(rawValue);
  }
}

PymModule parseIR(LLVMContextRef ctx, LLVMMemoryBufferRef memBuf) {
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

  return PymModule(m);
}
