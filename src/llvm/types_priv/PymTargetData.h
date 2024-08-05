#ifndef LLVMPYM_TYPES_PRIV_PYMTARGETDATA_H
#define LLVMPYM_TYPES_PRIV_PYMTARGETDATA_H

#include <llvm-c/Target.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PymLLVMObject.h"
#include "utils.h"

class PymTargetData : public PymLLVMObject<PymTargetData, LLVMTargetDataRef> {
public:
  explicit PymTargetData(LLVMTargetDataRef data);
  LLVMTargetDataRef get() const;
  
private:
  SHARED_POINTER_DEF(LLVMTargetDataRef, LLVMOpaqueTargetData);
};


#endif
