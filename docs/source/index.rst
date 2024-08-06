llvmpym documentation
=====================

llvmpym is a python library that provides LLVM binding. Target LLVM version: `18.1.8`.

The aim of this project is to provide a full python binding built on top of the LLVM
C API.


Currently this project is still in its very early stage. Using the library means
involving in the development (reporting bugs, providing ideas and advice, etc.) and
accepts potential bugs. Although the binary wheel is hosted on PyPI, it is recommended
to build it yourself.

`nanobind <https://github.com/wjakob/nanobind>`_ is used to build the binding.

Features
-----------

#. Editor integration (Language server integration: code completion, docs for API, etc.),
   though it is normal for pure python library.
#. Fine-tuned API and well-organized classes.
#. Follows latest LLVM version.
#. Sane and Python style memory management (i.e. You don't need to care about it).


Major differences from the existing LLVM python binding libraries
------------------------------------------------------------------

- `llvmlite <https://github.com/numba/llvmlite>`_
  
  Pros:
  
  - Possibly faster than llvmpym in case of IR builder since its implementation is
    pure python and
  - Better tailored for python use as for some API. For example, it provides
    ``attributes`` property to achieve all the attribute sets of an Function. While in
    llvmpym, ``f.get_attributes_at_index(i)`` is used for doing this job since
    llvmpym is totally built on top of LLVM C API, which doesn't provide ability
    to retrieve attribute sets.
  - Stable and well-tested.

  Cons:
  
  - Only support old version of LLVM (``v14``, moving towards ``v15``)
  - Originally tailored for `Numba <numba.pydata.org>`_'s use. Till today, some
    functionalities are not achieved, which is possibly due to sticking on old LLVM
    version.
  - Separate implementation of LLVM IR builder (in pure python) and binding. The
    information cannot be seamlessly exchanged.
  - For binding part, the categorization of classes is not as detailed as llvmpym.
  - Possibly high maintenance cost

- `llvmcpy <https://github.com/revng/llvmcpy>`_
  
  Pros:
  
  - Exceptionally low maintenance cost for tracking API change (automatically generated code).
  - Widely supported LLVM versions and python versions.

  Cons:

  - Interfaces is not well designed (automatically generated code).
  - C style memory management.
  - No editor integration (e.g. language server: code completion, docs)
  - When error occurs, tracing error could be tough since everyone runs programs
    on different machine, using different python version, LLVM version.
    
Status
------

.. csv-table:: LLVM C API Header File Mapping Status
  :header: "Header File", "Python Modules"

   "``Core.h``", "``core``"
   "``IRReader.h``", "``core``"
   "``Analysis.h``", "``core``, ``analysis``",
   "``ErrorHandling.h``", "``error_handling``"
   "``Support.h``", "``support``"
   "``TargetMachine.h``", "``target_machine``"
   "``BitReader.h``", "``bit_reader``, ``core``"
   "``BitWriter.h``", "``core``"
   "", "``utils``"

- All functions that operates on lambda functions(function pointers) is not bound 
  or handled incorrectly. Typically those parameters'(or returned value's) type name ends 
  with `Hanlder` or `Callback`, e.g. `LLVMDiagnosticHandler`, `LLVMYieldCallback`. Though
  very few API operates on lambda functions.

- All functions accepts or return a void pointer (`void *`) is not bound. Though
  very few API accepts or return a void pointer.


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


Special Thanks
--------------------

- `nanobind <https://github.com/wjakob/nanobind>`_
- `llvmlite <https://github.com/numba/llvmlite>`_
- `llvmcpy <https://github.com/revng/llvmcpy>`_
