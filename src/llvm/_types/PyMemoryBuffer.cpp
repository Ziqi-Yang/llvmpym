#include "PyMemoryBuffer.h"

std::unordered_map<LLVMMemoryBufferRef,
                   std::weak_ptr<LLVMOpaqueMemoryBuffer>> PyMemoryBuffer::mb_map;
std::mutex PyMemoryBuffer::map_mutex;

PyMemoryBuffer::PyMemoryBuffer(LLVMMemoryBufferRef mb) : mb(get_shared_mb(mb)) { }

LLVMMemoryBufferRef PyMemoryBuffer::get() const {
  return mb.get();
}


void PyMemoryBuffer::LLVMMemoryBufferRefDeleter::operator()
(LLVMMemoryBufferRef entries) const {
  if (entries) {
    LLVMDisposeMemoryBuffer(entries);
    
    std::lock_guard<std::mutex> lock(PyMemoryBuffer::map_mutex);
    PyMemoryBuffer::mb_map.erase(entries);
  }
}


std::shared_ptr<LLVMOpaqueMemoryBuffer> PyMemoryBuffer::get_shared_mb
(LLVMMemoryBufferRef entries) {
  std::lock_guard<std::mutex> lock(PyMemoryBuffer::map_mutex);
  auto it = PyMemoryBuffer::mb_map.find(entries);
  
  if (it != PyMemoryBuffer::mb_map.end()) {
    if (auto shared = it->second.lock()) {
      return shared;
    }
  }
  
  auto shared = std::shared_ptr<LLVMOpaqueMemoryBuffer>(entries, LLVMMemoryBufferRefDeleter());
  PyMemoryBuffer::mb_map[entries] = shared;
  return shared;
}
