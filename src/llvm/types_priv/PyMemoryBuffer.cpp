#include "PyMemoryBuffer.h"
#include <iostream>

std::unordered_map<LLVMMemoryBufferRef,
                   std::weak_ptr<LLVMOpaqueMemoryBuffer>> PyMemoryBuffer::obj_map;
std::mutex PyMemoryBuffer::map_mutex;

PyMemoryBuffer::PyMemoryBuffer(LLVMMemoryBufferRef obj) : obj(get_shared_obj(obj)) { }

LLVMMemoryBufferRef PyMemoryBuffer::get() const {
  return obj.get();
}

// This function doesn't call `reset` method on shared_ptr object, since
// it will then call deleter, which is undesired and will lead to crash.
void PyMemoryBuffer::reset() {
  LLVMMemoryBufferRef m = obj.get();
  if (m) {
    std::lock_guard<std::mutex> lock(PyMemoryBuffer::map_mutex);
    PyMemoryBuffer::obj_map.erase(m);
 }
}


void PyMemoryBuffer::Deleter::operator()
(LLVMMemoryBufferRef mb) const {
  if (mb) {
    std::lock_guard<std::mutex> lock(PyMemoryBuffer::map_mutex);
    auto it = PyMemoryBuffer::obj_map.find(mb);
    
    // the logic here is specially designed for `reset` function
    if (it != PyMemoryBuffer::obj_map.end()) {
      LLVMDisposeMemoryBuffer(mb);
      
      PyMemoryBuffer::obj_map.erase(mb);
    }
  }
}


std::shared_ptr<LLVMOpaqueMemoryBuffer> PyMemoryBuffer::get_shared_obj
(LLVMMemoryBufferRef mb) {
  std::lock_guard<std::mutex> lock(PyMemoryBuffer::map_mutex);
  auto it = PyMemoryBuffer::obj_map.find(mb);
  
  if (it != PyMemoryBuffer::obj_map.end()) {
    if (auto shared = it->second.lock()) {
      return shared;
    }
  }
  
  auto shared = std::shared_ptr<LLVMOpaqueMemoryBuffer>(mb, Deleter());
  PyMemoryBuffer::obj_map[mb] = shared;
  return shared;
}
