#include "PymModuleProvider.h"

PymModuleProvider::PymModuleProvider(LLVMModuleProviderRef obj)
: obj(get_shared_obj(obj)) {}

LLVMModuleProviderRef PymModuleProvider::get() const {
  return obj.get();
}

SHARED_POINTER_IMPL(PymModuleProvider, LLVMModuleProviderRef, LLVMOpaqueModuleProvider,
                    LLVMDisposeModuleProvider)
