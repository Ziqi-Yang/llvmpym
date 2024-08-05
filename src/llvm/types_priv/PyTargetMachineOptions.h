#ifndef LLVMPYM_TYPES_PRIV_PYTARGETMACHINEOPTIONS_H
#define LLVMPYM_TYPES_PRIV_PYTARGETMACHINEOPTIONS_H

#include <llvm-c/TargetMachine.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PyLLVMObject.h"
#include "utils.h"

class PyTargetMachineOptions : public PyLLVMObject<PyTargetMachineOptions, LLVMTargetMachineOptionsRef> {
public:
  explicit PyTargetMachineOptions(LLVMTargetMachineOptionsRef machine);
  LLVMTargetMachineOptionsRef get() const;
  
private:
  SHARED_POINTER_DEF(LLVMTargetMachineOptionsRef, LLVMOpaqueTargetMachineOptions);
};




#endif
