#ifndef LLVMPYM_TYPES_H
#define LLVMPYM_TYPES_H

#include <llvm-c/Core.h>
#include <utility>

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


class PyValue : public NonCopyable {
public:
  explicit PyValue(LLVMValueRef value) : value(value) {}

  PyValue(PyValue&& other) noexcept {
    move(std::move(other));
  }

  PyValue& operator=(PyValue&& other) noexcept {
    if (this != &other) {
      move(std::move(other));
    }
    return *this;
  }

  void move(PyValue &&other) noexcept {
    value = std::exchange(other.value, nullptr);
  }
  
private:
  LLVMValueRef value;
};





class PyContext : public NonCopyable {
public:
  explicit PyContext() : context(LLVMContextCreate()) {}

  explicit PyContext(LLVMContextRef context) : context(context) {}

  static PyContext getGlobalContext() {
    return PyContext(LLVMGetGlobalContext());
  }

  ~PyContext() {
    cleanup();
  }

  PyContext(PyContext&& other) noexcept {
    move(std::move(other));
  }

  PyContext& operator=(PyContext&& other) noexcept {
    if (this != &other) {
      move(std::move(other));
    }
    return *this;
  }

  void move(PyContext &&other) noexcept {
    cleanup();
    context = std::exchange(other.context, nullptr);
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


class PyModule : public NonCopyable {
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

  
  LLVMModuleRef get() {
    return module;
  }


  PyModule(PyModule&& other) noexcept {
    move(std::move(other));
  }

  PyModule& operator=(PyModule&& other) noexcept {
    if (this != &other) {
      move(std::move(other));
    }
    return *this;
  }

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
