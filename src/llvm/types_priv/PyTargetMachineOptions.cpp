#include "PyTargetMachineOptions.h"

PyTargetMachineOptions::PyTargetMachineOptions(LLVMTargetMachineOptionsRef obj)
: obj(get_shared_obj(obj)) {}

LLVMTargetMachineOptionsRef PyTargetMachineOptions::get() const {
  return obj.get();
}


SHARED_POINTER_IMPL(PyTargetMachineOptions, LLVMTargetMachineOptionsRef,
                    LLVMOpaqueTargetMachineOptions, LLVMDisposeTargetMachineOptions)
