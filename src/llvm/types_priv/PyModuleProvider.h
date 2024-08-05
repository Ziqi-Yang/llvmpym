#ifndef LLVMPYM_TYPES_PRIV_PYMODULEPROVIDER_H
#define LLVMPYM_TYPES_PRIV_PYMODULEPROVIDER_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PyLLVMObject.h"
#include "utils.h"

class PyModuleProvider : public PyLLVMObject<PyModuleProvider, LLVMModuleProviderRef>{
public:
  explicit PyModuleProvider(LLVMModuleProviderRef mp);
  LLVMModuleProviderRef get() const;
  
private:
  SHARED_POINTER_DEF(LLVMModuleProviderRef, LLVMOpaqueModuleProvider);
};


#endif
