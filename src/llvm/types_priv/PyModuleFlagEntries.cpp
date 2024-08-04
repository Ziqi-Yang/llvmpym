#include "PyModuleFlagEntries.h"

std::unordered_map<LLVMModuleFlagEntries,
                          std::weak_ptr<LLVMModuleFlagEntry>> PyModuleFlagEntries::entries_map;
std::mutex PyModuleFlagEntries::map_mutex;

PyModuleFlagEntries::PyModuleFlagEntries(LLVMModuleFlagEntries entries, size_t len)
: entries(get_shared_entries(entries)), len(len) { }

LLVMModuleFlagEntries PyModuleFlagEntries::get() const {
  return entries.get();
}

size_t PyModuleFlagEntries::getLen() const {
  return len;
}


void PyModuleFlagEntries::LLVMModuleFlagEntriesDeleter::operator()
(LLVMModuleFlagEntries entries) const {
  if (entries) {
    LLVMDisposeModuleFlagsMetadata(entries);
    
    std::lock_guard<std::mutex> lock(PyModuleFlagEntries::map_mutex);
    PyModuleFlagEntries::entries_map.erase(entries);
  }
}


std::shared_ptr<LLVMModuleFlagEntry> PyModuleFlagEntries::get_shared_entries
(LLVMModuleFlagEntries entries) {
  std::lock_guard<std::mutex> lock(PyModuleFlagEntries::map_mutex);
  auto it = PyModuleFlagEntries::entries_map.find(entries);
  
  if (it != PyModuleFlagEntries::entries_map.end()) {
    if (auto shared = it->second.lock()) {
      return shared;
    }
  }
  
  auto shared = std::shared_ptr<LLVMModuleFlagEntry>(entries, LLVMModuleFlagEntriesDeleter());
  PyModuleFlagEntries::entries_map[entries] = shared;
  return shared;
}
