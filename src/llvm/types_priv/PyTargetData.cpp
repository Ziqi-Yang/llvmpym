#include "PyTargetData.h"

PyTargetData::PyTargetData(LLVMTargetDataRef data)
: obj(get_shared_obj(data)) {}

LLVMTargetDataRef PyTargetData::get() const {
  return obj.get();
}

SHARED_POINTER_IMPL(PyTargetData, LLVMTargetDataRef, LLVMOpaqueTargetData,
                    LLVMDisposeTargetData)
