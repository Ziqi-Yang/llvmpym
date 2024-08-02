#include "PyMemoryBuffer.h"
#include <iostream>

std::unordered_map<LLVMMemoryBufferRef,
                   std::weak_ptr<LLVMOpaqueMemoryBuffer>> PyMemoryBuffer::mb_map;
std::mutex PyMemoryBuffer::map_mutex;

PyMemoryBuffer::PyMemoryBuffer(LLVMMemoryBufferRef mb) : mb(get_shared_mb(mb)) { }

LLVMMemoryBufferRef PyMemoryBuffer::get() const {
  return mb.get();
}

// This function doesn't call `reset` method on shared_ptr object, since
// it will then call deleter, which is undesired and will lead to crash.
void PyMemoryBuffer::reset() {
  LLVMMemoryBufferRef m = mb.get();
  if (m) {
    std::lock_guard<std::mutex> lock(PyMemoryBuffer::map_mutex);
    PyMemoryBuffer::mb_map.erase(m);
 }
}


void PyMemoryBuffer::LLVMMemoryBufferRefDeleter::operator()
(LLVMMemoryBufferRef mb) const {
  if (mb) {
    std::lock_guard<std::mutex> lock(PyMemoryBuffer::map_mutex);
    auto it = PyMemoryBuffer::mb_map.find(mb);
    
    // the logic here is specially designed for `reset` function
    if (it != PyMemoryBuffer::mb_map.end()) {
      LLVMDisposeMemoryBuffer(mb);
      
      PyMemoryBuffer::mb_map.erase(mb);
    }
  }
}


std::shared_ptr<LLVMOpaqueMemoryBuffer> PyMemoryBuffer::get_shared_mb
(LLVMMemoryBufferRef mb) {
  std::lock_guard<std::mutex> lock(PyMemoryBuffer::map_mutex);
  auto it = PyMemoryBuffer::mb_map.find(mb);
  
  if (it != PyMemoryBuffer::mb_map.end()) {
    if (auto shared = it->second.lock()) {
      return shared;
    }
  }
  
  auto shared = std::shared_ptr<LLVMOpaqueMemoryBuffer>(mb, LLVMMemoryBufferRefDeleter());
  PyMemoryBuffer::mb_map[mb] = shared;
  return shared;
}
