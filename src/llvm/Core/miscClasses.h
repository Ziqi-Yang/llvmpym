#ifndef LLVMPYM_CORE_MISCCLASS_H
#define LLVMPYM_CORE_MISCCLASS_H

#include <nanobind/nanobind.h>
#include <llvm-c/Core.h>


#define BUILDER_BIND_BINARY_OP(NAME, FUNCTION) \
  .def(#NAME, \
      [](PymBuilder &self, PymValue &lhs, PymValue &rhs, const char *name) { \
        return PymValueAuto(FUNCTION(self.get(), lhs.get(), rhs.get(), name)); \
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
      [](PymBuilder &self, PymValue &val, PymType &destType, const char *name) { \
        return PymValueAuto(FUNCTION(self.get(), val.get(), destType.get(), name)); \
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


void bindOtherClasses(nanobind::module_ &m);


#endif
