#ifndef LLVMPYM_TYPES_PRIV_PYMMODULEFLAGENTRIES_H
#define LLVMPYM_TYPES_PRIV_PYMMODULEFLAGENTRIES_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PymLLVMObject.h"
#include "utils.h"

typedef LLVMModuleFlagEntry *LLVMModuleFlagEntries;

class PymModuleFlagEntries : public PymLLVMObject<PymModuleFlagEntries, LLVMModuleFlagEntries>{
public:
  explicit PymModuleFlagEntries(LLVMModuleFlagEntries entries, size_t len);
  LLVMModuleFlagEntries get() const;
  size_t getLen() const;

private:
  size_t len;
  
  SHARED_POINTER_DEF(LLVMModuleFlagEntries, LLVMModuleFlagEntry);
};



#endif
