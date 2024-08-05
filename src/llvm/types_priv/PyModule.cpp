#include "PyModule.h"
#include <stdexcept>

PyModule::PyModule(const std::string &id) {
  obj = get_shared_obj(LLVMModuleCreateWithName(id.c_str()));
  if (!obj) {
    throw std::runtime_error("Failed to create Module");
  }
}

PyModule::PyModule(const std::string &id, LLVMContextRef context) {
  obj = get_shared_obj(LLVMModuleCreateWithNameInContext(id.c_str(), context));
  if (!obj) {
    throw std::runtime_error("Failed to create Module");
  }
}

PyModule::PyModule(LLVMModuleRef obj)
: obj(get_shared_obj(obj)) { }

LLVMModuleRef PyModule::get() const {
  return obj.get();
}

SHARED_POINTER_IMPL(PyModule, LLVMModuleRef, LLVMOpaqueModule, LLVMDisposeModule)
