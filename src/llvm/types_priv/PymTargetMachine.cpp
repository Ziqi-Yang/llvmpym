#include "PymTargetMachine.h"

PymTargetMachine::PymTargetMachine(LLVMTargetMachineRef machine)
: obj(get_shared_obj(machine)) { }

LLVMTargetMachineRef PymTargetMachine::get() const {
  return obj.get();
}

SHARED_POINTER_IMPL(PymTargetMachine, LLVMTargetMachineRef, LLVMOpaqueTargetMachine,
                    LLVMDisposeTargetMachine)

