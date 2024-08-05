#include "PymTargetData.h"

PymTargetData::PymTargetData(LLVMTargetDataRef data)
: obj(get_shared_obj(data)) {}

LLVMTargetDataRef PymTargetData::get() const {
  return obj.get();
}

SHARED_POINTER_IMPL(PymTargetData, LLVMTargetDataRef, LLVMOpaqueTargetData,
                    LLVMDisposeTargetData)
