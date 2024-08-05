#include "PymTargetMachineOptions.h"

PymTargetMachineOptions::PymTargetMachineOptions(LLVMTargetMachineOptionsRef obj)
: obj(get_shared_obj(obj)) {}

LLVMTargetMachineOptionsRef PymTargetMachineOptions::get() const {
  return obj.get();
}


SHARED_POINTER_IMPL(PymTargetMachineOptions, LLVMTargetMachineOptionsRef,
                    LLVMOpaqueTargetMachineOptions, LLVMDisposeTargetMachineOptions)
