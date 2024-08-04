#include "type.h"

#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/optional.h>
#include <llvm-c/Core.h>
#include <fmt/core.h>
#include <optional>
#include "../types_priv.h"
#include "../utils_priv.h"
#include "utils.h"

namespace nb = nanobind;
using namespace nb::literals;

template <typename T>
using optional = std::optional<T>;

void bindTypeClasses(nb::module_ &m) {
  auto TypeClass = nb::class_<PyType, PyLLVMObject<PyType, LLVMTypeRef>>(m, "Type", "Type");
  auto TypeIntClass = nb::class_<PyTypeInt, PyType>(m, "IntType", "IntType");
  auto TypeRealClass = nb::class_<PyTypeReal, PyType>(m, "RealType", "RealType");
  auto TypeFunctionClass = nb::class_<PyTypeFunction, PyType> (m, "FunctionType", "FunctionType");
  auto TypeStructClass = nb::class_<PyTypeStruct, PyType> (m, "StructType", "StructType");
  auto TypeSequenceClass = nb::class_<PyTypeSequence, PyType>(m, "SequenceType", "SequenceType");
  auto TypeArrayClass = nb::class_<PyTypeArray, PyTypeSequence>(m, "ArrayType", "ArrayType");
  auto TypePointerClass = nb::class_<PyTypePointer, PyTypeSequence>(m, "PointerType", "PointerType");
  auto TypeVectorClass = nb::class_<PyTypeVector, PyTypeSequence>(m, "VectorType", "VectorType");
  auto TypeVoidClass = nb::class_<PyTypeVoid, PyType>(m, "VoidType", "VoidType");
  auto TypeLabelClass = nb::class_<PyTypeLabel, PyType>(m, "LabelType", "LabelType");
  auto TypeX86MMXClass = nb::class_<PyTypeX86MMX, PyType>(m, "X86MMXType", "X86MMXType");
  auto TypeX86AMXClass = nb::class_<PyTypeX86AMX, PyType>(m, "X86AMXType", "X86AMXType");
  auto TypeTokenClass = nb::class_<PyTypeToken, PyType>(m, "TokenType", "TokenType");
  auto TypeMetadataClass = nb::class_<PyTypeMetadata, PyType>(m, "MetadataType", "MetadataType");
  auto TypeTargetExtClass = nb::class_<PyTypeTargetExt, PyType>(m, "TargetExtType", "TargetExtType");
  
  TypeClass
      .def("__repr__",
           [](PyType &self) {
             auto kind = get_repr_str(LLVMGetTypeKind(self.get()));
             return fmt::format("<Type kind='{}' sig=''>", kind, get_type_str(self));
           })
      .def("__str__", &get_type_str)
      .def_prop_ro("align",
                   [](PyType &t) {
                     return PyConstantExpr(LLVMAlignOf(t.get()));
                   })
      .def_prop_ro("size",
                   [](PyType &t) {
                     return PyConstantExpr(LLVMSizeOf(t.get()));
                   })
      .def_prop_ro("kind",
                   [](PyType &t) { return LLVMGetTypeKind(t.get()); },
                   "Obtain the enumerated type of a Type instance.")
      .def_prop_ro("is_sized",
                   [](PyType &t) { return LLVMTypeIsSized(t.get()) != 0; },
                   "Whether the type has a known size.\n\n"
                   "Things that don't have a size are abstract types, labels, and void.a")
      .def_prop_ro("context",
                   [](PyType &t) { return PyContext(LLVMGetTypeContext(t.get())); },
                   "Obtain the context to which this type instance is associated.")
      .def_prop_ro("sub_type_number",
                   [](PyType &t) {
                     return LLVMGetNumContainedTypes(t.get());
                   })
      .def_prop_ro("sub_types",
                   [](PyType &t) {
                     unsigned num = LLVMGetNumContainedTypes(t.get());
                     std::vector<LLVMTypeRef> arr(num);
                     LLVMGetSubtypes(t.get(), arr.data());
                     WRAP_VECTOR_FROM_DEST(PyTypeSequence, num, res, arr);
                     return res;
                   })
      .def("null",
           [](PyType &t) {
             return PyValueAuto(LLVMConstNull(t.get()));
           },
           "Obtain a constant value referring to the null instance of the type.")
      .def("pointer_null",
           [](PyType &t) {
             return PyValueAuto(LLVMConstPointerNull(t.get()));
           },
           "Obtain a constant that is a constant pointer pointing to NULL for the"
           "type.")
      .def("undef",
           [](PyType &t) {
             return PyUndefValue(LLVMGetUndef(t.get()));
           },
           "Obtain a constant value referring to an undefined value of a type.")
      .def("poison",
           [](PyType &t) {
             return PyPoisonValue(LLVMGetPoison(t.get()));
           },
           "Obtain a constant value referring to a poison value of a type.")
      .def("dump",
           [](PyType &self) {
             return LLVMDumpType(self.get());
           },
           "Dump a representation of a type to stderr.");

  
  TypeIntClass
      .def("__repr__",
           [](PyTypeInt &self) {
             return fmt::format("<IntType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PyTypeInt *t, PyContext &c, unsigned numBits) {
             new (t) PyTypeInt(LLVMIntTypeInContext(c.get(), numBits));
           }, "context"_a, "num_bits"_a)
      .def_static("Int1",
                  [](PyContext &c) { return PyTypeInt(LLVMInt1TypeInContext(c.get())); },
                  "context"_a)
      .def_static("Int8",
                  [](PyContext &c) { return PyTypeInt(LLVMInt8TypeInContext(c.get())); },
                  "context"_a)
      .def_static("Int16",
                  [](PyContext &c) { return PyTypeInt(LLVMInt16TypeInContext(c.get())); },
                  "context"_a)
      .def_static("Int32",
                  [](PyContext &c) { return PyTypeInt(LLVMInt32TypeInContext(c.get())); },
                  "context"_a)
      .def_static("Int64",
                  [](PyContext &c) { return PyTypeInt(LLVMInt64TypeInContext(c.get())); },
                  "context"_a)
      .def_static("Int128",
                  [](PyContext &c) { return PyTypeInt(LLVMInt128TypeInContext(c.get())); },
                  "context"_a)
      .def_prop_ro_static("GlobalInt1",
                          [](nb::handle) { return PyTypeInt(LLVMInt1Type()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalInt8",
                          [](nb::handle) { return PyTypeInt(LLVMInt8Type()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalInt16",
                          [](nb::handle) { return PyTypeInt(LLVMInt16Type()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalInt32",
                          [](nb::handle) { return PyTypeInt(LLVMInt32Type()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalInt64",
                          [](nb::handle) { return PyTypeInt(LLVMInt64Type()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalInt128",
                          [](nb::handle) { return PyTypeInt(LLVMInt128Type()); },
                          "Get type from global context.")
      .def_static("Global",
                  [](unsigned numBits) { return PyTypeInt(LLVMIntType(numBits)); },
                  "Get type from global context.")
      .def_prop_ro("width",
                   [](PyTypeInt &t) { return LLVMGetIntTypeWidth(t.get()); })
      .def("all_ones",
           [](PyTypeInt &t) {
             return PyValueAuto(LLVMConstAllOnes(t.get()));
           },
           "Obtain a constant value referring to the instance of the type"
           "consisting of all ones.");
  

  TypeRealClass
      .def("__repr__",
           [](PyTypeReal &self) {
             return fmt::format("<RealType {}>", get_type_str(self));
           })
      .def_static("Half",
                  [](PyContext &c) { return PyTypeReal(LLVMHalfTypeInContext(c.get())); },
                  "context"_a)
      .def_static("Bfloat",
                  [](PyContext &c) { return PyTypeReal(LLVMBFloatTypeInContext(c.get())); },
                  "context"_a)
      .def_static("Float",
                  [](PyContext &c) { return PyTypeReal(LLVMFloatTypeInContext(c.get())); },
                  "context"_a)
      .def_static("Double",
                  [](PyContext &c) { return PyTypeReal(LLVMDoubleTypeInContext(c.get())); },
                  "context"_a)
      .def_static("X86FP80",
                  [](PyContext &c) { return PyTypeReal(LLVMX86FP80TypeInContext(c.get())); },
                  "context"_a)
      .def_static("FP128",
                  [](PyContext &c) { return PyTypeReal(LLVMFP128TypeInContext(c.get())); },
                  "context"_a)
      .def_static("PPCFP128",
                  [](PyContext &c) { return PyTypeReal(LLVMPPCFP128TypeInContext(c.get())); },
                  "context"_a)
      .def_prop_ro_static("GlobalHalf",
                          [](nb::handle){ return PyTypeReal(LLVMHalfType()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalBfloat",
                          [](nb::handle){ return PyTypeReal(LLVMBFloatType()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalFloat",
                          [](nb::handle){ return PyTypeReal(LLVMFloatType()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalDouble",
                          [](nb::handle){ return PyTypeReal(LLVMDoubleType()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalX86FP80",
                          [](nb::handle){ return PyTypeReal(LLVMX86FP80Type()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalFP128",
                          [](nb::handle){ return PyTypeReal(LLVMFP128Type()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalPPCFP128",
                          [](nb::handle){ return PyTypeReal(LLVMPPCFP128Type()); },
                          "Get type from global context.");


  TypeFunctionClass
      .def("__repr__",
           [](PyTypeFunction &self) {
             return fmt::format("<FunctionType {}>", get_type_str(self));
           })
       .def("__init__",
            [](PyTypeFunction *t, PyType &returnType, std::vector<PyType> &paramTypes, bool isVarArg) {
              unsigned param_count = paramTypes.size();
              UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, paramTypes, rawParamTypes, param_count)
              new (t) PyTypeFunction(LLVMFunctionType(returnType.get(), rawParamTypes.data(),
                                                      param_count, isVarArg));
            },
            "return_type"_a, "param_types"_a, "is_var_arg"_a,
            "Obtain a function type consisting of a specified signature.")
      .def_prop_ro("is_vararg",
                   [](PyTypeFunction &t) { return LLVMIsFunctionVarArg(t.get()) != 0; },
                   "Returns whether a function type is variadic.")
      .def_prop_ro("return_type",
                   [](PyTypeFunction &t) { return PyTypeAuto(LLVMGetReturnType(t.get())); },
                   "Obtain the Type this function Type returns.")
      .def_prop_ro("params_num",
                   [](PyTypeFunction &t) { return LLVMCountParamTypes(t.get()); },
                   "Obtain the number of parameters this function accepts.")
      .def_prop_ro("param_types",
                   [](PyTypeFunction &t) {
                     unsigned param_number = LLVMCountParamTypes(t.get());
                     std::vector<LLVMTypeRef> dest(param_number);
                     LLVMGetParamTypes(t.get(), dest.data());
                     WRAP_VECTOR_FROM_DEST(PyTypeFunction, param_number, res, dest);
                     return res;
                   },
                   "Obtain the types of a function's parameters.");
  
  TypeStructClass
      .def("__repr__",
           [](PyTypeStruct &self) {
             return fmt::format("<StructType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PyTypeStruct *t, PyContext &c, std::vector<PyType> &elementTypes, bool packed) {
             unsigned elem_count = elementTypes.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, elementTypes, rawElemTypes, elem_count);
             new (t) PyTypeStruct(LLVMStructTypeInContext(c.get(), rawElemTypes.data(),
                                                          elem_count, packed));
           },
           "context"_a, "element_types"_a, "packed"_a,
           "Create a new structure type in context.")
      .def("__init__",
           [](PyTypeStruct *t, PyContext &c, std::string name) {
             new (t) PyTypeStruct(LLVMStructCreateNamed(c.get(), name.c_str()));
           },
           "context"_a, "name"_a = "",
           "Create an empty structure in the context having a specified name.")
      .def_static("Global",
                  [](std::vector<PyType> elementTypes, bool packed) {
                    unsigned elem_count = elementTypes.size();
                    UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, elementTypes, rawElemTypes, elem_count);
                    return PyTypeStruct(LLVMStructType(rawElemTypes.data(), elem_count, packed));
                  },
                  "element_types"_a, "packed"_a,
                  "Create a new structure type in the global context.")
      .def_prop_ro("name", [](PyTypeStruct &t) { return LLVMGetStructName(t.get()); })
      .def_prop_ro("elem_number",
                   [](PyTypeStruct &t) {
                     return LLVMCountStructElementTypes(t.get());
                   })
      .def_prop_ro("elem_types",
                   [](PyTypeStruct &t) {
                     unsigned num = LLVMCountStructElementTypes(t.get());
                     std::vector<LLVMTypeRef> dest(num);
                     LLVMGetStructElementTypes(t.get(), dest.data());
                     WRAP_VECTOR_FROM_DEST(PyTypeStruct, num, res, dest);
                     return res;
                   })
      .def_prop_ro("is_packed",
                   [](PyTypeStruct &t) { return LLVMIsPackedStruct(t.get()) != 0; },
                   "Determine whether a structure is packed.")
      .def_prop_ro("is_opaque",
                   [](PyTypeStruct &t) { return LLVMIsOpaqueStruct(t.get()) != 0; })
      .def_prop_ro("is_literal",
                   [](PyTypeStruct &t) { return LLVMIsLiteralStruct(t.get()) != 0; },
                   "Determine whether a structure is literal.")
      .def("set_body",
           [](PyTypeStruct &t, std::vector<PyType> elementTypes, bool packed) {
             unsigned elem_count = elementTypes.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, elementTypes, rawElemTypes, elem_count);
             return LLVMStructSetBody(t.get(), rawElemTypes.data(), elem_count, packed);
           },
           "elem_types"_a, "packed"_a,
           "Set the contents of a structure type.")
      .def("get_type_at_index",
           [](PyTypeStruct &t, unsigned i) -> optional<PyType*> {
             auto res = LLVMStructGetTypeAtIndex(t.get(), i);
             WRAP_OPTIONAL_RETURN(res, PyTypeAuto);
           },
           "index"_a,
           "Get the type of the element at a given index in the structure.");

  TypeSequenceClass
      .def("__repr__",
           [](PyTypeSequence &self) {
             return fmt::format("<SequenceType {}>", get_type_str(self));
           })
      .def_prop_ro("element_type", // TODO test pointer type
                   [](PyTypeSequence &t) {
                     return PyTypeAuto(LLVMGetElementType(t.get()));
                   });

  TypeArrayClass
      .def("__repr__",
           [](PyTypeArray &self) {
             return fmt::format("<ArrayType {}>", get_type_str(self));
           })
      // NOTE We use LLVMArrayType2 instead of LLVMArrayType to coordinate
      // with the `length` property
      .def("__init__", 
           [](PyTypeArray *t, PyType &elementType, uint64_t elementCount) {
             new (t) PyTypeArray(LLVMArrayType2(elementType.get(), elementCount));
           },
           "elem_type"_a, "elem_count"_a,
           "Create a fixed size array type that refers to a specific type.\n\n"
           "The created type will exist in the context that its element type"
           "exists in.")
       // LLVMGetArrayLength is deprecated in favor of the API accurate
       //  LLVMGetArrayLength2
      .def_prop_ro("length",
                   [](PyTypeArray &t) {
                     return LLVMGetArrayLength2(t.get());
                   });


  TypePointerClass
      .def("__repr__",
           [](PyTypePointer &self) {
             return fmt::format("<PointerType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PyTypePointer *t, PyContext &c, unsigned AddressSpace) {
             new (t) PyTypePointer(LLVMPointerTypeInContext(c.get(), AddressSpace));
           },
           "context"_a, "address_space"_a,
           "Create an opaque pointer type in a context.")
      .def("__init__",
           [](PyTypePointer *t, PyType &ElementType, unsigned AddressSpace) {
             new (t) PyTypePointer(LLVMPointerType(ElementType.get(), AddressSpace));
           },
           "elem_type"_a, "address_space"_a,
           "Create a pointer type that points to a defined type.\n\n"
           "The created type will exist in the context that its pointee type"
           "exists in.")
      .def_prop_ro("is_opaque",
                   [](PyTypePointer &t) { return LLVMPointerTypeIsOpaque(t.get()); })
      .def_prop_ro("address_space",
                   [](PyTypePointer &t) { return LLVMGetPointerAddressSpace(t.get()); });

  TypeVectorClass
      .def("__repr__",
           [](PyTypeVector &self) {
             return fmt::format("<VectorType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PyTypeVector *t, PyType &ElementType, unsigned ElementCount, bool IsScalable) {
             if (IsScalable) {
               new (t) PyTypeVector(LLVMScalableVectorType(ElementType.get(), ElementCount));
             } else {
               new (t) PyTypeVector(LLVMVectorType(ElementType.get(), ElementCount));
             }
           },
           "elem_type"_a, "elem_count"_a, "is_scalable"_a,
           "The created type will exist in the context thats its element type"
           "exists in.")
      .def_prop_ro_static("__len__",
                          [](PyTypeVector &t) { return LLVMGetVectorSize(t.get()); });

  
  TypeVoidClass
      .def("__repr__",
           [](PyTypeVoid &self) {
             return fmt::format("<VoidType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PyTypeVoid *t, PyContext &c) {
             new (t) PyTypeVoid(LLVMVoidTypeInContext(c.get()));
           },
           "context"_a)
      .def_prop_ro_static("Global",
                          [](nb::handle) { return PyTypeVoid(LLVMVoidType()); } );

  
  TypeLabelClass
      .def("__repr__",
           [](PyTypeLabel &self) {
             return fmt::format("<LabelType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PyTypeLabel *t, PyContext &c) {
             new (t) PyTypeLabel(LLVMLabelTypeInContext(c.get()));
           },
           "context"_a)
      .def_prop_ro_static("Global",
                          [](nb::handle) { return PyTypeLabel(LLVMLabelType()); } );


  TypeX86MMXClass
      .def("__repr__",
           [](PyTypeX86MMX &self) {
             return fmt::format("<X86MMXType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PyTypeX86MMX *t, PyContext &c) {
             new (t) PyTypeX86MMX(LLVMX86MMXTypeInContext(c.get()));
           },
           "context"_a)
      .def_prop_ro_static("Global",
                          [](nb::handle) { return PyTypeX86MMX(LLVMX86MMXType()); } );


  TypeX86AMXClass
      .def("__repr__",
           [](PyTypeX86AMX &self) {
             return fmt::format("<X86AMXType {}>", get_type_str(self));
           })
       .def("__init__",
            [](PyTypeX86AMX *t, PyContext &c) {
              new (t) PyTypeX86AMX(LLVMX86AMXTypeInContext(c.get()));
            },
            "context"_a)
       .def_prop_ro_static("Global",
                           [](nb::handle) { return PyTypeX86AMX(LLVMX86AMXType()); } );


  TypeTokenClass
      .def("__repr__",
           [](PyTypeToken &self) {
             return fmt::format("<TokenType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PyTypeToken *t, PyContext &c) {
             new (t) PyTypeToken(LLVMTokenTypeInContext(c.get()));
           },
           "context"_a);

  TypeMetadataClass
      .def("__repr__",
           [](PyTypeMetadata &self) {
             return fmt::format("<MetadataType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PyTypeMetadata *t, PyContext &c) {
             new (t) PyTypeMetadata(LLVMMetadataTypeInContext(c.get()));
           },
           "context"_a);

  TypeTargetExtClass
      .def("__repr__",
           [](PyTypeTargetExt &self) {
             return fmt::format("<TargetType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PyTypeVoid *t, PyContext &c, std::string &name, std::vector<PyType> typeParams,
              std::vector<unsigned> intParams) {
             unsigned typeParamSize = typeParams.size();
             unsigned intParamSize = intParams.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, typeParams, rawTypeParams, typeParamSize);
             new (t) PyTypeVoid(LLVMTargetExtTypeInContext
                                  (c.get(), name.c_str(), rawTypeParams.data(),
                                   typeParamSize, intParams.data(), intParamSize));
           });
}
