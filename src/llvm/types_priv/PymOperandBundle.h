#ifndef LLVMPYM_TYPES_PRIV_PYMOPERANDBUNDLE_H
#define LLVMPYM_TYPES_PRIV_PYMOPERANDBUNDLE_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PymLLVMObject.h"
#include "utils.h"

class PymOperandBundle : public PymLLVMObject<PymOperandBundle, LLVMOperandBundleRef> {
public:
  explicit PymOperandBundle(LLVMOperandBundleRef bundle);
  LLVMOperandBundleRef get() const;
  
private:
  SHARED_POINTER_DEF(LLVMOperandBundleRef, LLVMOpaqueOperandBundle);
};


#endif
