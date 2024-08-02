#include "PyPassManagerBase.h"

std::unordered_map<LLVMPassManagerRef,
                   std::weak_ptr<LLVMOpaquePassManager>> PyPassManagerBase::pm_map;
std::mutex PyPassManagerBase::map_mutex;

PyPassManagerBase::PyPassManagerBase(LLVMPassManagerRef pm)
: pm(get_shared_pm(pm)) { }

LLVMPassManagerRef PyPassManagerBase::get() const {
  return pm.get();
}


void PyPassManagerBase::LLVMOpaquePassManagerDeleter::operator()
(LLVMPassManagerRef pm) const {
  if (pm) {
    LLVMDisposePassManager(pm);
    
    std::lock_guard<std::mutex> lock(PyPassManagerBase::map_mutex);
    PyPassManagerBase::pm_map.erase(pm);
  }
}


std::shared_ptr<LLVMOpaquePassManager> PyPassManagerBase::get_shared_pm
(LLVMPassManagerRef pm) {
  std::lock_guard<std::mutex> lock(PyPassManagerBase::map_mutex);
  auto it = PyPassManagerBase::pm_map.find(pm);
  
  if (it != PyPassManagerBase::pm_map.end()) {
    if (auto shared = it->second.lock()) {
      return shared;
    }
  }
  
  auto shared = std::shared_ptr<LLVMOpaquePassManager>(pm, LLVMOpaquePassManagerDeleter());
  PyPassManagerBase::pm_map[pm] = shared;
  return shared;
}

