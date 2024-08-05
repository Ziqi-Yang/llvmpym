#include "enum.h"
#include <llvm-c/Core.h>
#include "../types_priv.h"

namespace nb = nanobind;

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

  nb::enum_<PymAttributeIndex>(m, "AttributeIndex", "AttributeIndex")
      .value("Return", PymAttributeIndex::Return)
      .value("Function", PymAttributeIndex::Function);

  nb::enum_<LLVMTailCallKind>(m, "TailCallKind", "TailCallKind")
      .value("LLVMTailCallKindNone", LLVMTailCallKind::LLVMTailCallKindNone)
      .value("LLVMTailCallKindTail", LLVMTailCallKind::LLVMTailCallKindTail)
      .value("LLVMTailCallKindMustTail", LLVMTailCallKind::LLVMTailCallKindMustTail)
      .value("LLVMTailCallKindNoTail", LLVMTailCallKind::LLVMTailCallKindNoTail);

  // TODO LLVMAttributeIndex

  nb::enum_<PymLLVMFastMathFlags>(m, "FastMathFlags", "FastMathFlags")
       .value("AllowReassoc", PymLLVMFastMathFlags::AllowReassoc)
       .value("NoNaNs", PymLLVMFastMathFlags::NoNaNs)
       .value("NoInfs", PymLLVMFastMathFlags::NoInfs)
       .value("NoSignedZeros", PymLLVMFastMathFlags::NoSignedZeros)
       .value("AllowReciprocal", PymLLVMFastMathFlags::AllowReciprocal)
       .value("AllowContract", PymLLVMFastMathFlags::AllowContract)
       .value("ApproxFunc", PymLLVMFastMathFlags::ApproxFunc)
       .value("None_", PymLLVMFastMathFlags::None)
       .value("All", PymLLVMFastMathFlags::All);

  // TODO LLVMFastMathFlags
}
