#ifndef LLVMPYM_TYPES_PRIV_PYTARGETDATA_H
#define LLVMPYM_TYPES_PRIV_PYTARGETDATA_H

#include <llvm-c/Target.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PyLLVMObject.h"
#include "utils.h"

class PyTargetData : public PyLLVMObject<PyTargetData, LLVMTargetDataRef> {
public:
  explicit PyTargetData(LLVMTargetDataRef data);
  LLVMTargetDataRef get() const;
  
private:
  SHARED_POINTER_DEF(LLVMTargetDataRef, LLVMOpaqueTargetData);
};


#endif
