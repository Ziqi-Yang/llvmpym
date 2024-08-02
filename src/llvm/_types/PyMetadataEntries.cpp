#include "PyMetadataEntries.h"

std::unordered_map<LLVMValueMetadataEntries,
                          std::weak_ptr<LLVMValueMetadataEntry>> PyMetadataEntries::entries_map;
std::mutex PyMetadataEntries::map_mutex;

PyMetadataEntries::PyMetadataEntries(LLVMValueMetadataEntries entries, size_t len)
: entries(get_shared_entries(entries)), len(len) { }

LLVMValueMetadataEntries PyMetadataEntries::get() const {
  return entries.get();
}

size_t PyMetadataEntries::getLen() const {
  return len;
}


void PyMetadataEntries::LLVMValueMetadataEntriesDeleter::operator()
(LLVMValueMetadataEntries entries) const {
  if (entries) {
    LLVMDisposeValueMetadataEntries(entries);
    
    std::lock_guard<std::mutex> lock(PyMetadataEntries::map_mutex);
    PyMetadataEntries::entries_map.erase(entries);
  }
}


std::shared_ptr<LLVMValueMetadataEntry> PyMetadataEntries::get_shared_entries
(LLVMValueMetadataEntries entries) {
  std::lock_guard<std::mutex> lock(PyMetadataEntries::map_mutex);
  auto it = PyMetadataEntries::entries_map.find(entries);
  
  if (it != PyMetadataEntries::entries_map.end()) {
    if (auto shared = it->second.lock()) {
      return shared;
    }
  }
  
  auto shared = std::shared_ptr<LLVMValueMetadataEntry>(entries, LLVMValueMetadataEntriesDeleter());
  PyMetadataEntries::entries_map[entries] = shared;
  return shared;
}



