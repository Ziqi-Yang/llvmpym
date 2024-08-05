#include "PymModule.h"
#include <stdexcept>

PymModule::PymModule(const std::string &id) {
  obj = get_shared_obj(LLVMModuleCreateWithName(id.c_str()));
  if (!obj) {
    throw std::runtime_error("Failed to create Module");
  }
}

PymModule::PymModule(const std::string &id, LLVMContextRef context) {
  obj = get_shared_obj(LLVMModuleCreateWithNameInContext(id.c_str(), context));
  if (!obj) {
    throw std::runtime_error("Failed to create Module");
  }
}

PymModule::PymModule(LLVMModuleRef obj)
: obj(get_shared_obj(obj)) { }

LLVMModuleRef PymModule::get() const {
  return obj.get();
}

SHARED_POINTER_IMPL(PymModule, LLVMModuleRef, LLVMOpaqueModule, LLVMDisposeModule)
