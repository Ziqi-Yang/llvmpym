#ifndef LLVMPYM_TYPES_PRIV_PYMODULEPROVIDER_H
#define LLVMPYM_TYPES_PRIV_PYMODULEPROVIDER_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PyLLVMObject.h"

class PyModuleProvider : public PyLLVMObject<PyModuleProvider, LLVMModuleProviderRef>{
public:
  explicit PyModuleProvider(LLVMModuleProviderRef mp);
  LLVMModuleProviderRef get() const;
  
private:
  std::shared_ptr<LLVMOpaqueModuleProvider> mp;

  struct LLVMModuleProviderRefDeleter {
    void operator()(LLVMModuleProviderRef mp) const;
  };

  static std::shared_ptr<LLVMOpaqueModuleProvider> get_shared_mp(LLVMModuleProviderRef mp);

  static std::unordered_map<LLVMModuleProviderRef,
                            std::weak_ptr<LLVMOpaqueModuleProvider>> mp_map;
  static std::mutex map_mutex;
};


#endif
