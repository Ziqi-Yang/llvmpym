#include "PyContext.h"

std::unordered_map<LLVMContextRef, std::weak_ptr<LLVMOpaqueContext>> PyContext::context_map;
std::mutex PyContext::map_mutex;

PyContext::PyContext() {
  is_global_context = false;
  context = get_shared_context(LLVMContextCreate(), is_global_context);
  if (!context) {
    throw std::runtime_error("Failed to create Context");
  }
}

PyContext::PyContext(LLVMContextRef ctx, bool is_global_context) {
  is_global_context = is_global_context;
  context = get_shared_context(ctx, is_global_context);
  if (!context) {
    throw std::runtime_error("Failed to create Context");
  }
}

PyContext::PyContext(LLVMContextRef ctx) {
  std::cout << 3 << std::endl;
  LLVMContextRef global_context = LLVMGetGlobalContext();
  is_global_context = global_context == context.get();

  context = get_shared_context(ctx, is_global_context);
  if (!context) {
    throw std::runtime_error("Failed to create Context");
  }
}

PyContext PyContext::getGlobalContext() {
  return PyContext(LLVMGetGlobalContext(), true);
}


LLVMContextRef PyContext::get() const {
  return context.get();
}


// NOTE don't store a pointer of parent in the Deleter, since when PyContext is
// a derived class, it will cause object slicing problem,
// also the stored parent object may be copied, since all the state in the `parent` object
//  is the state when we created LLVMContextDeleter object
// These problems can possibly be handled by make parent field shared_ptr type,
// but this makes things far more complex and ugly
void PyContext::LLVMContextDeleter::operator()(const LLVMContextRef context) const {
  if (context) {
    std::lock_guard<std::mutex> lock(PyContext::map_mutex);
    auto it = PyContext::context_map.find(context);
    
    if (it != PyContext::context_map.end()) {
      LLVMContextDispose(context);
      
      PyContext::context_map.erase(context);
    }
  }
}


/*
 * NOTE global context won't stored in context_map
 */
std::shared_ptr<LLVMOpaqueContext> PyContext::get_shared_context
(LLVMContextRef context, bool is_global_context) {
  if (is_global_context)
    return std::shared_ptr<LLVMOpaqueContext>(context, LLVMContextDeleter());
  
  std::lock_guard<std::mutex> lock(PyContext::map_mutex);
  auto it = PyContext::context_map.find(context);
  
  if (it != PyContext::context_map.end()) {
    if (auto shared = it->second.lock()) {
      return shared;
    }
  }
  
  auto shared = std::shared_ptr<LLVMOpaqueContext>(context, LLVMContextDeleter());
  PyContext::context_map[context] = shared;
  return shared;
}
