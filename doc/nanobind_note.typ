= Nanobind Note
The concrete example/solution lives in `llvmpym`'s codebase.

== Incomplete type
For example
```c
typedef struct LLVMOpaqueType *LLVMTypeRef;
```
`LLVMOpaqueType` is an incomplete type since it has no complete definition.
For this situation, we shouldn't allow it to appear in the exposed function parameters or return type. Instead, we can make a wrapper class and always use this class to exchange information.

== Lambda Function, but in C version
For C++ version `std::function`, nanobind can steamlessly convert it by including `#include <nanobind/stl/function.h>` header. \
The following is in C version.
```c
typedef void (*LLVMFatalErrorHandler)(const char *Reason);

void LLVMInstallFatalErrorHandler(LLVMFatalErrorHandler Handler);
```
The solution is to wrapper `LLVMFatalErrorHandler` in a function like this:
```cpp
static std::function<void(const char *)> PyLLVMFatalErrorHandler;

void PyLLVMFatalErrorHandler_(const char *Reason) {
  if (PyLLVMFatalErrorHandler) {
    PyLLVMFatalErrorHandler(Reason);
  }
}

void pyInstallFatalErrorHandler(const std::function<void(const char *)> &handler) {
  PyLLVMFatalErrorHandler = handler;
  LLVMInstallFatalErrorHandler(PyLLVMFatalErrorHandler_);
}
```
