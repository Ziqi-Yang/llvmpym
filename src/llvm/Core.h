#ifndef LLVMPYM_CORE_H
#define LLVMPYM_CORE_H

#include <nanobind/nanobind.h>
#include "_types.h"

namespace nb = nanobind;


#define PY_DECLARE_VALUE_CAST(name) \
  .def("to_" #name, \
       [](PyValue &v) -> std::optional<PyValue *> { \
         auto res = LLVMIsA##name(v.get()); \
         if (res) \
           return PyValueAuto(res); \
         return std::nullopt; \
       }, \
       "Origin function: LLVMIsA" #name "\n\n" \
       "None means conversion failed.\n\n" \
       "Note if the target class is not supported in python binding, then it will " \
       "return a generic PyValue type object") 


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
      "lhs"_a, "rhs"_a, "name"_a = "") 



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
  BUILDER_BIND_BINARY_OP(and_, LLVMBuildAnd) \
  BUILDER_BIND_BINARY_OP(or_, LLVMBuildOr) \
  BUILDER_BIND_BINARY_OP(xor, LLVMBuildXor)

#define BUILDER_BIND_CAST_OP(NAME, FUNCTION) \
  .def(#NAME, \
      [](PyBuilder &self, PyValue &val, PyType &destType, const char *name) { \
        return PyValueAuto(FUNCTION(self.get(), val.get(), destType.get(), name)); \
      }, \
      "val"_a, "dest_type"_a, "name"_a = "") 

#define BUILDER_BIND_CAST_OPS \
  BUILDER_BIND_CAST_OP(trunc, LLVMBuildTrunc) \
  BUILDER_BIND_CAST_OP(zext, LLVMBuildZExt) \
  BUILDER_BIND_CAST_OP(xext, LLVMBuildSExt) \
  BUILDER_BIND_CAST_OP(fp2ui, LLVMBuildFPToUI) \
  BUILDER_BIND_CAST_OP(fp2si, LLVMBuildFPToSI) \
  BUILDER_BIND_CAST_OP(ui2fp, LLVMBuildUIToFP) \
  BUILDER_BIND_CAST_OP(si2fp, LLVMBuildSIToFP) \
  BUILDER_BIND_CAST_OP(fp_trunc, LLVMBuildFPTrunc) \
  BUILDER_BIND_CAST_OP(fp_ext, LLVMBuildFPExt) \
  BUILDER_BIND_CAST_OP(ptr2int, LLVMBuildPtrToInt) \
  BUILDER_BIND_CAST_OP(int2ptr, LLVMBuildIntToPtr) \
  BUILDER_BIND_CAST_OP(bit_cast, LLVMBuildBitCast) \
  BUILDER_BIND_CAST_OP(addr_space_cast, LLVMBuildAddrSpaceCast) \
  BUILDER_BIND_CAST_OP(zext_or_bit_cast, LLVMBuildZExtOrBitCast) \
  BUILDER_BIND_CAST_OP(sext_or_bit_cast, LLVMBuildSExtOrBitCast) \
  BUILDER_BIND_CAST_OP(trunc_or_bit_cast, LLVMBuildTruncOrBitCast) \
  BUILDER_BIND_CAST_OP(pointer_cast, LLVMBuildPointerCast) \
  BUILDER_BIND_CAST_OP(fp_cast, LLVMBuildFPCast)



void populateCore(nb::module_ &m);
PyModule parseIR(LLVMContextRef ctx, LLVMMemoryBufferRef memBuf);


class PyFunctionIterator {
public:
  explicit PyFunctionIterator(PyFunction fn): fn(fn) {}
  explicit PyFunctionIterator(LLVMValueRef fn): fn(PyFunction(fn)) {}

  PyFunction get() {
    return fn;
  }

  PyFunction next() {
    auto res = LLVMGetNextFunction(fn.get());
    if (!res)
      throw nb::stop_iteration();
    auto prev = fn;
    fn = PyFunction(res);
    return prev;
  }
  
private:
  PyFunction fn;
};



#endif
