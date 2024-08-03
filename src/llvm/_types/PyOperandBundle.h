#ifndef PYOPERANDBUNDLE_H
#define PYOPERANDBUNDLE_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PyLLVMObject.h"

class PyOperandBundle : public PyLLVMObject<PyOperandBundle, LLVMOperandBundleRef> {
public:
  explicit PyOperandBundle(LLVMOperandBundleRef bundle);
  LLVMOperandBundleRef get() const;
  
private:
  std::shared_ptr<LLVMOpaqueOperandBundle> bundle;

  struct LLVMOperandBundleRefDeleter {
    void operator()(LLVMOperandBundleRef entries) const;
  };

  static std::shared_ptr<LLVMOpaqueOperandBundle> get_shared_bundle(LLVMOperandBundleRef entries);

  static std::unordered_map<LLVMOperandBundleRef,
                            std::weak_ptr<LLVMOpaqueOperandBundle>> bundle_map;
  static std::mutex map_mutex;
};


#endif
