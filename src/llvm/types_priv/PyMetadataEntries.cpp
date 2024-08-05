#include "PyMetadataEntries.h"

PyMetadataEntries::PyMetadataEntries(LLVMValueMetadataEntries obj, size_t len)
: obj(get_shared_obj(obj)), len(len) { }

LLVMValueMetadataEntries PyMetadataEntries::get() const {
  return obj.get();
}

size_t PyMetadataEntries::getLen() const {
  return len;
}

SHARED_POINTER_IMPL(PyMetadataEntries, LLVMValueMetadataEntries, LLVMValueMetadataEntry,
                    LLVMDisposeValueMetadataEntries)


