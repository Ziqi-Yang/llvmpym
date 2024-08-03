#ifndef LLVMPYM__TYPES_PYPASSMANAGERBASE_H
#define LLVMPYM__TYPES_PYPASSMANAGERBASE_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PyLLVMObject.h"

class PyPassManagerBase : public PyLLVMObject<PyPassManagerBase, LLVMPassManagerRef> {
public:
  explicit PyPassManagerBase(LLVMPassManagerRef pm);
  LLVMPassManagerRef get() const;
  
private:
  std::shared_ptr<LLVMOpaquePassManager> pm;

  struct LLVMOpaquePassManagerDeleter {
    void operator()(LLVMPassManagerRef pm) const;
  };

  static std::shared_ptr<LLVMOpaquePassManager> get_shared_pm(LLVMPassManagerRef pm);

  static std::unordered_map<LLVMPassManagerRef,
                            std::weak_ptr<LLVMOpaquePassManager>> pm_map;
  static std::mutex map_mutex;
};


#endif
