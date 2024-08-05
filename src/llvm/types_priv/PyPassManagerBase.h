#ifndef LLVMPYM_TYPES_PRIV_PYPASSMANAGERBASE_H
#define LLVMPYM_TYPES_PRIV_PYPASSMANAGERBASE_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PyLLVMObject.h"
#include "utils.h"

class PyPassManagerBase : public PyLLVMObject<PyPassManagerBase, LLVMPassManagerRef> {
public:
  explicit PyPassManagerBase(LLVMPassManagerRef pm);
  LLVMPassManagerRef get() const;
  
private:
  SHARED_POINTER_DEF(LLVMPassManagerRef, LLVMOpaquePassManager);
};


#endif
