#ifndef LLVMPYM_CORE_H
#define LLVMPYM_CORE_H

#include <nanobind/nanobind.h>
namespace nb = nanobind;

#define DEFINE_BINARY_OP(NAME, TYPE, FUNCTION) \
  .def_static(#NAME, \
      [](TYPE &lhs, TYPE &rhs) { \
        return PyValueAuto(FUNCTION(lhs.get(), rhs.get())); \
      }, \
      "lhs"_a, "rhs"_a)

#define BIND_CONSTANT_EXPR_BINARY_OP(NAME, FUNCTION) \
  DEFINE_BINARY_OP(NAME, PyConstant, FUNCTION)
  

#define BIND_CONSTANT_EXPR_BINARY_OPS \
  BIND_CONSTANT_EXPR_BINARY_OP(add, LLVMConstAdd) \
  BIND_CONSTANT_EXPR_BINARY_OP(add_nsw, LLVMConstNSWAdd) \
  BIND_CONSTANT_EXPR_BINARY_OP(add_nuw, LLVMConstNUWAdd) \
  BIND_CONSTANT_EXPR_BINARY_OP(sub, LLVMConstSub) \
  BIND_CONSTANT_EXPR_BINARY_OP(sub_nsw, LLVMConstNSWSub) \
  BIND_CONSTANT_EXPR_BINARY_OP(sub_nuw, LLVMConstNUWSub) \
  BIND_CONSTANT_EXPR_BINARY_OP(mul, LLVMConstMul) \
  BIND_CONSTANT_EXPR_BINARY_OP(mul_nsw, LLVMConstNSWMul) \
  BIND_CONSTANT_EXPR_BINARY_OP(mul_nuw, LLVMConstNUWMul) \
  BIND_CONSTANT_EXPR_BINARY_OP(xor, LLVMConstXor) \
  BIND_CONSTANT_EXPR_BINARY_OP(shl, LLVMConstShl)


void populateCore(nb::module_ &m);

#endif
