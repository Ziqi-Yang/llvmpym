#ifndef LLVMPYM_TYPES_PRIV_PYMMODULEPROVIDER_H
#define LLVMPYM_TYPES_PRIV_PYMMODULEPROVIDER_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PymLLVMObject.h"
#include "utils.h"

class PymModuleProvider : public PymLLVMObject<PymModuleProvider, LLVMModuleProviderRef>{
public:
  explicit PymModuleProvider(LLVMModuleProviderRef mp);
  LLVMModuleProviderRef get() const;
  
private:
  SHARED_POINTER_DEF(LLVMModuleProviderRef, LLVMOpaqueModuleProvider);
};


#endif
