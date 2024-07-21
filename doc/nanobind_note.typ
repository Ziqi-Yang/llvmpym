= Nanobind Note
The concrete example/solution lives in `llvmpym`'s codebase.

== Incomplete type
For example
```c
typedef struct LLVMOpaqueType *LLVMTypeRef;
```
`LLVMOpaqueType` is an incomplete type since it has no complete definition.
For this situation, we shouldn't allow it to appear in the exposed function parameters or return type. Instead, we can make a wrapper class and always use this class to exchange information.

== Function parameter `std::vector` type of Wrapper class
i.e. `std::vector<Wrapper>` -> `std::vector<underlaying>`
=== Wrapper Class is Non-copyable
```cpp
m.def("create_function_type",
    [](std::unique_ptr<PyType, nb::deleter<PyType>> returnType,
       std::vector<std::unique_ptr<PyType, nb::deleter<PyType>>> paramTypes,
       bool isVarArg) {
      unsigned param_count = paramTypes.size();
        
      std::vector<LLVMTypeRef> rawParamTypes;
      rawParamTypes.reserve(param_count);
        
      for (auto& pt : paramTypes) {
        rawParamTypes.push_back(pt->get());
      }
        
      return PyType(LLVMFunctionType(returnType->get(), rawParamTypes.data(),
                                     param_count, isVarArg));
    }, "return_type"_a, "param_types"_a, "is_var_arg"_a,
    "Obtain a function type consisting of a specified signature.");
    ```

=== Wrapper class is Copyable
```cpp
m.def("get_function_type",
      [](PyType &returnType, std::vector<PyType> &paramTypes, bool isVarArg) {
        unsigned param_count = paramTypes.size();
          
        std::vector<LLVMTypeRef> rawParamTypes;
        rawParamTypes.reserve(param_count);
          
        for (const auto& pt : paramTypes) {
          rawParamTypes.push_back(pt.get());
        }
          
        return PyTypeFunction(LLVMFunctionType(returnType.get(), rawParamTypes.data(),
                                               param_count, isVarArg));
      }, "return_type"_a, "param_types"_a, "is_var_arg"_a,
      "Obtain a function type consisting of a specified signature.");
```
