#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <llvm-c/ErrorHandling.h>

namespace nb = nanobind;
using namespace nb::literals;

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

void populateErrorHandling(nb::module_ &m) {
  m.def("install_fatal_error_handler", &pyInstallFatalErrorHandler, "handler"_a,
        "Install a fatal error handler. By default, if LLVM detects a fatal error, it"
        "will call exit(1). This may not be appropriate in many contexts. For example,"
        "doing exit(1) will bypass many crash reporting/tracing system tools. This"
        "function allows you to install a callback that will be invoked prior to the"
        "call to exit(1).");

  m.def("reset_fatal_error_handler", &LLVMResetFatalErrorHandler,
        "Reset the fatal error handler. This resets LLVM's fatal error handling"
        "behavior to the default.");

  m.def("enable_pretty_stack_trace", &LLVMEnablePrettyStackTrace,
        "Enable LLVM's built-in stack trace code. This intercepts the OS's crash"
        "signals and prints which component of LLVM you were in at the time if the"
        "crash.");
}
