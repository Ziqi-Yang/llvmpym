#include "PymDisasmContext.h"
#include <llvm-c/Disassembler.h>

PymDisasmContext::PymDisasmContext(LLVMDisasmContextRef obj)
: obj(get_shared_obj(obj)) { }

LLVMDisasmContextRef PymDisasmContext::get() const {
  return obj.get();
}

SHARED_POINTER_IMPL(PymDisasmContext, LLVMDisasmContextRef, void,
                    LLVMDisasmDispose)


