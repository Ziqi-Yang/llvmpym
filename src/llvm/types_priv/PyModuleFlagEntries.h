#ifndef LLVMPYM_TYPES_PRIV_PYMODULEFLAGENTRIES_H
#define LLVMPYM_TYPES_PRIV_PYMODULEFLAGENTRIES_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PyLLVMObject.h"

typedef LLVMModuleFlagEntry *LLVMModuleFlagEntries;

class PyModuleFlagEntries : public PyLLVMObject<PyModuleFlagEntries, LLVMModuleFlagEntries>{
public:
  explicit PyModuleFlagEntries(LLVMModuleFlagEntries entries, size_t len);
  LLVMModuleFlagEntries get() const;
  size_t getLen() const;

private:
  std::shared_ptr<LLVMModuleFlagEntry> entries;
  size_t len;

  struct LLVMModuleFlagEntriesDeleter {
    void operator()(LLVMModuleFlagEntries entries) const;
  };

  static std::shared_ptr<LLVMModuleFlagEntry> get_shared_entries(LLVMModuleFlagEntries entries);

  static std::unordered_map<LLVMModuleFlagEntries,
                            std::weak_ptr<LLVMModuleFlagEntry>> entries_map;
  static std::mutex map_mutex;
};



#endif