#ifndef LLVMPYM_TYPES_PRIV_PYMETADATAENTRIES_H
#define LLVMPYM_TYPES_PRIV_PYMETADATAENTRIES_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PyLLVMObject.h"
#include "utils.h"

typedef LLVMValueMetadataEntry *LLVMValueMetadataEntries;

class PyMetadataEntries : public PyLLVMObject<PyMetadataEntries, LLVMValueMetadataEntries> {
public:
  explicit PyMetadataEntries(LLVMValueMetadataEntries entries, size_t len);
  LLVMValueMetadataEntries get() const;
  size_t getLen() const;
  
private:
  size_t len;
  SHARED_POINTER_DEF(LLVMValueMetadataEntries, LLVMValueMetadataEntry);
};


#endif
