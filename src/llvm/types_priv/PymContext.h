#ifndef LLVMPYM_TYPES_PRIV_PYMCONTEXT_H
#define LLVMPYM_TYPES_PRIV_PYMCONTEXT_H

#include <llvm-c/Core.h>
#include <stdexcept>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PymLLVMObject.h"

class PymContext : public PymLLVMObject<PymContext, LLVMContextRef> {
public:
  explicit PymContext();
  explicit PymContext(LLVMContextRef context, bool is_global_context);
  explicit PymContext(LLVMContextRef context);
  static PymContext getGlobalContext();
  
  LLVMContextRef get() const;

private:
  std::shared_ptr<LLVMOpaqueContext> context;
  bool is_global_context;

  struct LLVMContextDeleter {
    void operator()(LLVMContextRef context) const;
  };

  static std::shared_ptr<LLVMOpaqueContext> get_shared_context(LLVMContextRef context,
   bool is_global_context);

  static std::unordered_map<LLVMContextRef, std::weak_ptr<LLVMOpaqueContext>> context_map;
  static std::mutex map_mutex;
};




#endif
