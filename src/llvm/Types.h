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

#define DEFINE_PY_WRAPPER_CLASS(ClassName, UnderlyingType, UnderlyingName) \
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
    UnderlyingType get() { \
      return UnderlyingName; \
    } \
    \
  private: \
    UnderlyingType UnderlyingName; \
  };


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


DEFINE_PY_WRAPPER_CLASS(PyValue, LLVMValueRef, value)
DEFINE_PY_WRAPPER_CLASS(PyType, LLVMTypeRef, type)
DEFINE_PY_WRAPPER_CLASS(PyDiagnosticInfo, LLVMDiagnosticInfoRef, diagnosticInfo)
DEFINE_PY_WRAPPER_CLASS(PyAttribute, LLVMAttributeRef, attribute)

class PyEnumAttribute : public PyAttribute {
public:
  using PyAttribute::PyAttribute;
};


class PyTypeAttribute : public PyAttribute {
public:
  using PyAttribute::PyAttribute;
};

class PyStringAttribute : public PyAttribute {
public:
  using PyAttribute::PyAttribute;
};





class PyContext : public NonCopyable {
public:
  explicit PyContext() : context(LLVMContextCreate()), is_global_context(false) {}

  explicit PyContext(LLVMContextRef context, bool is_global_context)
  : context(context), is_global_context(is_global_context) {}

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
