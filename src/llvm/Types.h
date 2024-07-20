#ifndef LLVMPYM_TYPES_H
#define LLVMPYM_TYPES_H

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


template <typename Derived>
class MoveOnly {
public:
  MoveOnly(const MoveOnly &) = delete;
  MoveOnly &operator=(const MoveOnly &) = delete;

  MoveOnly(MoveOnly &&other) noexcept {
    static_cast<Derived*>(this)->moveImpl(std::move(other));
  }

  MoveOnly &operator=(MoveOnly &&other) noexcept {
    if (this != &other) {
      static_cast<Derived*>(this)->moveImpl(std::move(other));
    }
    return *this;
  }

protected:
  MoveOnly() = default;
  virtual ~MoveOnly() = default;

private:
  void moveImpl(MoveOnly &&other) noexcept {
    static_cast<Derived*>(this)->move(std::move(static_cast<Derived&>(other)));
  }
};



class PyValue : public MoveOnly<PyValue> {
public:
  PyValue(LLVMValueRef value) : value(value) {}
  void move(PyValue &&other) noexcept {
    value = other.value;
    other.value = nullptr;
  }

private:
  LLVMValueRef value;

};



class PyContext : public MoveOnly<PyContext> {
public:
  explicit PyContext() {
    context = LLVMContextCreate();
  }

  explicit PyContext(LLVMContextRef context) {
    context = context;
  }

  static PyContext getGlobalContext() {
    return PyContext(LLVMGetGlobalContext());
  }
  
  ~PyContext() {
    cleanup();
  }

  void move(PyContext &&other) noexcept {
    cleanup();
    context = other.context;
    other.context = nullptr;
  }
  
private:
  LLVMContextRef context;

  void cleanup() {
    if (context) {
      LLVMContextDispose(context);
      context = nullptr;
    }
  }
};



class PyModule : public MoveOnly<PyModule> {
public:
  explicit PyModule(const std::string &id) {
    module = LLVMModuleCreateWithName(id.c_str());
    if (!module) {
      throw std::runtime_error("Failed to create LLVM module");
    }
  }

  ~PyModule() {
    cleanup();
  }

  
  void move(PyModule &&other) noexcept {
    cleanup();
    module = other.module;
    other.module = nullptr;
  }

  LLVMModuleRef get() {
    return module;
  }

private:
  LLVMModuleRef module;

  void cleanup() {
    if (module) {
      LLVMDisposeModule(module);
      module = nullptr;
    }
  }

  
public:
  void setModuleIdentifier(const std::string &identifier) {
    LLVMSetModuleIdentifier(module, identifier.c_str(), identifier.size());
  }

  std::string getModuleIdentifier() const {
    size_t len;
    const char *identifier = LLVMGetModuleIdentifier(module, &len);
    return std::string(identifier, len);
  }
};




#endif
