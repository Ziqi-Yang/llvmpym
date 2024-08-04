from llvmpym import core, utils
from llvmpym.analysis import VerifierFailureAction, view_function_cfg

G32 = core.IntType.GlobalInt32
m = core.Module()
fn = core.Function(m, core.FunctionType(G32, [G32], False), name="foo")
fn.verify(VerifierFailureAction.PrintMessage)

builder = core.Builder()
builder.position_at_end(fn.append_basic_block())
builder.ret_void()

print("\n-----")

fn.verify(VerifierFailureAction.PrintMessage)

print("\n-----")
x = m.verify(VerifierFailureAction.ReturnStatus)
print(x)
