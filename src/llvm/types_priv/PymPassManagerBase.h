#ifndef LLVMPYM_TYPES_PRIV_PYMPASSMANAGERBASE_H
#define LLVMPYM_TYPES_PRIV_PYMPASSMANAGERBASE_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PymLLVMObject.h"
#include "utils.h"

class PymPassManagerBase : public PymLLVMObject<PymPassManagerBase, LLVMPassManagerRef> {
public:
  explicit PymPassManagerBase(LLVMPassManagerRef pm);
  LLVMPassManagerRef get() const;
  
private:
  SHARED_POINTER_DEF(LLVMPassManagerRef, LLVMOpaquePassManager);
};


#endif
