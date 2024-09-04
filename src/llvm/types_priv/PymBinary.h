#ifndef LLVMPYM_TYPES_PRIV_PYMBINARY_H
#define LLVMPYM_TYPES_PRIV_PYMBINARY_H

#include <llvm-c/Object.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PymLLVMObject.h"
#include "utils.h"

class PymBinary : public PymLLVMObject<PymBinary, LLVMBinaryRef> {
public:
  explicit PymBinary(LLVMBinaryRef bin);
  LLVMBinaryRef get() const;
  
private:
  SHARED_POINTER_DEF(LLVMBinaryRef, LLVMOpaqueBinary);
};


#endif
