#ifndef LLVMPYM_TYPES_PRIV_PYMODULEFLAGENTRIES_H
#define LLVMPYM_TYPES_PRIV_PYMODULEFLAGENTRIES_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PyLLVMObject.h"
#include "utils.h"

typedef LLVMModuleFlagEntry *LLVMModuleFlagEntries;

class PyModuleFlagEntries : public PyLLVMObject<PyModuleFlagEntries, LLVMModuleFlagEntries>{
public:
  explicit PyModuleFlagEntries(LLVMModuleFlagEntries entries, size_t len);
  LLVMModuleFlagEntries get() const;
  size_t getLen() const;

private:
  size_t len;
  
  SHARED_POINTER_DEF(LLVMModuleFlagEntries, LLVMModuleFlagEntry);
};



#endif
