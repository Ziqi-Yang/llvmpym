// For type/value class hierarchy see doxygen/group__LLVMCCore.html
//
// ====================================

#ifndef LLVMPYM_TYPES_PRIV_H
#define LLVMPYM_TYPES_PRIV_H

#include <llvm-c/Core.h>
#include <llvm-c/TargetMachine.h>
#include <utility>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>

#include "types_priv/PymModule.h"
#include "types_priv/PymContext.h"
#include "types_priv/PymMetadataEntries.h"
#include "types_priv/PymModuleFlagEntries.h"
#include "types_priv/PymOperandBundle.h"
#include "types_priv/PymPassManagerBase.h"
#include "types_priv/PymMemoryBuffer.h"
#include "types_priv/PymModuleProvider.h"
#include "types_priv/PymTargetData.h"
#include "types_priv/PymTargetMachine.h"
#include "types_priv/PymTargetMachineOptions.h"
#include "types_priv/PymLLVMObject.h"
#include "types_priv/PymDisasmContext.h"


#define DEFINE_PY_WRAPPER_CLASS(ClassName, UnderlyingType) \
  class ClassName: public PymLLVMObject<ClassName, UnderlyingType> { \
  public: \
    explicit ClassName(UnderlyingType raw) \
    : raw(raw) {} \
    \
    UnderlyingType get() const { \
      return raw; \
    } \
    \
  private: \
    UnderlyingType raw; \
  };

#define DEFINE_PY_WRAPPER_CLASS_POLYMORPHIC(ClassName, UnderlyingType) \
  class ClassName: public PymLLVMObject<ClassName, UnderlyingType> { \
  public: \
    virtual ~ClassName() = default; \
    explicit ClassName(UnderlyingType raw) \
    : raw(raw) {} \
    \
    UnderlyingType get() const { \
      return raw; \
    } \
    \
  private: \
    UnderlyingType raw; \
  };


#define DEFINE_DIRECT_SUB_CLASS(ParentClassName, ClassName) \
  class ClassName : public ParentClassName { \
  public: \
    using ParentClassName::ParentClassName; \
  };


// NOTE BasicBlock(Value) and custom added entries are not included in
// this macro
#define PY_FOR_EACH_VALUE_SUBCLASS(macro) \
  macro(Argument)                           \
  macro(InlineAsm)                          \
  macro(User)                               \
    macro(Constant)                         \
      macro(BlockAddress)                   \
      macro(ConstantAggregateZero)          \
      macro(ConstantArray)                  \
      macro(ConstantDataSequential)         \
        macro(ConstantDataArray)            \
        macro(ConstantDataVector)           \
      macro(ConstantExpr)                   \
      macro(ConstantFP)                     \
      macro(ConstantInt)                    \
      macro(ConstantPointerNull)            \
      macro(ConstantStruct)                 \
      macro(ConstantTokenNone)              \
      macro(ConstantVector)                 \
      macro(GlobalValue)                    \
        macro(GlobalAlias)                  \
        macro(GlobalObject)                 \
          macro(Function)                   \
          macro(GlobalVariable)             \
          macro(GlobalIFunc)                \
      macro(UndefValue)                     \
      macro(PoisonValue)                    \
    macro(Instruction)                      \
      macro(UnaryOperator)                  \
      macro(BinaryOperator)                 \
      macro(CallInst)                       \
        macro(IntrinsicInst)                \
          macro(DbgInfoIntrinsic)           \
            macro(DbgVariableIntrinsic)     \
              macro(DbgDeclareInst)         \
            macro(DbgLabelInst)             \
          macro(MemIntrinsic)               \
            macro(MemCpyInst)               \
            macro(MemMoveInst)              \
            macro(MemSetInst)               \
      macro(CmpInst)                        \
        macro(FCmpInst)                     \
        macro(ICmpInst)                     \
      macro(ExtractElementInst)             \
      macro(GetElementPtrInst)              \
      macro(InsertElementInst)              \
      macro(InsertValueInst)                \
      macro(LandingPadInst)                 \
      macro(PHINode)                        \
      macro(SelectInst)                     \
      macro(ShuffleVectorInst)              \
      macro(StoreInst)                      \
      macro(BranchInst)                     \
      macro(IndirectBrInst)                 \
      macro(InvokeInst)                     \
      macro(ReturnInst)                     \
      macro(SwitchInst)                     \
      macro(UnreachableInst)                \
      macro(ResumeInst)                     \
      macro(CleanupReturnInst)              \
      macro(CatchReturnInst)                \
      macro(CatchSwitchInst)                \
      macro(CallBrInst)                     \
      macro(FuncletPadInst)                 \
        macro(CatchPadInst)                 \
        macro(CleanupPadInst)               \
      macro(UnaryInstruction)               \
        macro(AllocaInst)                   \
        macro(CastInst)                     \
          macro(AddrSpaceCastInst)          \
          macro(BitCastInst)                \
          macro(FPExtInst)                  \
          macro(FPToSIInst)                 \
          macro(FPToUIInst)                 \
          macro(FPTruncInst)                \
          macro(IntToPtrInst)               \
          macro(PtrToIntInst)               \
          macro(SExtInst)                   \
          macro(SIToFPInst)                 \
          macro(TruncInst)                  \
          macro(UIToFPInst)                 \
          macro(ZExtInst)                   \
        macro(ExtractValueInst)             \
        macro(LoadInst)                     \
        macro(VAArgInst)                    \
        macro(FreezeInst)                   \
      macro(AtomicCmpXchgInst)              \
      macro(AtomicRMWInst)                  \
      macro(FenceInst)

// customized classes structure
// the actual classes structure can be viewed on 'classllvm_1_1Value.html' page in
// LLVM doxygen site
// e.g.
// CallBase => CallInst & InvokeInst (refers to LLVM implementation code)
// NOTE
// rename python side name of PymBasicBlock to BasicBlockValue to avoid collision
// with PymBasicBlockWrapper
#define PY_FOR_EACH_VALUE_CLASS_RELATIONSHIP(macro) \
  macro(PymValue, PymMetadataAsValue) \
  macro(PymMetadataAsValue, PymMDNodeValue) \
  macro(PymMetadataAsValue, PymValueAsMetadataValue) \
  macro(PymMetadataAsValue, PymMDStringValue) \
  macro(PymValue, PymArgument) \
  macro(PymValue, PymBasicBlockValue) \
  macro(PymValue, PymInlineAsm) \
  macro(PymValue, PymUser) \
    macro(PymUser, PymConstant) \
      macro(PymConstant, PymConstantArray) \
      macro(PymConstant, PymConstantDataSequential) \
      macro(PymConstantDataSequential, PymConstantDataArray) \
      macro(PymConstantDataSequential, PymConstantDataVector) \
      macro(PymConstant, PymConstantExpr) \
      macro(PymConstant, PymConstantFP) \
      macro(PymConstant, PymConstantInt) \
      macro(PymConstant, PymConstantStruct) \
      macro(PymConstant, PymConstantVector) \
      macro(PymConstant, PymGlobalValue) \
        macro(PymGlobalValue, PymGlobalAlias) \
        macro(PymGlobalValue, PymGlobalObject) \
          macro(PymGlobalObject, PymFunction) \
          macro(PymGlobalObject, PymGlobalVariable) \
          macro(PymGlobalObject, PymGlobalIFunc) \
      macro(PymConstant, PymUndefValue) \
      macro(PymConstant, PymPoisonValue) \
    macro(PymUser, PymInstruction) \
      macro(PymInstruction, PymCallBase) \
        macro(PymCallBase, PymCallInst) \
        macro(PymCallBase, PymInvokeInst) \
      macro(PymInstruction, PymFCmpInst) \
      macro(PymInstruction, PymICmpInst) \
      macro(PymInstruction, PymGetElementPtrInst) \
      macro(PymInstruction, PymPHINode) \
      macro(PymInstruction, PymShuffleVectorInst) \
      macro(PymInstruction, PymReturnInst) \
      macro(PymInstruction, PymSwitchInst) \
      macro(PymInstruction, PymCatchSwitchInst) \
      macro(PymInstruction, PymCleanupReturnInst) \
      macro(PymInstruction, PymFuncletPadInst) \
        macro(PymFuncletPadInst, PymCatchPadInst) \
      macro(PymInstruction, PymAllocaInst) \
      macro(PymInstruction, PymIEValueInstBase) \
        macro(PymIEValueInstBase, PymInsertValueInst) \
        macro(PymIEValueInstBase, PymExtractValueInst) \
      macro(PymInstruction, PymBranchInst) \
      macro(PymInstruction, PymIndirectBrInst) \
      macro(PymInstruction, PymLandingPadInst) \
      macro(PymInstruction, PymLoadInst) \
      macro(PymInstruction, PymStoreInst) \
      macro(PymInstruction, PymAtomicRMWInst) \
      macro(PymInstruction, PymAtomicCmpXchgInst) \
      macro(PymInstruction, PymFenceInst)

#define PY_FOR_EACH_TYPE_CLASS_RELASIONSHIP(macro) \
  macro(PymType, PymTypeInt) \
  macro(PymType, PymTypeReal) \
  macro(PymType, PymTypeFunction) \
  macro(PymType, PymTypeStruct) \
  macro(PymType, PymTypeSequence) \
    macro(PymTypeSequence, PymTypeArray) \
    macro(PymTypeSequence, PymTypePointer) \
    macro(PymTypeSequence, PymTypeVector) \
  macro(PymType, PymTypeVoid) \
  macro(PymType, PymTypeLabel) \
  macro(PymType, PymTypeX86MMX) \
  macro(PymType, PymTypeX86AMX) \
  macro(PymType, PymTypeToken) \
  macro(PymType, PymTypeMetadata) \
  macro(PymType, PymTypeTargetExt) 


#define BIND_PYLLVMOBJECT_(ClassName, UnderlyingType, PymClassName) \
  nb::class_<PymLLVMObject<ClassName, UnderlyingType>> \
      (m, #PymClassName, "The base class.") \
      .def("__bool__", &PymLLVMObject<ClassName, UnderlyingType>::__bool__) \
      .def("__eq__", &PymLLVMObject<ClassName, UnderlyingType>::__equal__) \
      .def("__hash__", &PymLLVMObject<ClassName, UnderlyingType>::__hash__);


enum class PymAttributeIndex {
  Return = LLVMAttributeReturnIndex,
  Function = LLVMAttributeFunctionIndex
};

enum class PymLLVMFastMathFlags {
  AllowReassoc = LLVMFastMathAllowReassoc,
  NoNaNs = LLVMFastMathNoNaNs,
  NoInfs = LLVMFastMathNoInfs,
  NoSignedZeros = LLVMFastMathNoSignedZeros,
  AllowReciprocal = LLVMFastMathAllowReciprocal,
  AllowContract = LLVMFastMathAllowContract,
  ApproxFunc = LLVMFastMathApproxFunc,
  None = LLVMFastMathNone,
  All = LLVMFastMathAll
};

/*
 * Check three places: here, class inheritance, binding class inheritance
 * NOTE the `__bool__` method of PymIntrinsic is overridden
 */
#define BIND_PYLLVMOBJECT() \
  BIND_PYLLVMOBJECT_(PymValue, LLVMValueRef, PymValueObject) \
  BIND_PYLLVMOBJECT_(PymType, LLVMTypeRef, PymTypeObject) \
  BIND_PYLLVMOBJECT_(PymDiagnosticInfo, LLVMDiagnosticInfoRef, PymDiagnosticInfoObject) \
  BIND_PYLLVMOBJECT_(PymAttribute, LLVMAttributeRef, PymAttributeObject) \
  BIND_PYLLVMOBJECT_(PymNamedMDNode, LLVMNamedMDNodeRef, PymNamedMDNodeObject) \
  BIND_PYLLVMOBJECT_(PymUse, LLVMUseRef, PymUseObject) \
  BIND_PYLLVMOBJECT_(PymBasicBlock, LLVMBasicBlockRef, PymBasicBlockObject) \
  BIND_PYLLVMOBJECT_(PymBuilder, LLVMBuilderRef, PymBuilderObject) \
  BIND_PYLLVMOBJECT_(PymMetadata, LLVMMetadataRef, PymMetadataObject) \
  BIND_PYLLVMOBJECT_(PymIntrinsic, unsigned, PymIntrinsicObject) \
\
  BIND_PYLLVMOBJECT_(PymContext, LLVMContextRef, PymContextObject) \
  BIND_PYLLVMOBJECT_(PymMemoryBuffer, LLVMMemoryBufferRef, PymMemoryBufferObject) \
  BIND_PYLLVMOBJECT_(PymMetadataEntries, LLVMValueMetadataEntries, PymMetadataEntriesObject) \
  BIND_PYLLVMOBJECT_(PymModuleFlagEntries, LLVMModuleFlagEntries, PymModuleFlagEntriesObject) \
  BIND_PYLLVMOBJECT_(PymModule, LLVMModuleRef, PymModuleObject) \
  BIND_PYLLVMOBJECT_(PymModuleProvider, LLVMModuleProviderRef, PymModuleProviderObject) \
  BIND_PYLLVMOBJECT_(PymOperandBundle, LLVMOperandBundleRef, PymOperandBundleObject) \
  BIND_PYLLVMOBJECT_(PymPassManagerBase, LLVMPassManagerRef, PymPassManagerBaseObject) \
\
  BIND_PYLLVMOBJECT_(PymTarget, LLVMTargetRef, PymTargetObject) \
  BIND_PYLLVMOBJECT_(PymTargetMachine, LLVMTargetMachineRef, PymTargetMachineObject) \
  BIND_PYLLVMOBJECT_(PymTargetMachineOptions, LLVMTargetMachineOptionsRef, \
    PymTargetMachineOptionsObject) \
\
  BIND_PYLLVMOBJECT_(PymTargetData, LLVMTargetDataRef, PymTargetDataObject) \
\
  BIND_PYLLVMOBJECT_(PymDisasmContext, LLVMDisasmContextRef, PymDisasmContextObject)


// Core --------------------------------------------------------

 

DEFINE_PY_WRAPPER_CLASS_POLYMORPHIC(PymValue, LLVMValueRef)
DEFINE_PY_WRAPPER_CLASS_POLYMORPHIC(PymType, LLVMTypeRef)
DEFINE_PY_WRAPPER_CLASS(PymDiagnosticInfo, LLVMDiagnosticInfoRef)
DEFINE_PY_WRAPPER_CLASS_POLYMORPHIC(PymAttribute, LLVMAttributeRef)
DEFINE_PY_WRAPPER_CLASS(PymNamedMDNode, LLVMNamedMDNodeRef)
DEFINE_PY_WRAPPER_CLASS(PymUse, LLVMUseRef)
DEFINE_PY_WRAPPER_CLASS(PymBasicBlock, LLVMBasicBlockRef)
DEFINE_PY_WRAPPER_CLASS(PymBuilder, LLVMBuilderRef)

DEFINE_PY_WRAPPER_CLASS(PymMetadata, LLVMMetadataRef)

DEFINE_DIRECT_SUB_CLASS(PymMetadata, PymMDNode)
DEFINE_DIRECT_SUB_CLASS(PymMetadata, PymValueAsMetadata)
DEFINE_DIRECT_SUB_CLASS(PymMetadata, PymMDString)


DEFINE_DIRECT_SUB_CLASS(PymAttribute, PymEnumAttribute);
DEFINE_DIRECT_SUB_CLASS(PymAttribute, PymTypeAttribute);
DEFINE_DIRECT_SUB_CLASS(PymAttribute, PymStringAttribute);

PY_FOR_EACH_VALUE_CLASS_RELATIONSHIP(DEFINE_DIRECT_SUB_CLASS)
PY_FOR_EACH_TYPE_CLASS_RELASIONSHIP(DEFINE_DIRECT_SUB_CLASS)

DEFINE_PY_WRAPPER_CLASS(PymIntrinsic, unsigned)



DEFINE_DIRECT_SUB_CLASS(PymPassManagerBase, PymPassManager);
DEFINE_DIRECT_SUB_CLASS(PymPassManagerBase, PymFunctionPassManager);


#define DEFINE_ITERATOR_CLASS(TypeName, UnderlyingType, GetNextFn) \
  class TypeName { \
  public: \
    explicit TypeName(UnderlyingType val): val(val) {} \
  \
  UnderlyingType get() { \
    return val; \
  } \
  \
  UnderlyingType next() { \
  if (!val.get()) \
       throw nanobind::stop_iteration(); \
    auto prev = val; \
    val = UnderlyingType(GetNextFn(val.get())); \
    return prev; \
  } \
  \
  private: \
    UnderlyingType val; \
  };

#define BIND_ITERATOR_CLASS(ClassName, PymthonClassName) \
  nanobind::class_<ClassName>(m, PymthonClassName, PymthonClassName) \
  .def("__iter__", [](ClassName &self) { return self; }) \
  .def("__next__", &ClassName::next);





DEFINE_ITERATOR_CLASS(PymUseIterator, PymUse, LLVMGetNextUse)
// DEFINE_ITERATOR_CLASS(PymBasicBlockIterator, PymBasicBlock, LLVMGetNextBasicBlock)
// DEFINE_ITERATOR_CLASS(PymArgumentIterator, PymArgument, LLVMGetNextParam)
DEFINE_ITERATOR_CLASS(PymInstructionIterator, PymInstruction, LLVMGetNextInstruction)
DEFINE_ITERATOR_CLASS(PymGlobalVariableIterator, PymGlobalVariable, LLVMGetNextGlobal)
DEFINE_ITERATOR_CLASS(PymGlobalIFuncIterator, PymGlobalIFunc, LLVMGetNextGlobalIFunc)
DEFINE_ITERATOR_CLASS(PymGlobalAliasIterator, PymGlobalAlias, LLVMGetNextGlobalAlias)
DEFINE_ITERATOR_CLASS(PymNamedMDNodeIterator, PymNamedMDNode, LLVMGetNextNamedMetadata)
DEFINE_ITERATOR_CLASS(PymFunctionIterator, PymFunction, LLVMGetNextFunction)



// TargetMachine ---------------------------------------------------------

DEFINE_PY_WRAPPER_CLASS(PymTarget, LLVMTargetRef)
DEFINE_ITERATOR_CLASS(PymTargetIterator, PymTarget, LLVMGetNextTarget)



#endif
