= Nanobind Note
The concrete example/solution lives in `llvmpym`'s codebase.

== Incomplete type
For example
```c
typedef struct LLVMOpaqueType *LLVMTypeRef;
```
`LLVMOpaqueType` is an incomplete type since it has no complete definition.
For this situation, we shouldn't allow it to appear in the exposed function parameters or return type. Instead, we can make a wrapper class and always use this class to exchange information.
