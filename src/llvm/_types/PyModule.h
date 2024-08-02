#ifndef LLVMPYM_TYPE_PYMODULE_H
#define LLVMPYM_TYPE_PYMODULE_H

#include <llvm-c/Core.h>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <mutex>
#include <string>

class PyModule {
public:
  explicit PyModule(const std::string &id);
  explicit PyModule(const std::string &id, LLVMContextRef context);
  explicit PyModule(LLVMModuleRef module);

  LLVMModuleRef get() const;

  // Move semantics using shared_ptr
  PyModule(PyModule&& other) noexcept = default;
  PyModule& operator=(PyModule&& other) noexcept = default;

private:
  std::shared_ptr<LLVMOpaqueModule> module;

  struct LLVMModuleDeleter {
    void operator()(LLVMOpaqueModule* module) const;
  };

  static std::shared_ptr<LLVMOpaqueModule> get_shared_module(LLVMModuleRef module);

  // Static members for shared ownership management
  static std::unordered_map<LLVMOpaqueModule*, std::weak_ptr<LLVMOpaqueModule>> module_map;
  static std::mutex map_mutex;
};

#endif
