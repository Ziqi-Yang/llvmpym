#ifndef LLVMPYM_TYPES_PRIV_PYMODULE_H
#define LLVMPYM_TYPES_PRIV_PYMODULE_H

#include <llvm-c/Core.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <string>
#include "PyLLVMObject.h"
#include "utils.h"

class PyModule : public PyLLVMObject<PyModule, LLVMModuleRef> {
public:
  explicit PyModule(const std::string &id);
  explicit PyModule(const std::string &id, LLVMContextRef context);
  explicit PyModule(LLVMModuleRef module);

  LLVMModuleRef get() const;

private:
  SHARED_POINTER_DEF(LLVMModuleRef, LLVMOpaqueModule);
};

#endif
