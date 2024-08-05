#include "PyPassManagerBase.h"

PyPassManagerBase::PyPassManagerBase(LLVMPassManagerRef obj)
: obj(get_shared_obj(obj)) { }

LLVMPassManagerRef PyPassManagerBase::get() const {
  return obj.get();
}

SHARED_POINTER_IMPL(PyPassManagerBase, LLVMPassManagerRef, LLVMOpaquePassManager,
                    LLVMDisposePassManager)
