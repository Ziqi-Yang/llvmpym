llvmpym documentation
=====================

Status
------

.. csv-table:: LLVM C Header File Mapping Status
   :header: "Header File", "Python Module", "Status"

   "Core.h", "llvmpym.core", "☑"
   "ErrorHandling.h", "llvmpym.error_handling", "☑"
   "IRReader.h", "llvmpym.core", "☑"
   "Types.h", "llvmpym.core", "☑"
   "lto.h", "", "☐"
   

- All functions that operates on lambda functions(function pointers) is not binded 
  or handled correctly. Typically those parameters'(or returned value's) type name ends 
  with `Hanlder` or `Callback`, e.g. `LLVMDiagnosticHandler`, `LLVMYieldCallback`.


Table of contents
-----------------

.. toctree::
   :maxdepth: 1

   installation
   usage

.. toctree::
   :caption: Misc
   :maxdepth: 1

   tips
   development



