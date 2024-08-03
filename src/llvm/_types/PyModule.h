#ifndef LLVMPYM__TYPES_PYMODULE_H
#define LLVMPYM__TYPES_PYMODULE_H

#include <llvm-c/Core.h>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <mutex>
#include <string>
#include "PyLLVMObject.h"

class PyModule : public PyLLVMObject<PyModule, LLVMModuleRef> {
public:
  explicit PyModule(const std::string &id);
  explicit PyModule(const std::string &id, LLVMContextRef context);
  explicit PyModule(LLVMModuleRef module);

  LLVMModuleRef get() const;

private:
  std::shared_ptr<LLVMOpaqueModule> module;

  struct LLVMModuleDeleter {
    void operator()(LLVMModuleRef module) const;
  };

  static std::shared_ptr<LLVMOpaqueModule> get_shared_module(LLVMModuleRef module);

  static std::unordered_map<LLVMModuleRef, std::weak_ptr<LLVMOpaqueModule>> module_map;
  static std::mutex map_mutex;
};

#endif
