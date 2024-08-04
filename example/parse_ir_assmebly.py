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
    print(f'Function | name: "{f.name}", type: "{f.type}"')  
    module = f.parent
    assert m == module # point to the same module object
    assert m is not module # but python objects are not the same
    assert f.kind == core.ValueKind.Function
    for i, a in enumerate(f.args, 1):
        print(f'\tArgument | name: "{a.name}", type: "{a.type}"') 
        attrs = f.get_attributes_at_index(i)
        print(f"\t\tattrs: {attrs}")
    for bb in f.basic_blocks:
        print(f'\tBasicBlock | name: "{bb.name}" | value.type: "{bb.value.type}"')
        print("-------- Content of BasicBlock ---------")
        print(str(bb))
        print("-------- End of BasicBlock ---------")
        fn = bb.parent
        assert fn == f
        assert fn is not f
        assert bb.value.kind == core.ValueKind.BasicBlock
        for insr in bb.instructions:
            print(f'\t\tInstruction | name: "{insr.name}" | opcode: "{insr.opcode}" | type: \
"{insr.type}" | str: "{insr}"')
            assert insr.parent == bb
            assert insr.parent is not bb
            assert insr.kind == core.ValueKind.Instruction
            for i in range(insr.operands_num):
                operand = insr.get_operand(0)
                print(f'\t\t\tOperand | name: "{operand.name}" | type: "{operand.type}"')

    print("\n")

print("\n\n")
for g in m.global_variables:
    print(g)
    print(f'Global | name: "{g.name}" | type: "{g.type}"')
    assert g.kind == core.ValueKind.GlobalVariable
    print(f'\tvisibility: {g.visibility}')
    print()

