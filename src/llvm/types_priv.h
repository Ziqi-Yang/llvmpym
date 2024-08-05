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

#include "types_priv/PyModule.h"
#include "types_priv/PyContext.h"
#include "types_priv/PyMetadataEntries.h"
#include "types_priv/PyModuleFlagEntries.h"
#include "types_priv/PyOperandBundle.h"
#include "types_priv/PyPassManagerBase.h"
#include "types_priv/PyMemoryBuffer.h"
#include "types_priv/PyModuleProvider.h"
#include "types_priv/PyTargetMachine.h"
#include "types_priv/PyLLVMObject.h"


#define DEFINE_PY_WRAPPER_CLASS(ClassName, UnderlyingType) \
  class ClassName: public PyLLVMObject<ClassName, UnderlyingType> { \
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
  class ClassName: public PyLLVMObject<ClassName, UnderlyingType> { \
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


#define BIND_PYLLVMOBJECT_(ClassName, UnderlyingType, PyClassName) \
  nb::class_<PyLLVMObject<ClassName, UnderlyingType>> \
      (m, #PyClassName, "The base class.") \
      .def("__bool__", &PyLLVMObject<ClassName, UnderlyingType>::__bool__) \
      .def("__eq__", &PyLLVMObject<ClassName, UnderlyingType>::__equal__) \
      .def("__hash__", &PyLLVMObject<ClassName, UnderlyingType>::__hash__);


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

/*
 * Check three places: here, class inheritance, binding class inheritance
 * NOTE the `__bool__` method of PyIntrinsic is overridden
 */
#define BIND_PYLLVMOBJECT() \
  BIND_PYLLVMOBJECT_(PyValue, LLVMValueRef, PyValueObject) \
  BIND_PYLLVMOBJECT_(PyType, LLVMTypeRef, PyTypeObject) \
  BIND_PYLLVMOBJECT_(PyDiagnosticInfo, LLVMDiagnosticInfoRef, PyDiagnosticInfoObject) \
  BIND_PYLLVMOBJECT_(PyAttribute, LLVMAttributeRef, PyAttributeObject) \
  BIND_PYLLVMOBJECT_(PyNamedMDNode, LLVMNamedMDNodeRef, PyNamedMDNodeObject) \
  BIND_PYLLVMOBJECT_(PyUse, LLVMUseRef, PyUseObject) \
  BIND_PYLLVMOBJECT_(PyBasicBlock, LLVMBasicBlockRef, PyBasicBlockObject) \
  BIND_PYLLVMOBJECT_(PyBuilder, LLVMBuilderRef, PyBuilderObject) \
  BIND_PYLLVMOBJECT_(PyMetadata, LLVMMetadataRef, PyMetadataObject) \
  BIND_PYLLVMOBJECT_(PyIntrinsic, unsigned, PyIntrinsicObject) \
\
  BIND_PYLLVMOBJECT_(PyContext, LLVMContextRef, PyContextObject) \
  BIND_PYLLVMOBJECT_(PyMemoryBuffer, LLVMMemoryBufferRef, PyMemoryBufferObject) \
  BIND_PYLLVMOBJECT_(PyMetadataEntries, LLVMValueMetadataEntries, PyMetadataEntriesObject) \
  BIND_PYLLVMOBJECT_(PyModuleFlagEntries, LLVMModuleFlagEntries, PyModuleFlagEntriesObject) \
  BIND_PYLLVMOBJECT_(PyModule, LLVMModuleRef, PyModuleObject) \
  BIND_PYLLVMOBJECT_(PyModuleProvider, LLVMModuleProviderRef, PyModuleProviderObject) \
  BIND_PYLLVMOBJECT_(PyOperandBundle, LLVMOperandBundleRef, PyOperandBundleObject) \
  BIND_PYLLVMOBJECT_(PyPassManagerBase, LLVMPassManagerRef, PyPassManagerBaseObject) \
\
  BIND_PYLLVMOBJECT_(PyTarget, LLVMTargetRef, PyTargetObject) \
  BIND_PYLLVMOBJECT_(PyTargetMachine, LLVMTargetMachineRef, PyTargetMachineObject)



// Core --------------------------------------------------------

 

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

#define BIND_ITERATOR_CLASS(ClassName, PythonClassName) \
  nanobind::class_<ClassName>(m, PythonClassName, PythonClassName) \
  .def("__iter__", [](ClassName &self) { return self; }) \
  .def("__next__", &ClassName::next);





DEFINE_ITERATOR_CLASS(PyUseIterator, PyUse, LLVMGetNextUse)
// DEFINE_ITERATOR_CLASS(PyBasicBlockIterator, PyBasicBlock, LLVMGetNextBasicBlock)
// DEFINE_ITERATOR_CLASS(PyArgumentIterator, PyArgument, LLVMGetNextParam)
DEFINE_ITERATOR_CLASS(PyInstructionIterator, PyInstruction, LLVMGetNextInstruction)
DEFINE_ITERATOR_CLASS(PyGlobalVariableIterator, PyGlobalVariable, LLVMGetNextGlobal)
DEFINE_ITERATOR_CLASS(PyGlobalIFuncIterator, PyGlobalIFunc, LLVMGetNextGlobalIFunc)
DEFINE_ITERATOR_CLASS(PyGlobalAliasIterator, PyGlobalAlias, LLVMGetNextGlobalAlias)
DEFINE_ITERATOR_CLASS(PyNamedMDNodeIterator, PyNamedMDNode, LLVMGetNextNamedMetadata)
DEFINE_ITERATOR_CLASS(PyFunctionIterator, PyFunction, LLVMGetNextFunction)



// TargetMachine ---------------------------------------------------------

DEFINE_PY_WRAPPER_CLASS(PyTarget, LLVMTargetRef)
DEFINE_ITERATOR_CLASS(PyTargetIterator, PyTarget, LLVMGetNextTarget)



#endif
