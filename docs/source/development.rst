Development
===========

Enable NanoBind debug
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
#. Possibly you also need to add used LLVM components in ``CMakeLists.txt`` (
   inside ``llvm_map_components_to_libnames``). To search for what component
   a symbol/function is in, you can use `just find-components` command if you have
   `just <https://github.com/casey/just>`_ installed, otherwise you need to see
   the implementation detail in ``justfile`` file under the project root directory.
   Note ``just list-llvm-components`` is also useful, which listed all the
   LLVM components.


Add a new Class
----------------

To inherit from ``PyLLVMObject``:

#. ``nb::class_<PyTarget, PyLLVMObject<PyTarget, LLVMTargetRef>>(m, "Target", "Target")``
#. Add an entry in ``BIND_PYLLVMOBJECT`` macro in ``src/types_priv``
#. If you create new `.cpp` file, add it to ``nanobind_add_module`` function in
   ``CMakeLists.txt`` file.

Class Naming Style
^^^^^^^^^^^^^^^^^^^^^^^^^^^

`Pym` is used as prefix instead of `Py` is because some Python internal object
uses `Py` as their class prefix (e.g. `PyContext`). When build for Python version
which doesn't support `stable abi <https://docs.python.org/3/c-api/stable.html>`_
or not enabled (For ``llvmpym``, it's ``Python 311`` build), then NanoBind will
emit error on compilation.

   
Add a new Iterator Class
-------------------------

For example, `PyTarget`

#. Define iterator class in ``types_priv``: ``DEFINE_ITERATOR_CLASS(PyTargetIterator, PyTarget, LLVMGetNextTarget)``
#. Bind iterator class. In corresponding module file: ``BIND_ITERATOR_CLASS(PyTargetIterator, "TargetIterator")``
#. Possibly:

   .. code-block:: c++

      nb::class_<PyTarget, PyLLVMObject<PyTarget, LLVMTargetRef>>(m, "Target", "Target")
          .def("__iter__", [](PyTarget &self) { return PyTargetIterator(self); });


Docstring Style
----------------

https://www.sphinx-doc.org/en/master/usage/extensions/example_google.html

          
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

