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
  auto TypeClass = nb::class_<PymType, PymLLVMObject<PymType, LLVMTypeRef>>(m, "Type", "Type");
  auto TypeIntClass = nb::class_<PymTypeInt, PymType>(m, "IntType", "IntType");
  auto TypeRealClass = nb::class_<PymTypeReal, PymType>(m, "RealType", "RealType");
  auto TypeFunctionClass = nb::class_<PymTypeFunction, PymType> (m, "FunctionType", "FunctionType");
  auto TypeStructClass = nb::class_<PymTypeStruct, PymType> (m, "StructType", "StructType");
  auto TypeSequenceClass = nb::class_<PymTypeSequence, PymType>(m, "SequenceType", "SequenceType");
  auto TypeArrayClass = nb::class_<PymTypeArray, PymTypeSequence>(m, "ArrayType", "ArrayType");
  auto TypePointerClass = nb::class_<PymTypePointer, PymTypeSequence>(m, "PointerType", "PointerType");
  auto TypeVectorClass = nb::class_<PymTypeVector, PymTypeSequence>(m, "VectorType", "VectorType");
  auto TypeVoidClass = nb::class_<PymTypeVoid, PymType>(m, "VoidType", "VoidType");
  auto TypeLabelClass = nb::class_<PymTypeLabel, PymType>(m, "LabelType", "LabelType");
  auto TypeX86MMXClass = nb::class_<PymTypeX86MMX, PymType>(m, "X86MMXType", "X86MMXType");
  auto TypeX86AMXClass = nb::class_<PymTypeX86AMX, PymType>(m, "X86AMXType", "X86AMXType");
  auto TypeTokenClass = nb::class_<PymTypeToken, PymType>(m, "TokenType", "TokenType");
  auto TypeMetadataClass = nb::class_<PymTypeMetadata, PymType>(m, "MetadataType", "MetadataType");
  auto TypeTargetExtClass = nb::class_<PymTypeTargetExt, PymType>(m, "TargetExtType", "TargetExtType");
  
  TypeClass
      .def("__repr__",
           [](PymType &self) {
             auto kind = get_repr_str(LLVMGetTypeKind(self.get()));
             return fmt::format("<Type kind='{}' sig=''>", kind, get_type_str(self));
           })
      .def("__str__", &get_type_str)
      .def_prop_ro("align",
                   [](PymType &t) {
                     return PymConstantExpr(LLVMAlignOf(t.get()));
                   })
      .def_prop_ro("size",
                   [](PymType &t) {
                     return PymConstantExpr(LLVMSizeOf(t.get()));
                   })
      .def_prop_ro("kind",
                   [](PymType &t) { return LLVMGetTypeKind(t.get()); },
                   "Obtain the enumerated type of a Type instance.")
      .def_prop_ro("is_sized",
                   [](PymType &t) { return LLVMTypeIsSized(t.get()) != 0; },
                   "Whether the type has a known size.\n\n"
                   "Things that don't have a size are abstract types, labels, and void.a")
      .def_prop_ro("context",
                   [](PymType &t) { return PymContext(LLVMGetTypeContext(t.get())); },
                   "Obtain the context to which this type instance is associated.")
      .def_prop_ro("sub_type_number",
                   [](PymType &t) {
                     return LLVMGetNumContainedTypes(t.get());
                   })
      .def_prop_ro("sub_types",
                   [](PymType &t) {
                     unsigned num = LLVMGetNumContainedTypes(t.get());
                     std::vector<LLVMTypeRef> arr(num);
                     LLVMGetSubtypes(t.get(), arr.data());
                     WRAP_VECTOR_FROM_DEST(PymTypeSequence, num, res, arr);
                     return res;
                   })
      .def("null",
           [](PymType &t) {
             return PymValueAuto(LLVMConstNull(t.get()));
           },
           "Obtain a constant value referring to the null instance of the type.")
      .def("pointer_null",
           [](PymType &t) {
             return PymValueAuto(LLVMConstPointerNull(t.get()));
           },
           "Obtain a constant that is a constant pointer pointing to NULL for the"
           "type.")
      .def("undef",
           [](PymType &t) {
             return PymUndefValue(LLVMGetUndef(t.get()));
           },
           "Obtain a constant value referring to an undefined value of a type.")
      .def("poison",
           [](PymType &t) {
             return PymPoisonValue(LLVMGetPoison(t.get()));
           },
           "Obtain a constant value referring to a poison value of a type.")
      .def("dump",
           [](PymType &self) {
             return LLVMDumpType(self.get());
           },
           "Dump a representation of a type to stderr.");

  
  TypeIntClass
      .def("__repr__",
           [](PymTypeInt &self) {
             return fmt::format("<IntType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PymTypeInt *t, PymContext &c, unsigned numBits) {
             new (t) PymTypeInt(LLVMIntTypeInContext(c.get(), numBits));
           }, "context"_a, "num_bits"_a)
      .def_static("Int1",
                  [](PymContext &c) { return PymTypeInt(LLVMInt1TypeInContext(c.get())); },
                  "context"_a)
      .def_static("Int8",
                  [](PymContext &c) { return PymTypeInt(LLVMInt8TypeInContext(c.get())); },
                  "context"_a)
      .def_static("Int16",
                  [](PymContext &c) { return PymTypeInt(LLVMInt16TypeInContext(c.get())); },
                  "context"_a)
      .def_static("Int32",
                  [](PymContext &c) { return PymTypeInt(LLVMInt32TypeInContext(c.get())); },
                  "context"_a)
      .def_static("Int64",
                  [](PymContext &c) { return PymTypeInt(LLVMInt64TypeInContext(c.get())); },
                  "context"_a)
      .def_static("Int128",
                  [](PymContext &c) { return PymTypeInt(LLVMInt128TypeInContext(c.get())); },
                  "context"_a)
      .def_prop_ro_static("GlobalInt1",
                          [](nb::handle) { return PymTypeInt(LLVMInt1Type()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalInt8",
                          [](nb::handle) { return PymTypeInt(LLVMInt8Type()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalInt16",
                          [](nb::handle) { return PymTypeInt(LLVMInt16Type()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalInt32",
                          [](nb::handle) { return PymTypeInt(LLVMInt32Type()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalInt64",
                          [](nb::handle) { return PymTypeInt(LLVMInt64Type()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalInt128",
                          [](nb::handle) { return PymTypeInt(LLVMInt128Type()); },
                          "Get type from global context.")
      .def_static("Global",
                  [](unsigned numBits) { return PymTypeInt(LLVMIntType(numBits)); },
                  "Get type from global context.")
      .def_prop_ro("width",
                   [](PymTypeInt &t) { return LLVMGetIntTypeWidth(t.get()); })
      .def("all_ones",
           [](PymTypeInt &t) {
             return PymValueAuto(LLVMConstAllOnes(t.get()));
           },
           "Obtain a constant value referring to the instance of the type"
           "consisting of all ones.");
  

  TypeRealClass
      .def("__repr__",
           [](PymTypeReal &self) {
             return fmt::format("<RealType {}>", get_type_str(self));
           })
      .def_static("Half",
                  [](PymContext &c) { return PymTypeReal(LLVMHalfTypeInContext(c.get())); },
                  "context"_a)
      .def_static("Bfloat",
                  [](PymContext &c) { return PymTypeReal(LLVMBFloatTypeInContext(c.get())); },
                  "context"_a)
      .def_static("Float",
                  [](PymContext &c) { return PymTypeReal(LLVMFloatTypeInContext(c.get())); },
                  "context"_a)
      .def_static("Double",
                  [](PymContext &c) { return PymTypeReal(LLVMDoubleTypeInContext(c.get())); },
                  "context"_a)
      .def_static("X86FP80",
                  [](PymContext &c) { return PymTypeReal(LLVMX86FP80TypeInContext(c.get())); },
                  "context"_a)
      .def_static("FP128",
                  [](PymContext &c) { return PymTypeReal(LLVMFP128TypeInContext(c.get())); },
                  "context"_a)
      .def_static("PPCFP128",
                  [](PymContext &c) { return PymTypeReal(LLVMPPCFP128TypeInContext(c.get())); },
                  "context"_a)
      .def_prop_ro_static("GlobalHalf",
                          [](nb::handle){ return PymTypeReal(LLVMHalfType()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalBfloat",
                          [](nb::handle){ return PymTypeReal(LLVMBFloatType()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalFloat",
                          [](nb::handle){ return PymTypeReal(LLVMFloatType()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalDouble",
                          [](nb::handle){ return PymTypeReal(LLVMDoubleType()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalX86FP80",
                          [](nb::handle){ return PymTypeReal(LLVMX86FP80Type()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalFP128",
                          [](nb::handle){ return PymTypeReal(LLVMFP128Type()); },
                          "Get type from global context.")
      .def_prop_ro_static("GlobalPPCFP128",
                          [](nb::handle){ return PymTypeReal(LLVMPPCFP128Type()); },
                          "Get type from global context.");


  TypeFunctionClass
      .def("__repr__",
           [](PymTypeFunction &self) {
             return fmt::format("<FunctionType {}>", get_type_str(self));
           })
       .def("__init__",
            [](PymTypeFunction *t, PymType &returnType, std::vector<PymType> &paramTypes, bool isVarArg) {
              unsigned param_count = paramTypes.size();
              UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, paramTypes, rawParamTypes, param_count)
              new (t) PymTypeFunction(LLVMFunctionType(returnType.get(), rawParamTypes.data(),
                                                      param_count, isVarArg));
            },
            "return_type"_a, "param_types"_a, "is_var_arg"_a,
            "Obtain a function type consisting of a specified signature.")
      .def_prop_ro("is_vararg",
                   [](PymTypeFunction &t) { return LLVMIsFunctionVarArg(t.get()) != 0; },
                   "Returns whether a function type is variadic.")
      .def_prop_ro("return_type",
                   [](PymTypeFunction &t) { return PymTypeAuto(LLVMGetReturnType(t.get())); },
                   "Obtain the Type this function Type returns.")
      .def_prop_ro("params_num",
                   [](PymTypeFunction &t) { return LLVMCountParamTypes(t.get()); },
                   "Obtain the number of parameters this function accepts.")
      .def_prop_ro("param_types",
                   [](PymTypeFunction &t) {
                     unsigned param_number = LLVMCountParamTypes(t.get());
                     std::vector<LLVMTypeRef> dest(param_number);
                     LLVMGetParamTypes(t.get(), dest.data());
                     WRAP_VECTOR_FROM_DEST(PymTypeFunction, param_number, res, dest);
                     return res;
                   },
                   "Obtain the types of a function's parameters.");
  
  TypeStructClass
      .def("__repr__",
           [](PymTypeStruct &self) {
             return fmt::format("<StructType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PymTypeStruct *t, PymContext &c, std::vector<PymType> &elementTypes, bool packed) {
             unsigned elem_count = elementTypes.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, elementTypes, rawElemTypes, elem_count);
             new (t) PymTypeStruct(LLVMStructTypeInContext(c.get(), rawElemTypes.data(),
                                                          elem_count, packed));
           },
           "context"_a, "element_types"_a, "packed"_a,
           "Create a new structure type in context.")
      .def("__init__",
           [](PymTypeStruct *t, PymContext &c, std::string name) {
             new (t) PymTypeStruct(LLVMStructCreateNamed(c.get(), name.c_str()));
           },
           "context"_a, "name"_a = "",
           "Create an empty structure in the context having a specified name.")
      .def_static("Global",
                  [](std::vector<PymType> elementTypes, bool packed) {
                    unsigned elem_count = elementTypes.size();
                    UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, elementTypes, rawElemTypes, elem_count);
                    return PymTypeStruct(LLVMStructType(rawElemTypes.data(), elem_count, packed));
                  },
                  "element_types"_a, "packed"_a,
                  "Create a new structure type in the global context.")
      .def_prop_ro("name", [](PymTypeStruct &t) { return LLVMGetStructName(t.get()); })
      .def_prop_ro("elem_number",
                   [](PymTypeStruct &t) {
                     return LLVMCountStructElementTypes(t.get());
                   })
      .def_prop_ro("elem_types",
                   [](PymTypeStruct &t) {
                     unsigned num = LLVMCountStructElementTypes(t.get());
                     std::vector<LLVMTypeRef> dest(num);
                     LLVMGetStructElementTypes(t.get(), dest.data());
                     WRAP_VECTOR_FROM_DEST(PymTypeStruct, num, res, dest);
                     return res;
                   })
      .def_prop_ro("is_packed",
                   [](PymTypeStruct &t) { return LLVMIsPackedStruct(t.get()) != 0; },
                   "Determine whether a structure is packed.")
      .def_prop_ro("is_opaque",
                   [](PymTypeStruct &t) { return LLVMIsOpaqueStruct(t.get()) != 0; })
      .def_prop_ro("is_literal",
                   [](PymTypeStruct &t) { return LLVMIsLiteralStruct(t.get()) != 0; },
                   "Determine whether a structure is literal.")
      .def("set_body",
           [](PymTypeStruct &t, std::vector<PymType> elementTypes, bool packed) {
             unsigned elem_count = elementTypes.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, elementTypes, rawElemTypes, elem_count);
             return LLVMStructSetBody(t.get(), rawElemTypes.data(), elem_count, packed);
           },
           "elem_types"_a, "packed"_a,
           "Set the contents of a structure type.")
      .def("get_type_at_index",
           [](PymTypeStruct &t, unsigned i) -> optional<PymType*> {
             auto res = LLVMStructGetTypeAtIndex(t.get(), i);
             WRAP_OPTIONAL_RETURN(res, PymTypeAuto);
           },
           "index"_a,
           "Get the type of the element at a given index in the structure.");

  TypeSequenceClass
      .def("__repr__",
           [](PymTypeSequence &self) {
             return fmt::format("<SequenceType {}>", get_type_str(self));
           })
      .def_prop_ro("element_type", // TODO test pointer type
                   [](PymTypeSequence &t) {
                     return PymTypeAuto(LLVMGetElementType(t.get()));
                   });

  TypeArrayClass
      .def("__repr__",
           [](PymTypeArray &self) {
             return fmt::format("<ArrayType {}>", get_type_str(self));
           })
      // NOTE We use LLVMArrayType2 instead of LLVMArrayType to coordinate
      // with the `length` property
      .def("__init__", 
           [](PymTypeArray *t, PymType &elementType, uint64_t elementCount) {
             new (t) PymTypeArray(LLVMArrayType2(elementType.get(), elementCount));
           },
           "elem_type"_a, "elem_count"_a,
           "Create a fixed size array type that refers to a specific type.\n\n"
           "The created type will exist in the context that its element type"
           "exists in.")
       // LLVMGetArrayLength is deprecated in favor of the API accurate
       //  LLVMGetArrayLength2
      .def_prop_ro("length",
                   [](PymTypeArray &t) {
                     return LLVMGetArrayLength2(t.get());
                   });


  TypePointerClass
      .def("__repr__",
           [](PymTypePointer &self) {
             return fmt::format("<PointerType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PymTypePointer *t, PymContext &c, unsigned AddressSpace) {
             new (t) PymTypePointer(LLVMPointerTypeInContext(c.get(), AddressSpace));
           },
           "context"_a, "address_space"_a,
           "Create an opaque pointer type in a context.")
      .def("__init__",
           [](PymTypePointer *t, PymType &ElementType, unsigned AddressSpace) {
             new (t) PymTypePointer(LLVMPointerType(ElementType.get(), AddressSpace));
           },
           "elem_type"_a, "address_space"_a,
           "Create a pointer type that points to a defined type.\n\n"
           "The created type will exist in the context that its pointee type"
           "exists in.")
      .def_prop_ro("is_opaque",
                   [](PymTypePointer &t) { return LLVMPointerTypeIsOpaque(t.get()); })
      .def_prop_ro("address_space",
                   [](PymTypePointer &t) { return LLVMGetPointerAddressSpace(t.get()); });

  TypeVectorClass
      .def("__repr__",
           [](PymTypeVector &self) {
             return fmt::format("<VectorType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PymTypeVector *t, PymType &ElementType, unsigned ElementCount, bool IsScalable) {
             if (IsScalable) {
               new (t) PymTypeVector(LLVMScalableVectorType(ElementType.get(), ElementCount));
             } else {
               new (t) PymTypeVector(LLVMVectorType(ElementType.get(), ElementCount));
             }
           },
           "elem_type"_a, "elem_count"_a, "is_scalable"_a,
           "The created type will exist in the context thats its element type"
           "exists in.")
      .def_prop_ro_static("__len__",
                          [](PymTypeVector &t) { return LLVMGetVectorSize(t.get()); });

  
  TypeVoidClass
      .def("__repr__",
           [](PymTypeVoid &self) {
             return fmt::format("<VoidType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PymTypeVoid *t, PymContext &c) {
             new (t) PymTypeVoid(LLVMVoidTypeInContext(c.get()));
           },
           "context"_a)
      .def_prop_ro_static("Global",
                          [](nb::handle) { return PymTypeVoid(LLVMVoidType()); } );

  
  TypeLabelClass
      .def("__repr__",
           [](PymTypeLabel &self) {
             return fmt::format("<LabelType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PymTypeLabel *t, PymContext &c) {
             new (t) PymTypeLabel(LLVMLabelTypeInContext(c.get()));
           },
           "context"_a)
      .def_prop_ro_static("Global",
                          [](nb::handle) { return PymTypeLabel(LLVMLabelType()); } );


  TypeX86MMXClass
      .def("__repr__",
           [](PymTypeX86MMX &self) {
             return fmt::format("<X86MMXType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PymTypeX86MMX *t, PymContext &c) {
             new (t) PymTypeX86MMX(LLVMX86MMXTypeInContext(c.get()));
           },
           "context"_a)
      .def_prop_ro_static("Global",
                          [](nb::handle) { return PymTypeX86MMX(LLVMX86MMXType()); } );


  TypeX86AMXClass
      .def("__repr__",
           [](PymTypeX86AMX &self) {
             return fmt::format("<X86AMXType {}>", get_type_str(self));
           })
       .def("__init__",
            [](PymTypeX86AMX *t, PymContext &c) {
              new (t) PymTypeX86AMX(LLVMX86AMXTypeInContext(c.get()));
            },
            "context"_a)
       .def_prop_ro_static("Global",
                           [](nb::handle) { return PymTypeX86AMX(LLVMX86AMXType()); } );


  TypeTokenClass
      .def("__repr__",
           [](PymTypeToken &self) {
             return fmt::format("<TokenType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PymTypeToken *t, PymContext &c) {
             new (t) PymTypeToken(LLVMTokenTypeInContext(c.get()));
           },
           "context"_a);

  TypeMetadataClass
      .def("__repr__",
           [](PymTypeMetadata &self) {
             return fmt::format("<MetadataType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PymTypeMetadata *t, PymContext &c) {
             new (t) PymTypeMetadata(LLVMMetadataTypeInContext(c.get()));
           },
           "context"_a);

  TypeTargetExtClass
      .def("__repr__",
           [](PymTypeTargetExt &self) {
             return fmt::format("<TargetType {}>", get_type_str(self));
           })
      .def("__init__",
           [](PymTypeVoid *t, PymContext &c, std::string &name, std::vector<PymType> typeParams,
              std::vector<unsigned> intParams) {
             unsigned typeParamSize = typeParams.size();
             unsigned intParamSize = intParams.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, typeParams, rawTypeParams, typeParamSize);
             new (t) PymTypeVoid(LLVMTargetExtTypeInContext
                                  (c.get(), name.c_str(), rawTypeParams.data(),
                                   typeParamSize, intParams.data(), intParamSize));
           });
}
