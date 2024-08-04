#include "PyModuleProvider.h"

std::unordered_map<LLVMModuleProviderRef,
                   std::weak_ptr<LLVMOpaqueModuleProvider>> PyModuleProvider::mp_map;
std::mutex PyModuleProvider::map_mutex;

PyModuleProvider::PyModuleProvider(LLVMModuleProviderRef mp) : mp(get_shared_mp(mp)) {}

LLVMModuleProviderRef PyModuleProvider::get() const {
  return mp.get();
}


void PyModuleProvider::LLVMModuleProviderRefDeleter::operator()
(LLVMModuleProviderRef mp) const {
  if (mp) {
    LLVMDisposeModuleProvider(mp);
    
    std::lock_guard<std::mutex> lock(PyModuleProvider::map_mutex);
    PyModuleProvider::mp_map.erase(mp);
  }
}


std::shared_ptr<LLVMOpaqueModuleProvider> PyModuleProvider::get_shared_mp
(LLVMModuleProviderRef mp) {
  std::lock_guard<std::mutex> lock(PyModuleProvider::map_mutex);
  auto it = PyModuleProvider::mp_map.find(mp);
  
  if (it != PyModuleProvider::mp_map.end()) {
    if (auto shared = it->second.lock()) {
      return shared;
    }
  }
  
  auto shared = std::shared_ptr<LLVMOpaqueModuleProvider>(mp, LLVMModuleProviderRefDeleter());
  PyModuleProvider::mp_map[mp] = shared;
  return shared;
}

