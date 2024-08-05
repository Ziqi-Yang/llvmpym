#ifndef LLVMPYM_TYPES_PRIV_PYMMEMORYBUFFER_H
#define LLVMPYM_TYPES_PRIV_PYMMEMORYBUFFER_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PymLLVMObject.h"
#include "utils.h"

class PymMemoryBuffer : public PymLLVMObject<PymMemoryBuffer, LLVMMemoryBufferRef> {
public:
  explicit PymMemoryBuffer(LLVMMemoryBufferRef mb);
  LLVMMemoryBufferRef get() const;

  /*
   * This function reset the PymMemoryBuffer object, preventing it from 
   * being automatically disposed
   */
  void reset();
  
private:
  SHARED_POINTER_DEF(LLVMMemoryBufferRef, LLVMOpaqueMemoryBuffer);
};

#endif
