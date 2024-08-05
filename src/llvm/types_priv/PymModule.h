#ifndef LLVMPYM_TYPES_PRIV_PYMMODULE_H
#define LLVMPYM_TYPES_PRIV_PYMMODULE_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <string>
#include "PymLLVMObject.h"
#include "utils.h"

class PymModule : public PymLLVMObject<PymModule, LLVMModuleRef> {
public:
  explicit PymModule(const std::string &id);
  explicit PymModule(const std::string &id, LLVMContextRef context);
  explicit PymModule(LLVMModuleRef module);

  LLVMModuleRef get() const;

private:
  SHARED_POINTER_DEF(LLVMModuleRef, LLVMOpaqueModule);
};

#endif
