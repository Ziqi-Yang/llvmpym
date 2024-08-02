#include "PyOperandBundle.h"

std::unordered_map<LLVMOperandBundleRef,
                   std::weak_ptr<LLVMOpaqueOperandBundle>> PyOperandBundle::bundle_map;
std::mutex PyOperandBundle::map_mutex;

PyOperandBundle::PyOperandBundle(LLVMOperandBundleRef bundle)
: bundle(get_shared_bundle(bundle)) { }

LLVMOperandBundleRef PyOperandBundle::get() const {
  return bundle.get();
}


void PyOperandBundle::LLVMOperandBundleRefDeleter::operator()
(LLVMOperandBundleRef bundle) const {
  if (bundle) {
    LLVMDisposeOperandBundle(bundle);
    
    std::lock_guard<std::mutex> lock(PyOperandBundle::map_mutex);
    PyOperandBundle::bundle_map.erase(bundle);
  }
}


std::shared_ptr<LLVMOpaqueOperandBundle> PyOperandBundle::get_shared_bundle
(LLVMOperandBundleRef bundle) {
  std::lock_guard<std::mutex> lock(PyOperandBundle::map_mutex);
  auto it = PyOperandBundle::bundle_map.find(bundle);
  
  if (it != PyOperandBundle::bundle_map.end()) {
    if (auto shared = it->second.lock()) {
      return shared;
    }
  }
  
  auto shared = std::shared_ptr<LLVMOpaqueOperandBundle>(bundle, LLVMOperandBundleRefDeleter());
  PyOperandBundle::bundle_map[bundle] = shared;
  return shared;
}



