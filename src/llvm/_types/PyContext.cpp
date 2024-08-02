#include "PyContext.h"

std::unordered_map<LLVMOpaqueContext*, std::weak_ptr<LLVMOpaqueContext>> PyContext::context_map;
std::mutex PyContext::map_mutex;

PyContext::PyContext() {
  context = get_shared_context(LLVMContextCreate(), this);
  if (!context) {
    throw std::runtime_error("Failed to create Context");
  }

  is_global_context = false;
}

PyContext::PyContext(LLVMContextRef ctx, bool is_global_context) {
  context = get_shared_context(ctx, this);
  if (!context) {
    throw std::runtime_error("Failed to create Context");
  }

  is_global_context = is_global_context;
}

PyContext::PyContext(LLVMContextRef ctx) {
  context = get_shared_context(ctx, this);
  if (!context) {
    throw std::runtime_error("Failed to create Context");
  }
  
  LLVMContextRef global_context = LLVMGetGlobalContext();
  is_global_context = global_context == context.get();
}

PyContext PyContext::getGlobalContext() {
  return PyContext(LLVMGetGlobalContext(), true);
}


LLVMContextRef PyContext::get() const {
  return context.get();
}


void PyContext::LLVMContextDeleter::operator()(LLVMContextRef context) const {
  if (context && parent && !parent->is_global_context) {
    LLVMContextDispose(context);

    std::lock_guard<std::mutex> lock(PyContext::map_mutex);
    PyContext::context_map.erase(context);
  }
}


std::shared_ptr<LLVMOpaqueContext> PyContext::get_shared_context
(LLVMContextRef context, const PyContext *parent) {
  std::lock_guard<std::mutex> lock(PyContext::map_mutex);
  auto it = PyContext::context_map.find(context);
  
  if (it != PyContext::context_map.end()) {
    if (auto shared = it->second.lock()) {
      return shared;
    }
  }
  
  // global context will also be stored, but only one exists and lives forever
  auto shared = std::shared_ptr<LLVMOpaqueContext>(context, LLVMContextDeleter(parent));
  PyContext::context_map[context] = shared;
  return shared;
}
