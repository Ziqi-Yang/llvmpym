#ifndef LLVMPYM_TYPES_H
#define LLVMPYM_TYPES_H

#include <llvm-c/Core.h>
#include <utility>
#include <iostream>

/*
  We don't define MoveOnly class to also give `Move` operation a default method
  is because when the sub-class has a custom destructor, the compiler will not
  automatically generate a move constructor or move assignment operator
 */
class NonCopyable {
public:
  NonCopyable(const NonCopyable &) = delete;
  NonCopyable &operator=(const NonCopyable &) = delete;

protected:
  NonCopyable() = default;
  virtual ~NonCopyable() = default;
};


#define DEFINE_MOVE_SEMANTICS_CLEANUP(ClassName, ValueName) \
    ClassName(ClassName&& other) noexcept : ValueName(other.ValueName) { \
      other.ValueName = nullptr; \
    } \
\
    ClassName& operator=(ClassName&& other) noexcept { \
      if (this != &other) { \
        cleanup(); \
        ValueName = other.ValueName; \
        other.ValueName = nullptr; \
      } \
      return *this; \
    }

#define DEFINE_MOVE_SEMANTICS_CLEANUP_2(ClassName, Value0Name, Value1Name, Value1Default) \
    ClassName(ClassName&& other) noexcept \
    : Value0Name(other.Value0Name), Value1Name(other.Value1Name) { \
      other.Value0Name = nullptr; \
      other.Value1Name = Value1Default; \
    } \
\
    ClassName& operator=(ClassName&& other) noexcept { \
      if (this != &other) { \
        cleanup(); \
        Value0Name = other.Value0Name; \
        other.Value0Name = nullptr; \
\
        Value1Name = other.Value1Name; \
        other.Value1Name = Value1Default; \
      } \
      return *this; \
    }


// note the underlying type must be a pointer type
#define DEFINE_PY_WRAPPER_CLASS_SELF_DISPOSABLE_NONCOPYABLE(ClassName, UnderlyingType, DISPOSE_FUNC) \
  class ClassName : public NonCopyable { \
  public: \
    explicit ClassName(UnderlyingType raw) \
    : raw(raw) {} \
\
    ~ClassName() { \
      cleanup(); \
    } \
\
    UnderlyingType get() { \
      return raw; \
    } \
\
    ClassName(ClassName&& other) noexcept : raw(other.raw) { \
      other.raw = nullptr; \
    } \
\
    ClassName& operator=(ClassName&& other) noexcept { \
      if (this != &other) { \
        cleanup(); \
        raw = other.raw; \
        other.raw = nullptr; \
      } \
      return *this; \
    } \
\
    void resetNoClean() { \
      raw = nullptr; \
    }     \
\
  private: \
    UnderlyingType raw; \
\
    void cleanup() { \
      if (raw) { \
        DISPOSE_FUNC(raw); \
        raw = nullptr; \
      } \
    } \
  };


#define DEFINE_PY_WRAPPER_CLASS_COPYABLE(ClassName, UnderlyingType) \
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

#define DEFINE_PY_WRAPPER_CLASS_COPYABLE_POLYMORPHIC(ClassName, UnderlyingType) \
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


DEFINE_PY_WRAPPER_CLASS_COPYABLE_POLYMORPHIC(PyValue, LLVMValueRef)
DEFINE_PY_WRAPPER_CLASS_COPYABLE_POLYMORPHIC(PyType, LLVMTypeRef)
DEFINE_PY_WRAPPER_CLASS_COPYABLE(PyDiagnosticInfo, LLVMDiagnosticInfoRef)
DEFINE_PY_WRAPPER_CLASS_COPYABLE_POLYMORPHIC(PyAttribute, LLVMAttributeRef)
DEFINE_PY_WRAPPER_CLASS_COPYABLE(PyNamedMDNode, LLVMNamedMDNodeRef)
DEFINE_PY_WRAPPER_CLASS_COPYABLE(PyUse, LLVMUseRef)
DEFINE_PY_WRAPPER_CLASS_COPYABLE(PyBasicBlock, LLVMBasicBlockRef)
DEFINE_PY_WRAPPER_CLASS_COPYABLE(PyBuilder, LLVMBuilderRef)

DEFINE_PY_WRAPPER_CLASS_COPYABLE(PyMetadata, LLVMMetadataRef)
DEFINE_DIRECT_SUB_CLASS(PyMetadata, PyMDNode)
DEFINE_DIRECT_SUB_CLASS(PyMetadata, PyValueAsMetadata)
DEFINE_DIRECT_SUB_CLASS(PyMetadata, PyMDString)



DEFINE_DIRECT_SUB_CLASS(PyAttribute, PyEnumAttribute);
DEFINE_DIRECT_SUB_CLASS(PyAttribute, PyTypeAttribute);
DEFINE_DIRECT_SUB_CLASS(PyAttribute, PyStringAttribute);

PY_FOR_EACH_VALUE_CLASS_RELATIONSHIP(DEFINE_DIRECT_SUB_CLASS)
PY_FOR_EACH_TYPE_CLASS_RELASIONSHIP(DEFINE_DIRECT_SUB_CLASS)

DEFINE_PY_WRAPPER_CLASS_COPYABLE(PyIntrinsic, unsigned)

DEFINE_PY_WRAPPER_CLASS_SELF_DISPOSABLE_NONCOPYABLE
  (PyModuleProvider, LLVMModuleProviderRef, LLVMDisposeModuleProvider)

DEFINE_PY_WRAPPER_CLASS_SELF_DISPOSABLE_NONCOPYABLE
  (PyMemoryBuffer, LLVMMemoryBufferRef, LLVMDisposeMemoryBuffer)

DEFINE_PY_WRAPPER_CLASS_SELF_DISPOSABLE_NONCOPYABLE
  (PyPassManagerBase, LLVMPassManagerRef, LLVMDisposePassManager)

DEFINE_DIRECT_SUB_CLASS(PyPassManagerBase, PyPassManager);
DEFINE_DIRECT_SUB_CLASS(PyPassManagerBase, PyFunctionPassManager);


/*
  shared pointer is need to make a NonCopyable object able to be used in
  std::vector
  TODO test (nanobind)
  another approach is to keep a reference counter inside the counter, but this
  seems more complex and needs more integration (nanobind) effort (though it has guide)
 */
class PyOperandBundle : public std::enable_shared_from_this<PyOperandBundle>,
public NonCopyable {
public:
  explicit PyOperandBundle(LLVMOperandBundleRef bundle) : bundle(bundle) {}
  
  ~PyOperandBundle() {
    cleanup();
  }

  LLVMOperandBundleRef get() const {
    return bundle;
  }

  DEFINE_MOVE_SEMANTICS_CLEANUP(PyOperandBundle, bundle)

private:
  LLVMOperandBundleRef bundle;

  void cleanup() {
    if (bundle) {
      LLVMDisposeOperandBundle(bundle);
      bundle = nullptr;
    }
  }
};




typedef LLVMModuleFlagEntry *LLVMModuleFlagEntries;

class PyModuleFlagEntries : public NonCopyable {
public:
  explicit PyModuleFlagEntries(LLVMModuleFlagEntries entries, size_t len)
  : entries(entries), len(len) {}
  
  ~PyModuleFlagEntries() {
    cleanup();
  }

  LLVMModuleFlagEntries get() const {
    return entries;
  }

  size_t getLen() const {
    return len;
  }

  DEFINE_MOVE_SEMANTICS_CLEANUP_2(PyModuleFlagEntries, entries, len, 0)
  

private:
  LLVMModuleFlagEntries entries;
  size_t len;

  void cleanup() {
    if (entries) {
      LLVMDisposeModuleFlagsMetadata(entries);
      entries = nullptr;
    }
  }
};


typedef LLVMValueMetadataEntry *LLVMValueMetadataEntries;

class PyMetadataEntries : public NonCopyable {
public:
  explicit PyMetadataEntries(LLVMValueMetadataEntries entries, size_t len)
  : entries(entries), len(len) {}
  
  ~PyMetadataEntries() {
    cleanup();
  }

  LLVMValueMetadataEntries get() const {
    return entries;
  }

  size_t getLen() const {
    return len;
  }

  DEFINE_MOVE_SEMANTICS_CLEANUP_2(PyMetadataEntries, entries, len, 0)
  

private:
  LLVMValueMetadataEntries entries;
  size_t len;

  void cleanup() {
    if (entries) {
      LLVMDisposeValueMetadataEntries(entries);
      entries = nullptr;
    }
  }
};



class PyContext : public NonCopyable {
public:
  explicit PyContext() : context(LLVMContextCreate()), is_global_context(false) {}

  explicit PyContext(LLVMContextRef context, bool is_global_context)
  : context(context), is_global_context(is_global_context) {}

  explicit PyContext(LLVMContextRef context) : context(context) {
    // TODO check
    LLVMContextRef global_context = LLVMGetGlobalContext();
    is_global_context = global_context == context;
  }

  static PyContext getGlobalContext() {
    return PyContext(LLVMGetGlobalContext(), true);
  }

  ~PyContext() {
    cleanup();
  }

  LLVMContextRef get() const {
    return context;
  }

  DEFINE_MOVE_SEMANTICS_CLEANUP_2(PyContext, context, is_global_context, false)

  void cleanup() {
    // don't clean global context, which is managed by LLVM
    if (context && !is_global_context) {
      LLVMContextDispose(context);
      context = nullptr;
    }
  }


private:
  LLVMContextRef context;
  bool is_global_context;
};





class PyModule : public NonCopyable {
public:
  explicit PyModule(const std::string &id) {
    module = LLVMModuleCreateWithName(id.c_str());
    if (!module) {
      throw std::runtime_error("Failed to create LLVM module");
    }
  }

  explicit PyModule(const std::string &id, PyContext &context) {
    module = LLVMModuleCreateWithNameInContext(id.c_str(), context.get());
    if (!module) {
      throw std::runtime_error("Failed to create LLVM module");
    }
  }

  explicit PyModule(LLVMModuleRef module) : module(module) {}

  ~PyModule() {
    cleanup();
  }

  
  LLVMModuleRef get() {
    return module;
  }

  DEFINE_MOVE_SEMANTICS_CLEANUP(PyModule, module)

  void cleanup() {
    if (module) {
      LLVMDisposeModule(module);
      module = nullptr;
    }
  }
  
private:
  LLVMModuleRef module;
};





#endif
