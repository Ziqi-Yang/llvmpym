# Note use `pip install .` to install this package
# In `pip install --no-build-isolation -ve .` mode it won't work
from llvmpym.core import *

class TestContants:
    pass

class TestEnum:
    def test_enum_member_numbers(self):
        assert len(Opcode) == 67
        assert len(TypeKind) == 21
        assert len(Linkage) == 17
        assert len(Visibility) == 3
        assert len(UnnamedAddr) == 3
        assert len(DLLStorageClass) == 3
        assert len(CallConv) == 41
        assert len(ValueKind) == 27
        assert len(IntPredicate) == 10
        assert len(RealPredicate) == 16
        assert len(LandingPadClauseTy) == 2
        assert len(ThreadLocalMode) == 5
        assert len(AtomicOrdering) == 7
        assert len(AtomicRMWBinOp) == 15
        assert len(DiagnosticSeverity) == 4
        assert len(InlineAsmDialect) == 2
        assert len(ModuleFlagBehavior) == 6
        assert len(AttributeIndex) == 2
        assert len(TailCallKind) == 4
        assert len(FastMathFlags) == 9

        
    def test_value(self):
        assert IntPredicate.EQ.value == 32

    def test_docstring(self):
        assert TypeKind.Void.__doc__ == "type with no size"

        

class TestModule:
    def test_module(self):
        pass
    

class TestEquality:
    # TODO
    def test_value(self):
        x = ConstantInt(IntType.GlobalInt32, 100, True)
        y = ConstantInt(IntType.GlobalInt32, 100, True)
        assert x == y
