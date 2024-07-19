# Note use `pip install .` to install this package
# In `pip install --no-build-isolation -ve .` mode it won't work
from llvmpym.core import *

class TestEnum:
    def test_enum_member_numbers(self):
        assert len(Opcode) == 67
        assert len(TypeKind) == 21
        assert len(Linkage) == 17
        assert len(Visibility) == 3
        assert len(UnnamedAddr) == 3
        
    def test_value(self):
        assert IntPredicate.EQ.value == 32

    def test_docstring(self):
        assert TypeKind.Void.__doc__ == "type with no size"

        

class TestModule:
    def test_module(self):
        pass
    

