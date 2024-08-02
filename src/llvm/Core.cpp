#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <optional>
#include <exception>
#include <llvm-c/Core.h>
#include <llvm-c/IRReader.h>
#include <fmt/core.h>
#include "Core.h"
#include "_types.h"
#include "_utils.h"
#include <iostream>

namespace nb = nanobind;
using namespace nb::literals;

template <typename T>
using optional = std::optional<T>;


/**
 * Get a python side repr string for obj
 */
template <typename T>
inline const char *get_repr_str(T &&obj) {
  return nb::repr(nb::cast(obj)).c_str();
}

PyMetadataAsValue* getMoreSpcMetadataAsValue(LLVMValueRef raw) {
  if (auto v = LLVMIsAMDNode(raw)) {
    return new PyMDNodeValue(v);
  } else if (auto v = LLVMIsAMDString(raw)) {
    return new PyMDStringValue(v);
  } 
  return new PyMetadataAsValue(raw);
}

inline std::string get_type_str(PyType &t) {
  char *str = LLVMPrintTypeToString(t.get());
  std::string res(str);
  LLVMDisposeMessage(str);
  return res;
}

inline std::string get_value_str(LLVMValueRef v) {
  auto rawStr = LLVMPrintValueToString(v);
  std::string str(rawStr);
  LLVMDisposeMessage(rawStr);
  return str;
}

inline std::string get_value_name(LLVMValueRef v) {
  size_t len;
  const char *str = LLVMGetValueName2(v, &len);
  return std::string(str, len);
}

inline std::string gen_value_repr(const char *typeName, PyValue &v) {
  LLVMValueRef raw = v.get();
  auto name = get_value_name(raw);
  return fmt::format("<{} name='{}'>", typeName, name);
}


/**
 * NOTE only mapping needed specific instruction sub-classes
 * add the order here is according to `PY_FOR_EACH_VALUE_CLASS_RELATIONSHIP` macro
 * in Types.h
 * 
 * @param Inst: should be of LLVMInstructionValueKind
 */
PyInstruction* PyInstructionAuto(LLVMValueRef inst) {
  LLVMOpcode opcode = LLVMGetInstructionOpcode(inst);
  switch (opcode) {
  case LLVMCall:
    return new PyCallInst(inst);
  case LLVMInvoke:
    return new PyInvokeInst(inst);

  case LLVMFCmp:
    return new PyFCmpInst(inst);
  case LLVMICmp:
    return new PyICmpInst(inst);
  case LLVMGetElementPtr:
    return new PyGetElementPtrInst(inst);
  case LLVMPHI:
    return new PyPHINode(inst);
  case LLVMShuffleVector:
    return new PyShuffleVectorInst(inst);
  case LLVMRet:
    return new PyReturnInst(inst);
  case LLVMSwitch:
    return new PySwitchInst(inst);
  case LLVMCatchSwitch:
    return new PyCatchSwitchInst(inst);
  case LLVMCleanupRet:
    return new PyCleanupReturnInst(inst);
  // no kind corresponding to PyFuncletPadInst
  case LLVMCatchPad:
    return new PyCatchPadInst(inst);
  case LLVMAlloca:
    return new PyAllocaInst(inst);
  case LLVMInsertValue:
    return new PyInsertValueInst(inst);
  case LLVMExtractValue:
    return new PyExtractValueInst(inst);
  case LLVMBr:
    return new PyBranchInst(inst);
  case LLVMIndirectBr:
    return new PyIndirectBrInst(inst);
  case LLVMLandingPad:
    return new PyLandingPadInst(inst);
  case LLVMLoad:
    return new PyLoadInst(inst);
  case LLVMStore:
    return new PyStoreInst(inst);
  case LLVMAtomicRMW:
    return new PyAtomicRMWInst(inst);
  case LLVMAtomicCmpXchg:
    return new PyAtomicCmpXchgInst(inst);
  case LLVMFence:
    return new PyFenceInst(inst);

  default:
    return new PyInstruction(inst);
  }
}



PyType* PyTypeAuto(LLVMTypeRef rawType) {
  LLVMTypeKind kind = LLVMGetTypeKind(rawType);
  switch (kind) {
  case LLVMVoidTypeKind:
    return new PyTypeVoid(rawType);
  case LLVMHalfTypeKind:
  case LLVMFloatTypeKind:
  case LLVMDoubleTypeKind:
  case LLVMX86_FP80TypeKind:
  case LLVMFP128TypeKind:
  case LLVMPPC_FP128TypeKind:
  case LLVMBFloatTypeKind:
    return new PyTypeReal(rawType);
  case LLVMLabelTypeKind:
    return new PyTypeLabel(rawType);
  case LLVMIntegerTypeKind:
    return new PyTypeInt(rawType);
  case LLVMFunctionTypeKind:
    return new PyTypeFunction(rawType);
  case LLVMStructTypeKind:
    return new PyTypeStruct(rawType);
  case LLVMArrayTypeKind:
    return new PyTypeArray(rawType);
  case LLVMPointerTypeKind:
    return new PyTypePointer(rawType);
  case LLVMVectorTypeKind:
    return new PyTypeVector(rawType);
  case LLVMMetadataTypeKind:
    return new PyTypeMetadata(rawType);
  case LLVMX86_MMXTypeKind:
    return new PyTypeX86MMX(rawType);
  case LLVMTokenTypeKind:
    return new PyTypeToken(rawType);
  case LLVMScalableVectorTypeKind:
    return new PyTypeVector(rawType);
  case LLVMX86_AMXTypeKind:
    return new PyTypeX86AMX(rawType);
  case LLVMTargetExtTypeKind:
    return new PyTypeTargetExt(rawType);
  default:
    return new PyType(rawType);
  }
}

/**
 * It seems like the enum type doesn't cover all the sub-classes,
 * so user may still need to do a manual cast using `to_XXX` method
 */
PyValue* PyValueAuto(LLVMValueRef rawValue) {
  LLVMValueKind kind = LLVMGetValueKind(rawValue);
  switch (kind) {
  case LLVMArgumentValueKind:
    return new PyArgument(rawValue);
  case LLVMBasicBlockValueKind:
    return new PyBasicBlockValue(rawValue);
  // case LLVMMemoryUseValueKind:
  // case LLVMMemoryDefValueKind:
  // case LLVMMemoryPhiValueKind:

  case LLVMFunctionValueKind:
    return new PyFunction(rawValue);
  case LLVMGlobalAliasValueKind:
    return new PyGlobalAlias(rawValue);
  case LLVMGlobalIFuncValueKind:
    return new PyGlobalIFunc(rawValue);
  case LLVMGlobalVariableValueKind:
    return new PyGlobalVariable(rawValue);
  // case LLVMBlockAddressValueKind:
  case LLVMConstantExprValueKind:
    return new PyConstantExpr(rawValue);
  case LLVMConstantArrayValueKind:
    return new PyConstantArray(rawValue);
  case LLVMConstantStructValueKind:
    return new PyConstantStruct(rawValue);
  case LLVMConstantVectorValueKind:
    return new PyConstantVector(rawValue);

  case LLVMUndefValueValueKind:
    return new PyUndefValue(rawValue);
  case LLVMConstantDataArrayValueKind:
    return new PyConstantDataArray(rawValue);
  case LLVMConstantDataVectorValueKind:
    return new PyConstantDataVector(rawValue);
  case LLVMConstantIntValueKind:
    return new PyConstantInt(rawValue);
  case LLVMConstantFPValueKind:
    return new PyConstantFP(rawValue);
  case LLVMConstantTokenNoneValueKind:
  case LLVMConstantAggregateZeroValueKind:
  case LLVMConstantPointerNullValueKind:
    return new PyConstant(rawValue);

  case LLVMMetadataAsValueValueKind:
    return getMoreSpcMetadataAsValue(rawValue);
  case LLVMInlineAsmValueKind:
    return new PyInlineAsm(rawValue);

  case LLVMInstructionValueKind:
    return PyInstructionAuto(rawValue);
  case LLVMPoisonValueValueKind:
    return new PyPoisonValue(rawValue);
  // case LLVMConstantTargetNoneValueKind: // 
  default:
    return new PyValue(rawValue);
  }
}

PyAttribute* PyAttributeAuto(LLVMAttributeRef rawValue) {
  if (LLVMIsEnumAttribute(rawValue)) {
    return new PyEnumAttribute(rawValue);
  } else if (LLVMIsStringAttribute(rawValue)) {
    return new PyStringAttribute(rawValue);
  } else if (LLVMIsTypeAttribute(rawValue)) {
    return new PyTypeAttribute(rawValue);
  } else {
    return new PyAttribute(rawValue);
  }
}


void bindEnums(nb::module_ &m) {
  nb::enum_<LLVMOpcode>(m, "Opcode", "Opcode")
      .value("Ret", LLVMOpcode::LLVMRet)
      .value("Br", LLVMOpcode::LLVMBr)
      .value("Switch", LLVMOpcode::LLVMSwitch)
      .value("IndirectBr", LLVMOpcode::LLVMIndirectBr)
      .value("Invoke", LLVMOpcode::LLVMInvoke)
      .value("Unreachable", LLVMOpcode::LLVMUnreachable)
      .value("CallBr", LLVMOpcode::LLVMCallBr)
      .value("FNeg", LLVMOpcode::LLVMFNeg)
      .value("Add", LLVMOpcode::LLVMAdd)
      .value("FAdd", LLVMOpcode::LLVMFAdd)
      .value("Sub", LLVMOpcode::LLVMSub)
      .value("FSub", LLVMOpcode::LLVMFSub)
      .value("Mul", LLVMOpcode::LLVMMul)
      .value("FMul", LLVMOpcode::LLVMFMul)
      .value("UDiv", LLVMOpcode::LLVMUDiv)
      .value("SDiv", LLVMOpcode::LLVMSDiv)
      .value("FDiv", LLVMOpcode::LLVMFDiv)
      .value("URem", LLVMOpcode::LLVMURem)
      .value("SRem", LLVMOpcode::LLVMSRem)
      .value("FRem", LLVMOpcode::LLVMFRem)
      .value("Shl", LLVMOpcode::LLVMShl)
      .value("LShr", LLVMOpcode::LLVMLShr)
      .value("AShr", LLVMOpcode::LLVMAShr)
      .value("And", LLVMOpcode::LLVMAnd)
      .value("Or", LLVMOpcode::LLVMOr)
      .value("Xor", LLVMOpcode::LLVMXor)
      .value("Alloca", LLVMOpcode::LLVMAlloca)
      .value("Load", LLVMOpcode::LLVMLoad)
      .value("Store", LLVMOpcode::LLVMStore)
      .value("GetElementPtr", LLVMOpcode::LLVMGetElementPtr)
      .value("Trunc", LLVMOpcode::LLVMTrunc)
      .value("ZExt", LLVMOpcode::LLVMZExt)
      .value("SExt", LLVMOpcode::LLVMSExt)
      .value("FPToUI", LLVMOpcode::LLVMFPToUI)
      .value("FPToSI", LLVMOpcode::LLVMFPToSI)
      .value("UIToFP", LLVMOpcode::LLVMUIToFP)
      .value("SIToFP", LLVMOpcode::LLVMSIToFP)
      .value("FPTrunc", LLVMOpcode::LLVMFPTrunc)
      .value("FPExt", LLVMOpcode::LLVMFPExt)
      .value("PtrToInt", LLVMOpcode::LLVMPtrToInt)
      .value("IntToPtr", LLVMOpcode::LLVMIntToPtr)
      .value("BitCast", LLVMOpcode::LLVMBitCast)
      .value("AddrSpaceCast", LLVMOpcode::LLVMAddrSpaceCast)
      .value("ICmp", LLVMOpcode::LLVMICmp)
      .value("FCmp", LLVMOpcode::LLVMFCmp)
      .value("PHI", LLVMOpcode::LLVMPHI)
      .value("Call", LLVMOpcode::LLVMCall)
      .value("Select", LLVMOpcode::LLVMSelect)
      .value("UserOp1", LLVMOpcode::LLVMUserOp1)
      .value("UserOp2", LLVMOpcode::LLVMUserOp2)
      .value("VAArg", LLVMOpcode::LLVMVAArg)
      .value("ExtractElement", LLVMOpcode::LLVMExtractElement)
      .value("InsertElement", LLVMOpcode::LLVMInsertElement)
      .value("ShuffleVector", LLVMOpcode::LLVMShuffleVector)
      .value("ExtractValue", LLVMOpcode::LLVMExtractValue)
      .value("InsertValue", LLVMOpcode::LLVMInsertValue)
      .value("Freeze", LLVMOpcode::LLVMFreeze)
      .value("Fence", LLVMOpcode::LLVMFence)
      .value("AtomicCmpXchg", LLVMOpcode::LLVMAtomicCmpXchg)
      .value("AtomicRMW", LLVMOpcode::LLVMAtomicRMW)
      .value("Resume", LLVMOpcode::LLVMResume)
      .value("LandingPad", LLVMOpcode::LLVMLandingPad)
      .value("CleanupRet", LLVMOpcode::LLVMCleanupRet)
      .value("CatchRet", LLVMOpcode::LLVMCatchRet)
      .value("CatchPad", LLVMOpcode::LLVMCatchPad)
      .value("CleanupPad", LLVMOpcode::LLVMCleanupPad)
      .value("CatchSwitch", LLVMOpcode::LLVMCatchSwitch);
  

  nb::enum_<LLVMTypeKind>(m, "TypeKind", "TypeKind")
      .value("Void", LLVMTypeKind::LLVMVoidTypeKind, "type with no size")
      .value("Half", LLVMTypeKind::LLVMHalfTypeKind, "16 bit floating point type")
      .value("Float", LLVMTypeKind::LLVMFloatTypeKind, "32 bit floating point type")
      .value("Double", LLVMTypeKind::LLVMDoubleTypeKind, "64 bit floating point type")
      .value("X86_FP80", LLVMTypeKind::LLVMX86_FP80TypeKind, "80 bit floating point type (X87)")
      .value("FP128", LLVMTypeKind::LLVMFP128TypeKind, "128 bit floating point type (112-bit mantissa)")
      .value("PPC_FP128", LLVMTypeKind::LLVMPPC_FP128TypeKind, "128 bit floating point type (two 64-bits)")
      .value("Label", LLVMTypeKind::LLVMLabelTypeKind, "Labels")
      .value("Integer", LLVMTypeKind::LLVMIntegerTypeKind, "Arbitrary bit width integers")
      .value("Function", LLVMTypeKind::LLVMFunctionTypeKind, "Functions")
      .value("Struct", LLVMTypeKind::LLVMStructTypeKind, "Structures")
      .value("Array", LLVMTypeKind::LLVMArrayTypeKind, "Arrays")
      .value("Pointer", LLVMTypeKind::LLVMPointerTypeKind, "Pointers")
      .value("Vector", LLVMTypeKind::LLVMVectorTypeKind, "Fixed width SIMD vector type")
      .value("Metadata", LLVMTypeKind::LLVMMetadataTypeKind, "Metadata")
      .value("X86_MMX", LLVMTypeKind::LLVMX86_MMXTypeKind, "X86 MMX")
      .value("Token", LLVMTypeKind::LLVMTokenTypeKind, "Tokens")
      .value("ScalableVector", LLVMTypeKind::LLVMScalableVectorTypeKind, "Scalable SIMD vector type")
      .value("BFloat", LLVMTypeKind::LLVMBFloatTypeKind, "16 bit brain floating point type")
      .value("X86_AMX", LLVMTypeKind::LLVMX86_AMXTypeKind, "X86 AMX")
      .value("TargetExt", LLVMTypeKind::LLVMTargetExtTypeKind, "Target extension type");
  

  nb::enum_<LLVMLinkage>(m, "Linkage", "Linkage")
      .value("External", LLVMLinkage::LLVMExternalLinkage,
             "Externally visible function")
      .value("AvailableExternally", LLVMLinkage::LLVMAvailableExternallyLinkage)
      .value("LinkOnceAny", LLVMLinkage::LLVMLinkOnceAnyLinkage,
             "Keep one copy of function when linking (inline)")
      .value("LinkOnceODR", LLVMLinkage::LLVMLinkOnceODRLinkage,
             "Keep one copy of function when linking (inline), but only replaced"
             " by something equivalent.")
      .value("LinkOnceODRAutoHide", LLVMLinkage::LLVMLinkOnceODRAutoHideLinkage, "Obsolete")
      .value("WeakAny", LLVMLinkage::LLVMWeakAnyLinkage,
             "Keep one copy of function when linking (weak)")
      .value("WeakODR", LLVMLinkage::LLVMWeakODRLinkage,
             "Same, but only replaced by something equivalent.")
      .value("Appending", LLVMLinkage::LLVMAppendingLinkage,
             "Special purpose, only applies to global arrays")
      .value("Internal", LLVMLinkage::LLVMInternalLinkage,
             "Rename collisions when linking (static functions)")
      .value("Private", LLVMLinkage::LLVMPrivateLinkage,
             "Like Internal, but omit from symbol table")
      .value("DLLImport", LLVMLinkage::LLVMDLLImportLinkage, "Obsolete")
      .value("DLLExport", LLVMLinkage::LLVMDLLExportLinkage, "Obsolete")
      .value("ExternalWeak", LLVMLinkage::LLVMExternalWeakLinkage,
             "ExternalWeak linkage description")
      .value("Ghost", LLVMLinkage::LLVMGhostLinkage, "Obsolete")
      .value("Common", LLVMLinkage::LLVMCommonLinkage, "Tentative definitions")
      .value("LinkerPrivate", LLVMLinkage::LLVMLinkerPrivateLinkage,
             "Like Private, but linker removes.")
      .value("LinkerPrivateWeak", LLVMLinkage::LLVMLinkerPrivateWeakLinkage,
             "Like LinkerPrivate, but is weak.");
  

  nb::enum_<LLVMVisibility>(m, "Visibility", "Visibility")
      .value("Default", LLVMVisibility::LLVMDefaultVisibility, "The GV is visible")
      .value("Hidden", LLVMVisibility::LLVMHiddenVisibility, "The GV is hidden")
      .value("Protected", LLVMVisibility::LLVMProtectedVisibility, "The GV is protected");
  

  nb::enum_<LLVMUnnamedAddr>(m, "UnnamedAddr", "UnnamedAddr")
      .value("No", LLVMUnnamedAddr::LLVMNoUnnamedAddr, "Address of the GV is significant.")
      .value("Local", LLVMUnnamedAddr::LLVMLocalUnnamedAddr, "Address of the GV is locally insignificant.")
      .value("Global", LLVMUnnamedAddr::LLVMGlobalUnnamedAddr, "Address of the GV is globally insignificant.");
  

  nb::enum_<LLVMDLLStorageClass>(m, "DLLStorageClass", "DLLStorageClass")
      .value("Default", LLVMDLLStorageClass::LLVMDefaultStorageClass)
      .value("DLLImport", LLVMDLLStorageClass::LLVMDLLImportStorageClass, "Function to be imported from DLL.")
      .value("DLLExport", LLVMDLLStorageClass::LLVMDLLExportStorageClass, "Function to be accessible from DLL.");
  

  nb::enum_<LLVMCallConv>(m, "CallConv", "CallConv")
      .value("C", LLVMCallConv::LLVMCCallConv)
      .value("Fast", LLVMCallConv::LLVMFastCallConv)
      .value("Cold", LLVMCallConv::LLVMColdCallConv)
      .value("GHC", LLVMCallConv::LLVMGHCCallConv)
      .value("HiPE", LLVMCallConv::LLVMHiPECallConv)
      .value("AnyReg", LLVMCallConv::LLVMAnyRegCallConv)
      .value("PreserveMost", LLVMCallConv::LLVMPreserveMostCallConv)
      .value("PreserveAll", LLVMCallConv::LLVMPreserveAllCallConv)
      .value("Swift", LLVMCallConv::LLVMSwiftCallConv)
      .value("CXXFASTTLS", LLVMCallConv::LLVMCXXFASTTLSCallConv)
      .value("X86Stdcall", LLVMCallConv::LLVMX86StdcallCallConv)
      .value("X86Fastcall", LLVMCallConv::LLVMX86FastcallCallConv)
      .value("ARMAPCS", LLVMCallConv::LLVMARMAPCSCallConv)
      .value("ARMAAPCS", LLVMCallConv::LLVMARMAAPCSCallConv)
      .value("ARMAAPCSVFP", LLVMCallConv::LLVMARMAAPCSVFPCallConv)
      .value("MSP430INTR", LLVMCallConv::LLVMMSP430INTRCallConv)
      .value("X86ThisCall", LLVMCallConv::LLVMX86ThisCallCallConv)
      .value("PTXKernel", LLVMCallConv::LLVMPTXKernelCallConv)
      .value("PTXDevice", LLVMCallConv::LLVMPTXDeviceCallConv)
      .value("SPIRFUNC", LLVMCallConv::LLVMSPIRFUNCCallConv)
      .value("SPIRKERNEL", LLVMCallConv::LLVMSPIRKERNELCallConv)
      .value("IntelOCLBI", LLVMCallConv::LLVMIntelOCLBICallConv)
      .value("X8664SysV", LLVMCallConv::LLVMX8664SysVCallConv)
      .value("Win64", LLVMCallConv::LLVMWin64CallConv)
      .value("X86VectorCall", LLVMCallConv::LLVMX86VectorCallCallConv)
      .value("HHVM", LLVMCallConv::LLVMHHVMCallConv)
      .value("HHVMC", LLVMCallConv::LLVMHHVMCCallConv)
      .value("X86INTR", LLVMCallConv::LLVMX86INTRCallConv)
      .value("AVRINTR", LLVMCallConv::LLVMAVRINTRCallConv)
      .value("AVRSIGNAL", LLVMCallConv::LLVMAVRSIGNALCallConv)
      .value("AVRBUILTIN", LLVMCallConv::LLVMAVRBUILTINCallConv)
      .value("AMDGPUVS", LLVMCallConv::LLVMAMDGPUVSCallConv)
      .value("AMDGPUGS", LLVMCallConv::LLVMAMDGPUGSCallConv)
      .value("AMDGPUPS", LLVMCallConv::LLVMAMDGPUPSCallConv)
      .value("AMDGPUCS", LLVMCallConv::LLVMAMDGPUCSCallConv)
      .value("AMDGPUKERNEL", LLVMCallConv::LLVMAMDGPUKERNELCallConv)
      .value("X86RegCall", LLVMCallConv::LLVMX86RegCallCallConv)
      .value("AMDGPUHS", LLVMCallConv::LLVMAMDGPUHSCallConv)
      .value("MSP430BUILTIN", LLVMCallConv::LLVMMSP430BUILTINCallConv)
      .value("AMDGPULS", LLVMCallConv::LLVMAMDGPULSCallConv)
      .value("AMDGPUES", LLVMCallConv::LLVMAMDGPUESCallConv);
  

  nb::enum_<LLVMValueKind>(m, "ValueKind", "ValueKind")
      .value("Argument", LLVMValueKind::LLVMArgumentValueKind)
      .value("BasicBlock", LLVMValueKind::LLVMBasicBlockValueKind)
      .value("MemoryUse", LLVMValueKind::LLVMMemoryUseValueKind)
      .value("MemoryDef", LLVMValueKind::LLVMMemoryDefValueKind)
      .value("MemoryPhi", LLVMValueKind::LLVMMemoryPhiValueKind)
      .value("Function", LLVMValueKind::LLVMFunctionValueKind)
      .value("GlobalAlias", LLVMValueKind::LLVMGlobalAliasValueKind)
      .value("GlobalIFunc", LLVMValueKind::LLVMGlobalIFuncValueKind)
      .value("GlobalVariable", LLVMValueKind::LLVMGlobalVariableValueKind)
      .value("BlockAddress", LLVMValueKind::LLVMBlockAddressValueKind)
      .value("ConstantExpr", LLVMValueKind::LLVMConstantExprValueKind)
      .value("ConstantArray", LLVMValueKind::LLVMConstantArrayValueKind)
      .value("ConstantStruct", LLVMValueKind::LLVMConstantStructValueKind)
      .value("ConstantVector", LLVMValueKind::LLVMConstantVectorValueKind)
      .value("UndefValue", LLVMValueKind::LLVMUndefValueValueKind)
      .value("ConstantAggregateZero", LLVMValueKind::LLVMConstantAggregateZeroValueKind)
      .value("ConstantDataArray", LLVMValueKind::LLVMConstantDataArrayValueKind)
      .value("ConstantDataVector", LLVMValueKind::LLVMConstantDataVectorValueKind)
      .value("ConstantInt", LLVMValueKind::LLVMConstantIntValueKind)
      .value("ConstantFP", LLVMValueKind::LLVMConstantFPValueKind)
      .value("ConstantPointerNull", LLVMValueKind::LLVMConstantPointerNullValueKind)
      .value("ConstantTokenNone", LLVMValueKind::LLVMConstantTokenNoneValueKind)
      .value("MetadataAsValue", LLVMValueKind::LLVMMetadataAsValueValueKind)
      .value("InlineAsm", LLVMValueKind::LLVMInlineAsmValueKind)
      .value("Instruction", LLVMValueKind::LLVMInstructionValueKind)
      .value("PoisonValue", LLVMValueKind::LLVMPoisonValueValueKind)
      .value("ConstantTargetNone", LLVMValueKind::LLVMConstantTargetNoneValueKind);
  

  nb::enum_<LLVMIntPredicate>(m, "IntPredicate", "IntPredicate")
      .value("EQ", LLVMIntPredicate::LLVMIntEQ, "equal")
      .value("NE", LLVMIntPredicate::LLVMIntNE, "not equal")
      .value("UGT", LLVMIntPredicate::LLVMIntUGT, "unsigned greater than")
      .value("UGE", LLVMIntPredicate::LLVMIntUGE, "unsigned greater or equal")
      .value("ULT", LLVMIntPredicate::LLVMIntULT, "unsigned less than")
      .value("ULE", LLVMIntPredicate::LLVMIntULE, "unsigned less or equal")
      .value("SGT", LLVMIntPredicate::LLVMIntSGT, "signed greater than")
      .value("SGE", LLVMIntPredicate::LLVMIntSGE, "signed greater or equal")
      .value("SLT", LLVMIntPredicate::LLVMIntSLT, "signed less than")
      .value("SLE", LLVMIntPredicate::LLVMIntSLE, "signed less or equal");
  

  nb::enum_<LLVMRealPredicate>(m, "RealPredicate", "RealPredicate")
      .value("False_", LLVMRealPredicate::LLVMRealPredicateFalse, "Always false (always folded)")
      .value("OEQ", LLVMRealPredicate::LLVMRealOEQ, "True if ordered and equal")
      .value("OGT", LLVMRealPredicate::LLVMRealOGT, "True if ordered and greater than")
      .value("OGE", LLVMRealPredicate::LLVMRealOGE, "True if ordered and greater than or equal")
      .value("OLT", LLVMRealPredicate::LLVMRealOLT, "True if ordered and less than")
      .value("OLE", LLVMRealPredicate::LLVMRealOLE, "True if ordered and less than or equal")
      .value("ONE", LLVMRealPredicate::LLVMRealONE, "True if ordered and operands are unequal")
      .value("ORD", LLVMRealPredicate::LLVMRealORD, "True if ordered (no nans)")
      .value("UNO", LLVMRealPredicate::LLVMRealUNO, "True if unordered: isnan(X) | isnan(Y)")
      .value("UEQ", LLVMRealPredicate::LLVMRealUEQ, "True if unordered or equal")
      .value("UGT", LLVMRealPredicate::LLVMRealUGT, "True if unordered or greater than")
      .value("UGE", LLVMRealPredicate::LLVMRealUGE, "True if unordered, greater than, or equal")
      .value("ULT", LLVMRealPredicate::LLVMRealULT, "True if unordered or less than")
      .value("ULE", LLVMRealPredicate::LLVMRealULE, "True if unordered, less than, or equal")
      .value("UNE", LLVMRealPredicate::LLVMRealUNE, "True if unordered or not equal")
      .value("True_", LLVMRealPredicate::LLVMRealPredicateTrue, "Always true (always folded)");
  

  nb::enum_<LLVMLandingPadClauseTy>(m, "LandingPadClauseTy", "LandingPadClauseTy")
      .value("Catch", LLVMLandingPadClauseTy::LLVMLandingPadCatch, "A catch clause")
      .value("Filter", LLVMLandingPadClauseTy::LLVMLandingPadFilter, "A filter clause");
  

  nb::enum_<LLVMThreadLocalMode>(m, "ThreadLocalMode", "ThreadLocalMode")
      .value("NotThreadLocal", LLVMThreadLocalMode::LLVMNotThreadLocal)
      .value("GeneralDynamic", LLVMThreadLocalMode::LLVMGeneralDynamicTLSModel)
      .value("LocalDynamic", LLVMThreadLocalMode::LLVMLocalDynamicTLSModel)
      .value("InitialExec", LLVMThreadLocalMode::LLVMInitialExecTLSModel)
      .value("LocalExec", LLVMThreadLocalMode::LLVMLocalExecTLSModel);
  

  nb::enum_<LLVMAtomicOrdering>(m, "AtomicOrdering", "AtomicOrdering")
      .value("NotAtomic", LLVMAtomicOrdering::LLVMAtomicOrderingNotAtomic,
             "A load or store which is not atomic")
      .value("Unordered", LLVMAtomicOrdering::LLVMAtomicOrderingUnordered,
             "Lowest level of atomicity, guarantees somewhat sane results, lock free.")
      .value("Monotonic", LLVMAtomicOrdering::LLVMAtomicOrderingMonotonic,
             "guarantees that if you take all the operations affecting a specific"
             " address, a consistent ordering exists")
      .value("Acquire", LLVMAtomicOrdering::LLVMAtomicOrderingAcquire,
             "Acquire provides a barrier of the sort necessary to acquire a lock"
             " to access other memory with normal loads and stores.")
      .value("Release", LLVMAtomicOrdering::LLVMAtomicOrderingRelease,
             "Release is similar to Acquire, but with a barrier of the sort "
             "necessary to release a lock.")
      .value("AcquireRelease", LLVMAtomicOrdering::LLVMAtomicOrderingAcquireRelease,
             "provides both an Acquire and a Release barrier (for fences and "
             "operations which both read and write memory).")
      .value("SequentiallyConsistent", LLVMAtomicOrdering::LLVMAtomicOrderingSequentiallyConsistent,
             "provides Acquire semantics for loads and Release semantics for "
             "stores. Additionally, it guarantees that a total ordering exists "
             "between all SequentiallyConsistent operations.");
  

  nb::enum_<LLVMAtomicRMWBinOp>(m, "AtomicRMWBinOp", "AtomicRMWBinOp")
      .value("Xchg", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpXchg,
             "Set the new value and return the one old")
      .value("Add", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpAdd,
             "Add a value and return the old one")
      .value("Sub", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpSub,
             "Subtract a value and return the old one")
      .value("And", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpAnd,
             "And a value and return the old one")
      .value("Nand", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpNand,
             "Not-And a value and return the old one")
      .value("Or", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpOr,
             "OR a value and return the old one")
      .value("Xor", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpXor,
             "Xor a value and return the old one")
      .value("Max", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpMax,
             "Sets the value if it's greater than the original using a signed "
             "comparison and return the old one")
      .value("Min", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpMin,
             "Sets the value if it's Smaller than the original using a signed "
             "comparison and return the old one")
      .value("UMax", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpUMax,
             "Sets the value if it's greater than the original using an unsigned"
             " comparison and return the old one")
      .value("UMin", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpUMin,
             "Sets the value if it's greater than the original using an unsigned"
             " comparison and return the old one")
      .value("FAdd", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpFAdd,
             "Add a floating point value and return the old one")
      .value("FSub", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpFSub,
             "Subtract a floating point value and return the old one")
      .value("FMax", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpFMax,
             "Sets the value if it's greater than the original using an floating"
             " point comparison and return the old one")
      .value("FMin", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpFMin,
             "Sets the value if it's smaller than the original using an floating"
             " point comparison and return the old one");
  

  nb::enum_<LLVMDiagnosticSeverity>(m, "DiagnosticSeverity", "DiagnosticSeverity")
      .value("Error", LLVMDiagnosticSeverity::LLVMDSError)
      .value("Warning", LLVMDiagnosticSeverity::LLVMDSWarning)
      .value("Remark", LLVMDiagnosticSeverity::LLVMDSRemark)
      .value("Note", LLVMDiagnosticSeverity::LLVMDSNote);
  

  nb::enum_<LLVMInlineAsmDialect>(m, "InlineAsmDialect", "InlineAsmDialect")
      .value("ATT", LLVMInlineAsmDialect::LLVMInlineAsmDialectATT)
      .value("Intel", LLVMInlineAsmDialect::LLVMInlineAsmDialectIntel);
  

  // TODO move into Module?
  nb::enum_<LLVMModuleFlagBehavior>(m, "ModuleFlagBehavior", "ModuleFlagBehavior")
       .value("Error", LLVMModuleFlagBehavior::LLVMModuleFlagBehaviorError,
              "Adds a requirement that another module flag be present and have "
              "a specified value after linking is performed. The value must be "
              "a metadata pair, where the first element of the pair is the ID "
              "of the module flag to be restricted, and the second element of "
              "the pair is the value the module flag should be restricted to. "
              "This behavior can be used to restrict the allowable results (via"
              " triggering of an error) of linking IDs with the **Override** "
              "behavior.")
       .value("Warning", LLVMModuleFlagBehavior::LLVMModuleFlagBehaviorWarning,
              "Emits a warning if two values disagree. The result value will be "
              "the operand for the flag from the first module being linked.")
       .value("Require", LLVMModuleFlagBehavior::LLVMModuleFlagBehaviorRequire,
              "Adds a requirement that another module flag be present and have "
              "a specified value after linking is performed. The value must be "
              "a metadata pair, where the first element of the pair is the ID of"
              " the module flag to be restricted, and the second element of the "
              "pair is the value the module flag should be restricted to. This "
              "behavior can be used to restrict the allowable results (via "
              "triggering of an error) of linking IDs with the **Override** behavior.")
       .value("Override", LLVMModuleFlagBehavior::LLVMModuleFlagBehaviorOverride,
              "Uses the specified value, regardless of the behavior or value of "
              "the other module. If both modules specify **Override**, but the "
              "values differ, an error will be emitted.")
       .value("Append", LLVMModuleFlagBehavior::LLVMModuleFlagBehaviorAppend,
              "Appends the two values, which are required to be metadata nodes.")
       .value("AppendUnique", LLVMModuleFlagBehavior::LLVMModuleFlagBehaviorAppendUnique,
              "Appends the two values, which are required to be metadata nodes. "
              "However, duplicate entries in the second list are dropped during "
              "the append operation.");

  nb::enum_<PyAttributeIndex>(m, "AttributeIndex", "AttributeIndex")
      .value("Return", PyAttributeIndex::Return)
      .value("Function", PyAttributeIndex::Function);

  nb::enum_<LLVMTailCallKind>(m, "TailCallKind", "TailCallKind")
      .value("LLVMTailCallKindNone", LLVMTailCallKind::LLVMTailCallKindNone)
      .value("LLVMTailCallKindTail", LLVMTailCallKind::LLVMTailCallKindTail)
      .value("LLVMTailCallKindMustTail", LLVMTailCallKind::LLVMTailCallKindMustTail)
      .value("LLVMTailCallKindNoTail", LLVMTailCallKind::LLVMTailCallKindNoTail);

  // TODO LLVMAttributeIndex

  nb::enum_<PyLLVMFastMathFlags>(m, "FastMathFlags", "FastMathFlags")
       .value("AllowReassoc", PyLLVMFastMathFlags::AllowReassoc)
       .value("NoNaNs", PyLLVMFastMathFlags::NoNaNs)
       .value("NoInfs", PyLLVMFastMathFlags::NoInfs)
       .value("NoSignedZeros", PyLLVMFastMathFlags::NoSignedZeros)
       .value("AllowReciprocal", PyLLVMFastMathFlags::AllowReciprocal)
       .value("AllowContract", PyLLVMFastMathFlags::AllowContract)
       .value("ApproxFunc", PyLLVMFastMathFlags::ApproxFunc)
       .value("None_", PyLLVMFastMathFlags::None)
       .value("All", PyLLVMFastMathFlags::All);

  // TODO LLVMFastMathFlags
}



void bindGlobalFunctions(nb::module_ &m) {
  // TODO it seems it has no effect in python binding
  m.def("shutdown", &LLVMShutdown, "Deallocate and destroy all ManagedStatic variables.");
  
  m.def("get_version",
        []() {
          unsigned major, minor, patch;
          LLVMGetVersion(&major, &minor, &patch);
          return std::make_tuple(major, minor, patch);
        },
        "Return the major, minor, and patch version of LLVM\n"
        "The version components are returned via the function's three output "
        "parameters or skipped if a NULL pointer was supplied.");

  // NOTE these two functions seems useless in Python
  // m.def("create_message", &LLVMCreateMessage, "message"_a);
  // m.def("dispose_message", &LLVMDisposeMessage, "message"_a); // error, may need a wrapper for created message

  m.def("llvm_is_multithreaded",
        []() {
          return LLVMIsMultithreaded() != 0;
        });
}





void bindTypeClasses(nb::module_ &m) {
  nb::class_<PyType>(m, "Type", "Type")
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





void bindValueClasses(nb::module_ &m) {
  auto ValueClass = nb::class_<PyValue>(m, "Value", "Value");

  nb::class_<PyMetadataAsValue, PyValue>(m, "MetadataAsValue", "MetadataAsValue");
  auto MDNodeValueClass = nb::class_<PyMDNodeValue, PyMetadataAsValue>
                            (m, "MDNodeValue", "MDNodeValue");
  auto ValueAsMetadataValueClass = nb::class_<PyValueAsMetadataValue, PyMetadataAsValue>
                                (m, "ValueAsMetadataValue", "ValueAsMetadataValue");
  auto MDStringValueClass = nb::class_<PyMDStringValue, PyMetadataAsValue>
                         (m, "MDStringValue", "MDStringValue");
    
  auto ArgumentClass = nb::class_<PyArgument, PyValue>(m, "Argument", "Argument");
  auto BasicBlockValueClass = nb::class_<PyBasicBlockValue, PyValue>(m, "BasicBlockValue", "BasicBlockValue");
  auto InlineAsmClass = nb::class_<PyInlineAsm, PyValue>(m, "InlineAsm", "InlineAsm");
  auto UserClass = nb::class_<PyUser, PyValue>(m, "User", "User");
  auto ConstantClass = nb::class_<PyConstant, PyUser>(m, "Constant", "Constant");
  auto ConstantArrayClass = nb::class_<PyConstantArray, PyConstant>(m, "ConstantArray", "ConstantArray");
  auto ConstantDataSequentialClass = nb::class_<PyConstantDataSequential, PyConstant>
                                       (m, "ConstantDataSequential", "ConstantDataSequential");
  auto ConstantDataArrayClass = nb::class_<PyConstantDataArray, PyConstantDataSequential>
                                  (m, "ConstantDataArray", "ConstantDataArray");
  auto ConstantDataVectorClass = nb::class_<PyConstantDataVector, PyConstantDataSequential>
                                   (m, "ConstantDataVector", "ConstantDataVector");
  auto ConstantExprClass = nb::class_<PyConstantExpr, PyConstant>(m, "ConstantExpr", "ConstantExpr");
  auto ConstantFPClass = nb::class_<PyConstantFP, PyConstant>(m, "ConstantFP", "ConstantFP");
  auto ConstantIntClass = nb::class_<PyConstantInt, PyConstant>(m, "ConstantInt", "ConstantInt");
  auto ConstantStructClass = nb::class_<PyConstantStruct, PyConstant>(m, "ConstantStruct", "ConstantStruct");
  auto ConstantVectorClass = nb::class_<PyConstantVector, PyConstant>(m, "ConstantVector", "ConstantVector");
  auto GlobalValueClass = nb::class_<PyGlobalValue, PyConstant>(m, "GlobalValue", "GlobalValue");
  auto GlobalAliasClass = nb::class_<PyGlobalAlias, PyGlobalValue>(m, "GlobalAlias", "GlobalAlias");
  auto GlobalObjectClass = nb::class_<PyGlobalObject, PyGlobalValue>(m, "GlobalObject", "GlobalObject");
  auto FunctionClass = nb::class_<PyFunction, PyGlobalObject>(m, "Function", "Function");
  auto GlobalVariableClass = nb::class_<PyGlobalVariable, PyGlobalObject>(m, "GlobalVariable", "GlobalVariable");
  auto GlobalIFuncClass = nb::class_<PyGlobalIFunc, PyGlobalObject>(m, "GlobalIFunc", "GlobalIFunc");
  auto UndefValueClass = nb::class_<PyUndefValue, PyConstant>(m, "UndefValue", "UndefValue");
  auto PoisonValueClass = nb::class_<PyPoisonValue, PyConstant>(m, "PoisonValue", "PoisonValue");
  auto InstructionClass = nb::class_<PyInstruction, PyUser>(m, "Instruction", "Instruction");
  auto CallBaseClass = nb::class_<PyCallBase, PyInstruction>(m, "CallBase", "CallBase");
  auto CallInstClass = nb::class_<PyCallInst, PyCallBase>(m, "CallInst", "CallInst");
  auto InvokeInstClass = nb::class_<PyInvokeInst, PyCallBase>(m, "InvokeInst", "InvokeInst");
  auto FCmpInstClass = nb::class_<PyFCmpInst, PyInstruction>(m, "FCmpInst", "FCmpInst");
  auto ICmpInstClass = nb::class_<PyICmpInst, PyInstruction>(m, "ICmpInst", "ICmpInst");
  auto GetElementPtrInstClass = nb::class_<PyGetElementPtrInst, PyInstruction>(m, "GetElementPtrInst", "GetElementPtrInst");
  auto PHINodeClass = nb::class_<PyPHINode, PyInstruction>(m, "PHINode", "PHINode");
  auto ShuffleVectorInstClass = nb::class_<PyShuffleVectorInst, PyInstruction>(m, "ShuffleVectorInst", "ShuffleVectorInst");
  auto StoreInstClass = nb::class_<PyStoreInst, PyInstruction>(m, "StoreInst", "StoreInst");
  auto BranchInstClass = nb::class_<PyBranchInst, PyInstruction>(m, "BranchInst", "BranchInst");
  auto ReturnInstClass = nb::class_<PyReturnInst, PyInstruction>(m, "ReturnInst", "ReturnInst");
  auto SwitchInstClass = nb::class_<PySwitchInst, PyInstruction>(m, "SwitchInst", "SwitchInst");
  auto CatchSwitchInstClass = nb::class_<PyCatchSwitchInst, PyInstruction>
                                (m, "CatchSwitchInst", "CatchSwitchInst");
  auto CleanupReturnInstClass = nb::class_<PyCleanupReturnInst, PyInstruction>
                                  (m, "CleanupReturnInst", "CleanupReturnInst");
  auto FuncletPadInstClass = nb::class_<PyFuncletPadInst, PyInstruction>(m, "FuncletPadInst", "FuncletPadInst");
  auto CatchPadInstClass = nb::class_<PyCatchPadInst, PyFuncletPadInst>(m, "CatchPadInst", "CatchPadInst");
  auto AllocaInstClass = nb::class_<PyAllocaInst, PyInstruction>(m, "AllocaInst", "AllocaInst");
  auto LoadInstClass = nb::class_<PyLoadInst, PyInstruction>(m, "LoadInst", "LoadInst");
  auto AtomicCmpXchgInstClass = nb::class_<PyAtomicCmpXchgInst, PyInstruction>(m, "AtomicCmpXchgInst", "AtomicCmpXchgInst");
  auto AtomicRMWInstClass = nb::class_<PyAtomicRMWInst, PyInstruction>
                              (m, "AtomicRMWInst", "AtomicRMWInst");
  auto IEValueInstBaseClass = nb::class_<PyIEValueInstBase, PyInstruction>
                                (m, "IEValueInstBase", "IEValueInstBase");
  auto InsertValueInstClass = nb::class_<PyInsertValueInst, PyIEValueInstBase>
                                (m, "InsertValueInst", "InsertValueInst");
  auto ExtractValueInstClass = nb::class_<PyExtractValueInst, PyIEValueInstBase>
                                 (m, "ExtractValueInst", "ExtractValueInst");
  auto LandingPadInstClass = nb::class_<PyLandingPadInst, PyInstruction>
                               (m, "LandingPadInst", "LandingPadInst");
  auto IndirectBrInstClass = nb::class_<PyIndirectBrInst, PyInstruction>
                               (m, "IndirectBrInst", "IndirectBrInst");
  auto FenceInstClass = nb::class_<PyFenceInst, PyInstruction>
                          (m, "FenceInst", "FenceInst");

  MDNodeValueClass
      .def("__repr__",
           [](PyMDNodeValue &self) {
             return gen_value_repr("MDNodeValue", self);
           })
      // deprecated functions not binded:
      // LLVMMDNodeInContext, LLVMMDNode
      .def("as_metadata",
           [](PyMDStringValue &self) {
             return PyMDNode(LLVMValueAsMetadata(self.get()));
           })
      .def_prop_ro("num_operands",
                   [](PyMDNodeValue &self) {
                     return LLVMGetMDNodeNumOperands(self.get());
                   })
      .def_prop_ro("operands",
                   [](PyMDNodeValue &self) {
                     unsigned num = LLVMGetMDNodeNumOperands(self.get());
                     std::vector<LLVMValueRef> dest(num);
                     LLVMGetMDNodeOperands(self.get(), dest.data());
                     WRAP_VECTOR_FROM_DEST_AUTO(PyValue, num, res, dest);
                     return res;
                   },
                   "Obtain the given MDNode's operands.")
      .def_prop_ro("replace_operand_with",
                   [](PyMDNodeValue &self, unsigned index, PyMetadata &replacement) {
                     return LLVMReplaceMDNodeOperandWith
                              (self.get(), index, replacement.get());
                   },
                   "Replace an operand at a specific index in a llvm::MDNode value.");
  
  MDStringValueClass
      .def("__repr__",
           [](PyMDStringValue &self) {
             return gen_value_repr("MDStringValue", self);
           })
      // deprecated functions not binded:
      // LLVMMDStringInContext, LLVMMDString
      .def("as_metadata",
           [](PyMDStringValue &self) {
             return PyMDString(LLVMValueAsMetadata(self.get()));
           })
      .def_prop_ro("raw_string",
                   [](PyMDStringValue &v) {
                     unsigned len;
                     auto str = LLVMGetMDString(v.get(), &len);
                     return std::string(str, len);
                   },
                   "Obtain the underlying string from a MDString value.");

  ValueClass
      // TODO note there are many functions here that belongs `UserClass`
      .def("__repr__",
           [](PyValue &self) {
             auto raw = self.get();
             auto name = get_value_name(raw);
             auto kind = get_repr_str(LLVMGetValueKind(raw));
             return fmt::format("<Value name={} kind={} str='{}'>",
                                name, kind, get_value_str(raw));
           })
      .def("__str__",
           [](PyValue &self) {
             return get_value_str(self.get());
           })
      .def_prop_ro("type",
                   [](PyValue &v) { return PyTypeAuto(LLVMTypeOf(v.get())); })
      .def_prop_ro("kind",
                   [](PyValue &v) { return LLVMGetValueKind(v.get()); })
      // NOTE LLVMSetValueName and LLVMGetValueName are depreciated
      .def_prop_rw("name",
                   [](PyValue &v) {
                     return get_value_name(v.get());
                   },
                   [](PyValue &v, std::string &name) {
                     return LLVMSetValueName2(v.get(), name.c_str(), name.size());
                   })
      .def_prop_ro("is_constant",
                   [](PyValue &v) { return LLVMIsConstant(v.get()) != 0; })
      .def_prop_ro("is_undef",
                   [](PyValue &v) { return LLVMIsUndef(v.get()) != 0; })
      .def_prop_ro("is_poisonous",
                   [](PyValue &v) { return LLVMIsPoison(v.get()) != 0;})
      .def_prop_ro("first_use",
                   [](PyValue &v) -> optional<PyUse> {
                     auto res = LLVMGetFirstUse(v.get());
                     WRAP_OPTIONAL_RETURN(res, PyUse);
                   })
      .def_prop_ro("uses",
                   [](PyValue &self) {
                     return PyUseIterator(PyUse(LLVMGetFirstUse(self.get())));
                   })
      .def("as_metadata",
           [](PyValue &self) {
             return PyValueAsMetadata(LLVMValueAsMetadata(self.get()));
           })
      .def("dump",
           [](PyValue &v) { return LLVMDumpValue(v.get()); },
           "Dump a representation of a value to stderr.")
      .def_static("replace_all_uses_with",
                  [](PyValue &oldVal, PyValue &newVal) {
                    return LLVMReplaceAllUsesWith(oldVal.get(), newVal.get());
                  },
                  "Replace all uses of a value with another one.")
  PY_FOR_EACH_VALUE_SUBCLASS(PY_DECLARE_VALUE_CAST)
      .def("to_BasicBlockValue",
           [](PyValue &self) -> optional<PyBasicBlockValue> {
             auto res = LLVMIsABasicBlock(self.get());
             WRAP_OPTIONAL_RETURN(res, PyBasicBlockValue);
           })
      .def("to_MDNodeValue",
           [](PyValue &self) -> optional<PyMDNodeValue> {
             auto res = LLVMIsAMDNode(self.get());
             WRAP_OPTIONAL_RETURN(res, PyMDNodeValue);
           })
      .def("to_ValueAsMetadataValue",
           [](PyValue &self) -> optional<PyValueAsMetadataValue> {
             auto res = LLVMIsAValueAsMetadata(self.get());
             WRAP_OPTIONAL_RETURN(res, PyValueAsMetadataValue);
           })
      .def("to_MDStringValue",
           [](PyValue &self) -> optional<PyMDStringValue> {
             auto res = LLVMIsAMDString(self.get());
             WRAP_OPTIONAL_RETURN(res, PyMDStringValue);
           })
      // TODO test whether these property are only true to BasicBlockValue
      .def_prop_ro("is_basic_block",
                   [](PyValue &self) {
                     return LLVMValueIsBasicBlock(self.get()) != 0;
                   })
      .def("as_basic_block",
           [](PyValue &self) {
             return PyBasicBlock(LLVMValueAsBasicBlock(self.get()));
           });

  PoisonValueClass
      .def("__repr__",
           [](PyPoisonValue &self) {
             return gen_value_repr("PoisonValue", self);
           })
      .def("__init__",
           [](PyPoisonValue *p, PyType &t) {
             new (p) PyPoisonValue(LLVMGetPoison(t.get()));
           },
           "type"_a,
           "Obtain a constant value referring to a poison value of a type.");

  
  UndefValueClass
      .def("__repr__",
           [](PyUndefValue &self) {
             return gen_value_repr("UndefValue", self);
           })

      .def("__init__",
           [](PyUndefValue *uv, PyType &t) {
             new (uv) PyUndefValue(LLVMGetUndef(t.get()));
           },
           "type"_a,
           "Obtain a constant value referring to an undefined value of a type.");
  

  ShuffleVectorInstClass
      .def("__repr__",
           [](PyShuffleVectorInst &self) {
             return "<ShuffleVectorInst>";
           })
      .def_prop_ro("mask_elems_num",
                   [](PyShuffleVectorInst &self) {
                     return LLVMGetNumMaskElements(self.get());
                   })
      .def_static("get_undef_mask_elem",
                  [](PyShuffleVectorInst &self) {
                    return LLVMGetUndefMaskElem();
                  },
                  "returns a constant that specifies that the result of a "
                  "ShuffleVectorInst is undefined.")
      .def("get_mask_value",
           [](PyValue &self, unsigned index) {
             return LLVMGetMaskValue(self.get(), index);
           },
           "index"_a,
           "Get the mask value at position index in the mask of the ShuffleVector"
           "instruction.\n\n"
           "Returns the result of LLVMGetUndefMaskElem() if the mask value is"
           "poison at that position.");

  
  CatchPadInstClass
      .def("__repr__",
           [](PyCatchPadInst &self) {
             return "<CatchPadInst>";
           })
      .def_prop_rw("parent",
                   [](PyCatchPadInst &self) {
                     return PyCatchSwitchInst(LLVMGetParentCatchSwitch(self.get()));
                   },
                   [](PyCatchPadInst &self, PyCatchSwitchInst catchSwitch) {
                     return LLVMSetParentCatchSwitch(self.get(), catchSwitch.get());
                   });
  

  

  GlobalIFuncClass
      .def("__repr__",
           [](PyGlobalIFunc &self) {
             return "<GlobalIFunc>";
           })
      .def_prop_ro("next",
                   [](PyGlobalIFunc &self) -> optional<PyGlobalIFunc> {
                     auto res = LLVMGetNextGlobalIFunc(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalIFunc);
                   })
      .def_prop_ro("prev",
                   [](PyGlobalIFunc &self) -> optional<PyGlobalIFunc> {
                     auto res = LLVMGetPreviousGlobalIFunc(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalIFunc);
                   })
      .def_prop_rw("resolver",
                   [](PyGlobalIFunc &self) -> optional<PyConstant> {
                     auto res = LLVMGetGlobalIFuncResolver(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyConstant);
                   },
                   [](PyGlobalIFunc &self, PyConstant resolver) {
                     return LLVMSetGlobalIFuncResolver(self.get(), resolver.get());
                   })
      .def("destory",
           [](PyGlobalIFunc &self) {
             return LLVMEraseGlobalIFunc(self.get());
           },
           "Remove a global indirect function from its parent module and delete it.\n\n"
           "You shouldn't use it anymore after removal.")
      .def("remove_from_parent",
           [](PyGlobalIFunc &self) {
             return LLVMRemoveGlobalIFunc(self.get());
           },
           "Remove a global indirect function from its parent module.\n\n"
           "This unlinks the global indirect function from its containing module but"
           "keeps it alive.");

  InlineAsmClass
      .def("__repr__",
           [](PyInlineAsm &self) {
             return "<InlineAsm>";
           })
      .def("get_inline_asm",
           [](PyInlineAsm *iasm, PyType &ty, std::string asmString, std::string constraints,
              bool hasSideEffects, bool isAlignStack, LLVMInlineAsmDialect dialect,
              bool canThrow){
             new (iasm) PyInlineAsm(LLVMGetInlineAsm
                                      (ty.get(), asmString.c_str(), asmString.size(),
                                       constraints.c_str(), constraints.size(),
                                       hasSideEffects, isAlignStack, dialect, canThrow));
           },
           "type"_a, "asm"_a, "constraints"_a, "has_side_effects"_a,
           "is_align_stack"_a, "dialect"_a, "can_throw"_a,
           "Create the specified unique inline asm string.")
      .def_prop_ro("str",
                   [](PyInlineAsm &iasm) {
                     size_t len;
                     const char *str = LLVMGetInlineAsmAsmString(iasm.get(), &len);
                     return std::string(str, len);
                   },
                   "Get the template string used for an inline assembly snippet.")
      .def_prop_ro("constraint_str",
                   [](PyInlineAsm &iasm) {
                     size_t len;
                     const char *str = LLVMGetInlineAsmConstraintString(iasm.get(), &len);
                     return std::string(str, len);
                   },
                   "Get the raw constraint string for an inline assembly snippet.")
      .def_prop_ro("dialect",
                   [](PyInlineAsm &iasm) {
                     return LLVMGetInlineAsmDialect(iasm.get());
                   },
                   "Get the dialect used by the inline asm snippet.")
      .def_prop_ro("function_type",
                   [](PyInlineAsm &iasm) {
                     return PyTypeFunction(LLVMGetInlineAsmFunctionType(iasm.get()));
                   },
                   "Get the function type of the inline assembly snippet. "
                   "The same type that was passed into :func:`get_inline_asm` originally.")
      .def_prop_ro("has_side_effects",
                   [](PyInlineAsm &iasm) {
                     return LLVMGetInlineAsmHasSideEffects(iasm.get()) != 0;
                   },
                   "Get if the inline asm snippet has side effects.")
      .def_prop_ro("needs_aligned_stack",
                   [](PyInlineAsm &iasm) {
                     return LLVMGetInlineAsmNeedsAlignedStack(iasm.get()) != 0;
                   })
      .def_prop_ro("can_unwind",
                   [](PyInlineAsm &iasm) {
                     return LLVMGetInlineAsmCanUnwind(iasm.get()) != 0;
                   });

  InstructionClass
      .def("__repr__",
           [](PyInstruction &self) {
             auto opcode = LLVMGetInstructionOpcode(self.get());
             return fmt::format("<Instruction opcode={}>", get_repr_str(opcode));
           })
      .def_prop_ro("can_use_fast_math_flags",
                  [](PyInstruction &self) {
                    return LLVMCanValueUseFastMathFlags(self.get()) != 0;
                  },
                  "Check if a given value can potentially have fast math flags.\n"
                  "Will return true for floating point arithmetic instructions, and "
                  "for select, phi, and call instructions whose type is a floating "
                  "point type, or a vector or array thereof.\n"
                  "See https://llvm.org/docs/LangRef.html#fast-math-flags")
      .def_prop_ro("next",
                   [](PyInstruction &self) -> optional<PyInstruction> {
                     auto res = LLVMGetNextInstruction(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyInstruction);
                   },
                   "Obtain the instruction that occurs after the one specified.\n"
                   "The next instruction will be from the same basic block.\n\n"
                   "If this is the last instruction in a basic block, None will "
                   "be returned.")
      .def_prop_ro("prev",
                   [](PyInstruction &self) -> optional<PyInstruction> {
                     auto res = LLVMGetPreviousInstruction(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyInstruction);
                   },
                   "Obtain the instruction that occurred before this one.\n\n"
                   "If the instruction is the first instruction in a basic block, NULL"
                   "will be returned.")
      .def_prop_ro("has_metadata",
                   [](PyInstruction &self) {
                     return LLVMHasMetadata(self.get()) != 0;
                   })
      .def_prop_ro("debug_loc_directory",
                   [](PyInstruction &i) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocDirectory(i.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the directory of the debug location for this value")
      .def_prop_ro("debug_loc_filename",
                   [](PyInstruction &i) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocFilename(i.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the filename of the debug location for this value.")
      .def_prop_ro("debug_loc_line",
                   [](PyInstruction &i) { return LLVMGetDebugLocLine(i.get()); },
                   "Return the line number of the debug location for this value")
      .def_prop_ro("debug_loc_column",
                   [](PyInstruction &i) { return LLVMGetDebugLocColumn(i.get()); },
                   "Return the column number of the debug location for this value")
      .def_prop_ro("parent",
                   [](PyInstruction &self) -> optional<PyBasicBlock> {
                     auto res = LLVMGetInstructionParent(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyBasicBlock);
                   },
                   "Obtain the basic block to which an instruction belongs.")
      .def_prop_ro("opcode",
                   [](PyInstruction &self) {
                     return LLVMGetInstructionOpcode(self.get());
                   })
      .def_prop_ro("is_terminator",
                   [](PyInstruction &self) {
                     return LLVMIsATerminatorInst(self.get()) != nullptr;
                   })
      .def("clone",
           [](PyInstruction &self) -> optional<PyInstruction*> {
             auto res = LLVMInstructionClone(self.get());
             WRAP_OPTIONAL_RETURN(res, PyInstructionAuto);
           },
           "Create a copy of 'this' instruction that is identical in all ways"
           "except the following:\n"
           "  - The instruction has no parent"
           "  - The instruction has no name")
      .def("remove_from_parent",
           [](PyInstruction &self) {
             return LLVMInstructionRemoveFromParent(self.get());
           },
           "The instruction specified is removed from its containing building"
           "block but is kept alive.")
      .def("destory",
           [](PyInstruction &self) {
             return LLVMInstructionEraseFromParent(self.get());
           },
           "Remove and delete an instruction.\n\n"
           "The instruction specified is removed from its containing building"
           "block and then deleted.")
      .def("delete",
           [](PyInstruction &self) {
             return LLVMDeleteInstruction(self.get());
           },
           "Delete an instruction.\n\n"
           "The instruction specified is deleted. It must have previously been"
           "removed from its containing building block.")
      .def("get_metadata",
           [](PyInstruction &self, unsigned kindID) -> optional<PyValue*> {
             auto res = LLVMGetMetadata(self.get(), kindID);
             WRAP_OPTIONAL_RETURN(res, PyValueAuto);
           },
           "kind_id"_a)
      .def("set_metadata",
           [](PyInstruction &self, unsigned kindID, PyMetadataAsValue node) {
             return LLVMSetMetadata(self.get(), kindID, node.get());
           },
           "kind_id"_a, "value"_a)
      .def("get_all_metadata_no_debug_loc",
           [](PyInstruction &self) {
             size_t num;
             LLVMValueMetadataEntries entries =
               LLVMInstructionGetAllMetadataOtherThanDebugLoc(self.get(), &num);
             return PyMetadataEntries(entries, num);
           })
      /*
        The following methods only applys to instructions for which LLVMIsATerminatorInst
        returns true.
       
        NOTE the reason why we don't create a base class for them to inherit is
        because by inspecting the llvm c++ api (by following the call trace,
        the generated Instruction.def file) groups different opcode to be terminator than
        LLVMOpcode in C api, so possible C api is outdated?
        
        TODO we'll give it a test first.   
       */

      .def_prop_ro("successors_num",
                   [](PyInstruction &self) {
                     return LLVMGetNumSuccessors(self.get());
                   })
      .def("get_successor",
           [](PyInstruction &self, unsigned index) {
             return PyBasicBlock(LLVMGetSuccessor(self.get(), index));
           })
      .def("set_successor",
           [](PyInstruction &self, unsigned index, PyBasicBlock bb) {
             return LLVMSetSuccessor(self.get(), index, bb.get());
           })
       // TODO
      .def_static("get_nuw", 
                  [](PyInstruction &arithInst) {
                    return LLVMGetNUW(arithInst.get()) != 0;
                  },
                  "arith_inst"_a)
      .def_static("set_nuw",
                  [](PyInstruction &arithInst, bool hasNUW) {
                    return LLVMSetNUW(arithInst.get(), hasNUW);
                  },
                  "arith_inst"_a, "hasNUW"_a)
      .def_static("get_nsw",
                  [](PyInstruction &arithInst) {
                    return LLVMGetNSW(arithInst.get()) != 0;
                  },
                  "arithInst"_a)
      .def_static("set_nsw",
                  [](PyInstruction &arithInst, bool hasNSW) {
                    return LLVMSetNSW(arithInst.get(), hasNSW);
                  },
                  "arith_inst"_a, "hasNSW"_a)
      .def_static("get_exact",
                  [](PyInstruction &DivOrShrInst) {
                    return LLVMGetExact(DivOrShrInst.get()) != 0;
                  },
                  "div_or_shr_inst"_a)
      .def_static("set_exact",
                  [](PyInstruction &DivOrShrInst, bool isExact) {
                    return LLVMSetExact(DivOrShrInst.get(), isExact);
                  },
                  "div_or_shr_inst"_a, "is_exact"_a)
      .def_static("get_nneg",
                  [](PyInstruction &NonNegInst) {
                    return LLVMGetNNeg(NonNegInst.get()) != 0;
                  },
                  "non_neg_inst"_a,
                  "Gets if the instruction has the non-negative flag set.\n",
                  "Only valid for zext instructions.")
      .def_static("set_nned",
                  [](PyInstruction &NonNegInst, bool isNonNeg) {
                    return LLVMSetNNeg(NonNegInst.get(), isNonNeg);
                  },
                  "non_neg_inst"_a, "is_non_neg"_a,
                  "Sets the non-negative flag for the instruction.\n"
                  "Only valid for zext instructions.")
      .def_static("get_fast_math_flags",
                  [](PyInstruction &FPMathInst) {
                    return LLVMGetFastMathFlags(FPMathInst.get());
                  },
                  "fp_math_inst"_a,
                  "Get the flags for which fast-math-style optimizations are allowed "
                  "for this value.\n\n"
                  "Only valid on floating point instructions."
                  "See `can_use_fast_math_flags`.")
      .def_static("set_fast_math_flags",
                  [](PyInstruction &FPMathInst, LLVMFastMathFlags FMF) {
                    return LLVMSetFastMathFlags(FPMathInst.get(), FMF);
                  },
                  "fp_math_inst"_a, "fmf"_a,
                  "Sets the flags for which fast-math-style optimizations are allowed "
                  "for this value.\n\n"
                  "Only valid on floating point instructions.\n"
                  "See `can_use_fast_math_flags`.")
      .def_static("get_is_disjoint",
                  [](PyInstruction &inst) {
                    return LLVMGetIsDisjoint(inst.get()) != 0;
                  },
                  "inst"_a,
                  "Gets whether the instruction has the disjoint flag set.\n"
                  "Only valid for or instructions.")
      .def_static("set_is_disjoint",
                  [](PyInstruction &inst, bool isDisjoint) {
                    return LLVMSetIsDisjoint(inst.get(), isDisjoint);
                  },
                  "inst"_a, "is_disjoint"_a,
                  "Sets the disjoint flag for the instruction.\n"
                  "Only valid for or instructions.");

  /*
    Terminator only methods needs ===================================
   */

  BranchInstClass
      .def_prop_ro("is_conditional",
                   [](PyBranchInst &self) {
                     return LLVMIsConditional(self.get());
                   })
      .def_prop_rw("condition", // TODO test whether return NULL
                   [](PyBranchInst &self) {
                     return PyValueAuto(LLVMGetCondition(self.get()));
                   },
                   [](PyBranchInst &self, PyValue &cond) {
                     return LLVMSetCondition(self.get(), cond.get());
                   });

  SwitchInstClass
      .def("__repr__",
           [](PySwitchInst &self) {
             return "<SwitchInst>";
           })
      .def_prop_ro("default_dest",
                   [](PySwitchInst &self) {
                     return PyBasicBlock(LLVMGetSwitchDefaultDest(self.get()));
                   })
      .def_prop_ro("add_case",
                   [](PySwitchInst &self, PyConstantInt &onVal, PyBasicBlock dest) {
                     return LLVMAddCase(self.get(), onVal.get(), dest.get());
                   });

  IEValueInstBaseClass
      .def_prop_ro("indices_num",
           [](PyIEValueInstBase &self) {
             return LLVMGetNumIndices(self.get());
           })
      .def("indices",
           [](PyIEValueInstBase &self) {
             return LLVMGetIndices(self.get());
           });


  CallInstClass
      .def_prop_rw("is_tail_call",
                   [](PyCallInst &self) {
                     return LLVMIsTailCall(self.get()) != 0;
                   },
                   [](PyCallInst &self, bool isTailCall) {
                     return LLVMSetTailCall(self.get(), isTailCall);
                   })
      .def_prop_rw("tail_call_kind",
                   [](PyCallInst &self) {
                     return LLVMGetTailCallKind(self.get());
                   },
                   [](PyCallInst &self, LLVMTailCallKind kind) {
                     return LLVMSetTailCallKind(self.get(), kind);
                   });

  
  CallBaseClass
      .def_prop_ro("arg_num",
                   [](PyCallBase &self) {
                     return LLVMGetNumArgOperands(self.get());
                   })
      .def_prop_rw("call_conv",
                   [](PyCallBase &self) {
                     auto res = LLVMGetInstructionCallConv(self.get());
                     return static_cast<LLVMCallConv>(res);
                   },
                   [](PyCallBase &self, LLVMCallConv CC) {
                     return LLVMSetInstructionCallConv(self.get(), CC);
                   })
      .def_prop_ro("called_fn_type",
                   [](PyCallBase &self) {
                     return PyTypeFunction(LLVMGetCalledFunctionType(self.get()));
                   },
                   "Obtain the function type called by this instruction.")
      .def_prop_ro("called_value",
                   [](PyCallBase &self) {
                     return PyFunction(LLVMGetCalledValue(self.get()));
                   })
      .def_prop_ro("operand_bundles_num",
                   [](PyCallBase &self) {
                     return LLVMGetNumOperandBundles(self.get());
                   })
      .def("get_operand_bundle_at",
           [](PyCallBase &self, unsigned index) {
             return PyOperandBundle(LLVMGetOperandBundleAtIndex(self.get(), index));
           },
           "index"_a,
           "Obtain the operand bundle attached to this instruction at the given index.")
      .def("set_param_alignment",
           [](PyCallBase &self, LLVMAttributeIndex idx, unsigned align) {
             return LLVMSetInstrParamAlignment(self.get(), idx, align);
           },
           "index"_a, "align"_a)
      .def("add_call_site_attribute",
           [](PyCallBase &self, LLVMAttributeIndex idx, PyAttribute &attr) {
             return LLVMAddCallSiteAttribute(self.get(), idx, attr.get());
           },
           "index"_a, "attr"_a)
      .def("get_call_site_attribute_count",
           [](PyCallBase &self, LLVMAttributeIndex idx) {
             return LLVMGetCallSiteAttributeCount(self.get(), idx);
           })
      .def("get_call_site_attributes",
           [](PyCallBase &self, LLVMAttributeIndex idx) {
             unsigned num = LLVMGetCallSiteAttributeCount(self.get(), idx);
             std::vector<LLVMAttributeRef> attrs(num);
             LLVMGetCallSiteAttributes(self.get(), idx, attrs.data());
             WRAP_VECTOR_FROM_DEST_AUTO(PyAttribute, num, res, attrs);
             return res;
           })
      .def("get_call_site_enum_attribute",
           [](PyCallBase &self, LLVMAttributeIndex idx, unsigned kindID) {
             return PyEnumAttribute(LLVMGetCallSiteEnumAttribute(self.get(), idx,
                                                                 kindID));
           })
      .def("get_call_site_string_attribute",
           [](PyCallBase &self, LLVMAttributeIndex idx, std::string &kind) {
             return PyStringAttribute(LLVMGetCallSiteStringAttribute
                                        (self.get(), idx, kind.c_str(), kind.size()));
           })
      .def("remove_call_site_enum_attribute",
           [](PyCallBase &self, LLVMAttributeIndex idx, unsigned kindID) {
             return LLVMRemoveCallSiteEnumAttribute(self.get(), idx, kindID);
           })
      .def("remove_call_site_string_attribute",
           [](PyCallBase &self, LLVMAttributeIndex idx, std::string &kind) {
             return LLVMRemoveCallSiteStringAttribute
                      (self.get(), idx, kind.c_str(), kind.size());
           });

  InvokeInstClass
      .def("__repr__",
           [](PyInvokeInst &self) {
             return "<InvokeInst>";
           })
      .def_prop_rw("normal_dest",
                   [](PyInvokeInst &self) {
                     return PyBasicBlock(LLVMGetNormalDest(self.get()));
                   },
                   [](PyInvokeInst &self, PyBasicBlock bb) {
                     return LLVMSetNormalDest(self.get(), bb.get());
                   })
      .def_prop_rw("unwind_dest",
                   [](PyInvokeInst &self) {
                     return PyBasicBlock(LLVMGetUnwindDest(self.get()));
                   },
                   [](PyInvokeInst &self, PyBasicBlock bb) {
                     return LLVMSetUnwindDest(self.get(), bb.get());
                   });

  CleanupReturnInstClass
      .def("__repr__",
           [](PyCleanupReturnInst &self) {
             return "<CleanupReturnInst>";
           })
      .def_prop_rw("unwind_dest",
                   [](PyCleanupReturnInst &self) {
                     return PyBasicBlock(LLVMGetUnwindDest(self.get()));
                   },
                   [](PyCleanupReturnInst &self, PyBasicBlock bb) {
                     return LLVMSetUnwindDest(self.get(), bb.get());
                   });

  CatchSwitchInstClass
      .def("__repr__",
           [](PyCatchSwitchInst &self) {
             return "<CatchSwitchInst>";
           })
      .def_prop_rw("unwind_dest",
                   [](PyCatchSwitchInst &self) {
                     return PyBasicBlock(LLVMGetUnwindDest(self.get()));
                   },
                   [](PyCatchSwitchInst &self, PyBasicBlock bb) {
                     return LLVMSetUnwindDest(self.get(), bb.get());
                   })
      .def_prop_ro("handlers_num",
                   [](PyCatchSwitchInst &self) {
                     return LLVMGetNumHandlers(self.get());
                   })
      .def_prop_ro("handlers",
                   [](PyCatchSwitchInst &self) {
                     unsigned num = LLVMGetNumHandlers(self.get());
                     std::vector<LLVMBasicBlockRef> handlers;
                     LLVMGetHandlers(self.get(), handlers.data());
                     WRAP_VECTOR_FROM_DEST(PyBasicBlock, num, res, handlers);
                     return res;
                   },
                   "Obtain the basic blocks acting as handlers for a catchswitch "
                   "instruction.")
      .def("add_handler",
           [](PyCatchSwitchInst &self, PyBasicBlock dest) {
             return LLVMAddHandler(self.get(), dest.get());
           });


  FuncletPadInstClass
      .def("__repr__",
           [](PyFuncletPadInst &self) {
             return "<FuncletPadInst>";
           })
      .def_prop_ro("arg_num",
                   [](PyCallBase &self) {
                     // TODO change it into enum type
                     return LLVMGetNumArgOperands(self.get());
                   })
      .def("get_arg_operand",
           [](PyFuncletPadInst &self, unsigned index) {
             return PyValueAuto(LLVMGetArgOperand(self.get(), index));
           },
           "index"_a)
      .def("set_arg_operand",
           [](PyFuncletPadInst &self, unsigned index, PyValue &value) {
             return LLVMSetArgOperand(self.get(), index, value.get());
           });

  ICmpInstClass
      .def("__repr__",
           [](PyICmpInst &self) {
             return "<ICmpInst>";
           })
      .def_prop_ro("predicate",
                   [](PyInstruction &self) {
                     return LLVMGetICmpPredicate(self.get());
                   },
                   "Obtain the predicate of an instruction.");

  FCmpInstClass
      .def("__repr__",
           [](PyFCmpInst &self) {
             return "<FCmpInst>";
           })
      .def_prop_ro("predicate",
                   [](PyInstruction &self) {
                     return LLVMGetFCmpPredicate(self.get());
                   },
                   "Obtain the float predicate of an instruction.");

  GlobalVariableClass
      .def("__repr__",
           [](PyGlobalVariable &self) {
             return gen_value_repr("GlobalVariable", self);
           })
      .def("__init__",
           [](PyGlobalVariable *g, PyModule &m, PyType &type, const char *name) {
             new (g) PyGlobalVariable(LLVMAddGlobal(m.get(), type.get(), name));
           },
           "module"_a, "type"_a, "name"_a = "")
      .def_prop_rw("initializer",
                   [](PyGlobalVariable &self) {
                     return PyValueAuto(LLVMGetInitializer(self.get()));
                   },
                   [](PyGlobalVariable &self, PyConstant &c) {
                     return LLVMSetInitializer(self.get(), c.get());
                   },
                   nb::for_setter(nb::sig("def initializer(self, value: Constant) -> None")))
      .def_prop_rw("is_thread_local",
                   [](PyGlobalVariable &self) {
                     return LLVMIsThreadLocal(self.get()) != 0;
                   },
                   [](PyGlobalVariable &self, bool isThreadLocal) {
                     return LLVMSetThreadLocal(self.get(), isThreadLocal);
                   })
      .def_prop_rw("is_global_constant",
                   [](PyGlobalVariable &self) {
                     return LLVMIsGlobalConstant(self.get()) != 0;
                   },
                   [](PyGlobalVariable &self, bool isConstant) {
                     return LLVMSetGlobalConstant(self.get(), isConstant);
                   },
                   nb::for_setter
                     (nb::sig
                        ("def is_global_constant(self, is_constant: bool, /) -> None")))
      .def_prop_rw("thread_local_mode",
                   [](PyGlobalVariable &self) {
                     return LLVMGetThreadLocalMode(self.get());
                   },
                   [](PyGlobalVariable &self, LLVMThreadLocalMode mode) {
                     return LLVMSetThreadLocalMode(self.get(), mode);
                   },
                   nb::for_setter
                     (nb::sig("def thread_local_mode(self, mode: ThreadLocalMode, /) "
                              "-> None")))
      .def_prop_rw("is_externally_initialized",
                   [](PyGlobalVariable &self) {
                     return LLVMIsExternallyInitialized(self.get()) != 0;
                   },
                   [](PyGlobalVariable &self, bool isExtInit) {
                     return LLVMSetExternallyInitialized(self.get(), isExtInit);
                   },
                   nb::for_setter
                     (nb::sig
                        ("def is_externally_initialized(self, is_ext_init: bool, /)"
                         " -> None")))
      .def_prop_ro("next",
                   [](PyGlobalVariable &v) -> std::optional<PyGlobalVariable> {
                     auto res = LLVMGetNextGlobal(v.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalVariable);
                   })
      .def_prop_ro("prev",
                   [](PyGlobalVariable &v) -> std::optional<PyGlobalVariable> {
                     auto res = LLVMGetPreviousGlobal(v.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalVariable);
                   })
      .def_prop_ro("debug_loc_directory",
                   [](PyGlobalVariable &v) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocDirectory(v.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the directory of the debug location for this value")
      .def_prop_ro("debug_loc_filename",
                   [](PyGlobalVariable &v) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocFilename(v.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the filename of the debug location for this value")
      .def_prop_ro("debug_loc_line",
                   [](PyGlobalVariable &v) { return LLVMGetDebugLocLine(v.get()); },
                   "Return the line number of the debug location for this value")
      // TODO test + more suitable way? (python side also do `del v`?)
      // but python pass variable by value...
      .def("destory", 
           [](PyGlobalVariable &self) {
             return LLVMDeleteGlobal(self.get());
           },
           "Delete this variable. You are not supposed to use this variable later.");


  
  GlobalAliasClass
      .def("__repr__",
           [](PyGlobalAlias &self) {
             return gen_value_repr("GlobalAlias", self);
           })
      .def("__init__",
           [](PyGlobalAlias *g, PyModule &self, PyType &valueType, unsigned addrSpace,
              PyValue aliasee, const char *name) {
             new (g) PyGlobalAlias(LLVMAddAlias2
                                     (self.get(), valueType.get(), addrSpace,
                                      aliasee.get(), name));
           },
           "module"_a, "value_type"_a, "addr_space"_a, "aliasee"_a, "name"_a = "",
           "Add a GlobalAlias with the given value type, address space and aliasee.")
      .def_prop_ro("next",
           [](PyGlobalAlias &self) -> std::optional<PyGlobalAlias> {
             auto res = LLVMGetNextGlobalAlias(self.get());
             WRAP_OPTIONAL_RETURN(res, PyGlobalAlias);
           },
           "Advance a GlobalAlias iterator to the next GlobalAlias.\n\n"
           "Returns NULL if the iterator was already at the beginning and there are"
           "no previous global aliases.")
       .def_prop_ro("prev",
            [](PyGlobalAlias &self) -> std::optional<PyGlobalAlias> {
              auto res = LLVMGetPreviousGlobalAlias(self.get());
              WRAP_OPTIONAL_RETURN(res, PyGlobalAlias);
            },
            "Decrement a GlobalAlias iterator to the previous GlobalAlias.\n\n"
            "Returns NULL if the iterator was already at the beginning and there are"
            "no previous global aliases.")
       .def_prop_rw("aliasee",
                    [](PyGlobalAlias &self) {
                      return PyValueAuto(LLVMAliasGetAliasee(self.get()));
                    },
                    [](PyGlobalAlias &self, PyValue &aliasee) {
                      return LLVMAliasSetAliasee(self.get(), aliasee.get());
                    });
  

  FunctionClass
      .def("__repr__",
           [](PyFunction &self) {
             return gen_value_repr("Function", self);
           })
      .def("__init__",
           [](PyFunction *f, PyModule &m, PyTypeFunction &functionTy, std::string &name) {
             new (f) PyFunction(LLVMAddFunction(m.get(), name.c_str(), functionTy.get()));
           },
           "module"_a, "function_type"_a, "name"_a = "",
           "Add a function to a module under a specified name.")
      .def_prop_rw("call_conv",
                   [](PyFunction &self){
                     auto res = LLVMGetFunctionCallConv(self.get());
                     return static_cast<LLVMCallConv>(res);
                   },
                   [](PyFunction &self, LLVMCallConv CC) {
                     return LLVMSetFunctionCallConv(self.get(), CC);
                   })
      .def_prop_rw("gc",
                   [](PyFunction &self) {
                     return LLVMGetGC(self.get());
                   },
                   [](PyFunction &self, const char *name) {
                     return LLVMSetGC(self.get(), name);
                   },
                   nb::for_getter
                     ("Obtain the name of the garbage collector to use "
                      "during code generation."))
      .def_prop_ro("basic_blocks_num",
                   [](PyFunction &self) {
                     return LLVMCountBasicBlocks(self.get());
                   })
      .def_prop_ro("basic_blocks",
                   [](PyFunction &self) {
                     unsigned num = LLVMCountBasicBlocks(self.get());
                     std::vector<LLVMBasicBlockRef> BasicBlocks(num);
                     LLVMGetBasicBlocks(self.get(), BasicBlocks.data());
                     WRAP_VECTOR_FROM_DEST(PyBasicBlock, num,
                                           res, BasicBlocks);
                     return res;
                   })
      .def_prop_ro("first_basic_block",
                   [](PyFunction &self) -> optional<PyBasicBlock> {
                     auto res =  LLVMGetFirstBasicBlock(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyBasicBlock);
                   })
      .def_prop_ro("last_basic_block",
                   [](PyFunction &self) -> optional<PyBasicBlock> {
                     auto res = LLVMGetLastBasicBlock(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyBasicBlock);
                   })
      .def_prop_ro("basic_blocks",
                   [](PyFunction &self) {
                     auto res =  LLVMGetFirstBasicBlock(self.get());
                     return PyBasicBlockIterator(PyBasicBlock(res));
                   })
      .def_prop_ro("entry_basic_block",
                   [](PyFunction &self) {
                     return PyBasicBlock(LLVMGetEntryBasicBlock(self.get()));
                   })
      .def_prop_ro("has_personality_fn",
                   [](PyFunction &self) {
                     return LLVMHasPersonalityFn(self.get()) != 0;
                   })
      .def_prop_rw("personality_fn",
                   [](PyFunction &self) {
                     return PyFunction(LLVMGetPersonalityFn(self.get()));
                   },
                   [](PyFunction &self, PyFunction fn) {
                     return LLVMSetPersonalityFn(self.get(), fn.get());
                   })
      .def_prop_ro("intrinsic_id",
                   [](PyFunction &self) {
                     return LLVMGetIntrinsicID(self.get());
                   })
      .def_prop_ro("next",
                   [](PyFunction &f) -> std::optional<PyFunction> {
                     auto res = LLVMGetNextFunction(f.get());
                     WRAP_OPTIONAL_RETURN(res, PyFunction);
                   },
                   "Advance a Function iterator to the next Function.\n\n"
                   "Returns NULL if the iterator was already at the end and there are no more"
                   "functions.")
      .def_prop_ro("previous",
                   [](PyFunction &f) -> std::optional<PyFunction> {
                     auto res = LLVMGetPreviousFunction(f.get());
                     WRAP_OPTIONAL_RETURN(res, PyFunction);
                   },
                   "Decrement a Function iterator to the previous Function.\n\n"
                   "Returns NULL if the iterator was already at the beginning and there are"
                   "no previous functions.")
      .def_prop_ro("debug_loc_directory",
                   [](PyFunction &f) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocDirectory(f.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the directory of the debug location for this value")
      .def_prop_ro("debug_loc_filename",
                   [](PyFunction &f) {
                     unsigned len;
                     const char *res = LLVMGetDebugLocFilename(f.get(), &len);
                     return std::string(res, len);
                   },
                   "Return the filename of the debug location for this value")
      .def_prop_ro("debug_loc_line",
                   [](PyFunction &f) { return LLVMGetDebugLocLine(f.get()); },
                   "Return the line number of the debug location for this value")
      .def_prop_ro("param_num",
                   [](PyFunction &self) {
                     return LLVMCountParams(self.get());
                   })
      .def_prop_ro("params",
                   [](PyFunction &self) {
                     unsigned param_num = LLVMCountParams(self.get());
                     std::vector<LLVMValueRef> params(param_num);
                     LLVMGetParams(self.get(), params.data());
                     WRAP_VECTOR_FROM_DEST(PyArgument, param_num, res, params);
                     return res;
                   })
      .def_prop_ro("first_param",
                   [](PyFunction &self) -> optional<PyArgument> {
                     auto res = LLVMGetFirstParam(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyArgument);
                   })
      .def_prop_ro("last_param",
                   [](PyFunction &self) -> optional<PyArgument> {
                     auto res = LLVMGetLastParam(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyArgument);
                   })
      // .def_prop_ro("params", // also have the same name method
      //              [](PyFunction &self) {
      //                auto res = LLVMGetFirstParam(self.get());
      //                return PyArgumentIterator(PyArgument(res));
      //              })
      .def("get_param",
           [](PyFunction &self, unsigned index) {
             return PyArgument(LLVMGetParam(self.get(), index));
           },
           "index"_a,
           "Obtain the parameter at the specified index.\n\n"
           "Parameters are indexed from 0.")
      .def("destory", // TODO test
           [](PyFunction &self) {
             return LLVMDeleteFunction(self.get());
           },
           "Remove a function from its containing module and deletes it.\n\n"
           "Note you shouldn't use the the variable afterwards!")
      .def("add_attribute_at_index",
           [](PyFunction &self, LLVMAttributeIndex idx, PyAttribute attr) {
             return LLVMAddAttributeAtIndex(self.get(), idx, attr.get());
           },
           "index"_a, "attr"_a)
      .def("append_existing_basic_block",
           [](PyFunction &self, PyBasicBlock bb) {
             return LLVMAppendExistingBasicBlock(self.get(), bb.get());
           },
           "basic_block"_a,
           "Append the given basic block to the basic block list of the given function.")
      .def("append_basic_block",
           [](PyFunction &self, const char *name) {
             return PyBasicBlock(LLVMAppendBasicBlock(self.get(), name));
           },
           "name"_a = "",
           "Append a basic block to the end of a function using the global context.")
      .def("get_attribute_count_at_index",
           [](PyFunction &self, LLVMAttributeIndex idx) {
             return LLVMGetAttributeCountAtIndex(self.get(), idx);
           },
           "index"_a)
      .def("get_attributes_at_index",
           [](PyFunction &self, LLVMAttributeIndex idx) {
             unsigned cnt = LLVMGetAttributeCountAtIndex(self.get(), idx);
             std::vector<LLVMAttributeRef> attrs(cnt);
             LLVMGetAttributesAtIndex(self.get(), idx, attrs.data());
             WRAP_VECTOR_FROM_DEST_AUTO(PyAttribute, cnt, res, attrs);
             return res;
           },
           "index"_a)
      .def("get_enum_attribute_at_index",
           [](PyFunction &self, LLVMAttributeIndex idx, unsigned kindID) {
             return PyEnumAttribute(LLVMGetEnumAttributeAtIndex(self.get(), idx, kindID));
           },
           "index"_a, "kind"_a)
      .def("get_string_attribute_at_index",
           [](PyFunction &self, LLVMAttributeIndex idx, std::string &kind) {
             return PyStringAttribute(LLVMGetStringAttributeAtIndex
                         (self.get(), idx, kind.c_str(), kind.size()));
           },
           "index"_a, "kind"_a)
      .def("remove_enum_attribute_at_index",
           [](PyFunction &self, LLVMAttributeIndex idx, unsigned kindID) {
             return LLVMRemoveEnumAttributeAtIndex(self.get(), idx, kindID);
           },
           "index"_a, "kind"_a)
      .def("remove_string_attribute_at_index",
           [](PyFunction &self, LLVMAttributeIndex idx, std::string &kind) {
             return LLVMRemoveStringAttributeAtIndex
                      (self.get(), idx, kind.c_str(), kind.size());
           },
           "index"_a, "kind"_a)
      .def("add_target_dependent_attr",
           [](PyFunction &self, const char *A, const char *V) {
             return LLVMAddTargetDependentFunctionAttr(self.get(), A, V);
           },
           "A"_a, "V"_a,
           "Add a target-dependent attribute to a function");

  ArgumentClass
      .def("__repr__",
           [](PyArgument &self) {
             return gen_value_repr("Argument", self);
           })
      .def_prop_ro("parent",
                   [](PyArgument &self) {
                     return PyFunction(LLVMGetParamParent(self.get()));
                   },
                   "Obtain the function to which this argument belongs.")
      .def_prop_ro("next",
                   [](PyArgument &self) -> std::optional<PyArgument> {
                     auto res = LLVMGetNextParam(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyArgument);
                   })
      .def_prop_ro("prev",
                   [](PyArgument &self) -> std::optional<PyArgument> {
                     auto res = LLVMGetPreviousParam(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyArgument);
                   })
      .def("set_alignment",
           [](PyArgument &self, unsigned Align) {
             return LLVMSetParamAlignment(self.get(), Align);
           });


  UserClass
      .def("__repr__",
           [](PyUser &self) {
             return gen_value_repr("User", self);
           })
      .def("get_operand",
           [](PyUser &u, unsigned index) {
             return PyValueAuto(LLVMGetOperand(u.get(), index));
           },
           "index"_a,
           "Obtain an operand at a specific index.")
      .def("get_operand_use",
           [](PyUser &u, unsigned index) {
             return PyUse(LLVMGetOperandUse(u.get(), index));
           },
           "Obtain the use of an operand at a specific index")
      .def("set_operand",
           [](PyUser &u, unsigned index, PyValue &v) {
             return LLVMSetOperand(u.get(), index, v.get());
           },
           "index"_a, "value"_a,
           "Set an operand at a specific index")
       .def_prop_ro("operands_num",
                    [](PyUser &u) {
                      return LLVMGetNumOperands(u.get());
                    });

  ConstantClass
      .def("__repr__",
           [](PyConstant &self) {
             return gen_value_repr("Constant", self);
           })
      // note these constructors are also available in TypeClass
      .def_static("null",
                  [](PyType &t) {
                    return PyValueAuto(LLVMConstNull(t.get()));
                  }, "type"_a,
                  "Obtain a constant value referring to the null instance of the type.")
      .def_static("pointer_null",
                  [](PyType &t) {
                    return PyValueAuto(LLVMConstPointerNull(t.get()));
                  }, "type"_a,
                  "Obtain a constant that is a constant pointer pointing to NULL for the"
                  "type.")
      .def_static("all_ones",
                  [](PyTypeInt &t) {
                    return PyValueAuto(LLVMConstAllOnes(t.get()));
                  }, "type"_a,
                  "Obtain a constant value referring to the instance of the type"
                  "consisting of all ones.")
      .def_static("undef",
                  [](PyType &t) {
                    return PyUndefValue(LLVMGetUndef(t.get()));
                  }, "type"_a,
                  "Obtain a constant value referring to an undefined value of a type.")
      .def_static("poison",
                  [](PyType &t) {
                    return PyPoisonValue(LLVMGetPoison(t.get()));
                  }, "type"_a,
                  "Obtain a constant value referring to a poison value of a type.")
      .def_prop_ro("is_null",
                   [](PyConstant &c) { return LLVMIsNull(c.get()) != 0; });

  ConstantIntClass
      .def("__repr__",
           [](PyConstantInt &self) {
             return gen_value_repr("ConstantInt", self);
           })
      .def("__init__",
           [](PyConstantInt *c, PyTypeInt &t, unsigned long long N, bool SignExtend) {
             new (c) PyConstantInt(LLVMConstInt(t.get(), N, SignExtend));
           },
           "int_type"_a, "value"_a, "sign_extend"_a,
           "Original Function: LLVMConstInt.\n"
           "Obtain a constant value for an integer type.\n\n"
           "Parameters:\n--------"
           "int_type: IntTy Integer type to obtain value of.\n"
           "value: The value the returned instance should refer to.\n"
           "sign_extend: Whether to sign extend the produced value.")
      .def("__init__",
           [](PyConstantInt *c, PyTypeInt &t, unsigned NumWords, const uint64_t Words[]) {
             auto res = LLVMConstIntOfArbitraryPrecision
                          (t.get(), NumWords, Words);
             new (c) PyConstantInt(res);
           },
           "int_type"_a, "num_words"_a, "words"_a,
           "Origin Function: LLVMConstIntOfArbitraryPrecision.\n"
           "Obtain a constant value for an integer of arbitrary precision.")
      // LLVMConstIntOfString is discarded in favor of LLVMConstIntOfStringAndSize 
      .def("__init__", 
           [](PyConstantInt *c, PyTypeInt &t, std::string &text, uint8_t Radix) {
             new (c) PyConstantInt(LLVMConstIntOfStringAndSize
                                    (t.get(), text.c_str(), text.size(),
                                     Radix));
           },
           "int_type"_a, "text"_a, "radix"_a,
           "Original Function: LLVMConstIntOfStringAndSize\n"
           "Obtain a constant value for an integer parsed from a string.")
      .def_prop_ro("zext",
                   [](PyConstantInt &c) {
                     return LLVMConstIntGetZExtValue(c.get());
                   },
                   "Obtain the zero extended value.")
      .def_prop_ro("sext",
                   [](PyConstantInt &c) {
                     return LLVMConstIntGetSExtValue(c.get());
                   },
                   "Obtain the sign extended value.");

  ConstantFPClass
      .def("__repr__",
           [](PyConstantFP &self) {
             return gen_value_repr("ConstantFP", self);
           })
      .def("__init__",
           [](PyConstantFP *c, PyTypeReal &t, double value) {
             new (c) PyConstantFP(LLVMConstReal(t.get(), value));
           },
           "real_type"_a, "value"_a,
           "Original Function: LLVMConstReal.\n"
           "Obtain a constant value referring to a double floating point value.\n\n")
      // LLVMConstRealOfString is discarded in favor of LLVMConstRealOfStringAndSize
      .def("__init__",
           [](PyConstantFP *c, PyTypeReal &t, std::string text) {
             new (c) PyConstantFP(LLVMConstRealOfStringAndSize
                                   (t.get(), text.c_str(), text.size()));
           },
           "type"_a, "text"_a,
           "Original Function: LLVMConstRealOfStringAndSize\n"
           "Obtain a constant for a floating point value parsed from a string.")
      .def_prop_ro("double",
                   [](PyConstantFP &c) {
                     LLVMBool losesInfo;
                     double res = LLVMConstRealGetDouble(c.get(), &losesInfo);
                     return std::make_tuple(res, losesInfo);
                   },
                   "Obtain the double value for an floating point constant value.\n\n"
                   "The returned value is a tuple, with first one as the double"
                   "value and the second one indicating whether the some precision"
                   "was lost in the conversion.");

  ConstantDataArrayClass
      .def("__repr__",
           [](PyConstantDataArray &self) {
             return gen_value_repr("ConstantDataArray", self);
           })
       .def("__str__",
            [](PyConstantDataArray &c) {
              size_t len;
              auto str = LLVMGetAsString(c.get(), &len);
              return std::string(str, len);
            },
            "Get the given constant data sequential as a string.")
      .def_static("create_string_in_context",
                  [](PyContext &cxt, std::string &str, bool DontNullTerminate) {
                    return PyConstantDataArray
                             (LLVMConstStringInContext
                                (cxt.get(), str.c_str(), str.size(),
                                 DontNullTerminate));
                  },
                  "context"_a, "str"_a, "dont_null_terminate"_a,
                  "Create a ConstantDataSequential and initialize it"
                  "with a string.")
      .def_static("create_string",
                  [](std::string &str, bool DontNullTerminate) {
                    return PyConstantDataArray
                             (LLVMConstString
                                (str.c_str(), str.size(), DontNullTerminate));
                  },
                  "str"_a, "dont_null_terminate"_a,
                  "Create a ConstantDataSequential with string content"
                  "in the global context.")
       .def_prop_ro("is_string",
                    [](PyConstantDataArray &c) {
                      return LLVMIsConstantString(c.get()) != 0;
                    },
                    "Returns true if the specified constant is an array of i8.")
       .def("as_string",
            [](PyConstantDataArray &c) {
              size_t len;
              auto str = LLVMGetAsString(c.get(), &len);
              return std::string(str, len);
            },
            "Get the given constant data sequential as a string.");

  ConstantStructClass
      .def("__repr__",
           [](PyConstantStruct &self) {
             return gen_value_repr("ConstantStruct", self);
           })
      .def("__init__",
           [](PyConstantStruct *c, PyContext &cxt,
              std::vector<PyConstant> &vc, bool packed) {
             unsigned cnt = vc.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, vc, raw, cnt);
             new (c) PyConstantStruct(LLVMConstStructInContext
                                        (cxt.get(), raw.data(), cnt, packed));
           },
           "context"_a, "constant_values"_a, "packed"_a,
           "Create an anonymous ConstantStruct with the specified"
           "values.")
      .def("__init__",
           [](PyConstantStruct *c, PyType &structTy,
              std::vector<PyConstant> &constantVals) {
             unsigned cnt = constantVals.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, constantVals,
                                         raw, cnt);
             new (c) PyConstantStruct(LLVMConstNamedStruct(structTy.get(),
                                                           raw.data(), cnt));
           },
           "struct_type"_a, "constant_values"_a,
           "Create a non-anonymous ConstantStruct from values.")
      .def("get_element_at",
           [](PyConstantStruct &c, unsigned idx) -> std::optional<PyValue*>{
             auto res = LLVMGetAggregateElement(c.get(), idx);
             WRAP_OPTIONAL_RETURN(res, PyValueAuto);
           },
           "index"_a,
           "Returns null if the index is out of range, or it's not possible to "
           "determine the element (e.g., because the constant is a constant "
           "expression.)");
  

  ConstantArrayClass
      .def("__repr__",
           [](PyConstantArray &self) {
             return gen_value_repr("ConstantArray", self);
           })
       // LLVMConstArray is deprecated in favor of LLVMConstArray2
       .def("__init__",
            [](PyConstantArray *c, PyType &elemType,
               std::vector<PyConstant> ConstantVals) {
              uint64_t cnt = ConstantVals.size();
              UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, ConstantVals, raw, cnt);
              new (c) PyConstantArray(LLVMConstArray2(elemType.get(),
                                                      raw.data(), cnt));
            },
            "elem_type"_a, "constant_vals"_a,
            "Create a ConstantArray from values.")
       .def("get_element_at",
            [](PyConstantArray &c, unsigned idx) -> std::optional<PyValue*>{
              auto res = LLVMGetAggregateElement(c.get(), idx);
              WRAP_OPTIONAL_RETURN(res, PyValueAuto);
            },
            "index"_a,
            "Returns null if the index is out of range, or it's not possible to "
            "determine the element (e.g., because the constant is a constant "
            "expression.)");

  ConstantVectorClass
      .def("__repr__",
           [](PyConstantVector &self) {
             return gen_value_repr("ConstantVector", self);
           })
      .def("__init__",
           [](PyConstantVector *c, std::vector<PyConstant> values) {
             auto cnt = values.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, values, raw, cnt);
             new (c) PyConstantVector(LLVMConstVector(raw.data(), cnt));
           }, "values"_a)
      // LLVMGetElementAsConstant is deprecated in favor of  LLVMGetAggregateElement
      .def("get_element_at",
           [](PyConstantVector &c, unsigned idx) -> std::optional<PyValue*>{
             auto res = LLVMGetAggregateElement(c.get(), idx);
             WRAP_OPTIONAL_RETURN(res, PyValueAuto);
           },
           "index"_a,
           "Returns null if the index is out of range, or it's not possible to "
           "determine the element (e.g., because the constant is a constant "
           "expression.)");


  ConstantExprClass
      .def("__repr__",
           [](PyConstantExpr &self) {
             return gen_value_repr("ConstantExpr", self);
           })
      .def_prop_ro("opcode",
                  [](PyConstantExpr &c) {
                    return LLVMGetConstOpcode(c.get());
                  },
                   "value"_a)
      .def("get_icmp_predicate", // TODO test
           [](PyConstantExpr &self) {
             return LLVMGetICmpPredicate(self.get());
           },
           "Obtain the predicate of an instruction."
           "The opcode needs to be llvm::Instruction::ICmp.")
       .def("get_fcmp_predicate", // TODO test
            [](PyConstantExpr &self) {
              return LLVMGetFCmpPredicate(self.get());
            },
            "Obtain the float predicate of an instruction."
            "The opcode needs to be llvm::Instruction::FCmp.")
      .def_static("get_align_of",
                   [](PyType &t) {
                     return PyConstantExpr(LLVMAlignOf(t.get()));
                   },
                  "type"_a)
      .def_static("get_size_of",
                   [](PyType &t) {
                     return PyConstantExpr(LLVMSizeOf(t.get()));
                   },
                  "type"_a)
      .def_static("neg",
                  [](PyConstant &c) {
                    return PyValueAuto(LLVMConstNeg(c.get()));
                  },
                  "value"_a)
      .def_static("neg_nsw", // no signed wrap
                  [](PyConstant &c) {
                    return PyValueAuto(LLVMConstNSWNeg(c.get()));
                  },
                  "value"_a,
                  "LLVMConstNSWNeg")
      .def_static("neg_nuw", // no unsigned wrap
                   [](PyConstant &c) {
                     return PyValueAuto(LLVMConstNUWNeg(c.get()));
                   },
                   "value"_a,
                   "LLVMConstNUWNeg")
      .def_static("not_",
                  [](PyConstant &c) {
                    return PyValueAuto(LLVMConstNot(c.get()));
                  },
                  "value"_a)
  CONSTANT_EXPR_BIND_BINARY_OPS
      .def_static("icmp", // test passing ConstantFP type values
                  [](LLVMIntPredicate predicate, PyConstant &lhs, PyConstant &rhs) {
                    return PyValueAuto(LLVMConstICmp(predicate, lhs.get(), rhs.get()));
                  },
                  "predicate"_a, "lhs"_a, "rhs"_a)
      .def_static("fcmp", // test passing ConstantFP type values
                  [](LLVMRealPredicate predicate, PyConstant &lhs, PyConstant &rhs) {
                    return PyValueAuto(LLVMConstFCmp(predicate, lhs.get(), rhs.get()));
                  },
                  "predicate"_a, "lhs"_a, "rhs"_a)

      .def_static("gep2",
                  [](PyType &type, PyConstant &value, std::vector<PyValue> indices) {
                    unsigned num = indices.size();
                    UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, indices, raw, num);
                    return PyValueAuto(LLVMConstGEP2(type.get(), value.get(), raw.data(),
                                                     num));
                  },
                  "type"_a, "value"_a, "indices"_a)
      .def_static("gep2_in_bounds",
                  [](PyType &type, PyConstant &value, std::vector<PyValue> indices) {
                    unsigned num = indices.size();
                    UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, indices, raw, num);
                    return PyValueAuto(LLVMConstInBoundsGEP2(type.get(), value.get(),
                                                             raw.data(), num));
                  },
                  "type"_a, "value"_a, "indices"_a)

      .def_static("trunc",
                  [](PyConstant &value, PyType &toType) {
                    return PyValueAuto(LLVMConstTrunc(value.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("ptr2int",
                  [](PyConstant &c, PyType &toType) {
                    return PyValueAuto(LLVMConstIntToPtr(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("int2ptr",
                  [](PyConstant &c, PyType &toType) {
                    return PyValueAuto(LLVMConstPtrToInt(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("bit_cast",
                  [](PyConstant &c, PyType &toType) {
                    return PyValueAuto(LLVMConstBitCast(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("addr_space_cast",
                  [](PyConstant &c, PyType &toType) {
                    return PyValueAuto(LLVMConstAddrSpaceCast(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("trunc_or_bit_cast",
                  [](PyConstant &c, PyType &toType) {
                    return PyValueAuto(LLVMConstTruncOrBitCast(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("pointer_cast",
                  [](PyConstant &c, PyType &toType) {
                    return PyValueAuto(LLVMConstPointerCast(c.get(), toType.get()));
                  },
                  "value"_a, "to_type"_a)
      .def_static("extract_element", // TODO test ConstantArray
                  [](PyConstantVector &vector, PyConstant &index) {
                    return PyValueAuto(LLVMConstExtractElement(vector.get(), index.get()));
                  },
                  "vector"_a, "index"_a)
      .def_static("insert_element",
                  [](PyConstantVector &vector, PyConstant &value, PyConstant &index) {
                    return PyValueAuto(LLVMConstInsertElement(vector.get(), value.get(),
                                                              index.get()));
                  },
                  "vector"_a, "index"_a, "index"_a)
      .def_static("shuffle_vector",
                  [](PyConstantVector &vectorA, PyConstantVector &vectorB,
                     PyConstant &mask) {
                    return PyValueAuto(LLVMConstShuffleVector
                                         (vectorA.get(), vectorB.get(), mask.get()));
                  },
                  "vector_a"_a, "vector_b"_a, "mask"_a)
      .def_static("block_address",
                  [](PyConstant &value, PyBasicBlock &bb) {
                    return PyValueAuto(LLVMBlockAddress(value.get(), bb.get()));
                  });

  
  // LLVMConstInlineAsm is deprecated in favor of LLVMGetInlineAsm

  GlobalValueClass
      .def("__repr__",
           [](PyGlobalValue &self) {
             return gen_value_repr("GlobalValue", self);
           })
      .def_prop_rw("alignment",
                   [](PyGlobalValue &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PyGlobalValue &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_ro("parent",
                   [](PyGlobalValue &v) {
                     auto res = LLVMGetGlobalParent(v.get());
                     return PyModule(res);
                   })
      .def_prop_ro("is_declaration",
                   [](PyGlobalValue &v) {
                     return LLVMIsDeclaration(v.get()) != 0;
                   })
      .def_prop_rw("linkage",
                   [](PyGlobalValue &v) {
                     return LLVMGetLinkage(v.get());
                   },
                   [](PyGlobalValue &v, LLVMLinkage linkage) {
                     return LLVMSetLinkage(v.get(), linkage);
                   },
                   nb::for_setter
                     (nb::sig("def linkage(self, linkage: Linkage, /) -> None")))
      .def_prop_rw("section",
                   [](PyGlobalValue &v) {
                     return LLVMGetSection(v.get());
                   },
                   [](PyGlobalValue &v, const char *Section) {
                     return LLVMSetSection(v.get(), Section);
                   },
                   nb::for_setter
                     (nb::sig("def section(self, section: str, /) -> None")))
      .def_prop_rw("visibility",
                   [](PyGlobalValue &v) {
                     return LLVMGetVisibility(v.get());
                   },
                   [](PyGlobalValue &v, LLVMVisibility visibility) {
                     return LLVMSetVisibility(v.get(), visibility);
                   },
                   nb::for_setter
                     (nb::sig
                        ("def visibility(self, visibility: Visibility, /) -> None")))
      .def_prop_rw("dll_storage_class",
                   [](PyGlobalValue &v) {
                     return LLVMGetDLLStorageClass(v.get());
                   },
                   [](PyGlobalValue &v, LLVMDLLStorageClass cls) {
                     return LLVMSetDLLStorageClass(v.get(), cls);
                   })
      // LLVMHasUnnamedAddr is deprecated in favor of LLVMGetUnnamedAddress
      // LLVMSetUnnamedAddr is deprecated in favor of LLVMSetUnnamedAddress
      .def_prop_rw("unnamed_address",
                   [](PyGlobalValue &v) {
                     return LLVMGetUnnamedAddress(v.get());
                   },
                   [](PyGlobalValue &v, LLVMUnnamedAddr unnamedAddr) {
                     return LLVMSetUnnamedAddress(v.get(), unnamedAddr);
                   },
                   nb::for_setter
                     (nb::sig("def unnamed_address(self, addr: UnnamedAddr) -> None")))
      .def_prop_ro("value_type",
                   [](PyGlobalValue &v) {
                     return PyTypeAuto(LLVMGlobalGetValueType(v.get()));
                   },
                   "Returns the \"value type\" of a global value.  This differs "
                   "from the formal type of a global value which is always a pointer"
                   " type.");


  AllocaInstClass
      .def("__repr__",
           [](PyAllocaInst &self) {
             return "<AllocaInst>";
           })
      .def_prop_rw("alignment",
                   [](PyAllocaInst &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PyAllocaInst &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_ro("type",
                   [](PyAllocaInst &self) {
                     return PyTypeAuto(LLVMGetAllocatedType(self.get()));
                   });

  GetElementPtrInstClass
      .def("__repr__",
           [](PyGetElementPtrInst &self) {
             return "<GetElementPtrInst>";
           })
      .def_prop_ro("indices_num",
           [](PyGetElementPtrInst &self) {
             return LLVMGetNumIndices(self.get());
           })
      .def_prop_rw("is_bounds",
                   [](PyGetElementPtrInst &self) {
                     return LLVMIsInBounds(self.get()) != 0;
                   },
                   [](PyGetElementPtrInst &self, bool inBounds) {
                     return LLVMSetIsInBounds(self.get(), inBounds);
                   })
      .def_prop_ro("source_element_type",
                   [](PyGetElementPtrInst &self) {
                     return PyTypeAuto(LLVMGetGEPSourceElementType(self.get()));
                   });

  PHINodeClass
      .def("__repr__",
           [](PyPHINode &self) {
             return "<PHINode>";
           })
      .def_prop_ro("incoming_num",
                   [](PyPHINode &self) {
                     return LLVMCountIncoming(self.get());
                   },
                   "Obtain the number of incoming basic blocks to a PHI node.")
      .def("add_incoming",
           [](PyPHINode &self, std::vector<PyValue> incomingValues,
              std::vector<PyBasicBlock> incomingBlocks, unsigned Count) {
             unsigned cnt = incomingValues.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, incomingValues,
                                         rawValues, cnt);
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMBasicBlockRef, incomingBlocks,
                                         rawBlocks, cnt);
             return LLVMAddIncoming(self.get(), rawValues.data(), rawBlocks.data(),
                                    cnt);
           })
      .def("get_incoming_value",
           [](PyPHINode &self, unsigned index) {
             return PyValueAuto(LLVMGetIncomingValue(self.get(), index));
           })
      .def("get_incoming_block",
           [](PyPHINode &self, unsigned index) {
             return PyBasicBlock(LLVMGetIncomingBlock(self.get(), index));
           });

  FenceInstClass
      .def("__repr__",
           [](PyFenceInst &self) {
             return "<FenceInst>";
           })
      .def_prop_rw("ordering",
                   [](PyFenceInst &self) {
                     return LLVMGetOrdering(self.get());
                   },
                   [](PyFenceInst &self, LLVMAtomicOrdering ordering) {
                     return LLVMSetOrdering(self.get(), ordering);
                   })
      .def_prop_rw("is_single_thread",
                   [](PyFenceInst &self) {
                     return LLVMIsAtomicSingleThread(self.get()) != 0;
                   },
                   [](PyFenceInst &self, bool isSingleThread) {
                     return LLVMSetAtomicSingleThread(self.get(), isSingleThread);
                   });

  LoadInstClass
      .def("__repr__",
           [](PyLoadInst &self) {
             return "<LoadInst>";
           })
      .def_prop_rw("alignment",
                   [](PyLoadInst &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PyLoadInst &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_rw("is_volatile",
                   [](PyLoadInst &self) {
                     return LLVMGetVolatile(self.get()) != 0;
                   },
                   [](PyLoadInst &self, bool isVolatile) {
                     return LLVMSetVolatile(self.get(), isVolatile);
                   })
      .def_prop_rw("ordering",
                   [](PyLoadInst &self) {
                     return LLVMGetOrdering(self.get());
                   },
                   [](PyLoadInst &self, LLVMAtomicOrdering ordering) {
                     return LLVMSetOrdering(self.get(), ordering);
                   })
      .def_prop_rw("is_single_thread",
                   [](PyLoadInst &self) {
                     return LLVMIsAtomicSingleThread(self.get()) != 0;
                   },
                   [](PyLoadInst &self, bool isSingleThread) {
                     return LLVMSetAtomicSingleThread(self.get(), isSingleThread);
                   });

  StoreInstClass
      .def("__repr__",
           [](PyStoreInst &self) {
             return "<StoreInst>";
           })
      .def_prop_rw("alignment",
                   [](PyStoreInst &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PyStoreInst &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_rw("is_volatile",
                   [](PyStoreInst &self) {
                     return LLVMGetVolatile(self.get()) != 0;
                   },
                   [](PyStoreInst &self, bool isVolatile) {
                     return LLVMSetVolatile(self.get(), isVolatile);
                   })
      .def_prop_rw("ordering",
                   [](PyStoreInst &self) {
                     return LLVMGetOrdering(self.get());
                   },
                   [](PyStoreInst &self, LLVMAtomicOrdering ordering) {
                     return LLVMSetOrdering(self.get(), ordering);
                   })
      .def_prop_rw("is_single_thread",
                   [](PyStoreInst &self) {
                     return LLVMIsAtomicSingleThread(self.get()) != 0;
                   },
                   [](PyStoreInst &self, bool isSingleThread) {
                     return LLVMSetAtomicSingleThread(self.get(), isSingleThread);
                   });
  
  AtomicRMWInstClass
      .def("__repr__",
           [](PyAtomicRMWInst &self) {
             return "<AtomicRMWInst>";
           })
      .def_prop_rw("alignment",
                   [](PyAtomicRMWInst &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PyAtomicRMWInst &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_rw("is_volatile",
                   [](PyAtomicRMWInst &self) {
                     return LLVMGetVolatile(self.get()) != 0;
                   },
                   [](PyAtomicRMWInst &self, bool isVolatile) {
                     return LLVMSetVolatile(self.get(), isVolatile);
                   })
      .def_prop_rw("ordering",
                   [](PyAtomicRMWInst &self) {
                     return LLVMGetOrdering(self.get());
                   },
                   [](PyAtomicRMWInst &self, LLVMAtomicOrdering ordering) {
                     return LLVMSetOrdering(self.get(), ordering);
                   })
      .def_prop_rw("bin_op",
                   [](PyAtomicRMWInst &self) {
                     return LLVMGetAtomicRMWBinOp(self.get());
                   },
                   [](PyAtomicRMWInst &self, LLVMAtomicRMWBinOp binOp) {
                     return LLVMSetAtomicRMWBinOp(self.get(), binOp);
                   })
      .def_prop_rw("is_single_thread",
                   [](PyAtomicRMWInst &self) {
                     return LLVMIsAtomicSingleThread(self.get()) != 0;
                   },
                   [](PyAtomicRMWInst &self, bool isSingleThread) {
                     return LLVMSetAtomicSingleThread(self.get(), isSingleThread);
                   });
  
  AtomicCmpXchgInstClass
      .def("__repr__",
           [](PyAtomicCmpXchgInst &self) {
             return "<AtomicCmpXchgInst>";
           })
      .def_prop_rw("alignment",
                   [](PyAtomicCmpXchgInst &v) {
                     return LLVMGetAlignment(v.get());
                   },
                   [](PyAtomicCmpXchgInst &v, unsigned Bytes) {
                     return LLVMSetAlignment(v.get(), Bytes);
                   },
                   nb::for_setter
                     (nb::sig("def alignment(self, bytes: int, /) -> None")))
      .def_prop_rw("is_volatile",
                   [](PyLoadInst &self) {
                     return LLVMGetVolatile(self.get()) != 0;
                   },
                   [](PyLoadInst &self, bool isVolatile) {
                     return LLVMSetVolatile(self.get(), isVolatile);
                   })
      .def_prop_rw("is_weak",
                   [](PyAtomicCmpXchgInst &self) {
                     return LLVMGetWeak(self.get());
                   },
                   [](PyAtomicCmpXchgInst &self, bool isWeak) {
                     return LLVMSetWeak(self.get(), isWeak);
                   })
      .def_prop_rw("is_single_thread",
                   [](PyAtomicCmpXchgInst &self) {
                     return LLVMIsAtomicSingleThread(self.get()) != 0;
                   },
                   [](PyAtomicCmpXchgInst &self, bool isSingleThread) {
                     return LLVMSetAtomicSingleThread(self.get(), isSingleThread);
                   })
      .def_prop_rw("success_ordering",
                   [](PyAtomicCmpXchgInst &self) {
                     return LLVMGetCmpXchgSuccessOrdering(self.get());
                   },
                   [](PyAtomicCmpXchgInst &self, LLVMAtomicOrdering ordering) { 
                     return LLVMSetCmpXchgSuccessOrdering(self.get(), ordering);
                   })
      .def_prop_rw("failure_ordering",
                   [](PyAtomicCmpXchgInst &self) {
                     return LLVMGetCmpXchgFailureOrdering(self.get());
                   },
                   [](PyAtomicCmpXchgInst &self, LLVMAtomicOrdering ordering) {
                     return LLVMSetCmpXchgFailureOrdering(self.get(), ordering);
                   });

  GlobalObjectClass
      .def("__repr__",
           [](PyGlobalObject &self) {
             return gen_value_repr("GlobalObject", self);
           })
      .def("set_metadata",
           [](PyGlobalObject &g, unsigned kind, PyMetadata md) {
             return LLVMGlobalSetMetadata(g.get(), kind, md.get());
           },
           "Sets a metadata attachment, erasing the existing metadata attachment if"
           "it already exists for the given kind.")
      .def("erase_metadata",
           [](PyGlobalObject &g, unsigned kind) {
             return LLVMGlobalEraseMetadata(g.get(), kind);
           },
           "Erases a metadata attachment of the given kind if it exists.")
      .def("clear_metadata",
           [](PyGlobalObject &g) {
             return LLVMGlobalClearMetadata(g.get());
           },
           "Removes all metadata attachments from this value.")
      .def("copy_all_metadata",
           [](PyGlobalObject &g) {
             size_t num;
             LLVMValueMetadataEntries entries = LLVMGlobalCopyAllMetadata
                                                  (g.get(), &num);
             return PyMetadataEntries(entries, num);
           },
           "Retrieves an array of metadata entries representing the metadata attached to"
           "this value.");
  
  IndirectBrInstClass
      .def("__repr__",
           [](PyIndirectBrInst &self) {
             return "<IndirectBrInst>";
           })
      .def("add_destination",
           [](PyIndirectBrInst &self, PyBasicBlock &dest) {
             return LLVMAddDestination(self.get(), dest.get());
           },
           "dest"_a,
           "Add a destination to the indirectbr instruction.");

  LandingPadInstClass
      .def("__repr__",
           [](PyLandingPadInst &self) {
             return "<LandingPadInst>";
           })
      .def_prop_ro("num_clauses",
                   [](PyIndirectBrInst &self) {
                     return LLVMGetNumClauses(self.get());
                   })
      .def_prop_rw("is_cleanup",
                   [](PyLandingPadInst &self) {
                     return LLVMIsCleanup(self.get()) != 0;
                   },
                   [](PyLandingPadInst &self, bool val) {
                     return LLVMSetCleanup(self.get(), val);
                   })
      .def("get_clause",
           [](PyIndirectBrInst &self, unsigned index) {
             return PyValueAuto(LLVMGetClause(self.get(), index));
           },
           "index"_a)
      .def("add_clause",
           [](PyIndirectBrInst &self, PyConstant clause) {
             return LLVMAddClause(self.get(), clause.get());
           });

}





void bindOtherClasses(nb::module_ &m) {
  auto ContextClass =
    nb::class_<PyContext>
      (m, "Context",
       "Contexts are execution states for the core LLVM IR system.\n\n"
       "Most types are tied to a context instance. Multiple contexts can"
       "exist simultaneously. A single context is not thread safe. However,"
       "different contexts can execute on different threads simultaneously.");

  auto AttributeClass = nb::class_<PyAttribute>(m, "Attribute", "Attribute");
  auto EnumAttributeClass = nb::class_<PyEnumAttribute, PyAttribute>
                              (m, "EnumAttribute", "EnumAttribute");
  auto TypeAttributeClass = nb::class_<PyTypeAttribute, PyAttribute>
                              (m, "TypeAttribute", "TypeAttribute");
  auto StringAttributeClass = nb::class_<PyStringAttribute, PyAttribute>
                                (m, "StringAttribute", "StringAttribute");
  
  
  auto BasicBlockClass = nb::class_<PyBasicBlock>
                                  (m, "BasicBlock", "BasicBlock");
  auto DiagnosticInfoClass = nb::class_<PyDiagnosticInfo>
                               (m, "DiagnosticInfo", "DiagnosticInfo");

  auto NamedMDNodeClass = nb::class_<PyNamedMDNode>(m, "NamedMDNode", "NamedMDNode");
  auto ModuleClass =
    nb::class_<PyModule>
      (m, "Module",
       "Modules represent the top-level structure in an LLVM program. An LLVM"
       "module is effectively a translation unit or a collection of translation "
       "units merged together.");

  auto ModuleFlagEntriesClass = nb::class_<PyModuleFlagEntries>
                                  (m, "ModuleFlagEntry", "ModuleFlagEntry");
  auto MetadataClass = nb::class_<PyMetadata>(m, "Metadata", "Metadata");
  auto MDNodeClass = nb::class_<PyMDNode, PyMetadata>(m, "MDNode", "MDNode");
  auto MDStringClass = nb::class_<PyMDString, PyMetadata>(m, "MDString", "MDString");
  auto ValueAsMetadata = nb::class_<PyValueAsMetadata, PyMetadata>
                           (m, "ValueAsMetadata", "ValueAsMetadata");

  
  auto MetadataEntriesClass = nb::class_<PyMetadataEntries>
                              (m, "MetadataEntry", "MetadataEntry");
  
  auto UseClass = nb::class_<PyUse>(m, "Use", "Use");

  auto IntrinsicClass = nb::class_<PyIntrinsic>(m, "Intrinsic", "Intrinsic");
  auto OperandBundleClass = nb::class_<PyOperandBundle>(m, "OperandBundle",
                                                        "OperandBundle");
  auto BuilderClass = nb::class_<PyBuilder>(m, "Builder", "Builder");
  auto ModuleProviderClass = nb::class_<PyModuleProvider>
                               (m, "ModuleProvider", "ModuleProvider");
  auto MemoryBufferClass = nb::class_<PyMemoryBuffer>
                             (m, "MemoryBuffer", "MemoryBuffer");
  
  // no need to create PyPassManagerBase binding
  auto PassManagerClass = nb::class_<PyPassManager>
                            (m, "PassManager", "PassManager");
  auto FunctionPassManagerClass = nb::class_<PyFunctionPassManager>
                                    (m, "FunctionPassManager", "FunctionPassManager");

  MetadataClass
      .def("__repr__",
           [](PyMetadata &self) {
             return "<Metadata>";
           })
      .def_static("get_md_kind_id",
                  [](const std::string &name) {
                    return LLVMGetMDKindID(name.c_str(), name.size());
                  },
                  "name"_a = "")
      .def("as_value",
           [](PyMetadata &self, PyContext &c) {
             return PyMetadataAsValue(LLVMMetadataAsValue(c.get(), self.get()));
           },
           "context"_a);


  MDStringClass
      .def("__repr__",
           [](PyMDString &self) {
             return "<MDString>";
           })
      .def("__init__",
           [](PyMDString *mds, PyContext &context, std::string &name) {
             new (mds) PyMDString(LLVMMDStringInContext2
                                    (context.get(), name.c_str(), name.size()));
           },
           "context"_a, "name"_a = "",
           "Create an MDString value from a given string value.")
      .def("as_value",
           [](PyMDString &self, PyContext &c) {
             return PyMDStringValue(LLVMMetadataAsValue(c.get(), self.get()));
           },
           "context"_a);

  MDNodeClass
      .def("__repr__",
           [](PyMDNode &self) {
             return "<MDNode>";
           })
      .def("__init__",
           [](PyMDNode *mdd, PyContext &c, std::vector<PyMetadata> mds) {
             size_t num = mds.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMMetadataRef, mds, raw, num);
             new (mdd) PyMDNode(LLVMMDNodeInContext2(c.get(), raw.data(), num));
           },
           "context"_a, "metadata"_a)
      .def("as_value",
           [](PyMDNode &self, PyContext &c) {
             return PyMDNodeValue(LLVMMetadataAsValue(c.get(), self.get()));
           },
           "context"_a);


  PassManagerClass
      .def("__repr__",
           [](PyPassManager &self) {
             return "<PassManager>";
           })
      .def("__init__",
           [](PyPassManager *pm) {
             new (pm) PyPassManager(LLVMCreatePassManager());
           },
           "Constructs a new whole-module pass pipeline. This type of pipeline is"
           "suitable for link-time optimization and whole-module transformations.")
      .def("run",
           [](PyPassManager &self, PyModule &module) {
             return LLVMRunPassManager(self.get(), module.get()) != 0;
           },
           "module"_a,
           "Initializes, executes on the provided module, and finalizes all of the"
           "passes scheduled in the pass manager. Returns true if any of the passes"
           "modified the module, false otherwise.");

  FunctionPassManagerClass
      .def("__repr__",
           [](PyFunctionPassManager &self) {
             return "<FunctionPassManager>";
           })
      .def("__init__",
           [](PyFunctionPassManager *fpm, PyModule &module) {
             new (fpm) PyFunctionPassManager(LLVMCreateFunctionPassManagerForModule
                                               (module.get()));
           },
           "module"_a,
           "Constructs a new function-by-function pass pipeline over the module"
           "provider. It does not take ownership of the module provider. This type of"
           "pipeline is suitable for code generation and JIT compilation tasks.")
      .def("initialize",
           [](PyFunctionPassManager &self) {
             return LLVMInitializeFunctionPassManager(self.get()) != 0;
           },
           "Initializes all of the function passes scheduled in the function pass"
           "manager. Returns true if any of the passes modified the module, false "
           "otherwise.")
      .def("run",
           [](PyFunctionPassManager &self, PyFunction f) {
             return LLVMRunFunctionPassManager(self.get(), f.get()) != 0;
           },
           "f"_a,
           "Executes all of the function passes scheduled in the function pass manager"
           "on the provided function. Returns true if any of the passes modified the"
           "function, false otherwise.")
      .def("finalize",
           [](PyFunctionPassManager &self) {
             return LLVMFinalizeFunctionPassManager(self.get()) != 0;
           },
           "Finalizes all of the function passes scheduled in the function pass"
           "manager. Returns 1 if any of the passes modified the module, 0 otherwise.");
  

  ModuleProviderClass
      .def("__repr__",
           [](PyModuleProvider &self) {
             return "<ModuleProvider>";
           })
      .def("create_function_pass_manager",
           [](PyModuleProvider &self) {
             return PyFunctionPassManager(LLVMCreateFunctionPassManager(self.get()));
           },
           "Deprecated: Use :func:`Module.create_function_pass_manager` instead.");

  MemoryBufferClass
      .def("__repr__",
           [](PyMemoryBuffer &self) {
             return "<MemoryBuffer>";
           })
      .def_static("from_file",
                  [](const char *Path) {
                    LLVMMemoryBufferRef OutMemBuf;
                    char *OutMessage;
                    auto success = LLVMCreateMemoryBufferWithContentsOfFile
                                     (Path, &OutMemBuf, &OutMessage);
                    if (!success) {
                      throw std::runtime_error(OutMessage);
                    }

                    return PyMemoryBuffer(OutMemBuf);
                  },
                  "path"_a,
                  ":raises RuntimeError")
      .def_static("from_stdin",
                  []() {
                    LLVMMemoryBufferRef OutMemBuf;
                    char *OutMessage;
                    auto success = LLVMCreateMemoryBufferWithSTDIN
                                     (&OutMemBuf, &OutMessage);
                    if (!success) {
                      throw std::runtime_error(OutMessage);
                    }
                    return PyMemoryBuffer(OutMemBuf);
                  },
                  ":raises RuntimeError")
      .def_static("from_str",
                  [](std::string &inputData, const char *BufferName, bool RequiresNullTerminator) {
                    return PyMemoryBuffer(LLVMCreateMemoryBufferWithMemoryRange
                                            (inputData.c_str(), inputData.size(),
                                             BufferName, RequiresNullTerminator));
                  },
                  "input_data"_a, "buffer_name"_a, "requires_null_terminator"_a)
      .def_static("from_str",
                  [](const std::string &inputData, const char *BufferName) ->
                   optional<PyMemoryBuffer>{
                    auto res = LLVMCreateMemoryBufferWithMemoryRangeCopy
                                 (inputData.c_str(), inputData.size(), BufferName);
                    WRAP_OPTIONAL_RETURN(res, PyMemoryBuffer);
                  },
                  "input_data"_a, "buffer_name"_a)
      .def_prop_ro("buffer_start",
                   [](PyMemoryBuffer &self) {
                     return LLVMGetBufferStart(self.get());
                   })
      .def_prop_ro("buffer_size",
                   [](PyMemoryBuffer &self) {
                     return LLVMGetBufferSize(self.get());
                   });
  

  BuilderClass
      .def("__repr__",
           [](PyBuilder &self) {
             return "<Builder>";
           })
      .def("__init__",
           [](PyBuilder *b, PyContext &c) {
             new (b) PyBuilder(LLVMCreateBuilderInContext(c.get()));
           })
      .def("__init__",
           [](PyBuilder *b) {
             new (b) PyBuilder(LLVMCreateBuilder());
           })
      .def_prop_ro("insert_block",
                   [](PyBuilder &self) {
                     return PyBasicBlock(LLVMGetInsertBlock(self.get()));
                   })
      // note LLVMSetCurrentDebugLocation is deprecated in favor of
      // LLVMSetCurrentDebugLocation2. Also the LLVMGetCurrentDebugLocation
      .def_prop_rw("current_debug_location",
                   [](PyBuilder &self) {
                     return PyMetadata(LLVMGetCurrentDebugLocation2(self.get()));
                   },
                   [](PyBuilder &self, PyMetadata &loc) {
                     return LLVMSetCurrentDebugLocation2(self.get(), loc.get());
                   })
      .def_prop_rw("default_fp_math_tag",
                   [](PyBuilder &self) {
                     return PyMetadata(LLVMBuilderGetDefaultFPMathTag(self.get()));
                   },
                   [](PyBuilder &self, PyMetadata &fpMathTag) {
                     return LLVMBuilderSetDefaultFPMathTag(self.get(), fpMathTag.get());
                   },
                   nb::for_getter("Get the dafult floating-point math metadata."))
      .def("position",
           [](PyBuilder &self, PyBasicBlock bb, PyInstruction &inst) {
             return LLVMPositionBuilder(self.get(), bb.get(), inst.get());
           },
           "Original Function: LLVMPositionBuilder.")
      .def("position_before",
           [](PyBuilder &self, PyInstruction &inst) {
             return LLVMPositionBuilderBefore(self.get(), inst.get());
           },
           "instruction"_a,
           "Original function: LLVMPositionBuilderBefore.")
      .def("position_at_end",
           [](PyBuilder &self, PyBasicBlock bb) {
             return LLVMPositionBuilderAtEnd(self.get(), bb.get());
           },
           "basicblock"_a,
           "Original function: LLVMPositionBuilderAtEnd.")
      .def("clear_insert_position",
           [](PyBuilder &self) {
             return LLVMClearInsertionPosition(self.get());
           })
      .def("insert",
           [](PyBuilder &self, PyBasicBlock &bb) {
             return LLVMInsertExistingBasicBlockAfterInsertBlock(self.get(), bb.get());
           },
           "basic_block"_a,
           "Insert the given basic block after the insertion point of the given "
           "builder.\n\n"
           "The insertion point must be valid.")
      .def("insert",
           [](PyBuilder &self, PyInstruction &inst) {
             return LLVMInsertIntoBuilder(self.get(), inst.get());
           },
           "instruction"_a)
      .def("insert_with_name",
           [](PyBuilder &self, PyInstruction &inst, const char *name) {
             return LLVMInsertIntoBuilderWithName(self.get(), inst.get(), name);
           },
           "instruction"_a, "name"_a = "")
      .def("destory",
           [](PyBuilder &self) {
             return LLVMDisposeBuilder(self.get());
           },
           "Original Function: LLVMDisposeBuilder.")
      .def("set_instruction_debug_location",
           [](PyBuilder &self, PyInstruction &inst) {
             return LLVMSetInstDebugLocation(self.get(), inst.get());
           },
           "instruction"_a,
           "Attempts to set the debug location for the given instruction using the"
           "current debug location for the given builder.  If the builder has no current"
           "debug location, this function is a no-op.")
      .def("add_metadata_to_instruction",
           [](PyBuilder &self, PyInstruction &inst) {
             return LLVMAddMetadataToInst(self.get(), inst.get());
           },
           "instruction"_a,
           "Adds the metadata registered with the given builder to the given"
           "instruction.")
       /*
        * The following methods are used to build instructions
        * the returned instructions are as specific are possible
        * But if the instruction type doesn't meant to be used in other API functions,
        * then we make it as PyInstruction Type to reduce the Instruction sub-classes number
        */
      .def("ret_void",
           [](PyBuilder &self) {
             return PyReturnInst(LLVMBuildRetVoid(self.get()));
           })
      .def("ret",
           [](PyBuilder &self, PyValue &v) {
             return PyReturnInst(LLVMBuildRet(self.get(), v.get()));
           },
           "value"_a)
      .def("aggregate_ret",
           [](PyBuilder &self, std::vector<PyValue> &values) {
             unsigned num = values.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, values, raw, num);
             return PyReturnInst(LLVMBuildAggregateRet(self.get(), raw.data(), num));
           },
           "values"_a)
      .def("br",
           [](PyBuilder &self, PyBasicBlock &dest) {
             return PyBranchInst(LLVMBuildBr(self.get(), dest.get()));
           },
           "dest"_a)
      .def("cond_br",
           [](PyBuilder &self, PyValue &If, PyBasicBlock &Then,
              PyBasicBlock &Else) {
             return PyBranchInst(LLVMBuildCondBr(self.get(), If.get(),
                                                 Then.get(), Else.get()));
           },
           "If"_a, "Then"_a, "Else"_a)
      .def("switch",
           [](PyBuilder &self, PyValue &value, PyBasicBlock &Else,
              unsigned numCases) {
             return PySwitchInst(LLVMBuildSwitch(self.get(), value.get(), Else.get(), numCases));
           },
           "value"_a, "Else"_a, "num_cases"_a)
      .def("indirect_br",
           [](PyBuilder &self, PyValue &addr, unsigned numDests) {
             return PyIndirectBrInst(LLVMBuildIndirectBr(self.get(), addr.get(), numDests));
           },
           "addr"_a, "num_dests"_a)
      .def("invoke",
           [](PyBuilder &self, PyType &type, PyFunction &fn, std::vector<PyValue> args,
              PyBasicBlock Then, PyBasicBlock Catch, const char *name) {
             unsigned args_num  = args.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);
             auto res = LLVMBuildInvoke2(self.get(), type.get(), fn.get(),
                                         rawArgs.data(), args_num, Then.get(),
                                         Catch.get(), name);
             return PyInvokeInst(res);
           },
           "type"_a, "fn"_a, "args"_a, "Then"_a, "Catch"_a, "name"_a = "",
           "Original Function: LLVMBuildInvoke2.")
      .def("invoke_with_operand_bundles",
           [](PyBuilder &self, PyType &type, PyFunction &fn, std::vector<PyValue> args,
              PyBasicBlock Then, PyBasicBlock Catch,
              std::vector<PyOperandBundle> bundles, const char *name) {
             unsigned args_num  = args.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);

             unsigned bundles_num = bundles.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMOperandBundleRef, bundles, rawBundles,
                                         bundles_num);
             
             auto res = LLVMBuildInvokeWithOperandBundles
                          (self.get(), type.get(), fn.get(),
                           rawArgs.data(), args_num, Then.get(),
                           Catch.get(), rawBundles.data(), bundles_num,
                           name);
             return PyInvokeInst(res);
           },
           "type"_a, "fn"_a, "args"_a, "Then"_a, "Catch"_a, "bundles"_a,
           "name"_a = "")
      .def("unreachable",
           [](PyBuilder &self) {
             return PyInstruction(LLVMBuildUnreachable(self.get()));
           })
      .def("resume",
           [](PyBuilder &self, PyValue &exn) {
             return PyInstruction(LLVMBuildResume(self.get(), exn.get()));
           },
           "exn"_a)
      .def("landing_pad",
           [](PyBuilder &self, PyType &type, PyValue &PersFn, unsigned numClauses,
              const char *name) {
             return PyLandingPadInst(LLVMBuildLandingPad
                                    (self.get(), type.get(), PersFn.get(), numClauses,
                                     name));
           },
           "type"_a, "pers_fn"_a, "num_clauses"_a, "name"_a = "")
      .def("cleanup_ret",
           [](PyBuilder &self, PyValue &catchPad, PyBasicBlock bb) {
             return PyCleanupReturnInst(LLVMBuildCleanupRet(self.get(), catchPad.get(),
                                                      bb.get()));
           },
           "catch_pad"_a, "bb"_a)
      .def("catch_pad",
           [](PyBuilder &self, PyValue &parentPad, std::vector<PyValue> args,
              const char *name) {
             unsigned args_num  = args.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);

             auto res = LLVMBuildCatchPad(self.get(), parentPad.get(), rawArgs.data(),
                                          args_num, name);
             return PyCatchPadInst(res);
           },
           "parent_pad"_a, "args"_a, "name"_a = "")
      .def("cleanup_pad",
           [](PyBuilder &self, PyValue &parentPad, std::vector<PyValue> args,
              const char *name) {
             unsigned args_num  = args.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);

             auto res = LLVMBuildCleanupPad(self.get(), parentPad.get(), rawArgs.data(),
                                          args_num, name);
             return PyInstruction(res);
           },
           "parent_pad"_a, "args"_a, "name"_a = "")
      .def("catch_switch",
           [](PyBuilder &self, PyValue &parentPad, PyBasicBlock &unwindBB,
              unsigned numHandlers, const char *name) {
             auto res = LLVMBuildCatchSwitch(self.get(), parentPad.get(),
                                             unwindBB.get(), numHandlers,
                                             name);
             return PyCatchSwitchInst(res);
           },
           "parent_pad"_a, "unwind_bb"_a, "num_handlers"_a, "name"_a = "")
       /*
         Arithmetic
        */
  BUILDER_BIND_BINARY_OPS
      .def("binop",
           [](PyBuilder &self, LLVMOpcode op, PyValue &lhs, PyValue &rhs,
              const char *name) {
             return PyValueAuto(LLVMBuildBinOp(self.get(), op, lhs.get(), rhs.get(), name));
           })
      .def("neg",
           [](PyBuilder &self, PyValue &v, const char *name) {
             return PyValueAuto(LLVMBuildNeg(self.get(), v.get(), name));
           })
      .def("neg_nsw",
           [](PyBuilder &self, PyValue &v, const char *name) {
             return PyValueAuto(LLVMBuildNSWNeg(self.get(), v.get(), name));
           })
      .def("neg_nuw",
           [](PyBuilder &self, PyValue &v, const char *name) {
             return PyValueAuto(LLVMBuildNUWNeg(self.get(), v.get(), name));
           })
      .def("fneg",
           [](PyBuilder &self, PyValue &v, const char *name) {
             return PyValueAuto(LLVMBuildFNeg(self.get(), v.get(), name));
           })
      .def("not_",
           [](PyBuilder &self, PyValue &v, const char *name) {
             return PyValueAuto(LLVMBuildNot(self.get(), v.get(), name));
           })
      .def("malloc",
           [](PyBuilder &self, PyType &type, const char *name) {
             return PyCallInst(LLVMBuildMalloc(self.get(), type.get(), name));
           },
           "type"_a, "name"_a = "")
      .def("array_malloc",
           [](PyBuilder &self, PyType &type, PyValue &val, const char *name) {
             return PyCallInst(LLVMBuildArrayAlloca(self.get(), type.get(), val.get(), name));
           },
           "type"_a, "value"_a, "name"_a = "")
      .def("memset",
           [](PyBuilder &self, PyValue &ptr, PyValue &val, PyValue &len, unsigned align) {
             auto res = LLVMBuildMemSet(self.get(), ptr.get(), val.get(), len.get(),
                                        align);
             return PyCallInst(res);
           },
           "ptr"_a, "val"_a, "len"_a, "align"_a)
      .def("memcpy",
           [](PyBuilder &self, PyValue &dest, unsigned dstAlign, PyValue &src,
              unsigned srcAlign, PyValue size) {
             auto res = LLVMBuildMemCpy(self.get(), dest.get(), dstAlign, src.get(),
                                        srcAlign, size.get());
             return PyCallInst(res);
           },
           "dest"_a, "dest_align"_a, "src"_a, "src_align"_a, "size"_a,
           "Creates and inserts a memcpy between the specified pointers.")
      .def("mem_move",
           [](PyBuilder &self, PyValue &dest, unsigned dstAlign, PyValue &src,
              unsigned srcAlign, PyValue size) {
             auto res = LLVMBuildMemMove(self.get(), dest.get(), dstAlign, src.get(),
                                         srcAlign, size.get());
             return PyCallInst(res);
           },
           "dest"_a, "dest_align"_a, "src"_a, "src_align"_a, "size"_a,
           "Creates and inserts a memmove between the specified pointers.")
      .def("alloca",
           [](PyBuilder &self, PyType &type, const char *name) {
             return PyAllocaInst(LLVMBuildAlloca(self.get(), type.get(), name));
           },
           "type"_a, "name"_a = "")
      .def("array_alloca",
           [](PyBuilder &self, PyType &type, PyValue &val, const char *name) {
             return PyAllocaInst(LLVMBuildArrayAlloca(self.get(), type.get(),
                                                      val.get(), name));
           },
           "type"_a, "value"_a, "name"_a = "")
      .def("free",
           [](PyBuilder &self, PyValue pointer) {
             return PyCallInst(LLVMBuildFree(self.get(), pointer.get()));
           },
           "pointer"_a)
      .def("load2",
           [](PyBuilder &self, PyType &type, PyValue &pointer, const char *name) {
             return PyLoadInst(LLVMBuildLoad2(self.get(), type.get(), pointer.get(),
                                              name));
           },
           "type"_a, "ptr"_a, "name"_a = "")
      .def("store",
           [](PyBuilder &self, PyValue &val, PyValue &ptr) {
             return PyStoreInst(LLVMBuildStore(self.get(), val.get(), ptr.get()));
           },
           "value"_a, "ptr"_a)
      .def("gep2",
           [](PyBuilder &self, PyType &type, PyValue &ptr, std::vector<PyValue> indices,
              const char *name) {
             unsigned num_indices = indices.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, indices, rawIndices, num_indices);
             return PyValueAuto(LLVMBuildGEP2(self.get(), type.get(), ptr.get(),
                                rawIndices.data(), num_indices, name));
           },
           "type"_a,  "ptr"_a, "indices"_a, "name"_a = "")
     .def("in_bounds_gep2",
          [](PyBuilder &self, PyType &type, PyValue &ptr, std::vector<PyValue> indices,
             const char *name) {
            unsigned num_indices = indices.size();
            UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, indices, rawIndices, num_indices);
            return PyValueAuto(LLVMBuildGEP2(self.get(), type.get(), ptr.get(),
                                             rawIndices.data(), num_indices, name));
          },
          "type"_a,  "ptr"_a, "indices"_a, "name"_a = "")
     .def("struct_gep2",
          [](PyBuilder &self, PyType &type, PyValue &ptr, unsigned index, const char *name) {
            auto res = LLVMBuildStructGEP2(self.get(), type.get(), ptr.get(),
                                           index, name);
            return PyValueAuto(res);
          },
          "type"_a, "ptr"_a, "index"_a, "name"_a = "")
     .def("global_string",
          [](PyBuilder &self, const char *str, const char *name) {
            return PyValueAuto(LLVMBuildGlobalString(self.get(), str, name));
          })
     .def("global_string_ptr",
          [](PyBuilder &self, const char *str, const char *name) {
            return PyValueAuto(LLVMBuildGlobalStringPtr(self.get(), str, name));
          })
       /* cast start  */
  BUILDER_BIND_CAST_OPS
     .def("cast",
          [](PyBuilder &self, LLVMOpcode opcode, PyValue &value,  PyType &destType,
             const char *name) {
            return PyValueAuto(LLVMBuildCast(self.get(), opcode, value.get(),
                                             destType.get(), name));
          },
          "opcode"_a, "value"_a, "dest_type"_a, "name"_a = "")
     .def("int_cast_2",
          [](PyBuilder &self, PyValue &value, PyType &destType, const char *name){
            return PyValueAuto(LLVMBuildFPCast
                                 (self.get(), value.get(), destType.get(), name));
          },
          "value"_a, "dest_type"_a, "name"_a = "")
     .def("int_cast",
          [](PyBuilder &self, PyValue &value, PyType &destType, const char *name){
            return PyValueAuto(LLVMBuildIntCast
                                 (self.get(), value.get(), destType.get(), name));
          },
          "value"_a, "dest_type"_a, "name"_a = "",
          "Deprecated: This cast is always signed. Use LLVMBuildIntCast2 instead.")
     .def_static("get_cast_opcode",
                 [](PyValue &src, bool srcIsSigned, PyType &destType, bool destIsSigned) {
                   return LLVMGetCastOpcode(src.get(), srcIsSigned, destType.get(),
                                            destIsSigned);
                 },
                 "src"_a, "src_is_signed"_a, "dest_type"_a, "dest_is_signed"_a)
     /* end */
     .def("icmp",
          [](PyBuilder &self, LLVMIntPredicate op, PyValue &lhs, PyValue &rhs,
             const char *name) {
            return PyValueAuto(LLVMBuildICmp(self.get(), op, lhs.get(), rhs.get(),
                                             name));
          },
          "op"_a, "lhs"_a, "rhs"_a, "name"_a = "")
     .def("phi",
          [](PyBuilder &self, PyType &type, const char *name) {
            return PyPHINode(LLVMBuildPhi(self.get(), type.get(), name));
          },
          "type"_a, "name"_a = "")
     .def("call_2",
          [](PyBuilder &self, PyTypeFunction &type, PyFunction &fn, std::vector<PyValue> args,
             const char *name) {
            unsigned args_num = args.size();
            UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);
            return PyCallInst(LLVMBuildCall2
                                (self.get(), type.get(), fn.get(), rawArgs.data(),
                                 args_num, name));
          },
          "fn_type"_a, "fn"_a, "args"_a, "name"_a = "")
     .def("call_with_operand_bundles",
          [](PyBuilder &self, PyTypeFunction type, PyFunction fn,
             std::vector<PyValue> args,
             std::vector<PyOperandBundle> bundles,
             const char *name) {
            unsigned args_num = args.size();
            UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, rawArgs, args_num);

            unsigned bundles_num = bundles.size();
            UNWRAP_VECTOR_WRAPPER_CLASS(LLVMOperandBundleRef, bundles, rawBundles,
                                                bundles_num);
            
            return PyCallInst(LLVMBuildCallWithOperandBundles
                                (self.get(), type.get(), fn.get(), rawArgs.data(),
                                 args_num, rawBundles.data(), bundles_num, name));
          })
     .def("select",
          [](PyBuilder &self, PyValue &If, PyValue &Then, PyValue &Else,
             const char *name) {
            return PyCallInst(LLVMBuildSelect
                                (self.get(), If.get(), Then.get(), Else.get(),
                                 name));
          },
          "If"_a, "Then"_a, "Else"_a, "name"_a = "")
     .def("vaarg",
          [](PyBuilder &self, PyValue &list, PyType &type, const char *name) {
            return PyValue(LLVMBuildVAArg(self.get(), list.get(), type.get(),
                                          name));
          },
          "list"_a, "type"_a, "name"_a = "")
     .def("extract_element",
          [](PyBuilder &self, PyValue &vecVal, PyValue &index, const char *name) {
            return PyValueAuto(LLVMBuildExtractElement
                                 (self.get(), vecVal.get(), index.get(), name));
          },
          "vec"_a, "index"_a, "name"_a = "")
     .def("insert_element",
          [](PyBuilder &self, PyValue &vecVal, PyValue &eltVal,
             PyValue &index, const char *name) {
            return PyValueAuto(LLVMBuildInsertElement
                                 (self.get(), vecVal.get(), eltVal.get(),
                                  index.get(), name));
          },
          "vec"_a, "element"_a, "index"_a, "name"_a = "")
    .def("shuffle_vector",
         [](PyBuilder &self, PyValue &v1, PyValue &v2, PyValue &mask, const char *name) {
           return PyShuffleVectorInst(LLVMBuildShuffleVector
                                (self.get(), v1.get(), v2.get(), mask.get(), name));
         },
         "v1"_a, "v2"_a, "mask"_a, "name"_a = "")
    .def("extract_value",
         [](PyBuilder &self, PyValue &aggVal, unsigned index, const char *name) {
           return PyValueAuto(LLVMBuildExtractValue
                                (self.get(), aggVal.get(), index, name));
         },
         "agg"_a, "index"_a, "name"_a = "")
    .def("insert_value",
         [](PyBuilder &self, PyValue &aggVal, PyValue &eltVal, unsigned index,
            const char *name) {
           return PyValueAuto(LLVMBuildInsertValue
                                (self.get(), aggVal.get(), eltVal.get(), index,
                                 name));
         },
         "agg"_a, "elt"_a, "index"_a, "name"_a = "")
    .def("freeze",
         [](PyBuilder &self, PyValue &val, const char *name) {
           return PyValueAuto(LLVMBuildFreeze(self.get(), val.get(), name));
         },
         "val"_a, "name"_a = "")
    .def("is_null",
         [](PyBuilder &self, PyValue &val, const char *name) {
           return PyValueAuto(LLVMBuildIsNull(self.get(), val.get(), name));
         },
         "value"_a, "name"_a = "")
    .def("is_not_null",
         [](PyBuilder &self, PyValue &val, const char *name) {
           return PyValueAuto(LLVMBuildIsNotNull(self.get(), val.get(), name));
         },
         "value"_a, "name"_a = "")
    .def("ptr_diff_2",
         [](PyBuilder &self, PyType &elemType, PyValue &lhs, PyValue &rhs,
            const char *name) {
           return PyValueAuto(LLVMBuildPtrDiff2
                                (self.get(), elemType.get(), lhs.get(),
                                 rhs.get(), name));
         },
         "elem_type"_a, "lhs"_a, "rhs"_a, "name"_a = "")
    .def("fence",
         [](PyBuilder &self, LLVMAtomicOrdering ordering, bool singleThread,
            const char *name) {
           return PyFenceInst(LLVMBuildFence(self.get(), ordering, singleThread, name));
         },
         "ordering"_a, "singleThread"_a, "name"_a = "")
    .def("atomic_rmw",
         [](PyBuilder &self, LLVMAtomicRMWBinOp op, PyValue &ptr, PyValue val,
            LLVMAtomicOrdering ordering, bool singleThread) {
           auto res = LLVMBuildAtomicRMW(self.get(), op, ptr.get(), val.get(), ordering,
                                         singleThread);
           return PyAtomicRMWInst(res);
         },
         "op"_a, "ptr"_a, "val"_a, "ordering"_a, "singleThread"_a)
    .def("atomic_cmp_xchg",
         [](PyBuilder &self, PyValue &ptr, PyValue &cmp, PyValue &New,
            LLVMAtomicOrdering successOrdering, LLVMAtomicOrdering failureOrdering,
            bool singleThread) {
           auto res = LLVMBuildAtomicCmpXchg(self.get(), ptr.get(), cmp.get(),
                                             New.get(), successOrdering,
                                             failureOrdering, singleThread);
           return PyAtomicCmpXchgInst(res);
         });

  

  BasicBlockClass
      .def("__repr__",
           [](PyBasicBlock &self) {
             return "<BasicBlock>";
           })
      .def("__init__",
           [](PyBasicBlock *bb, PyContext &c, const char *name) {
             new (bb) PyBasicBlock(LLVMCreateBasicBlockInContext(c.get(), name));
           },
           "context"_a, "name"_a = "",
           "Create a new basic block without inserting it into a function.")
      .def("__init__",
           [](PyBasicBlock *bb, PyContext &c, PyFunction &f, const char *name) {
             new (bb) PyBasicBlock(LLVMAppendBasicBlockInContext
                                     (c.get(), f.get(), name));
           },
           "context"_a, "function"_a, "name"_a = "",
           "Create a new basic block without inserting it into a function.")
      .def("__init__",
           [](PyBasicBlock *bb, PyContext &c, PyBasicBlock &BB, const char *name) {
             new (bb) PyBasicBlock(LLVMInsertBasicBlockInContext
                                     (c.get(), BB.get(), name));
           },
           "context"_a, "bb"_a, "name"_a = "",
           "Insert a basic block in a function before another basic block.\n\n"
           "The function to add to is determined by the function of the"
           "passed basic block.")
      .def("__init__",
           [](PyBasicBlock *bb, PyBasicBlock &BB, const char *name) {
             new (bb) PyBasicBlock(LLVMInsertBasicBlock(BB.get(), name));
           },
           "insert_before_bb"_a, "name"_a = "",
           "Insert a basic block in a function using the global context.")
      .def_prop_ro("name",
                   [](PyBasicBlock &self) {
                     return LLVMGetBasicBlockName(self.get());
                   })
      .def_prop_ro("parent",
                   [](PyBasicBlock &self) {
                     return PyFunction(LLVMGetBasicBlockParent(self.get()));
                   },
                   "Obtain the function to which a basic block belongs.")
      .def_prop_ro("terminator",
                   [](PyBasicBlock &self) -> optional<PyInstruction> {
                     auto res = LLVMGetBasicBlockTerminator(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyInstruction);
                   },
                   "Obtain the terminator instruction for a basic block.\n\n"
                   "If the basic block does not have a terminator (it is not well-formed"
                   "if it doesn't), then NULL is returned.")
      .def_prop_ro("value",
                   [](PyBasicBlock &self) {
                     return PyBasicBlockValue(LLVMBasicBlockAsValue(self.get()));
                   })
      .def_prop_ro("next",
                   [](PyBasicBlock &self) -> optional<PyBasicBlock> {
                     auto res = LLVMGetNextBasicBlock(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyBasicBlock);
                   })
      .def_prop_ro("prev",
                   [](PyBasicBlock &self) -> optional<PyBasicBlock> {
                     auto res = LLVMGetPreviousBasicBlock(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyBasicBlock);
                   })
      .def_prop_ro("first_instruction",
                   [](PyBasicBlock &self) -> optional<PyInstruction> {
                     auto res = LLVMGetFirstInstruction(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyInstruction);
                   })
      .def_prop_ro("last_instruction",
                   [](PyBasicBlock &self) -> optional<PyInstruction> {
                     auto res = LLVMGetLastInstruction(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyInstruction);
                   })
      .def_prop_ro("instructions",
                   [](PyBasicBlock &self) {
                     auto res = LLVMGetFirstInstruction(self.get());
                     return PyInstructionIterator(PyInstruction(res));
                   })
      .def("create_and_insert_before",
           [](PyBasicBlock &self, const char *name) {
             return PyBasicBlock(LLVMInsertBasicBlock(self.get(), name));
           },
           "Insert a basic block in a function using the global context.")
      .def("destroy", // TODO test
           [](PyBasicBlock &self) {
             return LLVMDeleteBasicBlock(self.get());
           },
           "Remove a basic block from a function and delete it.\n\n"
           "This deletes the basic block from its containing function and deletes"
           "the basic block itself.")
      .def("remove_from_parent",
           [](PyBasicBlock &self) {
             return LLVMRemoveBasicBlockFromParent(self.get());
           },
           "Remove a basic block from a function.\n\n"
           "This deletes the basic block from its containing function but keep"
           "the basic block alive.")
      .def("move_before",
           [](PyBasicBlock &self, PyBasicBlock posBB) {
             return LLVMMoveBasicBlockBefore(self.get(), posBB.get());
           },
           "pos"_a,
           "Move a basic block to before another one.")
      .def("move_after",
           [](PyBasicBlock &self, PyBasicBlock posBB) {
             return LLVMMoveBasicBlockAfter(self.get(), posBB.get());
           },
           "pos",
           "Move a basic block to after another one.");


  OperandBundleClass
      .def("__repr__",
           [](PyOperandBundle &self) {
             return "<OperandBundle>";
           })
      .def("__init__",
           [](PyOperandBundle *ob, std::string &tag, std::vector<PyValue> args) {
             unsigned arg_num = args.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, args, raw_args, arg_num);
             new (ob) PyOperandBundle(LLVMCreateOperandBundle
                                        (tag.c_str(), tag.size(), raw_args.data(),
                                         arg_num));
           })
      .def_prop_ro("tag",
                   [](PyOperandBundle &self) {
                     size_t len;
                     auto tag = LLVMGetOperandBundleTag(self.get(), &len);
                     return std::string(tag, len);
                   })
      .def_prop_ro("operands_num",
                   [](PyOperandBundle &self) {
                     return LLVMGetNumOperandBundleArgs(self.get());
                   })
      .def("get_operand_at",
           [](PyOperandBundle &self, unsigned index) {
             return PyValueAuto(LLVMGetOperandBundleArgAtIndex(self.get(), index));
           });

    

  IntrinsicClass
      .def("__repr__",
           [](PyIntrinsic &self) {
             return "<Intrinsic>";
           })
       .def("__repr__",
            [](PyIntrinsic &self) {
              size_t len;
              auto raw_name = LLVMIntrinsicGetName(self.get(), &len);
              auto name = std::string(raw_name, len);
              return fmt::format("<Instrinsic id={} name={}>", self.get(), name);
            })
       .def_static("lookup",
                    [](std::string &name) {
                      return PyIntrinsic(LLVMLookupIntrinsicID(name.c_str(), name.size()));
                    },
                   "name"_a = "",
                    "Obtain the intrinsic ID number which matches the given function name.")
      .def("get_type",
                  [](PyIntrinsic &self, PyContext &context, std::vector<PyType> paramTypes) {
                    size_t paramCnt = paramTypes.size();
                    UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, paramTypes, params, paramCnt);
                    return PyTypeFunction(LLVMIntrinsicGetType
                                            (context.get(), self.get(), params.data(),
                                             paramCnt));

                  },
                  "context"_a, "param_types"_a,
                  "Retrieves the type of an intrinsic.  For overloaded intrinsics,"
                  " parameter types must be provided to uniquely identify an overload.")
      .def_prop_ro("id",
                   [](PyIntrinsic &self) {
                     return self.get();
                   })
      .def_prop_ro("name",
                   [](PyIntrinsic &self) {
                    size_t len;
                    auto name = LLVMIntrinsicGetName(self.get(), &len);
                    return std::string(name, len);
                   },
                   "Retrieves the name of an intrinsic.")
      .def_prop_ro("is_overloaded",
                   [](PyIntrinsic &self) {
                     return LLVMIntrinsicIsOverloaded(self.get()) != 0;
                   })
      // note LLVMIntrinsicCopyOverloadedName is deprecated
      .def("copy_overloaded_name",
                  [](PyIntrinsic &self, std::vector<PyType> paramTypes) {
                    size_t paramCnt = paramTypes.size();
                    UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, paramTypes, params, paramCnt);
                    size_t nameLen;
                    auto name = LLVMIntrinsicCopyOverloadedName
                                  (self.get(), params.data(), paramCnt, &nameLen);
                    return std::string(name, nameLen);
                  },
                  "param_types"_a,
                  "Deprecated: Use :func:`copy_overloaded_name2` instead.")
      .def("copy_overloaded_name2",
                  [](PyIntrinsic &self, PyModule &module, std::vector<PyType> paramTypes) {
                    size_t nameLen;
                    size_t paramCnt = paramTypes.size();
                    UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, paramTypes, params, paramCnt);
                    auto name = LLVMIntrinsicCopyOverloadedName2
                                  (module.get(), self.get(), params.data(), paramCnt, &nameLen);
                    return std::string(name, nameLen);
                  },
                  "module"_a, "param_types"_a,
                  "Copies the name of an overloaded intrinsic identified by a given"
                  " list of parameter types.\n\n"
                  "This version also supports unnamed types.");
  


  MetadataEntriesClass
      .def("__repr__",
           [](PyMetadataEntries &self) {
             return "<MetadataEntries>";
           })
      .def("get_kind",
           [](PyMetadataEntries &self, unsigned index) {
             return LLVMValueMetadataEntriesGetKind(self.get(), index);
           },
           "index"_a,
           "Returns the kind of a value metadata entry at a specific index.")
      .def("get_metadata",
           [](PyMetadataEntries &self, unsigned index) {
             return PyMetadata(LLVMValueMetadataEntriesGetMetadata(self.get(), index));
           },
           "index"_a,
           "Returns the underlying metadata node of a value metadata entry at a"
           "specific index.");

  UseClass
      .def("__repr__",
           [](PyUse &self) {
             return "<Use>";
           })
      .def_prop_ro("next",
                   [](PyUse &u) -> optional<PyUse> {
                     auto res = LLVMGetNextUse(u.get());
                     WRAP_OPTIONAL_RETURN(res, PyUse);
                   },
                   "Obtain the next use of a value.\n\n"
                   "This effectively advances the iterator. It returns NULL if you are on"
                   "the final use and no more are available.")
      .def_prop_ro("user",
                   [](PyUse &u) { return PyUser(LLVMGetUser(u.get())); },
                   "Obtain the user value for a user.\n",
                   "The returned value corresponds to a llvm::User type.")
      .def_prop_ro("used_value",
                   [](PyUse &u) {
                     return PyValueAuto(LLVMGetUsedValue(u.get()));
                   });
  
  
  AttributeClass
      .def("__repr__",
           [](PyAttribute &self) {
             return "<Attribute>";
           })
      .def_prop_ro("is_enum",
           [](PyAttribute &attr) {
             return LLVMIsEnumAttribute(attr.get()) != 0;
           })
      .def_prop_ro("is_string",
           [](PyAttribute &attr) {
             return LLVMIsStringAttribute(attr.get()) != 0;
           })
      .def_prop_ro("is_type",
           [](PyAttribute &attr) {
             return LLVMIsTypeAttribute(attr.get()) != 0;
           });
  
  EnumAttributeClass
      .def("__repr__",
           [](PyEnumAttribute &self) {
             return "<EnumAttribute>";
           })
      .def("__init__",
           [](PyEnumAttribute *t, PyContext &c, unsigned kindID, uint64_t val) {
             new (t) PyEnumAttribute(LLVMCreateEnumAttribute(c.get(), kindID, val));
           },
           "context"_a, "kind_id"_a, "val"_a)
      .def("__repr__",
           [](PyEnumAttribute &self) {
             auto kind = LLVMGetEnumAttributeKind(self.get());
             auto value = LLVMGetEnumAttributeValue(self.get());
             return fmt::format("<EnumAttribute kind={} value={}>", kind, value);
           })
      .def_prop_ro("kind",
                   [](PyEnumAttribute &attr) {
                     return LLVMGetEnumAttributeKind(attr.get());
                   }, "attr"_a)
      .def_prop_ro("value",
                   [](PyEnumAttribute &attr) {
                     return LLVMGetEnumAttributeValue(attr.get());
                   },
                   "Get the enum attribute's value. 0 is returned if none exists.")
      .def_static("get_enum_attribute_kind_for_name",
              [](const std::string &name){
                return LLVMGetEnumAttributeKindForName(name.c_str(), name.size());
              },
              "name"_a = "",
              "Return an unique id given the name of a enum attribute,"
              "or 0 if no attribute by that name exists.\n\n"
              "See http://llvm.org/docs/LangRef.html#parameter-attributes"
              "and http://llvm.org/docs/LangRef.html#function-attributes"
              "for the list of available attributes.\n\n"
              "NB: Attribute names and/or id are subject to change without"
              "going through the C API deprecation cycle.")
      .def_static("get_last_enum_attribute_kind", &LLVMGetLastEnumAttributeKind);

  TypeAttributeClass
      .def("__repr__",
           [](PyTypeAttribute &self) {
             return "<TypeAttribute>";
           })
      .def("__init__",
           [](PyTypeAttribute *t, PyContext &context, unsigned kind_id, PyType &type) {
             new (t) PyTypeAttribute(LLVMCreateTypeAttribute
                                       (context.get(), kind_id, type.get()));
           },
           "context"_a, "kind_id"_a, "type"_a)
      .def("__repr__",
           [](PyTypeAttribute &self) {
             auto value = LLVMGetTypeAttributeValue(self.get());
             auto type_kind = LLVMGetTypeKind(value);
             return fmt::format("<TypeAttribute value={}>", get_repr_str(type_kind));
           })
      .def_prop_ro("value",
                   [](PyTypeAttribute &ta){
                     return PyTypeAuto(LLVMGetTypeAttributeValue(ta.get()));
                   },
                   "Get the type attribute's value.");

  StringAttributeClass
      .def("__repr__",
           [](PyStringAttribute &self) {
             return "<StringAttribute>";
           })
      .def("__init__",
           [](PyStringAttribute *t, PyContext &c, const std::string &kind, const std::string &value) {
             auto raw = LLVMCreateStringAttribute(c.get(),
                                                  kind.c_str(), kind.size(),
                                                  value.c_str(), value.size());
             new (t) PyStringAttribute(raw);
           },
           "context"_a, "kind"_a, "value"_a)
      .def("__repr__",
           [](PyStringAttribute &self) {
             unsigned kind_length;
             const char *raw_kind = LLVMGetStringAttributeKind(self.get(), &kind_length);
             auto kind = std::string(raw_kind, kind_length);
             
             unsigned value_length;
             const char *raw_value = LLVMGetStringAttributeValue(self.get(), &value_length);
             auto value = std::string(raw_value, value_length);

             return fmt::format("<TypeAttribute kind={} value={}>", kind, value);
           })
      .def_prop_ro("kind",
                   [](PyStringAttribute &ta) {
                     unsigned length;
                     const char *kind = LLVMGetStringAttributeKind(ta.get(), &length);
                     return std::string(kind, length);
                   })
      .def_prop_ro("value",
                   [](PyStringAttribute &ta) {
                     unsigned length;
                     const char *value = LLVMGetStringAttributeValue(ta.get(), &length);
                     return std::string(value, length);
                   },
                   "Get the type attribute's value.");
  

  DiagnosticInfoClass
      .def("__repr__",
           [](PyDiagnosticInfo &self) {
             return "<DiagnosticInfo>";
           })
      // .def(nb::init<>()) // NOTE currently no constructor function for python, we'll see
      .def_prop_ro("description",
                   [](PyDiagnosticInfo &d) {
                     char *diagInfoDesc = LLVMGetDiagInfoDescription(d.get());
                     std::string diagInfoDescCopy(diagInfoDesc);
                     LLVMDisposeMessage(diagInfoDesc);
                     return diagInfoDescCopy;
                   },
                   "Return a string representation of the DiagnosticInfo.")
      .def_prop_ro("severity",
                   [](PyDiagnosticInfo &d) {
                     return LLVMGetDiagInfoSeverity(d.get());
                   },
                   "Return an enum LLVMDiagnosticSeverity.");

  
  NamedMDNodeClass
      .def("__repr__",
           [](PyNamedMDNode &self) {
             return "<NamedMDNode>";
           })
      .def_prop_ro("next",
                   [](PyNamedMDNode &nmdn) -> optional<PyNamedMDNode>{
                     auto res = LLVMGetNextNamedMetadata(nmdn.get());
                     WRAP_OPTIONAL_RETURN(res, PyNamedMDNode);
                   },
                   "Advance a NamedMDNode iterator to the next NamedMDNode.\n\n"
                   "Returns NULL if the iterator was already at the end and there"
                   " are no more named metadata nodes.")
      .def_prop_ro("prev",
                   [](PyNamedMDNode &nmdn) -> optional<PyNamedMDNode> {
                     auto res = LLVMGetPreviousNamedMetadata(nmdn.get());
                     WRAP_OPTIONAL_RETURN(res, PyNamedMDNode);
                   },
                   "Decrement a NamedMDNode iterator to the previous NamedMDNode.\n\n"
                   "Returns NULL if the iterator was already at the beginning and there are"
                   "no previous named metadata nodes.")
      .def_prop_ro("name",
                   [](PyNamedMDNode &nmdn) {
                     size_t len;
                     const char *name =  LLVMGetNamedMetadataName(nmdn.get(), &len);
                     return std::string(name, len);
                   },
                   "Retrieve the name of a NamedMDNode.");

  
  ContextClass
      .def("__repr__",
           [](PyContext &self) {
             return "<Context>";
           })
      .def(nb::init<>(), "Create a new context.")
      .def("__enter__",
           [](PyContext &self) {
             return &self;
           })
      .def("__exit__",
           [](PyContext &self, nb::args args, nb::kwargs kwargs) {})
      .def_static("get_global_context", &PyContext::getGlobalContext,
                  "Obtain the global context instance.")
      .def_prop_ro("diagnostic_context",
                   [](PyContext &c) {
                     // FIXME The function cannot work correctly (always None) since
                     // `LLVMContextSetDiagnosticHandler` cannot, which set
                     // the diagnostic context
                     return LLVMContextGetDiagnosticContext(c.get());
                   },
                   "Get the diagnostic context of this context.")
      .def_prop_rw("should_discard_value_names", // TODO convert LLVMBool to bool
                   [](PyContext &c) -> bool { return LLVMContextShouldDiscardValueNames(c.get()) != 0; },
                   [](PyContext &c, bool discard) {
                     return LLVMContextSetDiscardValueNames(c.get(), discard);
                   },
                   nb::for_getter
                     ("Retrieve whether the given context is set to"
                      "discard all value names.\n\n"
                      "Return true if the Context runtime configuration "
                      "is set to discard all value names. When true, "
                      "only GlobalValue names will be available in the IR."),
                   nb::for_setter
                     ("Set whether the given context discards all value names.\n\n"
                      "If true, only the names of GlobalValue objects"
                      "will be available in the IR.\n"
                      "This can be used to save memory and runtime, "
                      "especially in release mode."))
      .def("set_diagnostic_handler", // FIXME
           [](PyContext &c, LLVMDiagnosticHandler handler, void * diagnosticContext){
             return LLVMContextSetDiagnosticHandler(c.get(), handler, diagnosticContext);
           },
           "handler"_a, "diagnostic_context"_a,
           "Set the diagnostic handler for this context.")
      // .def("get_diagnostic_handler", FIXME
      //      [](PyContext &c)  { return LLVMContextGetDiagnosticHandler(c.get()); },
      //      "Get the diagnostic handler of this context.")
      .def("set_yield_callback", // FIXME
           [](PyContext &c, LLVMYieldCallback callback, void *opaqueHandle){
             return LLVMContextSetYieldCallback(c.get(), callback, opaqueHandle);
           },
           "callback"_a, "opaque_handle"_a,
           "Set the yield callback function for this context.")
      // .def("parse_ir",
      //      [](PyContext &self, PyMemoryBuffer &memBuf) {
      //        try {
      //          auto res = parseIR(self.get(), memBuf.get());
      //          memBuf.resetNoClean(); // We Cannot reuse the memory buffer again
      //          return res;
      //        } catch (const std::exception& ex) {
      //          // TODO test whether it is still available after a filed operation.
      //          memBuf.resetNoClean();
      //          throw ex;
      //        }
      //      },
      //      "memory_buffer"_a,
      //      "Read LLVM IR from a memory buffer and convert it into an in-memory Module"
      //      "object.\n\n"
      //      ":raises RuntimeError\n"
      //      "NOTE that you cannot use passed-in memory_buffer after this operation.")
      .def("create_builder",
           [](PyContext &self) {
             return PyBuilder(LLVMCreateBuilderInContext(self.get()));
           })
      .def("create_basic_block",
           [](PyContext &self, const char *name) {
             return PyBasicBlock(LLVMCreateBasicBlockInContext
                                          (self.get(), name));
           },
           "name"_a = "",
           "Create a new basic block without inserting it into a function.")
      .def("append_basic_block",
           [](PyContext &self, PyFunction fn, const char *name) {
             return PyBasicBlock(LLVMAppendBasicBlockInContext
                                          (self.get(), fn.get(), name));
           },
           "fn"_a, "name"_a = "",
           "Append a basic block to the end of a function.")
      .def("insert_basic_block",
           [](PyContext &self, PyBasicBlock bb, const char *name) {
             return PyBasicBlock(LLVMInsertBasicBlockInContext
                                          (self.get(), bb.get(), name));
           },
           "bb"_a, "name"_a = "",
           "Insert a basic block in a function before another basic block.\n\n"
           "The function to add to is determined by the function of the"
           "passed basic block.")
      .def("get_md_kind_id",
           [](PyContext &c, const std::string &name) {
             return LLVMGetMDKindIDInContext(c.get(), name.c_str(), name.size());
           },
           "name"_a = "")
      .def("create_enum_attribute",
           [](PyContext &c, unsigned kindID, uint64_t val) {
             return PyEnumAttribute(LLVMCreateEnumAttribute(c.get(), kindID, val));
           },
           "kind_id"_a, "val"_a,
           "Create an enum attribute.")
      .def("create_type_attribute",
           [](PyContext &context, unsigned kind_id, PyType &type) {
             return PyTypeAttribute(LLVMCreateTypeAttribute
                                      (context.get(), kind_id, type.get()));
           },
           "kind_id"_a, "type"_a,
           "Create a type attribute")
      .def("create_string_attribute",
           [](PyContext &c, const std::string &kind, const std::string &value) {
             auto raw = LLVMCreateStringAttribute(c.get(),
                                                  kind.c_str(), kind.size(),
                                                  value.c_str(), value.size());
             return PyStringAttribute(raw);
           })
      .def("get_type_by_name_2", // TODO also create one in PyType static method
           [](PyContext &c, const char *name) -> optional<PyType*> {
             auto res = LLVMGetTypeByName2(c.get(), name);
             WRAP_OPTIONAL_RETURN(res, PyTypeAuto);
           },
           "name"_a = "")
      .def("create_md_string_2",
           [](PyContext &self, std::string &name) {
             return PyMDString(LLVMMDStringInContext2(self.get(), name.c_str(), name.size()));
           },
           "name"_a = "",
           "Create an MDString value from a given string value.")
      .def("create_md_node_2",
           [](PyContext &self, std::vector<PyMetadata> mds) {
             size_t num = mds.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMMetadataRef, mds, raw, num);
             return PyMDNode(LLVMMDNodeInContext2(self.get(), raw.data(), num));
           },
           "metadata"_a)
      .def("get_metadata_as_value",
           [](PyContext &self, PyMetadata &md) {
             return getMoreSpcMetadataAsValue(LLVMMetadataAsValue
                                                (self.get(), md.get()));
           });

  

  ModuleClass
      .def(nb::init<const std::string &>(), "name"_a = "")
      .def("__repr__",
           [](PyModule &self) {
             size_t len;
             const char *raw_identifier = LLVMGetModuleIdentifier(self.get(), &len);
             auto id = std::string(raw_identifier, len);
             return fmt::format("<Module id={}>", id);
           })
      .def("__str__",
           [](PyModule &m) {
             char *str = LLVMPrintModuleToString(m.get());
             std::string strCopy(str);
             LLVMDisposeMessage(str);
             return strCopy;
           },
           "Return a string representation of the module")
      .def("__enter__",
           [](PyModule &self) {
             return &self;
           })
      .def("__exit__",
           [](PyModule &self, nb::args args, nb::kwargs kwargs) {})
      .def_prop_ro("first_global_variable",
                   [](PyModule &m) -> optional<PyGlobalVariable> {
                     auto res = LLVMGetFirstGlobal(m.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalVariable);
                   })
      .def_prop_ro("last_global_variable",
                   [](PyModule &self) -> optional<PyGlobalVariable> {
                     auto res = LLVMGetLastGlobal(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalVariable);
                   })
      .def_prop_ro("global_variables",
                   [](PyModule &m) {
                     auto res = LLVMGetFirstGlobal(m.get());
                     return PyGlobalVariableIterator(PyGlobalVariable(res));
                   })
      .def_prop_ro("first_global_ifunc",
                   [](PyModule &self) -> optional<PyGlobalIFunc> {
                     auto res = LLVMGetFirstGlobalIFunc(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalIFunc);
                   })
      .def_prop_ro("last_global_ifunc",
                   [](PyModule &self) -> optional<PyGlobalIFunc> {
                     auto res = LLVMGetLastGlobalIFunc(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalIFunc);
                   })
      .def_prop_ro("global_ifuncs",
                   [](PyModule &self) {
                     auto res = LLVMGetFirstGlobalIFunc(self.get());
                     return PyGlobalIFuncIterator(PyGlobalIFunc(res));
                   })
      .def_prop_ro("first_global_alias",
                   [](PyModule &self) -> optional<PyGlobalAlias> {
                     auto res = LLVMGetFirstGlobalAlias(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalAlias);
                   })
      .def_prop_ro("last_global_alias",
                   [](PyModule &self) -> optional<PyGlobalAlias> {
                     auto res = LLVMGetLastGlobalAlias(self.get());
                     WRAP_OPTIONAL_RETURN(res, PyGlobalAlias);
                   })
      .def_prop_ro("global_aliases",
                   [](PyModule &self) {
                     auto res = LLVMGetFirstGlobalAlias(self.get());
                     return PyGlobalAliasIterator(PyGlobalAlias(res));
                   })
      .def_prop_ro("first_named_metadata",
                   [](PyModule &m) -> optional<PyNamedMDNode> {
                     auto res = LLVMGetFirstNamedMetadata(m.get());
                     WRAP_OPTIONAL_RETURN(res, PyNamedMDNode);
                   },
                   "Obtain an iterator to the first NamedMDNode in a Module.")
      .def_prop_ro("last_named_metadata",
                   [](PyModule &m) -> optional<PyNamedMDNode> {
                     auto res = LLVMGetLastNamedMetadata(m.get());
                     WRAP_OPTIONAL_RETURN(res, PyNamedMDNode);
                   },
                   "Obtain an iterator to the last NamedMDNode in a Module.")
      .def_prop_ro("named_metadatas",
                   [](PyModule &m) {
                     auto res = LLVMGetFirstNamedMetadata(m.get());
                     return PyNamedMDNodeIterator(PyNamedMDNode(res));
                   },
                   "Obtain an iterator to the first NamedMDNode in a Module.")
      .def_prop_ro("context",
                   [](PyModule &m) {
                     return PyContext(LLVMGetModuleContext(m.get()));
                   },
                   "Obtain the context to which this module is associated.")
      .def_prop_rw("id",
                   [](PyModule &m) {
                     size_t len;
                     const char *identifier = LLVMGetModuleIdentifier(m.get(), &len);
                     return std::string(identifier, len);
                   },
                   [](PyModule &m, const std::string &id) {
                     return LLVMSetModuleIdentifier(m.get(), id.c_str(),
                                                    id.size());
                   },
                   nb::for_getter("Get the module identifier.\n"
                                  "Origin Function: LLVMSetModuleIdentifier."),
                   nb::for_setter("Set the module identifier.\n"
                                  "Origin Function: LLVMGetModuleIdentifier."))
      .def_prop_rw("source_file_name",
                   [](PyModule &m) {
                     size_t len;
                     const char* sfn = LLVMGetSourceFileName(m.get(), &len);
                     return std::string(sfn, len);
                   },
                   [](PyModule &m, const std::string &name) {
                     return LLVMSetSourceFileName(m.get(), name.c_str(), name.size());
                   },
                   "name"_a = "")
      .def_prop_rw("data_layout",
                   [](PyModule &m) {
                     return LLVMGetDataLayoutStr(m.get());
                   },
                   [](PyModule &m, const std::string &dlstr) {
                     return LLVMSetDataLayout(m.get(), dlstr.c_str());
                   },
                   nb::for_getter("Obtain the data layout for a module."),
                   nb::for_setter("Set the data layout for a module."))
      .def_prop_rw("target",
                   [](PyModule &m) {
                     return LLVMGetTarget(m.get());
                   },
                   [](PyModule &m, const std::string &triple) {
                     return LLVMSetTarget(m.get(), triple.c_str());
                   },
                   nb::for_getter("Obtain the target triple for a module."),
                   nb::for_setter("Set the target triple for a module."))
      .def_prop_rw("inline_asm",
                   [](PyModule &m) {
                     size_t len;
                     const char *iasm = LLVMGetModuleInlineAsm(m.get(), &len);
                     return std::string(iasm, len);
                   },
                   [](PyModule &m, std::string &iasm) {
                     // NOTE LLVMSetModuleInlineAsm is deprecated
                     return LLVMSetModuleInlineAsm2(m.get(), iasm.c_str(), iasm.size());
                   })
      .def_prop_ro("first_function",
                   [](PyModule &m) {
                     return PyFunction(LLVMGetFirstFunction(m.get()));
                   },
                   "Obtain an iterator to the first Function in a Module.")
      .def_prop_ro("last_function",
                   [](PyModule &m) {
                     return PyFunction(LLVMGetLastFunction(m.get()));
                   },
                   "Obtain an iterator to the last Function in a Module.")
      .def_prop_ro("functions",
                   [](PyModule &m) {
                     return PyFunctionIterator(PyFunction(LLVMGetFirstFunction(m.get())));
                   })
      .def("create_function_pass_manager",
           [](PyModule &self) {
             return PyFunctionPassManager(LLVMCreateFunctionPassManagerForModule(self.get()));
           },
           "Constructs a new function-by-function pass pipeline over the module"
           "provider. It does not take ownership of the module provider. This type of"
           "pipeline is suitable for code generation and JIT compilation tasks.")
      .def("create_module_provider",
           [](PyModule &self) {
             return PyModuleProvider(LLVMCreateModuleProviderForExistingModule
                                       (self.get()));
           })
      .def("get_intrinsic_declaration",
           [](PyModule &module, unsigned ID, std::vector<PyType> paramTypes) {
             size_t paramCnt = paramTypes.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, paramTypes, params, paramCnt);
             return PyFunction(LLVMGetIntrinsicDeclaration(module.get(), ID, params.data(),
                                                           paramCnt));
           },
           "id"_a, "param_types"_a,
           "Create or insert the declaration of an intrinsic.  For overloaded intrinsics,"
           "parameter types must be provided to uniquely identify an overload.")
      .def("add_alias",
           [](PyModule &self, PyType &valueType, unsigned addrSpace, PyValue aliasee,
              const char *name) {
             return PyGlobalAlias(LLVMAddAlias2
                                  (self.get(), valueType.get(), addrSpace,
                                   aliasee.get(), name));
           },
           "value_type"_a, "addr_space"_a, "aliasee"_a, "name"_a = "",
           "Add a GlobalAlias with the given value type, address space and aliasee.")
      .def("get_named_global_alias",
           [](PyModule &self, std::string &name) {
             return PyGlobalAlias(LLVMGetNamedGlobalAlias
                                  (self.get(), name.c_str(), name.size()));
           },
           "name"_a = "",
           "Obtain a GlobalAlias value from by its name.")
      .def("add_global",
           [](PyModule &self, PyType &type, const char *name) {
             return PyGlobalVariable(LLVMAddGlobal(self.get(), type.get(), name));
           },
           "type"_a, "name"_a = "")
      .def("add_global_in_address_space",
           [](PyModule &self, PyType &type, const char *name, unsigned addressSpace) {
             return PyGlobalVariable(LLVMAddGlobalInAddressSpace
                                  (self.get(), type.get(), name, addressSpace));
           },
           "type"_a, "name"_a = "", "address_space"_a = 0)
      .def("get_named_global",
           [](PyModule &self, const char *name) {
             return PyGlobalVariable(LLVMGetNamedGlobal(self.get(), name));
           })
      .def("add_global_indirect_func",
           [](PyModule &self, PyType &type, unsigned addrSpace, PyConstant resolver,
              std::string &name) {
             return PyGlobalIFunc(LLVMAddGlobalIFunc
                                    (self.get(), name.c_str(), name.size(), type.get(),
                                     addrSpace, resolver.get()));
           },
           "type"_a, "addr_space"_a, "resolver"_a, "name"_a = "")
      .def("get_named_global_ifunc",
           [](PyModule &self, std::string &name) -> std::optional<PyGlobalIFunc> {
             auto res = LLVMGetNamedGlobalIFunc(self.get(), name.c_str(), name.size());
             WRAP_OPTIONAL_RETURN(res, PyGlobalIFunc);
           })
      .def("add_function",
           [](PyModule &m, PyTypeFunction &functionTy, std::string &name) {
             return PyFunction(LLVMAddFunction(m.get(), name.c_str(), functionTy.get()));
           },
           "function_type"_a, "name"_a = "",
           "Add a function to a module under a specified name.")
      .def("get_named_function",
           [](PyModule &m, std::string &name) {
             return PyFunction(LLVMGetNamedFunction(m.get(), name.c_str()));
           }, "name"_a = "",
           "Obtain a Function value from a Module by its name.")
      .def("get_named_metadata",
           [](PyModule &m, std::string &name) -> optional<PyNamedMDNode> {
             auto res = LLVMGetNamedMetadata(m.get(), name.c_str(), name.size());
             WRAP_OPTIONAL_RETURN(res, PyNamedMDNode);
           },
           "name"_a = "",
           "Retrieve a NamedMDNode with the given name, returning NULL if no such"
           "node exists.")
      .def("get_or_insert_named_metadata",
           [](PyModule &m, std::string &name) {
             return PyNamedMDNode
                      (LLVMGetOrInsertNamedMetadata
                         (m.get(), name.c_str(), name.size()));
           },
           "name"_a = "",
           "Retrieve a NamedMDNode with the given name, creating a new node if no such"
           "node exists.")
      .def("get_named_metadata_operands_num",
           [](PyModule &m, std::string &name) {
             return LLVMGetNamedMetadataNumOperands(m.get(), name.c_str());
           }, "name"_a = "",
           "Obtain the number of operands for named metadata in a module.")
      .def("get_named_metadata_operands",
           [](PyModule &m, std::string &name) {
             int num = LLVMGetNamedMetadataNumOperands(m.get(), name.c_str());
             std::vector<LLVMValueRef> dest(num);
             LLVMGetNamedMetadataOperands(m.get(), name.c_str(), dest.data());
             WRAP_VECTOR_FROM_DEST_AUTO(PyValue, num, res, dest);
             return res;
           },
           "name"_a = "",
           "Obtain the named metadata operands for a module.\n\n"
           "The passed LLVMValueRef pointer should refer to an array of"
           "LLVMValueRef at least LLVMGetNamedMetadataNumOperands long. This"
           "array will be populated with the LLVMValueRef instances. Each"
           "instance corresponds to a llvm::MDNode.")
      .def("add_named_metadata_operand",
           [](PyModule &m, std::string &name, PyValue &val) {
             return LLVMAddNamedMetadataOperand(m.get(), name.c_str(), val.get());
           },
           "Add an operand to named metadata.")
      .def("clone",
           [](PyModule &m) {
             return PyModule(LLVMCloneModule(m.get()));
           },
           "Return an exact copy of the specified module.")
      .def("copy_module_flags_metadata",
           [](PyModule &m) {
             size_t Len;
             LLVMModuleFlagEntry *flags = LLVMCopyModuleFlagsMetadata(m.get(), &Len);
             auto res = PyModuleFlagEntries(flags, Len);
             return res;
           },
           "Returns the module flags as an array of flag-key-value triples.")
      .def("get_flag",
           [](PyModule &m, const std::string &key) -> optional<PyMetadata> {
             auto res = LLVMGetModuleFlag(m.get(), key.c_str(), key.size());
             WRAP_OPTIONAL_RETURN(res, PyMetadata);
           },
           "key"_a,
           "Return the corresponding value if Key appears in module flags, otherwise"
           "return null.")
      .def("add_flag",
           [](PyModule &m, LLVMModuleFlagBehavior behavior,
              const std::string key, PyMetadata &val) {
             return LLVMAddModuleFlag(m.get(), behavior, key.c_str(), key.size(),
                                      val.get());
           },
           "behavior"_a, "key"_a, "val"_a,
           "Add a module-level flag to the module-level flags metadata if it doesn't"
           "already exist.")
      .def("dump",
           [](PyModule &m) {
             return LLVMDumpModule(m.get());
           },
           "Dump a representation of a module to stderr.")
      .def("print_to_file",
           [](PyModule &m, const std::string &filename) {
             char *errorMessage = nullptr;
             LLVMBool res = LLVMPrintModuleToFile(m.get(), filename.c_str(),
                                                  &errorMessage);
             bool success = res == 0;
             
             if (!success) {
               std::string errorStr(errorMessage);
               LLVMDisposeMessage(errorMessage);
               throw std::runtime_error(errorStr);
             }
           },
           "filename"_a,
           "Print a representation of a module to a file.\n"
           ":raises RuntimeError")
      .def("append_inline_asm",
           [](PyModule &m, std::string &iasm) {
             return LLVMAppendModuleInlineAsm(m.get(), iasm.c_str(), iasm.size());
           })
      .def("get_type_by_name",
           [](PyModule &m, std::string &name) {
             return PyTypeAuto(LLVMGetTypeByName(m.get(), name.c_str()));
           }, "name"_a = "",
           "Deprecated: Use LLVMGetTypeByName2 instead.");

  ModuleFlagEntriesClass
      .def("__repr__",
           [](PyModuleFlagEntries &self) {
             return "<ModuleFlagEntries>";
           })
      .def("get_behavior",
           [](PyModuleFlagEntries &self, unsigned index) {
             // TODO test whether LLVMModuleFlagEntriesGetFlagBehavior will
             // check index is in bound
             return LLVMModuleFlagEntriesGetFlagBehavior(self.get(), index);
           },
           "index"_a,
           "Returns the flag behavior for a module flag entry at a specific index.")
      .def("get_key",
           [](PyModuleFlagEntries &self, unsigned index) {
             size_t len;
             const char *str = LLVMModuleFlagEntriesGetKey(self.get(), index, &len);
             return std::string(str, len);
           },
           "index"_a,
           "Returns the key for a module flag entry at a specific index.")
      .def("get_metadata",
           [](PyModuleFlagEntries &self, unsigned index) {
             // TODO can this return null?
             return PyMetadata(LLVMModuleFlagEntriesGetMetadata(self.get(), index));
           },
           "index"_a,
           "Returns the metadata for a module flag entry at a specific index.");
}

void bindIterators(nb::module_ &m) {
  BIND_ITERATOR_CLASS(PyUseIterator, "UseIterator")
  BIND_ITERATOR_CLASS(PyBasicBlockIterator, "BasicBlockIterator")
  BIND_ITERATOR_CLASS(PyArgumentIterator, "ArgumentIterator")
  BIND_ITERATOR_CLASS(PyInstructionIterator, "InstructionIterator")
  BIND_ITERATOR_CLASS(PyGlobalVariableIterator, "GlobalVariableIterator")
  BIND_ITERATOR_CLASS(PyGlobalIFuncIterator, "GlobalIFuncIterator")
  BIND_ITERATOR_CLASS(PyGlobalAliasIterator, "GlobalAliasIterator")
  BIND_ITERATOR_CLASS(PyNamedMDNodeIterator, "NamedMDNodeIterator")
  BIND_ITERATOR_CLASS(PyFunctionIterator, "FunctionIterator")
}


void populateCore(nb::module_ &m) {
  bindEnums(m);
  bindGlobalFunctions(m);
  bindTypeClasses(m);
  bindValueClasses(m);
  bindOtherClasses(m);
  bindIterators(m);
}


PyModule parseIR(LLVMContextRef ctx, LLVMMemoryBufferRef memBuf) {
  LLVMModuleRef m = nullptr;
  char *errMsg = nullptr;
  bool success = LLVMParseIRInContext(ctx, memBuf, &m, &errMsg) == 0;
             
  if (!success) {
    std::string errorMessage;
    if (m) LLVMDisposeModule(m);
    if (errMsg) errorMessage = std::string(errMsg);
    LLVMDisposeMessage(errMsg);
    throw std::runtime_error(errorMessage);
  }

  return PyModule(m);
}
