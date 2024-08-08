#include "Target.h"

#include <llvm-c/Target.h>
#include <nanobind/nanobind.h>
#include "types_priv.h"
#include "utils_priv.h"

namespace nb = nanobind;
using namespace nb::literals;

void populateTarget(nb::module_ &m) {
  auto TargetDataClass = nb::class_<PymTargetData,
                                  PymLLVMObject<PymTargetData, LLVMTargetDataRef>>
                           (m, "TargetData", "TargetData");

  auto TargetLibraryInfoClass =
    nb::class_<PymTargetLibraryInfo,
               PymLLVMObject<PymTargetLibraryInfo, LLVMTargetLibraryInfoRef>>
      (m, "TargetData", "TargetData");


  nb::enum_<LLVMByteOrdering>(m, "ByteOrdering", "ByteOrdering")
      .value("BigEndian", LLVMByteOrdering::LLVMBigEndian)
      .value("LittleEndian", LLVMByteOrdering::LLVMLittleEndian);


  PYM_TARGET_FOR_EACH_INIT_TARGET_INFO();
  PYM_TARGET_FOR_EACH_INIT_TARGET();
  PYM_TARGET_FOR_EACH_INIT_TARGET_MC();
  PYM_ASM_PRINTER_FOR_EACH_INIT_ASM_PRINTER();
  PYM_TARGET_FOR_EACH_INIT_ASM_PARSER();
  PYM_DISASSEMBLER_FOR_EACH_INIT_DISASSEMBLER();

  m.def("init_all_target_infos", &LLVMInitializeAllTargetInfos);
  m.def("init_all_targets", &LLVMInitializeAllTargets);
  m.def("init_all_target_m_cs", &LLVMInitializeAllTargetMCs);
  m.def("init_all_asm_printers", &LLVMInitializeAllAsmPrinters);
  m.def("init_all_asm_parsers", &LLVMInitializeAllAsmParsers);
  m.def("init_all_disassemblers", &LLVMInitializeAllDisassemblers);
  
  m.def("init_native_target",
        []() {
          return LLVMInitializeNativeTarget() != 0; 
        },
        "The main program should call this function to"
        "initialize the native target corresponding to the host.  This is useful"
        "for JIT applications to ensure that the target gets linked in correctly.");

  m.def("init_native_asm_parser",
        []() {
          return LLVMInitializeNativeAsmParser() != 0;
        });

  m.def("init_native_asm_printer",
        []() {
          return LLVMInitializeNativeAsmPrinter() != 0;
        });

  m.def("init_native_disassembler",
        []() {
          return LLVMInitializeNativeDisassembler() != 0;
        });

  TargetDataClass
      .def("__str__",
           [](PymTargetData &self) {
             auto res = LLVMCopyStringRepOfTargetData(self.get());
             RETURN_MESSAGE(res);
           })
      .def_prop_ro("byte_order",
                   [](PymTargetData &self) {
                     return LLVMByteOrder(self.get());
                   })
      .def_prop_ro("ptr_size",
                   [](PymTargetData &self) {
                     return LLVMPointerSize(self.get());
                   })
      .def_prop_ro("int_ptr",
                   [](PymTargetData &self) {
                     return PymTypePointer(LLVMIntPtrType(self.get()));
                   },
                   "Returns the integer type that is the same size as a pointer "
                   "on a target.")
      .def("__init__",
           [](PymTargetData *td, const char *stringRep) {
             new (td) PymTargetData(LLVMCreateTargetData(stringRep));
           },
           "string_rep"_a)
      .def("get_ptr_size_for_addr_space",
           [](PymTargetData &self, unsigned AS) {
             return LLVMPointerSizeForAS(self.get(), AS);
           },
           "as"_a)
      .def("get_int_ptr_for_addr_space",
           [](PymTargetData &self, unsigned AS) {
             return PymTypePointer(LLVMIntPtrTypeForAS(self.get(), AS));
           },
           "as"_a)
      .def("get_int_ptr_for_addr_space_in_context",
           [](PymTargetData &self, PymContext &context, unsigned AS) {
             auto res = LLVMIntPtrTypeForASInContext(context.get(), self.get(), AS);
             return PymTypePointer(res);
           },
           "context"_a, "as"_a)
      .def("size_of_type_in_bits",
           [](PymTargetData &self, PymType &t) {
             return LLVMSizeOfTypeInBits(self.get(), t.get());
           },
           "type"_a,
           "Computes the size of a type in bytes for a target.")
      .def("store_size_of_type",
           [](PymTargetData &self, PymType &t) {
             return LLVMStoreSizeOfType(self.get(), t.get());
           },
           "type"_a,
           "Computes the storage size of a type in bytes for a target.")
      .def("abi_size_of_type",
           [](PymTargetData &self, PymType &t) {
             return LLVMABISizeOfType(self.get(), t.get());
           },
           "type"_a,
           "Computes the ABI size of a type in bytes for a target.")
      .def("abi_alignment_of_type",
           [](PymTargetData &self, PymType &t) {
             return LLVMABIAlignmentOfType(self.get(), t.get());
           },
           "type"_a,
           "Computes the ABI alignment of a type in bytes for a target.")
      .def("call_frame_alignment_of_type",
           [](PymTargetData &self, PymType &t) {
             return LLVMCallFrameAlignmentOfType(self.get(), t.get());
           },
           "type"_a,
           "Computes the call frame alignment of a type in bytes for a target.")
      .def("preferred_alignment_of_type",
           [](PymTargetData &self, PymType &t) {
             return LLVMPreferredAlignmentOfType(self.get(), t.get());
           },
           "type"_a,
           "Computes the preferred alignment of a type in bytes for a target.")
      .def("preferred_alignment_of_global",
           [](PymTargetData &self, PymGlobalVariable &gv) {
             return LLVMPreferredAlignmentOfGlobal(self.get(), gv.get());
           },
           "gv"_a,
           "Computes the preferred alignment of a global variable in bytes for a target.")
      .def("element_at_offset",
           [](PymTargetData &self, PymTypeStruct &st, unsigned long long offset) {
             return LLVMElementAtOffset(self.get(), st.get(), offset);
           },
           "st"_a, "offset"_a,
           "Computes the structure element that contains the byte offset for a target.")
      .def("offset_of_element",
           [](PymTargetData &self, PymTypeStruct &st, unsigned element) {
             return LLVMOffsetOfElement(self.get(), st.get(), element);
           },
           "st"_a, "element"_a,
           "Computes the byte offset of the indexed struct element for a target.");
  
}

