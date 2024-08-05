#include "PymOperandBundle.h"

PymOperandBundle::PymOperandBundle(LLVMOperandBundleRef obj)
: obj(get_shared_obj(obj)) { }

LLVMOperandBundleRef PymOperandBundle::get() const {
  return obj.get();
}


SHARED_POINTER_IMPL(PymOperandBundle, LLVMOperandBundleRef, LLVMOpaqueOperandBundle,
                    LLVMDisposeOperandBundle)

