# Note use `pip install .` to install this package
# In `pip install --no-build-isolation -ve .` mode it won't work

from llvmpym import core

if __name__ == '__main__':
    a = core.Module("hi")
    a.id = "Hello"
    print(a.id)

    c = core.Context.get_global_context()
    # c.should_discard_value_names = True
    # print(c.should_discard_value_names)

