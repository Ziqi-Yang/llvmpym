#ifndef LLVMPYM_TYPE_PYCONTEXT_H
#define LLVMPYM_TYPE_PYCONTEXT_H

#include <llvm-c/Core.h>
#include <stdexcept>
#include <memory>
#include <unordered_map>
#include <mutex>

class PyContext {
public:
  explicit PyContext();
  explicit PyContext(LLVMContextRef context, bool is_global_context);
  explicit PyContext(LLVMContextRef context);
  
  static PyContext getGlobalContext();
  
  LLVMContextRef get() const;

private:
  std::shared_ptr<LLVMOpaqueContext> context;
  bool is_global_context;

  struct LLVMContextDeleter {
    LLVMContextDeleter(const PyContext* parent) : parent(parent) {}
    void operator()(LLVMOpaqueContext* context) const;
    
    const PyContext* parent;
  };

  static std::shared_ptr<LLVMOpaqueContext> get_shared_context(LLVMContextRef context,
   const PyContext* py_context);

  static std::unordered_map<LLVMOpaqueContext*, std::weak_ptr<LLVMOpaqueContext>> context_map;
  static std::mutex map_mutex;
};




#endif
