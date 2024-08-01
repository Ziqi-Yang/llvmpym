from llvmpym import core, utils

asm_str = r'''
    ; ModuleID = '<string>'
    target triple = "unknown-unknown-unknown"
    %struct.glob_type = type { i64, [2 x i64] }

    @glob = global i32 0
    @glob_b = global i8 0
    @glob_f = global float 1.5
    @glob_struct = global %struct.glob_type {i64 0, [2 x i64] [i64 0, i64 0]}

    define i32 @sum(i32 %.1, i32 %.2) {
      %.3 = add i32 %.1, %.2
      %.4 = add i32 0, %.3
      ret i32 %.4
    }

    define void @foo() {
      call void asm sideeffect "nop", ""()
      ret void
    }

    declare void @a_readonly_func(i8 *) readonly
'''

m = utils.parse_assembly(asm_str)
for f in m.functions:
    print(f'Function: {f.name}/`{f.type}`')



