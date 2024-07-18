from llvmpym.core import *

class TestEnum:
    def test_enum_member_numbers(self):
        assert len(Opcode) == 67
        assert len(TypeKind) == 21
        assert len(Linkage) == 17
        assert len(Visibility) == 3
        assert len(UnnamedAddr) == 3
        
    def test_value(self):
        assert IntPredicate.IntEQ.value == 32

    def test_docstring(self):
        assert TypeKind.VoidTypeKind.__doc__ == "type with no size"

        

class TestModule:
    def test_module(self):
        pass
    

