#include "PyModuleProvider.h"

PyModuleProvider::PyModuleProvider(LLVMModuleProviderRef obj)
: obj(get_shared_obj(obj)) {}

LLVMModuleProviderRef PyModuleProvider::get() const {
  return obj.get();
}

SHARED_POINTER_IMPL(PyModuleProvider, LLVMModuleProviderRef, LLVMOpaqueModuleProvider,
                    LLVMDisposeModuleProvider)
