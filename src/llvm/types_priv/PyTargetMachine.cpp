#include "PyTargetMachine.h"

PyTargetMachine::PyTargetMachine(LLVMTargetMachineRef machine)
: obj(get_shared_obj(machine)) { }

LLVMTargetMachineRef PyTargetMachine::get() const {
  return obj.get();
}

SHARED_POINTER_IMPL(PyTargetMachine, LLVMTargetMachineRef, LLVMOpaqueTargetMachine,
                    LLVMDisposeTargetMachine)

