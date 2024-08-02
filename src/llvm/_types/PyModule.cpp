#include "PyModule.h"

std::unordered_map<LLVMOpaqueModule*, std::weak_ptr<LLVMOpaqueModule>> PyModule::module_map;
std::mutex PyModule::map_mutex;


PyModule::PyModule(const std::string &id) {
  module = get_shared_module(LLVMModuleCreateWithName(id.c_str()));
  if (!module) {
    throw std::runtime_error("Failed to create Module");
  }
}

PyModule::PyModule(const std::string &id, LLVMContextRef context) {
  module = get_shared_module(LLVMModuleCreateWithNameInContext(id.c_str(), context));
  if (!module) {
    throw std::runtime_error("Failed to create Module");
  }
}

PyModule::PyModule(LLVMModuleRef module)
: module(get_shared_module(module)) {}

LLVMModuleRef PyModule::get() const {
  return module.get();
}


void PyModule::LLVMModuleDeleter::operator()(LLVMOpaqueModule* module) const {
  if (module) {
    LLVMDisposeModule(module);
    
    std::lock_guard<std::mutex> lock(PyModule::map_mutex);
    PyModule::module_map.erase(module);
  }
}


std::shared_ptr<LLVMOpaqueModule> PyModule::get_shared_module(LLVMModuleRef module) {
  std::lock_guard<std::mutex> lock(PyModule::map_mutex);
  auto it = PyModule::module_map.find(module);
  
  if (it != PyModule::module_map.end()) {
    if (auto shared = it->second.lock()) {
      return shared;
    }
  }
  
  auto shared = std::shared_ptr<LLVMOpaqueModule>(module, LLVMModuleDeleter());
  PyModule::module_map[module] = shared;
  return shared;
}
