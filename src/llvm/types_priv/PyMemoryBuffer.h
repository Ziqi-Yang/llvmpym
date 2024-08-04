#ifndef LLVMPYM_TYPES_PRIV_PYMEMORYBUFFER_H
#define LLVMPYM_TYPES_PRIV_PYMEMORYBUFFER_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PyLLVMObject.h"

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
  std::shared_ptr<LLVMOpaqueMemoryBuffer> mb;

  struct LLVMMemoryBufferRefDeleter {
    void operator()(LLVMMemoryBufferRef mb) const;
  };

  static std::shared_ptr<LLVMOpaqueMemoryBuffer> get_shared_mb(LLVMMemoryBufferRef mb);

  static std::unordered_map<LLVMMemoryBufferRef,
                            std::weak_ptr<LLVMOpaqueMemoryBuffer>> mb_map;
  static std::mutex map_mutex;
};

#endif
