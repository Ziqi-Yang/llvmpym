#include "PymModuleFlagEntries.h"

PymModuleFlagEntries::PymModuleFlagEntries(LLVMModuleFlagEntries obj, size_t len)
: obj(get_shared_obj(obj)), len(len) { }

LLVMModuleFlagEntries PymModuleFlagEntries::get() const {
  return obj.get();
}

size_t PymModuleFlagEntries::getLen() const {
  return len;
}

SHARED_POINTER_IMPL(PymModuleFlagEntries, LLVMModuleFlagEntries, LLVMModuleFlagEntry,
                    LLVMDisposeModuleFlagsMetadata)
