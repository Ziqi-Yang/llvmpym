#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/optional.h>
#include <optional>
#include <llvm-c/Core.h>
#include "Core.h"
#include "Types.h"
#include "Utils.h"

namespace nb = nanobind;
using namespace nb::literals;


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
    return new PyTypeSequence(rawType);
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
    return new PyBasicBlock(rawValue);
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
  case LLVMBlockAddressValueKind:
    return new PyBlockAddress(rawValue);
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
  case LLVMConstantAggregateZeroValueKind:
    return new PyConstantAggregateZero(rawValue);
  case LLVMConstantDataArrayValueKind:
    return new PyConstantDataArray(rawValue);
  case LLVMConstantDataVectorValueKind:
    return new PyConstantDataVector(rawValue);
  case LLVMConstantIntValueKind:
    return new PyConstantInt(rawValue);
  case LLVMConstantFPValueKind:
    return new PyConstantFP(rawValue);
  case LLVMConstantPointerNullValueKind:
    return new PyConstantPointerNull(rawValue);
  case LLVMConstantTokenNoneValueKind:
    return new PyConstantTokenNone(rawValue);

  // case LLVMMetadataAsValueValueKind: //
  case LLVMInlineAsmValueKind:
    return new PyInlineAsm(rawValue);

  case LLVMInstructionValueKind:
    return new PyInstruction(rawValue);
  case LLVMPoisonValueValueKind:
    return new PyPoisonValue(rawValue);
  // case LLVMConstantTargetNoneValueKind: // 
  default:
    return new PyValue(rawValue);
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
      .value("False", LLVMRealPredicate::LLVMRealPredicateFalse, "Always false (always folded)")
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
      .value("True", LLVMRealPredicate::LLVMRealPredicateTrue, "Always true (always folded)");
  

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
       .value("None", PyLLVMFastMathFlags::None)
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

  // NOTE these two functions are useless in Python?
  // m.def("create_message", &LLVMCreateMessage, "message"_a);
  // m.def("dispose_message", &LLVMDisposeMessage, "message"_a); // error, may need a wrapper for created message


  m.def("set_mk_kind_id", [](const std::string &name) {
    return LLVMGetMDKindID(name.c_str(), name.size());
  }, "name"_a);

  m.def("get_enum_attribute_kind_for_name",
        [](const std::string &name){
          return LLVMGetEnumAttributeKindForName(name.c_str(), name.size());
        }, "name"_a,
        "Return an unique id given the name of a enum attribute,"
        "or 0 if no attribute by that name exists.\n\n"
        "See http://llvm.org/docs/LangRef.html#parameter-attributes"
        "and http://llvm.org/docs/LangRef.html#function-attributes"
        "for the list of available attributes.\n\n"
        "NB: Attribute names and/or id are subject to change without"
        "going through the C API deprecation cycle.");

  m.def("get_last_enum_attribute_kind", &LLVMGetLastEnumAttributeKind);

  m.def("get_inline_asm",
        [](PyType &ty, std::string asmString, std::string constraints,
           bool hasSideEffects, bool isAlignStack, LLVMInlineAsmDialect dialect,
           bool canThrow){
          return PyInlineAsm(LLVMGetInlineAsm
                               (ty.get(), asmString.c_str(), asmString.size(),
                                constraints.c_str(), constraints.size(),
                                hasSideEffects, isAlignStack, dialect, canThrow));
        }, "type"_a, "asm"_a, "constraints"_a, "has_side_effects"_a,
        "is_align_stack"_a, "dialect"_a, "can_throw"_a,
        "Create the specified uniqued inline asm string.");

  m.def("replace_all_uses_with",
        [](PyValue &oldVal, PyValue &newVal) {
          return LLVMReplaceAllUsesWith(oldVal.get(), newVal.get());
        },
        "Replace all uses of a value with another one.");
}





void bindTypeClasses(nb::module_ &m) {
  nb::class_<PyType>(m, "Type", "Type")
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
      .def("__str__",
           [](PyType &t) {
             char *str = LLVMPrintTypeToString(t.get());
             std::string res(str);
             LLVMDisposeMessage(str);
             return res;
           })
      .def_prop_ro("sub_type_number",
             [](PyTypeSequence &t) { return LLVMGetNumContainedTypes(t.get()); })
      .def_prop_ro("sub_types",
                   [](PyTypeSequence &t) {
                     unsigned num = LLVMGetNumContainedTypes(t.get());
                     LLVMTypeRef *arr;
                     LLVMGetSubtypes(t.get(), arr);
                     WRAP_VECTOR_FROM_DEST(PyTypeSequence, num, res, arr);
                     return res;
                   })
      .def_prop_ro("Null",
                  [](PyType &t) {
                    return PyValueAuto(LLVMConstNull(t.get()));
                  },
                   "Obtain a constant value referring to the null instance of the type.")
      .def_static("PointerNull",
                  [](PyType &t) {
                    return PyValueAuto(LLVMConstPointerNull(t.get()));
                  },
                  "Obtain a constant that is a constant pointer pointing to NULL for the"
                  "type.")
      .def_prop_ro("AllOnes",
                  [](PyTypeInt &t) {
                    return PyValueAuto(LLVMConstAllOnes(t.get()));
                  },
                  "Obtain a constant value referring to the instance of the type"
                  "consisting of all ones.")
      .def_prop_ro("Undef",
                  [](PyType &t) {
                    return PyUndefValue(LLVMGetUndef(t.get()));
                  },
                  "Obtain a constant value referring to an undefined value of a type.")
      .def_prop_ro("Poison",
                  [](PyType &t) {
                    return PyPoisonValue(LLVMGetPoison(t.get()));
                  },
                   "Obtain a constant value referring to a poison value of a type.");
  
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
      .def_static("global",
                  [](unsigned numBits) { return PyTypeInt(LLVMIntType(numBits)); },
                  "Get type from global context.")
      .def_prop_ro("width",
                   [](PyTypeInt &t) { return LLVMGetIntTypeWidth(t.get()); });
  

  TypeRealClass
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
       .def("__init__",
            [](PyTypeFunction *t, PyType &returnType, std::vector<PyType> &paramTypes, bool isVarArg) {
              unsigned param_count = paramTypes.size();
              UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, paramTypes, rawParamTypes, param_count)
              new (t) PyTypeFunction(LLVMFunctionType(returnType.get(), rawParamTypes.data(),
                                                     param_count, isVarArg));
            }, "return_type"_a, "param_types"_a, "is_var_arg"_a,
            "Obtain a function type consisting of a specified signature.")
      .def_prop_ro("is_vararg",
                   [](PyTypeFunction &t) { return LLVMIsFunctionVarArg(t.get()) != 0; },
                   "Returns whether a function type is variadic.")
      .def_prop_ro("return_type",
                   [](PyTypeFunction &t) { return PyTypeAuto(LLVMGetReturnType(t.get())); },
                   "Obtain the Type this function Type returns.")
      .def_prop_ro("param_number",
                   [](PyTypeFunction &t) { return LLVMCountParamTypes(t.get()); },
                   "Obtain the number of parameters this function accepts.")
      .def_prop_ro("param_types",
                   [](PyTypeFunction &t) {
                     unsigned param_number = LLVMCountParamTypes(t.get());
                     LLVMTypeRef *dest;
                     LLVMGetParamTypes(t.get(), dest);
                     WRAP_VECTOR_FROM_DEST(PyTypeFunction, param_number, res, dest);
                     return res;
                   },
                   "Obtain the types of a function's parameters.");
  
  TypeStructClass
      .def("__init__",
           [](PyTypeStruct *t, PyContext &c, std::vector<PyType> &elementTypes, bool packed) {
             unsigned elem_count = elementTypes.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, elementTypes, rawElemTypes, elem_count);
             new (t) PyTypeStruct(LLVMStructTypeInContext(c.get(), rawElemTypes.data(),
                                                         elem_count, packed));
           }, "context"_a, "element_types"_a, "packed"_a,
           "Create a new structure type in context.")
      .def("__init__",
           [](PyTypeStruct *t, PyContext &c, std::string name) {
             new (t) PyTypeStruct(LLVMStructCreateNamed(c.get(), name.c_str()));
           }, "context"_a, "name"_a,
           "Create an empty structure in the context having a specified name.")
      .def_static("global",
        [](std::vector<PyType> elementTypes, bool packed) {
          unsigned elem_count = elementTypes.size();
          UNWRAP_VECTOR_WRAPPER_CLASS(LLVMTypeRef, elementTypes, rawElemTypes, elem_count);
          return PyTypeStruct(LLVMStructType(rawElemTypes.data(), elem_count, packed));
        },
        "element_types"_a, "packed"_a,
        "Create a new structure type in the global context.")
      .def_prop_ro("name", [](PyTypeStruct &t) { return LLVMGetStructName(t.get()); })
      .def_prop_ro("elem_number", [](PyTypeStruct &t) { return LLVMCountStructElementTypes(t.get()); })
      .def_prop_ro("elem_types",
                   [](PyTypeStruct &t) {
                     unsigned num = LLVMCountStructElementTypes(t.get());
                     LLVMTypeRef *dest;
                     LLVMGetStructElementTypes(t.get(), dest);
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
           "Set the contents of a structure type.")
      .def("get_type_at_index",
           [](PyTypeStruct &t, unsigned i) {
             return PyTypeAuto(LLVMStructGetTypeAtIndex(t.get(), i));
           },
           "Get the type of the element at a given index in the structure.");

  TypeSequenceClass
      .def_prop_ro("element_type", // TODO test pointer type
                   [](PyTypeSequence &t) { return PyTypeAuto(LLVMGetElementType(t.get())); });

  TypeArrayClass
      .def("__init__", // NOTE LLVMArrayType and LLVMArrayType2 are nearly the same
           [](PyTypeArray *t, PyType &elementType, unsigned elementCount) {
             new (t) PyTypeArray(LLVMArrayType(elementType.get(), elementCount));
           },
           "elem_type"_a, "elem_count"_a,
           "Create a fixed size array type that refers to a specific type.\n\n"
           "The created type will exist in the context that its element type"
           "exists in.")
      .def_prop_ro("length", // LLVMGetArrayLength and LLVMGetArrayLength2 are nearly the same
                   [](PyTypeArray &t) { return LLVMGetArrayLength(t.get()); });


  TypePointerClass
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
      .def("__init__",
           [](PyTypeVoid *t, PyContext &c) {
             new (t) PyTypeVoid(LLVMVoidTypeInContext(c.get()));
           },
           "context"_a)
      .def_prop_ro_static("Global",
                          [](nb::handle) { return PyTypeVoid(LLVMVoidType()); } );
  TypeLabelClass
      .def("__init__",
           [](PyTypeLabel *t, PyContext &c) {
             new (t) PyTypeLabel(LLVMLabelTypeInContext(c.get()));
           },
           "context"_a)
      .def_prop_ro_static("Global",
                          [](nb::handle) { return PyTypeLabel(LLVMLabelType()); } );


  TypeX86MMXClass
      .def("__init__",
           [](PyTypeX86MMX *t, PyContext &c) {
             new (t) PyTypeX86MMX(LLVMX86MMXTypeInContext(c.get()));
           },
           "context"_a)
      .def_prop_ro_static("Global",
                          [](nb::handle) { return PyTypeX86MMX(LLVMX86MMXType()); } );


  TypeX86AMXClass
       .def("__init__",
            [](PyTypeX86AMX *t, PyContext &c) {
              new (t) PyTypeX86AMX(LLVMX86AMXTypeInContext(c.get()));
            },
            "context"_a)
       .def_prop_ro_static("Global",
                           [](nb::handle) { return PyTypeX86AMX(LLVMX86AMXType()); } );


  TypeTokenClass
      .def("__init__",
           [](PyTypeToken *t, PyContext &c) {
             new (t) PyTypeToken(LLVMTokenTypeInContext(c.get()));
           },
           "context"_a);

  TypeMetadataClass
      .def("__init__",
           [](PyTypeMetadata *t, PyContext &c) {
             new (t) PyTypeMetadata(LLVMMetadataTypeInContext(c.get()));
           },
           "context"_a);

  TypeTargetExtClass
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

  // my custom addition
  auto AMDNodeClass = nb::class_<PyAMDNode, PyValue>(m, "AMDNode", "AMDNode");
  auto ValueAsMetadataClass = nb::class_<PyValueAsMetadata, PyValue>
                                (m, "ValueAsMetadata", "ValueAsMetadata");
  auto AMDStringClass = nb::class_<PyAMDString, PyValue>(m, "AMDString", "AMDString");

  
  // classes as specified in LLVM_FOR_EACH_VALUE_SUBCLASS
  auto ArgumentClass = nb::class_<PyArgument, PyValue>(m, "Argument", "Argument");
  auto BasicBlockClass = nb::class_<PyBasicBlock, PyValue>(m, "BasicBlock", "BasicBlock");
  auto InlineAsmClass = nb::class_<PyInlineAsm, PyValue>(m, "InlineAsm", "InlineAsm");
  auto UserClass = nb::class_<PyUser, PyValue>(m, "User", "User");
  auto ConstantClass = nb::class_<PyConstant, PyUser>(m, "Constant", "Constant");
  auto BlockAddressClass = nb::class_<PyBlockAddress, PyConstant>(m, "BlockAddress", "BlockAddress");
  auto ConstantAggregateZeroClass = nb::class_<PyConstantAggregateZero, PyConstant>(m, "ConstantAggregateZero", "ConstantAggregateZero");
  auto ConstantArrayClass = nb::class_<PyConstantArray, PyConstant>(m, "ConstantArray", "ConstantArray");
  auto ConstantDataSequentialClass = nb::class_<PyConstantDataSequential, PyConstant>(m, "ConstantDataSequential", "ConstantDataSequential");
  auto ConstantDataArrayClass = nb::class_<PyConstantDataArray, PyConstantDataSequential>(m, "ConstantDataArray", "ConstantDataArray");
  auto ConstantDataVectorClass = nb::class_<PyConstantDataVector, PyConstantDataSequential>(m, "ConstantDataVector", "ConstantDataVector");
  auto ConstantExprClass = nb::class_<PyConstantExpr, PyConstant>(m, "ConstantExpr", "ConstantExpr");
  auto ConstantFPClass = nb::class_<PyConstantFP, PyConstant>(m, "ConstantFP", "ConstantFP");
  auto ConstantIntClass = nb::class_<PyConstantInt, PyConstant>(m, "ConstantInt", "ConstantInt");
  auto ConstantPointerNullClass = nb::class_<PyConstantPointerNull, PyConstant>(m, "ConstantPointerNull", "ConstantPointerNull");
  auto ConstantStructClass = nb::class_<PyConstantStruct, PyConstant>(m, "ConstantStruct", "ConstantStruct");
  auto ConstantTokenNoneClass = nb::class_<PyConstantTokenNone, PyConstant>(m, "ConstantTokenNone", "ConstantTokenNone");
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
  auto UnaryOperatorClass = nb::class_<PyUnaryOperator, PyInstruction>(m, "UnaryOperator", "UnaryOperator");
  auto BinaryOperatorClass = nb::class_<PyBinaryOperator, PyInstruction>(m, "BinaryOperator", "BinaryOperator");
  auto CallInstClass = nb::class_<PyCallInst, PyInstruction>(m, "CallInst", "CallInst");
  auto IntrinsicInstClass = nb::class_<PyIntrinsicInst, PyCallInst>(m, "IntrinsicInst", "IntrinsicInst");
  auto DbgInfoIntrinsicClass = nb::class_<PyDbgInfoIntrinsic, PyIntrinsicInst>(m, "DbgInfoIntrinsic", "DbgInfoIntrinsic");
  auto DbgVariableIntrinsicClass = nb::class_<PyDbgVariableIntrinsic, PyDbgInfoIntrinsic>(m, "DbgVariableIntrinsic", "DbgVariableIntrinsic");
  auto DbgDeclareInstClass = nb::class_<PyDbgDeclareInst, PyDbgVariableIntrinsic>(m, "DbgDeclareInst", "DbgDeclareInst");
  auto DbgLabelInstClass = nb::class_<PyDbgLabelInst, PyIntrinsicInst>(m, "DbgLabelInst", "DbgLabelInst");
  auto MemIntrinsicClass = nb::class_<PyMemIntrinsic, PyIntrinsicInst>(m, "MemIntrinsic", "MemIntrinsic");
  auto MemCpyInstClass = nb::class_<PyMemCpyInst, PyIntrinsicInst>(m, "MemCpyInst", "MemCpyInst");
  auto MemMoveInstClass = nb::class_<PyMemMoveInst, PyIntrinsicInst>(m, "MemMoveInst", "MemMoveInst");
  auto MemSetInstClass = nb::class_<PyMemSetInst, PyIntrinsicInst>(m, "MemSetInst", "MemSetInst");
  auto CmpInstClass = nb::class_<PyCmpInst, PyInstruction>(m, "CmpInst", "CmpInst");
  auto FCmpInstClass = nb::class_<PyFCmpInst, PyCmpInst>(m, "FCmpInst", "FCmpInst");
  auto ICmpInstClass = nb::class_<PyICmpInst, PyCmpInst>(m, "ICmpInst", "ICmpInst");
  auto ExtractElementInstClass = nb::class_<PyExtractElementInst, PyInstruction>(m, "ExtractElementInst", "ExtractElementInst");
  auto GetElementPtrInstClass = nb::class_<PyGetElementPtrInst, PyInstruction>(m, "GetElementPtrInst", "GetElementPtrInst");
  auto InsertElementInstClass = nb::class_<PyInsertElementInst, PyInstruction>(m, "InsertElementInst", "InsertElementInst");
  auto InsertValueInstClass = nb::class_<PyInsertValueInst, PyInstruction>(m, "InsertValueInst", "InsertValueInst");
  auto LandingPadInstClass = nb::class_<PyLandingPadInst, PyInstruction>(m, "LandingPadInst", "LandingPadInst");
  auto PHINodeClass = nb::class_<PyPHINode, PyInstruction>(m, "PHINode", "PHINode");
  auto SelectInstClass = nb::class_<PySelectInst, PyInstruction>(m, "SelectInst", "SelectInst");
  auto ShuffleVectorInstClass = nb::class_<PyShuffleVectorInst, PyInstruction>(m, "ShuffleVectorInst", "ShuffleVectorInst");
  auto StoreInstClass = nb::class_<PyStoreInst, PyInstruction>(m, "StoreInst", "StoreInst");
  auto BranchInstClass = nb::class_<PyBranchInst, PyInstruction>(m, "BranchInst", "BranchInst");
  auto IndirectBrInstClass = nb::class_<PyIndirectBrInst, PyInstruction>(m, "IndirectBrInst", "IndirectBrInst");
  auto InvokeInstClass = nb::class_<PyInvokeInst, PyInstruction>(m, "InvokeInst", "InvokeInst");
  auto ReturnInstClass = nb::class_<PyReturnInst, PyInstruction>(m, "ReturnInst", "ReturnInst");
  auto SwitchInstClass = nb::class_<PySwitchInst, PyInstruction>(m, "SwitchInst", "SwitchInst");
  auto UnreachableInstClass = nb::class_<PyUnreachableInst, PyInstruction>(m, "UnreachableInst", "UnreachableInst");
  auto ResumeInstClass = nb::class_<PyResumeInst, PyInstruction>(m, "ResumeInst", "ResumeInst");
  auto CleanupReturnInstClass = nb::class_<PyCleanupReturnInst, PyInstruction>(m, "CleanupReturnInst", "CleanupReturnInst");
  auto CatchReturnInstClass = nb::class_<PyCatchReturnInst, PyInstruction>(m, "CatchReturnInst", "CatchReturnInst");
  auto CatchSwitchInstClass = nb::class_<PyCatchSwitchInst, PyInstruction>(m, "CatchSwitchInst", "CatchSwitchInst");
  auto CallBrInstClass = nb::class_<PyCallBrInst, PyInstruction>(m, "CallBrInst", "CallBrInst");
  auto FuncletPadInstClass = nb::class_<PyFuncletPadInst, PyInstruction>(m, "FuncletPadInst", "FuncletPadInst");
  auto CatchPadInstClass = nb::class_<PyCatchPadInst, PyFuncletPadInst>(m, "CatchPadInst", "CatchPadInst");
  auto CleanupPadInstClass = nb::class_<PyCleanupPadInst, PyFuncletPadInst>(m, "CleanupPadInst", "CleanupPadInst");
  auto UnaryInstructionClass = nb::class_<PyUnaryInstruction, PyInstruction>(m, "UnaryInstruction", "UnaryInstruction");
  auto AllocaInstClass = nb::class_<PyAllocaInst, PyUnaryInstruction>(m, "AllocaInst", "AllocaInst");
  auto CastInstClass = nb::class_<PyCastInst, PyUnaryInstruction>(m, "CastInst", "CastInst");
  auto AddrSpaceCastInstClass = nb::class_<PyAddrSpaceCastInst, PyCastInst>(m, "AddrSpaceCastInst", "AddrSpaceCastInst");
  auto BitCastInstClass = nb::class_<PyBitCastInst, PyCastInst>(m, "BitCastInst", "BitCastInst");
  auto FPExtInstClass = nb::class_<PyFPExtInst, PyCastInst>(m, "FPExtInst", "FPExtInst");
  auto FPToSIInstClass = nb::class_<PyFPToSIInst, PyCastInst>(m, "FPToSIInst", "FPToSIInst");
  auto FPToUIInstClass = nb::class_<PyFPToUIInst, PyCastInst>(m, "FPToUIInst", "FPToUIInst");
  auto FPTruncInstClass = nb::class_<PyFPTruncInst, PyCastInst>(m, "FPTruncInst", "FPTruncInst");
  auto IntToPtrInstClass = nb::class_<PyIntToPtrInst, PyCastInst>(m, "IntToPtrInst", "IntToPtrInst");
  auto PtrToIntInstClass = nb::class_<PyPtrToIntInst, PyCastInst>(m, "PtrToIntInst", "PtrToIntInst");
  auto SExtInstClass = nb::class_<PySExtInst, PyCastInst>(m, "SExtInst", "SExtInst");
  auto SIToFPInstClass = nb::class_<PySIToFPInst, PyCastInst>(m, "SIToFPInst", "SIToFPInst");
  auto TruncInstClass = nb::class_<PyTruncInst, PyCastInst>(m, "TruncInst", "TruncInst");
  auto UIToFPInstClass = nb::class_<PyUIToFPInst, PyCastInst>(m, "UIToFPInst", "UIToFPInst");
  auto ZExtInstClass = nb::class_<PyZExtInst, PyCastInst>(m, "ZExtInst", "ZExtInst");
  auto ExtractValueInstClass = nb::class_<PyExtractValueInst, PyUnaryInstruction>(m, "ExtractValueInst", "ExtractValueInst");
  auto LoadInstClass = nb::class_<PyLoadInst, PyUnaryInstruction>(m, "LoadInst", "LoadInst");
  auto VAArgInstClass = nb::class_<PyVAArgInst, PyUnaryInstruction>(m, "VAArgInst", "VAArgInst");
  auto FreezeInstClass = nb::class_<PyFreezeInst, PyUnaryInstruction>(m, "FreezeInst", "FreezeInst");
  auto AtomicCmpXchgInstClass = nb::class_<PyAtomicCmpXchgInst, PyInstruction>(m, "AtomicCmpXchgInst", "AtomicCmpXchgInst");
  auto AtomicRMWInstClass = nb::class_<PyAtomicRMWInst, PyInstruction>(m, "AtomicRMWInst", "AtomicRMWInst");
  auto FenceInstClass = nb::class_<PyFenceInst, PyInstruction>(m, "FenceInst", "FenceInst");


  ValueClass
      .def_prop_ro("type",
                   // TODO PyType convertion to more specific type according to kind
                   [](PyValue &v) { return PyTypeAuto(LLVMTypeOf(v.get())); })
      .def_prop_ro("kind",
                   [](PyValue &v) { return LLVMGetValueKind(v.get()); })
      // NOTE LLVMSetValueName and LLVMGetValueName are depreciated
      .def_prop_rw("name",
                   [](PyValue &v) {
                     size_t len;
                     const char *str = LLVMGetValueName2(v.get(), &len);
                     return std::string(str, len);
                   },
                   [](PyValue &v, std::string &name) {
                     return LLVMSetValueName2(v.get(), name.c_str(), name.size());
                   },
                   nb::for_setter("def name(self, name: str, /) -> None"))
       .def_prop_ro("is_constant",
                    [](PyValue &v) { return LLVMIsConstant(v.get()) != 0; })
       .def_prop_ro("is_undef",
                    [](PyValue &v) { return LLVMIsUndef(v.get()) != 0; })
       .def_prop_ro("is_poisonous",
                    [](PyValue &v) { return LLVMIsPoison(v.get()) != 0;})
       .def_prop_ro("first_use",
                    [](PyValue &v) { return PyUse(LLVMGetFirstUse(v.get())); })
       .def("dump",
            [](PyValue &v) { return LLVMDumpValue(v.get()); },
            "Dump a representation of a value to stderr.")
       .def("__str__",
            [](PyValue &v) { return std::string(LLVMPrintValueToString(v.get())); })
  PY_FOR_EACH_VALUE_SUBCLASS(PY_DECLARE_VALUE_CAST)
       .def("to_AMDNode",
            [](PyValue &v) -> std::optional<PyAMDNode> {
              auto res = LLVMIsAMDNode(v.get());
              if (res)
                return PyAMDNode(res);
              return std::nullopt;
            })
       .def("to_ValueAsMetadata",
            [](PyValue &v) -> std::optional<PyValueAsMetadata> {
              auto res = LLVMIsAMDNode(v.get());
              if (res)
                return PyValueAsMetadata(res);
              return std::nullopt;
            })
       .def("to_AMDString",
            [](PyValue &v) -> std::optional<PyAMDString> {
              auto res = LLVMIsAMDString(v.get());
              if (res)
                return PyAMDString(res);
              return std::nullopt;
            });


  InlineAsmClass
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
                   "Return the column number of the debug location for this value");

  GlobalVariableClass
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
                   "Return the line number of the debug location for this value");

  FunctionClass
      .def_prop_ro("next",
                   [](PyFunction &f) -> std::optional<PyFunction> {
                     auto res = LLVMGetNextFunction(f.get());
                     if (res != nullptr)
                       return PyFunction(res);
                     return std::nullopt;
                   },
                   "Advance a Function iterator to the next Function.\n\n"
                   "Returns NULL if the iterator was already at the end and there are no more"
                   "functions.")
      .def_prop_ro("previous",
                   [](PyFunction &f) -> std::optional<PyFunction> {
                     auto res = LLVMGetPreviousFunction(f.get());
                     if (res != nullptr)
                       return PyFunction(res);
                     return std::nullopt;
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
                   "Return the line number of the debug location for this value");

  UserClass
      .def("get_operand",
           [](PyUser &u, unsigned index) {
             return PyValueAuto(LLVMGetOperand(u.get(), index));
           }, "index"_a,
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
           "Set an operand at a specific index")
       .def_prop_ro("operands_number",
                    [](PyUser &u) {
                      return LLVMGetNumOperands(u.get());
                    });

  ConstantClass
      // note these constructors are also available in TypeClass
      .def_static("Null",
                  [](PyType &t) {
                    return PyValueAuto(LLVMConstNull(t.get()));
                  }, "type"_a,
                  "Obtain a constant value referring to the null instance of the type.")
      .def_static("PointerNull",
                  [](PyType &t) {
                    return PyValueAuto(LLVMConstPointerNull(t.get()));
                  }, "type"_a,
                  "Obtain a constant that is a constant pointer pointing to NULL for the"
                  "type.")
      .def_static("AllOnes",
                  [](PyTypeInt &t) {
                    return PyValueAuto(LLVMConstAllOnes(t.get()));
                  }, "type"_a,
                  "Obtain a constant value referring to the instance of the type"
                  "consisting of all ones.")
      .def_static("Undef",
                  [](PyType &t) {
                    return PyUndefValue(LLVMGetUndef(t.get()));
                  }, "type"_a,
                  "Obtain a constant value referring to an undefined value of a type.")
      .def_static("Poison",
                  [](PyType &t) {
                    return PyPoisonValue(LLVMGetPoison(t.get()));
                  }, "type"_a,
                  "Obtain a constant value referring to a poison value of a type.")
      .def_prop_ro("is_null",
                   [](PyConstant &c) { return LLVMIsNull(c.get()) != 0; });


  ConstantIntClass
      .def("__init__",
           [](PyConstantInt *c, PyTypeInt &t, unsigned long long N, bool SignExtend) {
             new (c) PyConstantInt(LLVMConstInt
                                            (t.get(), N, SignExtend));
           },
           "int_type"_a, "value"_a, "sign_extend"_a,
           "Obtain a constant value for an integer type.\n\n"
           "Parameters:\n--------"
           "int_type: IntTy Integer type to obtain value of.\n"
           "value: The value the returned instance should refer to.\n"
           "sign_extend: Whether to sign extend the produced value.")
      .def_static("ArbitraryPrecision",
                  [](PyTypeInt &t, unsigned NumWords, const uint64_t Words[]) {
                    auto res = LLVMConstIntOfArbitraryPrecision
                                 (t.get(), NumWords, Words);
                    return PyConstantInt(res);
                  },
                  "int_type"_a, "num_words"_a, "words"_a,
                  "Obtain a constant value for an integer of arbitrary precision.")
      // LLVMConstIntOfString is discarded in favor of LLVMConstIntOfStringAndSize 
      .def_static("String", 
                  [](PyTypeInt &t, std::string &text, uint8_t Radix) {
                    return PyConstantInt(LLVMConstIntOfStringAndSize
                                           (t.get(), text.c_str(), text.size(),
                                            Radix));
                  },
                  "int_type"_a, "text"_a, "radix"_a,
                  "Obtain a constant value for an integer parsed from a string.")
      .def_prop_ro("zero_extension",
                   [](PyConstantInt &c) {
                     return LLVMConstIntGetZExtValue(c.get());
                   },
                   "Obtain the zero extended value for an integer constant value.")
      .def_prop_ro("signed_extension",
                   [](PyConstantInt &c) {
                     return LLVMConstIntGetSExtValue(c.get());
                   });

  ConstantFPClass
      .def("__init__",
           [](PyConstantFP *c, PyTypeReal &t, double value) {
             new (c) PyConstantFP(LLVMConstReal(t.get(), value));
           },
           "real_type"_a, "value"_a,
           "Obtain a constant value referring to a double floating point value.\n\n")
      // LLVMConstRealOfString is discarded in favor of LLVMConstRealOfStringAndSize
      .def_static("String",
                  [](PyTypeReal &t, std::string text) {
                    return PyConstantFP(LLVMConstRealOfStringAndSize
                                          (t.get(), text.c_str(), text.size()));
                  },
                  "Obtain a constant for a floating point value parsed from a string.")
      .def("get_double",
           [](PyConstantFP &c) {
             LLVMBool losesInfo;
             double res = LLVMConstRealGetDouble(c.get(), &losesInfo);
             return std::make_tuple(res, losesInfo);
           },
           "Obtain the double value for an floating point constant value.\n\n"
           "Parameters\n--------"
           "loses_info: indicates if some precision was lost in the conversion.");

  ConstantDataArrayClass
      .def_static("StringInContext",
                  [](PyContext &cxt, std::string &str, bool DontNullTerminate) {
                    return PyConstantDataArray
                             (LLVMConstStringInContext
                                (cxt.get(), str.c_str(), str.size(), DontNullTerminate));
                  },
                  "context"_a, "str"_a, "dont_null_terminate"_a,
                  "Create a ConstantDataSequential and initialize it with a string.")
      .def_static("String",
                  [](std::string &str, bool DontNullTerminate) {
                    return PyConstantDataArray
                             (LLVMConstString
                                (str.c_str(), str.size(), DontNullTerminate));
                  },
                  "str"_a, "dont_null_terminate"_a,
                  "Create a ConstantDataSequential with string content in the global context.")
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
      .def("__init__",
           [](PyConstantStruct *c, PyContext &cxt, std::vector<PyConstant> &vc, bool packed) {
             unsigned cnt = vc.size();
             UNWRAP_VECTOR_WRAPPER_CLASS(LLVMValueRef, vc, raw, cnt);
             new (c) PyConstantStruct(LLVMConstStructInContext
                                        (cxt.get(), raw.data(), cnt, packed));
           },
           "context"_a, "constant_values"_a, "packed"_a,
           "Create an anonymous ConstantStruct with the specified values.");
}






void populateCore(nb::module_ &m) {
  bindEnums(m);
  bindGlobalFunctions(m);
  bindTypeClasses(m);
  bindValueClasses(m);

  nb::class_<PyContext>
    (m, "Context",
     "Contexts are execution states for the core LLVM IR system.\n\n"
     "Most types are tied to a context instance. Multiple contexts can"
     "exist simultaneously. A single context is not thread safe. However,"
     "different contexts can execute on different threads simultaneously.")
      .def(nb::init<>(), "Create a new context.")
      .def_static("get_global_context", &PyContext::getGlobalContext, "Obtain the global context instance.")
      .def_prop_ro("diagnostic_context", // TODO more check: in my test it simply None
                   [](PyContext &c) { return LLVMContextGetDiagnosticContext(c.get()); },
                   "Get the diagnostic context of this context.")
      .def_prop_rw("should_discard_value_names", // TODO convert LLVMBool to bool
                   [](PyContext &c) -> bool { return LLVMContextShouldDiscardValueNames(c.get()) != 0; },
                   [](PyContext &c, bool discard) {
                     return LLVMContextSetDiscardValueNames(c.get(), discard);
                   },
                   nb::for_getter(nb::sig("def should_discard_value_names(self, /) -> bool")),
                   nb::for_setter(nb::sig("def should_discard_value_names(self, bool /) -> None")),
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
      .def("get_md_kind_id",
           [](PyContext &c, const std::string &name) {
             return LLVMGetMDKindIDInContext(c.get(), name.c_str(), name.size());
           }, "name"_a)
      .def("create_enum_attribute",
           [](PyContext &c, unsigned kindID, uint64_t val) {
             return PyEnumAttribute(LLVMCreateEnumAttribute(c.get(), kindID, val));
           }, "kind_id"_a, "val"_a,
           "Create an enum attribute.")
      .def("create_type_attribute",
           [](PyContext &context, unsigned kind_id, PyType &type) {
             return PyTypeAttribute(LLVMCreateTypeAttribute(context.get(), kind_id, type.get()));
           }, "kind_id"_a, "type"_a,
           "Create a type attribute")
      .def("create_string_attribute",
           [](PyContext &c, const std::string &kind, const std::string &value) {
             auto raw = LLVMCreateStringAttribute(c.get(),
                                                  kind.c_str(), kind.size(),
                                                  value.c_str(), value.size());
             return PyStringAttribute(raw);
           })
      .def("get_type_by_name_2", // TODO also create one in PyType static method
           [](PyContext &c, const std::string &name) {
             return PyTypeAuto(LLVMGetTypeByName2(c.get(), name.c_str()));
           });

  
  
  nb::class_<PyAttribute>(m, "Attribute", "Attribute")
        .def("is_enum", [](PyAttribute &attr) {
          return LLVMIsEnumAttribute(attr.get()) != 0;
        })
        .def("is_string", [](PyAttribute &attr) {
          return LLVMIsStringAttribute(attr.get()) != 0;
        })
        .def("is_type", [](PyAttribute &attr) {
          return LLVMIsTypeAttribute(attr.get()) != 0;
        });
  nb::class_<PyEnumAttribute, PyAttribute>(m, "EnumAttribute", "EnumAttribute")
      .def("__init__",
           [](PyEnumAttribute *t, PyContext &c, unsigned kindID, uint64_t val) {
             new (t) PyEnumAttribute(LLVMCreateEnumAttribute(c.get(), kindID, val));
           }, "context"_a, "kind_id"_a, "val"_a)
      .def_prop_ro("kind",
                   [](PyEnumAttribute &attr) {
                     return LLVMGetEnumAttributeKind(attr.get());
                   }, "attr"_a)
      .def_prop_ro("value",
                   [](PyEnumAttribute &attr) {
                     return LLVMGetEnumAttributeValue(attr.get());
                   });

  nb::class_<PyTypeAttribute, PyAttribute>(m, "TypeAttribute", "TypeAttribute")
      .def("__init__",
           [](PyTypeAttribute *t, PyContext &context, unsigned kind_id, PyType &type) {
             new (t) PyTypeAttribute(LLVMCreateTypeAttribute(context.get(), kind_id, type.get()));
           }, "context"_a, "kind_id"_a, "type"_a)
      .def_prop_ro("value",
                   [](PyTypeAttribute &ta){
                     return PyTypeAuto(LLVMGetTypeAttributeValue(ta.get()));
                   }, "Get the type attribute's value.");

  nb::class_<PyStringAttribute, PyAttribute>(m, "StringAttribute", "StringAttribute")
      .def("__init__",
           [](PyTypeAttribute *t, PyContext &c, const std::string &kind, const std::string &value) {
             auto raw = LLVMCreateStringAttribute(c.get(),
                                                  kind.c_str(), kind.size(),
                                                  value.c_str(), value.size());
             new (t) PyStringAttribute(raw);
           }, "context"_a, "kind"_a, "value"_a)
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
                   }, "Get the type attribute's value.");
  
  
  nb::class_<PyDiagnosticInfo>(m, "DiagnosticInfo", "DiagnosticInfo")
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

  nb::class_<PyNamedMDNode>(m, "NamedMDNode", "NamedMDNode")
      .def_prop_ro("next",
                   [](PyNamedMDNode &nmdn) -> std::optional<PyNamedMDNode>{
                     auto res = LLVMGetNextNamedMetadata(nmdn.get());
                     if (res != nullptr) 
                       return PyNamedMDNode(res);
                     return std::nullopt;
                   },
                   "Advance a NamedMDNode iterator to the next NamedMDNode.\n\n"
                   "Returns NULL if the iterator was already at the end and there"
                   " are no more named metadata nodes.")
      .def_prop_ro("prev",
                   [](PyNamedMDNode &nmdn) -> std::optional<PyNamedMDNode> {
                     auto res = LLVMGetPreviousNamedMetadata(nmdn.get());
                     if (res != nullptr)
                       return PyNamedMDNode(res);
                     return std::nullopt;
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

  nb::class_<PyModule>(m, "Module",
     "Modules represent the top-level structure in an LLVM program. An LLVM"
     "module is effectively a translation unit or a collection of translation "
     "units merged together.")
      .def(nb::init<const std::string &>(), "id"_a)
      .def_prop_ro("first_global",
                   [](PyModule &m) { return PyValueAuto(LLVMGetFirstGlobal(m.get())); })
      .def_prop_ro("first_named_metadata",
                   [](PyModule &m) {
                     return PyNamedMDNode(LLVMGetFirstNamedMetadata(m.get()));
                   },
                   "Obtain an iterator to the first NamedMDNode in a Module.")
      .def_prop_ro("last_named_metadata",
                   [](PyModule &m) {
                     return PyNamedMDNode(LLVMGetLastNamedMetadata(m.get()));
                   },
                   "Obtain an iterator to the last NamedMDNode in a Module.")
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
                   nb::for_setter(nb::sig("def id(self, id: str, /) -> None")),
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
                   })
      .def_prop_rw("data_layout",
                   [](PyModule &m) {
                     return LLVMGetDataLayoutStr(m.get());
                   },
                   [](PyModule &m, const std::string &dlstr) {
                     return LLVMSetDataLayout(m.get(), dlstr.c_str());
                   },
                   nb::for_setter(nb::sig("def data_layout(self, data_layout_str: str, /) -> None")),
                   nb::for_getter("Obtain the data layout for a module."),
                   nb::for_setter("Set the data layout for a module."))
      .def_prop_rw("target",
                   [](PyModule &m) {
                     return LLVMGetTarget(m.get());
                   },
                   [](PyModule &m, const std::string &triple) {
                     return LLVMSetTarget(m.get(), triple.c_str());
                   },
                   nb::for_setter(nb::sig("def triple(self, triple: str, /) -> None")),
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
                   },
                   nb::for_setter(nb::sig("def inline_asm(self, str, /) -> None")))
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
      .def("__str__",
           [](PyModule &m) {
             char *str = LLVMPrintModuleToString(m.get());
             std::string strCopy(str);
             LLVMDisposeMessage(str);
             return strCopy;
           },
           "Return a string representation of the module")
      .def("add_function",
           [](PyModule &m, std::string &name, PyTypeFunction &functionTy) {
             return PyFunction(LLVMAddFunction(m.get(), name.c_str(), functionTy.get()));
           }, "name"_a, "function_type"_a,
           "Add a function to a module under a specified name.")
      .def("get_named_function",
           [](PyModule &m, std::string &name) {
             return PyFunction(LLVMGetNamedFunction(m.get(), name.c_str()));
           }, "name"_a,
           "Obtain a Function value from a Module by its name.")
      .def("get_named_metadata",
           [](PyModule &m, std::string &name) -> std::optional<PyNamedMDNode> {
             auto res = LLVMGetNamedMetadata(m.get(), name.c_str(), name.size());
             if (res != nullptr)
               return PyNamedMDNode(res);
             return std::nullopt;
           }, "name"_a,
           "Retrieve a NamedMDNode with the given name, returning NULL if no such"
           "node exists.")
      .def("get_or_insert_named_metadata",
           [](PyModule &m, std::string &name) {
             return PyNamedMDNode
                      (LLVMGetOrInsertNamedMetadata
                         (m.get(), name.c_str(), name.size()));
           }, "name"_a,
           "Retrieve a NamedMDNode with the given name, creating a new node if no such"
           "node exists.")
      .def("get_named_metadata_num_operands",
           [](PyModule &m, std::string &name) {
             return LLVMGetNamedMetadataNumOperands(m.get(), name.c_str());
           }, "name"_a,
           "Obtain the number of operands for named metadata in a module.")
      .def("get_named_metadata_operands",
           [](PyModule &m, std::string &name) {
             int num = LLVMGetNamedMetadataNumOperands(m.get(), name.c_str());
             LLVMValueRef *dest;
             LLVMGetNamedMetadataOperands(m.get(), name.c_str(), dest);
             WRAP_VECTOR_FROM_DEST(PyValue, num, res, dest);
             return res;
           }, "name"_a,
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
           }, "Return an exact copy of the specified module.")
      // NOTE: I think LLVM C api has a bug: https://github.com/llvm/llvm-project/pull/99800
      // FIXME: currently all LLVMModuleFlagEntry related function are not binded
      // .def("copy_module_flags_metadata",
      //      [](PyModule &m) {
      //        size_t Len;
      //        LLVMModuleFlagEntry *flags = LLVMCopyModuleFlagsMetadata(M, &Len);

      //        std::vector<PyModuleFlagEntry> flags_vector;
      //        for (size_t i = 0; i < Len; ++i) {
      //          flags_vector.push_back(PyModuleFlagEntry(flag[i]));
      //        }

      //        LLVMDisposeModuleFlagsMetadata(flags);
      //        return flags_vector;
      //      })
      .def("get_flag",
           [](PyModule &m, const std::string &key) {
             return PyMetadata(LLVMGetModuleFlag(m.get(), key.c_str(), key.size()));
           }, "key"_a,
           "Return the corresponding value if Key appears in module flags, otherwise"
           "return null.")
      .def("add_flag",
           [](PyModule &m, LLVMModuleFlagBehavior behavior,
              const std::string key, PyMetadata &val) {
             return LLVMAddModuleFlag(m.get(), behavior, key.c_str(), key.size(),
                                      val.get());
           }, "behavior"_a, "key"_a, "val"_a,
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
               return std::make_tuple(success, errorStr);
             }
             
             return std::make_tuple(success, std::string(""));
           }, "filename"_a,
           "Print a representation of a module to a file. The ErrorMessage needs to be"
           "disposed with LLVMDisposeMessage. Returns 0 on success, 1 otherwise.")
      .def("append_inline_asm",
            [](PyModule &m, std::string &iasm) {
              return LLVMAppendModuleInlineAsm(m.get(), iasm.c_str(), iasm.size());
            })
      .def("get_type_by_name",
           [](PyModule &m, std::string &name) {
             return PyTypeAuto(LLVMGetTypeByName(m.get(), name.c_str()));
           }, "name"_a,
           "Deprecated: Use LLVMGetTypeByName2 instead.");


  // nb::class_<PyModuleFlagEntry_>(m, "ModuleFlagEntry", "ModuleFlagEntry");
  nb::class_<PyMetadata>(m, "Metadata", "Metadata");
  
  nb::class_<PyUse>(m, "Use", "Use")
      .def_prop_ro("next",
                   [](PyUse &u) -> std::optional<PyUse> {
                     auto res = LLVMGetNextUse(u.get());
                     if (res)
                       return PyUse(res);
                     return std::nullopt;
                   },
                   "Obtain the next use of a value.\n\n"
                   "This effectively advances the iterator. It returns NULL if you are on"
                   "the final use and no more are available.")
      .def_prop_ro("user",
                   [](PyUse &u) { return PyUser(LLVMGetUser(u.get())); },
                   "Obtain the user value for a user.\n",
                   "The returned value corresponds to a llvm::User type.")
      .def_prop_ro("used_value",
                   [](PyUse &u) { return PyValueAuto(LLVMGetUsedValue(u.get())); });
}
