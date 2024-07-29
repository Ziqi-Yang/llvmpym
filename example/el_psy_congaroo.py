# Note use `pip install .` to install this package
# In `pip install --no-build-isolation -ve .` mode it won't work

import llvmpym.core as core

with core.Context() as c:
    core.FunctionType()
