#ifndef LLVMPYM_TYPES_H
#define LLVMPYM_TYPES_H

#include <llvm-c/Core.h>
#include <utility>

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


#define DEFINE_MOVE_SEMANTICS(ClassName) \
  ClassName(ClassName&& other) noexcept { \
    move(std::move(other)); \
  } \
  ClassName& operator=(ClassName&& other) noexcept { \
    if (this != &other) { \
      move(std::move(other)); \
    } \
    return *this; \
  }

#define DEFINE_PY_WRAPPER_CLASS_NONCOPYABLE(ClassName, UnderlyingType, UnderlyingName) \
  class ClassName : public NonCopyable { \
  public: \
    explicit ClassName(UnderlyingType UnderlyingName) \
    : UnderlyingName(UnderlyingName) {} \
    \
    DEFINE_MOVE_SEMANTICS(ClassName) \
    \
    void move(ClassName &&other) noexcept { \
      UnderlyingName = std::exchange(other.UnderlyingName, nullptr); \
    } \
    \
    UnderlyingType get() const { \
      return UnderlyingName; \
    } \
    \
  private: \
    UnderlyingType UnderlyingName; \
  };

#define DEFINE_PY_WRAPPER_CLASS_NONCOPYABLE_POLYMORPHIC(ClassName, UnderlyingType, UnderlyingName) \
  class ClassName : public NonCopyable { \
  public: \
    virtual ~ClassName() = default; \
    explicit ClassName(UnderlyingType UnderlyingName) \
    : UnderlyingName(UnderlyingName) {} \
    \
    DEFINE_MOVE_SEMANTICS(ClassName) \
    \
    void move(ClassName &&other) noexcept { \
      UnderlyingName = std::exchange(other.UnderlyingName, nullptr); \
    } \
    \
    UnderlyingType get() const { \
      return UnderlyingName; \
    } \
    \
  private: \
    UnderlyingType UnderlyingName; \
  };



#define DEFINE_PY_WRAPPER_CLASS_COPYABLE(ClassName, UnderlyingType, UnderlyingName) \
  class ClassName { \
  public: \
    explicit ClassName(UnderlyingType UnderlyingName) \
    : UnderlyingName(UnderlyingName) {} \
    \
    UnderlyingType get() const { \
      return UnderlyingName; \
    } \
    \
  private: \
    UnderlyingType UnderlyingName; \
  };

#define DEFINE_PY_WRAPPER_CLASS_COPYABLE_POLYMORPHIC(ClassName, UnderlyingType, UnderlyingName) \
  class ClassName { \
  public: \
    virtual ~ClassName() = default; \
    explicit ClassName(UnderlyingType UnderlyingName) \
    : UnderlyingName(UnderlyingName) {} \
    \
    UnderlyingType get() const { \
      return UnderlyingName; \
    } \
    \
  private: \
    UnderlyingType UnderlyingName; \
  };


#define DEFINE_DIRECT_SUB_CLASS(ParentClassName, ClassName) \
  class ClassName : public ParentClassName { \
  public: \
    using ParentClassName::ParentClassName; \
  };


#define PY_DECLARE_VALUE_CAST(name) \
  .def("to_" #name, \
       [](PyValue &v) -> std::optional<Py##name> { \
         auto res = LLVMIsA##name(v.get()); \
         if (res) \
           return Py##name(res); \
         return std::nullopt; \
       }, \
       "None means conversion failed.")

// note AMDNode, ValueAsMetadata and AMDString are not there
// since it provides `LLVMIsAMDNode` function rather than `LLVMIsAAMDNode` function...
// (they are also not in LLVM_FOR_EACH_VALUE_SUBCLASS)
#define PY_FOR_EACH_VALUE_SUBCLASS(macro) \
  macro(Argument)                           \
  macro(BasicBlock)                         \
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

// my addition: the first 3
#define PY_FOR_EACH_VALUE_CLASS_RELATIONSHIP(macro) \
  macro(PyValue, PyAMDNode) \
  macro(PyValue, PyValueAsMetadata) \
  macro(PyValue, PyAMDString) \
  macro(PyValue, PyArgument)                           \
  macro(PyValue, PyBasicBlock)                         \
  macro(PyValue, PyInlineAsm)                          \
  macro(PyValue, PyUser)                               \
    macro(PyUser, PyConstant)                         \
      macro(PyConstant, PyBlockAddress)                   \
      macro(PyConstant, PyConstantAggregateZero)          \
      macro(PyConstant, PyConstantArray)                  \
      macro(PyConstant, PyConstantDataSequential)         \
        macro(PyConstantDataSequential, PyConstantDataArray)            \
        macro(PyConstantDataSequential, PyConstantDataVector)           \
      macro(PyConstant, PyConstantExpr)                   \
      macro(PyConstant, PyConstantFP)                     \
      macro(PyConstant, PyConstantInt)                    \
      macro(PyConstant, PyConstantPointerNull)            \
      macro(PyConstant, PyConstantStruct)                 \
      macro(PyConstant, PyConstantTokenNone)              \
      macro(PyConstant, PyConstantVector)                 \
      macro(PyConstant, PyGlobalValue)                    \
        macro(PyGlobalValue, PyGlobalAlias)                  \
        macro(PyGlobalValue, PyGlobalObject)                 \
          macro(PyGlobalObject, PyFunction)                   \
          macro(PyGlobalObject, PyGlobalVariable)             \
          macro(PyGlobalObject, PyGlobalIFunc)                \
      macro(PyConstant, PyUndefValue)                     \
      macro(PyConstant, PyPoisonValue)                    \
    macro(PyUser, PyInstruction)                      \
      macro(PyInstruction, PyUnaryOperator)                  \
      macro(PyInstruction, PyBinaryOperator)                 \
      macro(PyInstruction, PyCallInst)                       \
        macro(PyCallInst, PyIntrinsicInst)                \
          macro(PyIntrinsicInst, PyDbgInfoIntrinsic)           \
            macro(PyDbgInfoIntrinsic, PyDbgVariableIntrinsic)     \
              macro(PyDbgVariableIntrinsic, PyDbgDeclareInst)         \
            macro(PyIntrinsicInst, PyDbgLabelInst)             \
          macro(PyIntrinsicInst, PyMemIntrinsic)               \
            macro(PyIntrinsicInst, PyMemCpyInst)               \
            macro(PyIntrinsicInst, PyMemMoveInst)              \
            macro(PyIntrinsicInst, PyMemSetInst)               \
      macro(PyInstruction, PyCmpInst)                        \
        macro(PyCmpInst, PyFCmpInst)                     \
        macro(PyCmpInst, PyICmpInst)                     \
      macro(PyInstruction, PyExtractElementInst)             \
      macro(PyInstruction, PyGetElementPtrInst)              \
      macro(PyInstruction, PyInsertElementInst)              \
      macro(PyInstruction, PyInsertValueInst)                \
      macro(PyInstruction, PyLandingPadInst)                 \
      macro(PyInstruction, PyPHINode)                        \
      macro(PyInstruction, PySelectInst)                     \
      macro(PyInstruction, PyShuffleVectorInst)              \
      macro(PyInstruction, PyStoreInst)                      \
      macro(PyInstruction, PyBranchInst)                     \
      macro(PyInstruction, PyIndirectBrInst)                 \
      macro(PyInstruction, PyInvokeInst)                     \
      macro(PyInstruction, PyReturnInst)                     \
      macro(PyInstruction, PySwitchInst)                     \
      macro(PyInstruction, PyUnreachableInst)                \
      macro(PyInstruction, PyResumeInst)                     \
      macro(PyInstruction, PyCleanupReturnInst)              \
      macro(PyInstruction, PyCatchReturnInst)                \
      macro(PyInstruction, PyCatchSwitchInst)                \
      macro(PyInstruction, PyCallBrInst)                     \
      macro(PyInstruction, PyFuncletPadInst)                 \
        macro(PyFuncletPadInst, PyCatchPadInst)                 \
        macro(PyFuncletPadInst, PyCleanupPadInst)               \
      macro(PyInstruction, PyUnaryInstruction)               \
        macro(PyUnaryInstruction, PyAllocaInst)                   \
        macro(PyUnaryInstruction, PyCastInst)                     \
          macro(PyCastInst, PyAddrSpaceCastInst)          \
          macro(PyCastInst, PyBitCastInst)                \
          macro(PyCastInst, PyFPExtInst)                  \
          macro(PyCastInst, PyFPToSIInst)                 \
          macro(PyCastInst, PyFPToUIInst)                 \
          macro(PyCastInst, PyFPTruncInst)                \
          macro(PyCastInst, PyIntToPtrInst)               \
          macro(PyCastInst, PyPtrToIntInst)               \
          macro(PyCastInst, PySExtInst)                   \
          macro(PyCastInst, PySIToFPInst)                 \
          macro(PyCastInst, PyTruncInst)                  \
          macro(PyCastInst, PyUIToFPInst)                 \
          macro(PyCastInst, PyZExtInst)                   \
        macro(PyUnaryInstruction, PyExtractValueInst)             \
        macro(PyUnaryInstruction, PyLoadInst)                     \
        macro(PyUnaryInstruction, PyVAArgInst)                    \
        macro(PyUnaryInstruction, PyFreezeInst)                   \
      macro(PyInstruction, PyAtomicCmpXchgInst)              \
      macro(PyInstruction, PyAtomicRMWInst)                  \
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


DEFINE_PY_WRAPPER_CLASS_COPYABLE_POLYMORPHIC(PyValue, LLVMValueRef, value)
DEFINE_PY_WRAPPER_CLASS_COPYABLE_POLYMORPHIC(PyType, LLVMTypeRef, type)
DEFINE_PY_WRAPPER_CLASS_COPYABLE(PyDiagnosticInfo, LLVMDiagnosticInfoRef, diagnosticInfo)
DEFINE_PY_WRAPPER_CLASS_COPYABLE_POLYMORPHIC(PyAttribute, LLVMAttributeRef, attribute)
DEFINE_PY_WRAPPER_CLASS_COPYABLE(PyMetadata, LLVMMetadataRef, metadata)
DEFINE_PY_WRAPPER_CLASS_COPYABLE(PyNamedMDNode, LLVMNamedMDNodeRef, namedMDNode)
DEFINE_PY_WRAPPER_CLASS_COPYABLE(PyUse, LLVMUseRef, Use)
DEFINE_PY_WRAPPER_CLASS_COPYABLE(PyBasicBlockWrapper, LLVMBasicBlockRef, basicBlock)

DEFINE_DIRECT_SUB_CLASS(PyAttribute, PyEnumAttribute);
DEFINE_DIRECT_SUB_CLASS(PyAttribute, PyTypeAttribute);
DEFINE_DIRECT_SUB_CLASS(PyAttribute, PyStringAttribute);

PY_FOR_EACH_VALUE_CLASS_RELATIONSHIP(DEFINE_DIRECT_SUB_CLASS)
PY_FOR_EACH_TYPE_CLASS_RELASIONSHIP(DEFINE_DIRECT_SUB_CLASS)


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

  DEFINE_MOVE_SEMANTICS(PyModuleFlagEntries)

  void move(PyModuleFlagEntries &&other) noexcept {
    cleanup();
    entries = std::exchange(other.entries, nullptr);
  }
  

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

  DEFINE_MOVE_SEMANTICS(PyMetadataEntries)

  void move(PyMetadataEntries &&other) noexcept {
    cleanup();
    entries = std::exchange(other.entries, nullptr);
  }
  

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

  DEFINE_MOVE_SEMANTICS(PyContext)

  void move(PyContext &&other) noexcept {
    cleanup();
    context = std::exchange(other.context, nullptr);
    is_global_context = other.is_global_context;
    other.is_global_context = false;
  }

private:
  LLVMContextRef context;
  bool is_global_context;

  void cleanup() {
    // don't clean global context, which is managed by LLVM
    if (context && !is_global_context) {
      LLVMContextDispose(context);
      context = nullptr;
    }
  }
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

  DEFINE_MOVE_SEMANTICS(PyModule)

  void move(PyModule &&other) noexcept {
    cleanup();
    module = std::exchange(other.module, nullptr);
  }
  
private:
  LLVMModuleRef module;

  void cleanup() {
    if (module) {
      LLVMDisposeModule(module);
      module = nullptr;
    }
  }
};



#endif
