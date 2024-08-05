#include "PymMetadataEntries.h"

PymMetadataEntries::PymMetadataEntries(LLVMValueMetadataEntries obj, size_t len)
: obj(get_shared_obj(obj)), len(len) { }

LLVMValueMetadataEntries PymMetadataEntries::get() const {
  return obj.get();
}

size_t PymMetadataEntries::getLen() const {
  return len;
}

SHARED_POINTER_IMPL(PymMetadataEntries, LLVMValueMetadataEntries, LLVMValueMetadataEntry,
                    LLVMDisposeValueMetadataEntries)


