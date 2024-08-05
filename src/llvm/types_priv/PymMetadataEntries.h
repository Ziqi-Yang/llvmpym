#ifndef LLVMPYM_TYPES_PRIV_PYMMETADATAENTRIES_H
#define LLVMPYM_TYPES_PRIV_PYMMETADATAENTRIES_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PymLLVMObject.h"
#include "utils.h"

typedef LLVMValueMetadataEntry *LLVMValueMetadataEntries;

class PymMetadataEntries : public PymLLVMObject<PymMetadataEntries, LLVMValueMetadataEntries> {
public:
  explicit PymMetadataEntries(LLVMValueMetadataEntries entries, size_t len);
  LLVMValueMetadataEntries get() const;
  size_t getLen() const;
  
private:
  size_t len;
  SHARED_POINTER_DEF(LLVMValueMetadataEntries, LLVMValueMetadataEntry);
};


#endif
