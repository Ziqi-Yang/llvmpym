Development
===========

Enable nanobind debug
---------------------

Set the value of environment variable ``SKBUILD_CMAKE_BUILD_TYPE`` to ``Debug``.

Debug memory issues
-------------------

e.g. ``Misaligned address error``

Use ``gdb``. For example:

.. code-block:: bash

   gdb --args python ./example/parse_ir_assmebly.py

Inside ``gdb``, do ``run`` and ``bt``.

In some cases, the backtrace doesn't show you all the symbols, you need to use `valgrind`,
for example:

.. code-block:: bash

   valgrind --leak-check=full python ./example/llvmir_builder.py


Check the correctness of generated stub file
--------------------------------------------

The generated stub file is possible to contain errors like you use a Python keyword
as an parameter name of a function. Run ``cd ./docs && make html`` to check it, which
is handled by *sphinx-autoapi*.



Add a python sub-module
-------------------------

Like `core`, `support` under `llvmpym` module.

Steps:

#. module definition file inside ``src/llvm`` directory
#. module registry in ``src/llvmpym_ext.cpp``
#. add module definition file as an dependency of the final extension file in ``CMakeLists.txt``
   (``nanobind_add_module``)
#. ``CMakeLists.txt`` generate stub file (i.e. ``nanobind_add_stub`` and ``install``)
#. add a python file in ``src/llvmpym`` directory to remap module from
   `llvmpym.llvmpym_ext.<module>` to `llvmpym.<module>`


NanoBind Pitfalls
-----------------

Default Argument
^^^^^^^^^^^^^^^^^^

For example

.. code-block:: cpp

   PyModule parseAssembly(std::string &iasm, PyContext &ctx) {
     auto memBuf = LLVMCreateMemoryBufferWithMemoryRangeCopy(iasm.c_str(), iasm.size(), "");
     return parseIR(ctx.get(), memBuf);
   }

   void populateUtils(nb::module_ &m) {
     m.def("parse_assembly", &parseAssembly,
     "isam"_a, "context"_a = PyContext::getGlobalContext());
   }

The ``PyContext::getGlobalContext()`` function here will cause problems. According
to observation, the an PyContext object will be generated and managed by NanoBind
as long as you imported the library. When encountering memory bugs, you cannot see
concrete symbol name in the stacktrace in Debug mode. For commit
`285d53db92264e55a705195df9d1a6c7a024d3b6 <https://github.com/Ziqi-Yang/llvmpym/commit/285d53db92264e55a705195df9d1a6c7a024d3b6>`_, the code above, `example/llvmir_builder.py line 25 <https://github.com/Ziqi-Yang/llvmpym/blob/285d53db92264e55a705195df9d1a6c7a024d3b6/example/llvmir_builder.py#L25>`_ and the ``print(m)`` at the end of file will cause an memory bug (``free(): invalid pointer``)
when program is finishing, which is seemingly irrelevant of the execution
and may relate to underlying operating principles of LLVM and NanoBind.

As a contrast, the following went smooth.
   
.. code-block:: cpp

   PyModule parseAssembly(std::string &iasm) {
     auto context = PyContext::getGlobalContext();
     auto memBuf = LLVMCreateMemoryBufferWithMemoryRangeCopy(iasm.c_str(), iasm.size(), "");
     return parseIR(context.get(), memBuf);
   }


   void populateUtils(nb::module_ &m) {
     m.def("parse_assembly", &parseAssembly, "isam"_a);
   }

In conclusion, it is suggested that classes which do self memory control shouldn't be
appeared as a default argument of some functions.


Resources / References
----------------------

LLVM online reference are all of the latest version. To view reference of a certain version,
please manually build the docs.

- `LLVM C API doxygen <https://llvm.org/docs/doxygen/group__LLVMCCore.html>`_
- `LLVM Reference Manual <https://llvm.org/docs/LangRef.html>`_

