#include "PyModuleFlagEntries.h"

PyModuleFlagEntries::PyModuleFlagEntries(LLVMModuleFlagEntries obj, size_t len)
: obj(get_shared_obj(obj)), len(len) { }

LLVMModuleFlagEntries PyModuleFlagEntries::get() const {
  return obj.get();
}

size_t PyModuleFlagEntries::getLen() const {
  return len;
}

SHARED_POINTER_IMPL(PyModuleFlagEntries, LLVMModuleFlagEntries, LLVMModuleFlagEntry,
                    LLVMDisposeModuleFlagsMetadata)
