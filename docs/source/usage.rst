Usage
=====

Naming of the Python API
------------------------

(note this part of docs uses some parts of `llvmcpy <https://github.com/revng/llvmcpy>`_'s docs)

The basic idea behind this project is to take the LLVM-C API, create a class for each data type and create a method for that class for each function in the API taking an argument of that data type as first argument.

This means that the following functions:

.. code-block:: c
                
   LLVMModuleRef LLVMCloneModule (LLVMModuleRef M)

Will become:

.. code-block:: python
                
   class Module:
       def clone(self):
           # ...


Properties
^^^^^^^^^^^^^
For mant function starting with LLVMGet or LLVMSet in the LLVM-C API, a property is generated. For example, consider the following two functions:

.. code-block:: c

   void LLVMSetValueName (LLVMValueRef Val, const char *Name);
   const char* LLVMGetValueName(LLVMValueRef Val);

In *llvmpym* the Get/Set prefixes disappear, along with ``Value`` (the name of the class) and you can use them as properties of the ``Value`` class, e.g.:

.. code-block:: python
                
   my_value.name = "sum"
   print(my_value.name)

