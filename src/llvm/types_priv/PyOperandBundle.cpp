#include "PyOperandBundle.h"

PyOperandBundle::PyOperandBundle(LLVMOperandBundleRef obj)
: obj(get_shared_obj(obj)) { }

LLVMOperandBundleRef PyOperandBundle::get() const {
  return obj.get();
}


SHARED_POINTER_IMPL(PyOperandBundle, LLVMOperandBundleRef, LLVMOpaqueOperandBundle,
                    LLVMDisposeOperandBundle)

