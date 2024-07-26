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
        return PyValueAuto(FUNCTION(self.get(), lhs.get(), rhs.get(), name)); \
      }, \
      "lhs"_a, "rhs"_a, "name"_a) 



#define BUILDER_BIND_BINARY_OPS \
  BUILDER_BIND_BINARY_OP(add, LLVMBuildAdd) \
  BUILDER_BIND_BINARY_OP(add_nsw, LLVMBuildNSWAdd) \
  BUILDER_BIND_BINARY_OP(add_nuw, LLVMBuildNUWAdd) \
  BUILDER_BIND_BINARY_OP(fadd, LLVMBuildFAdd) \
  BUILDER_BIND_BINARY_OP(sub, LLVMBuildSub) \
  BUILDER_BIND_BINARY_OP(sub_nsw, LLVMBuildNSWSub) \
  BUILDER_BIND_BINARY_OP(sub_nuw, LLVMBuildNUWSub) \
  BUILDER_BIND_BINARY_OP(fsub, LLVMBuildFSub) \
  BUILDER_BIND_BINARY_OP(mul, LLVMBuildMul) \
  BUILDER_BIND_BINARY_OP(mul_nsw, LLVMBuildNSWMul) \
  BUILDER_BIND_BINARY_OP(mul_nuw, LLVMBuildNUWMul) \
  BUILDER_BIND_BINARY_OP(fmul, LLVMBuildFMul) \
  BUILDER_BIND_BINARY_OP(udiv, LLVMBuildUDiv) \
  BUILDER_BIND_BINARY_OP(exact_udiv, LLVMBuildExactUDiv) \
  BUILDER_BIND_BINARY_OP(sdiv, LLVMBuildSDiv) \
  BUILDER_BIND_BINARY_OP(exact_sdiv, LLVMBuildExactSDiv) \
  BUILDER_BIND_BINARY_OP(fdiv, LLVMBuildFDiv) \
  BUILDER_BIND_BINARY_OP(urem, LLVMBuildURem) \
  BUILDER_BIND_BINARY_OP(srem, LLVMBuildSRem) \
  BUILDER_BIND_BINARY_OP(frem, LLVMBuildFRem) \
  BUILDER_BIND_BINARY_OP(shl, LLVMBuildShl) \
  BUILDER_BIND_BINARY_OP(lshr, LLVMBuildLShr) \
  BUILDER_BIND_BINARY_OP(ashr, LLVMBuildAShr) \
  BUILDER_BIND_BINARY_OP(and, LLVMBuildAnd) \
  BUILDER_BIND_BINARY_OP(or, LLVMBuildOr) \
  BUILDER_BIND_BINARY_OP(xor, LLVMBuildXor)

void populateCore(nb::module_ &m);

#endif
