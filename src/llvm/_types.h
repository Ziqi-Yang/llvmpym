#ifndef LLVMPYM__TYPES_H
#define LLVMPYM__TYPES_H

#include <llvm-c/Core.h>
#include <utility>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>

#include "_types/PyModule.h"
#include "_types/PyContext.h"
#include "_types/PyMetadataEntries.h"
#include "_types/PyModuleFlagEntries.h"
#include "_types/PyOperandBundle.h"
#include "_types/PyPassManagerBase.h"
#include "_types/PyMemoryBuffer.h"
#include "_types/PyModuleProvider.h"


#define DEFINE_PY_WRAPPER_CLASS(ClassName, UnderlyingType) \
  class ClassName { \
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
  class ClassName { \
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
// rename python side name of PyBasicBlock to BasicBlockValue to avoid collision
// with PyBasicBlockWrapper
#define PY_FOR_EACH_VALUE_CLASS_RELATIONSHIP(macro) \
  macro(PyValue, PyMetadataAsValue) \
  macro(PyMetadataAsValue, PyMDNodeValue) \
  macro(PyMetadataAsValue, PyValueAsMetadataValue) \
  macro(PyMetadataAsValue, PyMDStringValue) \
  macro(PyValue, PyArgument) \
  macro(PyValue, PyBasicBlockValue) \
  macro(PyValue, PyInlineAsm) \
  macro(PyValue, PyUser) \
    macro(PyUser, PyConstant) \
      macro(PyConstant, PyConstantArray) \
      macro(PyConstant, PyConstantDataSequential) \
      macro(PyConstantDataSequential, PyConstantDataArray) \
      macro(PyConstantDataSequential, PyConstantDataVector) \
      macro(PyConstant, PyConstantExpr) \
      macro(PyConstant, PyConstantFP) \
      macro(PyConstant, PyConstantInt) \
      macro(PyConstant, PyConstantStruct) \
      macro(PyConstant, PyConstantVector) \
      macro(PyConstant, PyGlobalValue) \
        macro(PyGlobalValue, PyGlobalAlias) \
        macro(PyGlobalValue, PyGlobalObject) \
          macro(PyGlobalObject, PyFunction) \
          macro(PyGlobalObject, PyGlobalVariable) \
          macro(PyGlobalObject, PyGlobalIFunc) \
      macro(PyConstant, PyUndefValue) \
      macro(PyConstant, PyPoisonValue) \
    macro(PyUser, PyInstruction) \
      macro(PyInstruction, PyCallBase) \
        macro(PyCallBase, PyCallInst) \
        macro(PyCallBase, PyInvokeInst) \
      macro(PyInstruction, PyFCmpInst) \
      macro(PyInstruction, PyICmpInst) \
      macro(PyInstruction, PyGetElementPtrInst) \
      macro(PyInstruction, PyPHINode) \
      macro(PyInstruction, PyShuffleVectorInst) \
      macro(PyInstruction, PyReturnInst) \
      macro(PyInstruction, PySwitchInst) \
      macro(PyInstruction, PyCatchSwitchInst) \
      macro(PyInstruction, PyCleanupReturnInst) \
      macro(PyInstruction, PyFuncletPadInst) \
        macro(PyFuncletPadInst, PyCatchPadInst) \
      macro(PyInstruction, PyAllocaInst) \
      macro(PyInstruction, PyIEValueInstBase) \
        macro(PyIEValueInstBase, PyInsertValueInst) \
        macro(PyIEValueInstBase, PyExtractValueInst) \
      macro(PyInstruction, PyBranchInst) \
      macro(PyInstruction, PyIndirectBrInst) \
      macro(PyInstruction, PyLandingPadInst) \
      macro(PyInstruction, PyLoadInst) \
      macro(PyInstruction, PyStoreInst) \
      macro(PyInstruction, PyAtomicRMWInst) \
      macro(PyInstruction, PyAtomicCmpXchgInst) \
      macro(PyInstruction, PyFenceInst)

#define PY_FOR_EACH_TYPE_CLASS_RELASIONSHIP(macro) \
  macro(PyType, PyTypeInt) \
  macro(PyType, PyTypeReal) \
  macro(PyType, PyTypeFunction) \
  macro(PyType, PyTypeStruct) \
  macro(PyType, PyTypeSequence) \
    macro(PyTypeSequence, PyTypeArray) \
    macro(PyTypeSequence, PyTypePointer) \
    macro(PyTypeSequence, PyTypeVector) \
  macro(PyType, PyTypeVoid) \
  macro(PyType, PyTypeLabel) \
  macro(PyType, PyTypeX86MMX) \
  macro(PyType, PyTypeX86AMX) \
  macro(PyType, PyTypeToken) \
  macro(PyType, PyTypeMetadata) \
  macro(PyType, PyTypeTargetExt) 


enum class PyAttributeIndex {
  Return = LLVMAttributeReturnIndex,
  Function = LLVMAttributeFunctionIndex
};

enum class PyLLVMFastMathFlags {
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


DEFINE_PY_WRAPPER_CLASS_POLYMORPHIC(PyValue, LLVMValueRef)
DEFINE_PY_WRAPPER_CLASS_POLYMORPHIC(PyType, LLVMTypeRef)
DEFINE_PY_WRAPPER_CLASS(PyDiagnosticInfo, LLVMDiagnosticInfoRef)
DEFINE_PY_WRAPPER_CLASS_POLYMORPHIC(PyAttribute, LLVMAttributeRef)
DEFINE_PY_WRAPPER_CLASS(PyNamedMDNode, LLVMNamedMDNodeRef)
DEFINE_PY_WRAPPER_CLASS(PyUse, LLVMUseRef)
DEFINE_PY_WRAPPER_CLASS(PyBasicBlock, LLVMBasicBlockRef)
DEFINE_PY_WRAPPER_CLASS(PyBuilder, LLVMBuilderRef)

DEFINE_PY_WRAPPER_CLASS(PyMetadata, LLVMMetadataRef)
DEFINE_DIRECT_SUB_CLASS(PyMetadata, PyMDNode)
DEFINE_DIRECT_SUB_CLASS(PyMetadata, PyValueAsMetadata)
DEFINE_DIRECT_SUB_CLASS(PyMetadata, PyMDString)


DEFINE_DIRECT_SUB_CLASS(PyAttribute, PyEnumAttribute);
DEFINE_DIRECT_SUB_CLASS(PyAttribute, PyTypeAttribute);
DEFINE_DIRECT_SUB_CLASS(PyAttribute, PyStringAttribute);

PY_FOR_EACH_VALUE_CLASS_RELATIONSHIP(DEFINE_DIRECT_SUB_CLASS)
PY_FOR_EACH_TYPE_CLASS_RELASIONSHIP(DEFINE_DIRECT_SUB_CLASS)

DEFINE_PY_WRAPPER_CLASS(PyIntrinsic, unsigned)





DEFINE_DIRECT_SUB_CLASS(PyPassManagerBase, PyPassManager);
DEFINE_DIRECT_SUB_CLASS(PyPassManagerBase, PyFunctionPassManager);



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
    auto res = GetNextFn(val.get()); \
    if (!res) \
      throw nanobind::stop_iteration(); \
    auto prev = val; \
    val = UnderlyingType(res); \
    return prev; \
  } \
  \
  private: \
    UnderlyingType val; \
  };

#define BIND_ITERATOR_CLASS(ClassName, PythonClassName) \
  nanobind::class_<ClassName>(m, PythonClassName, PythonClassName) \
      .def("__iter__", [](ClassName &self) { return self; }) \
      .def("__next__", &ClassName::next);


DEFINE_ITERATOR_CLASS(PyUseIterator, PyUse, LLVMGetNextUse)
DEFINE_ITERATOR_CLASS(PyBasicBlockIterator, PyBasicBlock, LLVMGetNextBasicBlock)
DEFINE_ITERATOR_CLASS(PyArgumentIterator, PyArgument, LLVMGetNextParam)
DEFINE_ITERATOR_CLASS(PyInstructionIterator, PyInstruction, LLVMGetNextInstruction)
DEFINE_ITERATOR_CLASS(PyGlobalVariableIterator, PyGlobalVariable, LLVMGetNextGlobal)
DEFINE_ITERATOR_CLASS(PyGlobalIFuncIterator, PyGlobalIFunc, LLVMGetNextGlobalIFunc)
DEFINE_ITERATOR_CLASS(PyGlobalAliasIterator, PyGlobalAlias, LLVMGetNextGlobalAlias)
DEFINE_ITERATOR_CLASS(PyNamedMDNodeIterator, PyNamedMDNode, LLVMGetNextNamedMetadata)
DEFINE_ITERATOR_CLASS(PyFunctionIterator, PyFunction, LLVMGetNextFunction)






#endif
