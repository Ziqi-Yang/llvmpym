#ifndef LLVMPYM_TYPES_PRIV_PYTARGETMACHINE_H
#define LLVMPYM_TYPES_PRIV_PYTARGETMACHINE_H

#include <llvm-c/TargetMachine.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PyLLVMObject.h"
#include "utils.h"

class PyTargetMachine : public PyLLVMObject<PyTargetMachine, LLVMTargetMachineRef> {
public:
  explicit PyTargetMachine(LLVMTargetMachineRef machine);
  LLVMTargetMachineRef get() const;
  
private:
  SHARED_POINTER_DEF(LLVMTargetMachineRef, LLVMOpaqueTargetMachine);
};



#endif
