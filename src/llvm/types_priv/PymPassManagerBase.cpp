#include "PymPassManagerBase.h"

PymPassManagerBase::PymPassManagerBase(LLVMPassManagerRef obj)
: obj(get_shared_obj(obj)) { }

LLVMPassManagerRef PymPassManagerBase::get() const {
  return obj.get();
}

SHARED_POINTER_IMPL(PymPassManagerBase, LLVMPassManagerRef, LLVMOpaquePassManager,
                    LLVMDisposePassManager)
