#ifndef LLVMPYM__TYPES_PYMODULEFLAGENTRIES_H
#define LLVMPYM__TYPES_PYMODULEFLAGENTRIES_H

#include <llvm-c/Core.h>
#include <stdexcept>
#include <memory>
#include <unordered_map>
#include <mutex>

typedef LLVMModuleFlagEntry *LLVMModuleFlagEntries;

class PyModuleFlagEntries {
public:
  explicit PyModuleFlagEntries(LLVMModuleFlagEntries entries, size_t len);
  LLVMModuleFlagEntries get();
  size_t getLen();

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
