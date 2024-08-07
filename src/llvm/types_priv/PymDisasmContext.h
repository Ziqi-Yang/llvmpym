#ifndef LLVMPYM_TYPES_PRIV_PYMDISASMCONTEXT_H
#define LLVMPYM_TYPES_PRIV_PYMDISASMCONTEXT_H

#include <llvm-c/DisassemblerTypes.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "PymLLVMObject.h"
#include "utils.h"

class PymDisasmContext : public PymLLVMObject<PymDisasmContext, LLVMDisasmContextRef> {
public:
  explicit PymDisasmContext(LLVMDisasmContextRef data);
  LLVMDisasmContextRef get() const;
  
private:
  SHARED_POINTER_DEF(LLVMDisasmContextRef, void);
};


#endif
