#ifndef LLVMPYM_CORE_H
#define LLVMPYM_CORE_H

#include <nanobind/nanobind.h>
namespace nb = nanobind;

#define CONSTANT_EXPR_BIND_BINARY_OP(NAME, FUNCTION) \
  .def_static(#NAME, \
      [](PyConstant &lhs, PyConstant &rhs) { \
        return PyValueAuto(FUNCTION(lhs.get(), rhs.get())); \
      }, \
      "lhs"_a, "rhs"_a)
  

#define CONSTANT_EXPR_BIND_BINARY_OPS \
  CONSTANT_EXPR_BIND_BINARY_OP(add, LLVMConstAdd) \
  CONSTANT_EXPR_BIND_BINARY_OP(add_nsw, LLVMConstNSWAdd) \
  CONSTANT_EXPR_BIND_BINARY_OP(add_nuw, LLVMConstNUWAdd) \
  CONSTANT_EXPR_BIND_BINARY_OP(sub, LLVMConstSub) \
  CONSTANT_EXPR_BIND_BINARY_OP(sub_nsw, LLVMConstNSWSub) \
  CONSTANT_EXPR_BIND_BINARY_OP(sub_nuw, LLVMConstNUWSub) \
  CONSTANT_EXPR_BIND_BINARY_OP(mul, LLVMConstMul) \
  CONSTANT_EXPR_BIND_BINARY_OP(mul_nsw, LLVMConstNSWMul) \
  CONSTANT_EXPR_BIND_BINARY_OP(mul_nuw, LLVMConstNUWMul) \
  CONSTANT_EXPR_BIND_BINARY_OP(xor, LLVMConstXor) \
  CONSTANT_EXPR_BIND_BINARY_OP(shl, LLVMConstShl)




#define BUILDER_BIND_BINARY_OP(NAME, FUNCTION) \
  .def(#NAME, \
      [](PyBuilder &self, PyValue &lhs, PyValue &rhs, const char *name) { \
        return PyCallInst(FUNCTION(self.get(), lhs.get(), rhs.get(), name)); \
      }) 



#define BUILDER_BIND_BINARY_OPS \
  BUILDER_BIND_BINARY_OP(add, LLVMBuildAdd) \
  BUILDER_BIND_BINARY_OP(add_nsw, LLVMBuildNSWAdd) \
  BUILDER_BIND_BINARY_OP(add_nuw, LLVMBuildNUWAdd)

void populateCore(nb::module_ &m);

#endif
