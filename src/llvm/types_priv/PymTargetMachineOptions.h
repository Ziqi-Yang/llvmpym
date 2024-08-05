#ifndef LLVMPYM_TYPES_PRIV_PYMTARGETMACHINEOPTIONS_H
#define LLVMPYM_TYPES_PRIV_PYMTARGETMACHINEOPTIONS_H

#include <llvm-c/TargetMachine.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PymLLVMObject.h"
#include "utils.h"

class PymTargetMachineOptions : public PymLLVMObject<PymTargetMachineOptions, LLVMTargetMachineOptionsRef> {
public:
  explicit PymTargetMachineOptions(LLVMTargetMachineOptionsRef machine);
  LLVMTargetMachineOptionsRef get() const;
  
private:
  SHARED_POINTER_DEF(LLVMTargetMachineOptionsRef, LLVMOpaqueTargetMachineOptions);
};




#endif
