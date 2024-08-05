#ifndef LLVMPYM_CORE_VALUE_H
#define LLVMPYM_CORE_VALUE_H

#include <nanobind/nanobind.h>
#include <llvm-c/Core.h>

#define PY_DECLARE_VALUE_CAST(name) \
  .def("to_" #name, \
       [](PymValue &v) -> std::optional<PymValue *> { \
         auto res = LLVMIsA##name(v.get()); \
         if (res) \
           return PymValueAuto(res); \
         return std::nullopt; \
       }, \
       "Origin function: LLVMIsA" #name "\n\n" \
       "None means conversion failed.\n\n" \
       "Note if the target class is not supported in python binding, then it will " \
       "return a generic PymValue type object") 


#define CONSTANT_EXPR_BIND_BINARY_OP(NAME, FUNCTION) \
  .def_static(#NAME, \
      [](PymConstant &lhs, PymConstant &rhs) { \
        return PymValueAuto(FUNCTION(lhs.get(), rhs.get())); \
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





void bindValueClasses(nanobind::module_ &m);


#endif
