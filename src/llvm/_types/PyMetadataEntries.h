#ifndef LLVMPYM_TYPE_PYMETADATAENTRIES_H
#define LLVMPYM_TYPE_PYMETADATAENTRIES_H

#include <llvm-c/Core.h>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <mutex>

typedef LLVMValueMetadataEntry *LLVMValueMetadataEntries;

class PyMetadataEntries {
public:
  explicit PyMetadataEntries(LLVMValueMetadataEntries entries, size_t len);
  LLVMValueMetadataEntries get();
  size_t getLen();
  
private:
  std::shared_ptr<LLVMValueMetadataEntry> entries;
  size_t len;

  struct LLVMValueMetadataEntriesDeleter {
    void operator()(LLVMValueMetadataEntries entries) const;
  };

  static std::shared_ptr<LLVMValueMetadataEntry> get_shared_entries(LLVMValueMetadataEntries entries);

  // Static members for shared ownership management
  static std::unordered_map<LLVMValueMetadataEntries,
                            std::weak_ptr<LLVMValueMetadataEntry>> entries_map;
  static std::mutex map_mutex;
};


#endif
