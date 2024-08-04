from llvmpym import core

double = core.RealType.GlobalDouble;
fn_type = core.FunctionType(double, (double, double), False)

with core.Module(name=__file__) as m:
    fn = core.Function(m, fn_type, "fpadd")

    block = fn.append_basic_block("entry")
    builder = core.Builder()
    builder.position_at_end(block)

    a, b = fn.args
    result = builder.fadd(a, b, name="res")
    builder.ret(result)

    print(m)
    

