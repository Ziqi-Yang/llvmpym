# the logic is taken from llvmlite
import llvmpym.core as core
from llvmpym.core import IntType, IntPredicate

fn_type = core.FunctionType(IntType.GlobalInt32,
                            [IntType.GlobalInt32, IntType.GlobalInt32],
                            False)
m = core.Module()
fn = core.Function(m, fn_type, name="foo")
bb_entry = fn.append_basic_block()

builder = core.Builder()
builder.position_at_end(bb_entry)

stackint = builder.alloca(IntType.GlobalInt32)
assert isinstance(stackint.type, IntType) # remove lsp diagnostic
builder.store(core.ConstantInt(stackint.type, 123, True), stackint)
myint = builder.load2(stackint.type, stackint)

# Currently, every property goes through an function call through LLVM
# involving WRAP and UNWRAP multiple many times through both python - LLVM-C
# layer and LLVM-C - LLVM-C++ layer. There may be some performance loss.
# In the future, we may add cache to object properties.
add_inst = builder.add(fn.params[0], fn.params[1])
mul_inst = builder.mul(add_inst, core.ConstantInt(IntType.GlobalInt32, 123, True))
pred = builder.icmp(IntPredicate.SLE, add_inst, mul_inst)
builder.ret(mul_inst)

bb_block = fn.append_basic_block()
builder.position_at_end(bb_block)

bb_exit = fn.append_basic_block()

pred = builder.trunc(add_inst, IntType.GlobalInt1)
builder.cond_br(pred, bb_block, bb_exit)

builder.position_at_end(bb_exit)
builder.ret(myint)

print(m)

