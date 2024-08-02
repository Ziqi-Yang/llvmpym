#ifndef LLVMPYM__TYPES_PYMEMORYBUFFER_H
#define LLVMPYM__TYPES_PYMEMORYBUFFER_H

#include <llvm-c/Core.h>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <mutex>

class PyMemoryBuffer {
public:
  explicit PyMemoryBuffer(LLVMMemoryBufferRef mb);
  LLVMMemoryBufferRef get() const;
  
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
