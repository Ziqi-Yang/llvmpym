#ifndef LLVMPYM_TYPES_PRIV_PYMTARGETMACHINE_H
#define LLVMPYM_TYPES_PRIV_PYMTARGETMACHINE_H

#include <llvm-c/TargetMachine.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PymLLVMObject.h"
#include "utils.h"

class PymTargetMachine : public PymLLVMObject<PymTargetMachine, LLVMTargetMachineRef> {
public:
  explicit PymTargetMachine(LLVMTargetMachineRef machine);
  LLVMTargetMachineRef get() const;
  
private:
  SHARED_POINTER_DEF(LLVMTargetMachineRef, LLVMOpaqueTargetMachine);
};



#endif
