#include "PymMemoryBuffer.h"
#include <iostream>

std::unordered_map<LLVMMemoryBufferRef,
                   std::weak_ptr<LLVMOpaqueMemoryBuffer>> PymMemoryBuffer::obj_map;
std::mutex PymMemoryBuffer::map_mutex;

PymMemoryBuffer::PymMemoryBuffer(LLVMMemoryBufferRef obj) : obj(get_shared_obj(obj)) { }

LLVMMemoryBufferRef PymMemoryBuffer::get() const {
  return obj.get();
}

// This function doesn't call `reset` method on shared_ptr object, since
// it will then call deleter, which is undesired and will lead to crash.
void PymMemoryBuffer::reset() {
  LLVMMemoryBufferRef m = obj.get();
  if (m) {
    std::lock_guard<std::mutex> lock(PymMemoryBuffer::map_mutex);
    PymMemoryBuffer::obj_map.erase(m);
 }
}


void PymMemoryBuffer::Deleter::operator()
(LLVMMemoryBufferRef mb) const {
  if (mb) {
    std::lock_guard<std::mutex> lock(PymMemoryBuffer::map_mutex);
    auto it = PymMemoryBuffer::obj_map.find(mb);
    
    // the logic here is specially designed for `reset` function
    if (it != PymMemoryBuffer::obj_map.end()) {
      LLVMDisposeMemoryBuffer(mb);
      
      PymMemoryBuffer::obj_map.erase(mb);
    }
  }
}


std::shared_ptr<LLVMOpaqueMemoryBuffer> PymMemoryBuffer::get_shared_obj
(LLVMMemoryBufferRef mb) {
  std::lock_guard<std::mutex> lock(PymMemoryBuffer::map_mutex);
  auto it = PymMemoryBuffer::obj_map.find(mb);
  
  if (it != PymMemoryBuffer::obj_map.end()) {
    if (auto shared = it->second.lock()) {
      return shared;
    }
  }
  
  auto shared = std::shared_ptr<LLVMOpaqueMemoryBuffer>(mb, Deleter());
  PymMemoryBuffer::obj_map[mb] = shared;
  return shared;
}
