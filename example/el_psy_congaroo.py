# Note use `pip install .` to install this package
# In `pip install --no-build-isolation -ve .` mode it won't work

import llvmpym.core as core

c = core.Context.get_global_context()
s = r'target triple = "unknown-unknown-unknown"'
mem_buf = core.MemoryBuffer.from_str(s, "")
res = c.parse_ir(mem_buf)
print(res)
