#ifndef LLVMPYM_TYPES_PRIV_PYMEMORYBUFFER_H
#define LLVMPYM_TYPES_PRIV_PYMEMORYBUFFER_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PyLLVMObject.h"
#include "utils.h"

class PyMemoryBuffer : public PyLLVMObject<PyMemoryBuffer, LLVMMemoryBufferRef> {
public:
  explicit PyMemoryBuffer(LLVMMemoryBufferRef mb);
  LLVMMemoryBufferRef get() const;

  /*
   * This function reset the PyMemoryBuffer object, preventing it from 
   * being automatically disposed
   */
  void reset();
  
private:
  SHARED_POINTER_DEF(LLVMMemoryBufferRef, LLVMOpaqueMemoryBuffer);
};

#endif
