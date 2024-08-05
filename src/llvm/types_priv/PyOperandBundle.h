#ifndef LLVMPYM_TYPES_PRIV_PYOPERANDBUNDLE_H
#define LLVMPYM_TYPES_PRIV_PYOPERANDBUNDLE_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PyLLVMObject.h"
#include "utils.h"

class PyOperandBundle : public PyLLVMObject<PyOperandBundle, LLVMOperandBundleRef> {
public:
  explicit PyOperandBundle(LLVMOperandBundleRef bundle);
  LLVMOperandBundleRef get() const;
  
private:
  SHARED_POINTER_DEF(LLVMOperandBundleRef, LLVMOpaqueOperandBundle);
};


#endif
