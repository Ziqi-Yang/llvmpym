#include "PymBinary.h"

PymBinary::PymBinary(LLVMBinaryRef bin)
: obj(get_shared_obj(bin)) { }

LLVMBinaryRef PymBinary::get() const {
  return obj.get();
}

SHARED_POINTER_IMPL(PymBinary, LLVMBinaryRef, LLVMOpaqueBinary, LLVMDisposeBinary)


