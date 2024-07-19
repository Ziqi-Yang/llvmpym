#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <llvm-c/Core.h>

namespace nb = nanobind;
using namespace nb::literals;

class PyModule {
public:
  explicit PyModule(const std::string &id) {
    module = LLVMModuleCreateWithName(id.c_str());
    if (!module) {
      throw std::runtime_error("Failed to create LLVM module");
    }
  }

  ~PyModule() {
    if (module) 
      LLVMDisposeModule(module);
  }

  // Delete copy constructor and copy assignment
  PyModule(const PyModule &) = delete;
  PyModule &operator=(const PyModule &) = delete;

  // Move constructor
  PyModule(PyModule &&other) noexcept : module(other.module) {
    other.module = nullptr;
  }
	
	// Move assignment
  PyModule &operator=(PyModule &&other) noexcept {
    if (this != &other) {
      if (module) {
        LLVMDisposeModule(module);
      }
      module = other.module;
      other.module = nullptr;
    }
    return *this;
  }

  LLVMModuleRef get() {
    return module;
  }

  void setModuleIdentifier(const std::string &identifier) {
    LLVMSetModuleIdentifier(module, identifier.c_str(), identifier.size());
  }

  std::string getModuleIdentifier() const {
    size_t len;
    const char *identifier = LLVMGetModuleIdentifier(module, &len);
    return std::string(identifier, len);
  }

private:
  LLVMModuleRef module;
};


void populateCore(nb::module_ &m) {
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
      .value("VoidTypeKind", LLVMTypeKind::LLVMVoidTypeKind, "type with no size")
      .value("HalfTypeKind", LLVMTypeKind::LLVMHalfTypeKind, "16 bit floating point type")
      .value("FloatTypeKind", LLVMTypeKind::LLVMFloatTypeKind, "32 bit floating point type")
      .value("DoubleTypeKind", LLVMTypeKind::LLVMDoubleTypeKind, "64 bit floating point type")
      .value("X86_FP80TypeKind", LLVMTypeKind::LLVMX86_FP80TypeKind, "80 bit floating point type (X87)")
      .value("FP128TypeKind", LLVMTypeKind::LLVMFP128TypeKind, "128 bit floating point type (112-bit mantissa)")
      .value("PPC_FP128TypeKind", LLVMTypeKind::LLVMPPC_FP128TypeKind, "128 bit floating point type (two 64-bits)")
      .value("LabelTypeKind", LLVMTypeKind::LLVMLabelTypeKind, "Labels")
      .value("IntegerTypeKind", LLVMTypeKind::LLVMIntegerTypeKind, "Arbitrary bit width integers")
      .value("FunctionTypeKind", LLVMTypeKind::LLVMFunctionTypeKind, "Functions")
      .value("StructTypeKind", LLVMTypeKind::LLVMStructTypeKind, "Structures")
      .value("ArrayTypeKind", LLVMTypeKind::LLVMArrayTypeKind, "Arrays")
      .value("PointerTypeKind", LLVMTypeKind::LLVMPointerTypeKind, "Pointers")
      .value("VectorTypeKind", LLVMTypeKind::LLVMVectorTypeKind, "Fixed width SIMD vector type")
      .value("MetadataTypeKind", LLVMTypeKind::LLVMMetadataTypeKind, "Metadata")
      .value("X86_MMXTypeKind", LLVMTypeKind::LLVMX86_MMXTypeKind, "X86 MMX")
      .value("TokenTypeKind", LLVMTypeKind::LLVMTokenTypeKind, "Tokens")
      .value("ScalableVectorTypeKind", LLVMTypeKind::LLVMScalableVectorTypeKind, "Scalable SIMD vector type")
      .value("BFloatTypeKind", LLVMTypeKind::LLVMBFloatTypeKind, "16 bit brain floating point type")
      .value("X86_AMXTypeKind", LLVMTypeKind::LLVMX86_AMXTypeKind, "X86 AMX")
      .value("TargetExtTypeKind", LLVMTypeKind::LLVMTargetExtTypeKind, "Target extension type");

  nb::enum_<LLVMLinkage>(m, "Linkage", "Linkage")
      .value("ExternalLinkage", LLVMLinkage::LLVMExternalLinkage, "Externally visible function")
      .value("AvailableExternallyLinkage", LLVMLinkage::LLVMAvailableExternallyLinkage)
      .value("LinkOnceAnyLinkage", LLVMLinkage::LLVMLinkOnceAnyLinkage, "Keep one copy of function when linking (inline)")
      .value("LinkOnceODRLinkage", LLVMLinkage::LLVMLinkOnceODRLinkage, "Keep one copy of function when linking (inline), but only replaced by something equivalent.")
      .value("LinkOnceODRAutoHideLinkage", LLVMLinkage::LLVMLinkOnceODRAutoHideLinkage, "Obsolete")
      .value("WeakAnyLinkage", LLVMLinkage::LLVMWeakAnyLinkage, "Keep one copy of function when linking (weak)")
      .value("WeakODRLinkage", LLVMLinkage::LLVMWeakODRLinkage, "Same, but only replaced by something equivalent.")
      .value("AppendingLinkage", LLVMLinkage::LLVMAppendingLinkage, "Special purpose, only applies to global arrays")
      .value("InternalLinkage", LLVMLinkage::LLVMInternalLinkage, "Rename collisions when linking (static functions)")
      .value("PrivateLinkage", LLVMLinkage::LLVMPrivateLinkage, "Like Internal, but omit from symbol table")
      .value("DLLImportLinkage", LLVMLinkage::LLVMDLLImportLinkage, "Obsolete")
      .value("DLLExportLinkage", LLVMLinkage::LLVMDLLExportLinkage, "Obsolete")
      .value("ExternalWeakLinkage", LLVMLinkage::LLVMExternalWeakLinkage, "ExternalWeak linkage description")
      .value("GhostLinkage", LLVMLinkage::LLVMGhostLinkage, "Obsolete")
      .value("CommonLinkage", LLVMLinkage::LLVMCommonLinkage, "Tentative definitions")
      .value("LinkerPrivateLinkage", LLVMLinkage::LLVMLinkerPrivateLinkage, "Like Private, but linker removes.")
      .value("LinkerPrivateWeakLinkage", LLVMLinkage::LLVMLinkerPrivateWeakLinkage, "Like LinkerPrivate, but is weak.");

  nb::enum_<LLVMVisibility>(m, "Visibility", "Visibility")
      .value("DefaultVisibility", LLVMVisibility::LLVMDefaultVisibility, "The GV is visible")
      .value("HiddenVisibility", LLVMVisibility::LLVMHiddenVisibility, "The GV is hidden")
      .value("ProtectedVisibility", LLVMVisibility::LLVMProtectedVisibility, "The GV is protected");

  nb::enum_<LLVMUnnamedAddr>(m, "UnnamedAddr", "UnnamedAddr")
      .value("NoUnnamedAddr", LLVMUnnamedAddr::LLVMNoUnnamedAddr, "Address of the GV is significant.")
      .value("LocalUnnamedAddr", LLVMUnnamedAddr::LLVMLocalUnnamedAddr, "the GV is locally insignificant.")
      .value("GlobalUnnamedAddr", LLVMUnnamedAddr::LLVMGlobalUnnamedAddr, "globally insignificant.");

  nb::enum_<LLVMDLLStorageClass>(m, "DLLStorageClass", "DLLStorageClass")
      .value("DefaultStorageClass", LLVMDLLStorageClass::LLVMDefaultStorageClass)
      .value("DLLImportStorageClass", LLVMDLLStorageClass::LLVMDLLImportStorageClass, "Function to be imported from DLL.")
      .value("DLLExportStorageClass", LLVMDLLStorageClass::LLVMDLLExportStorageClass, "from DLL.");

  nb::enum_<LLVMCallConv>(m, "CallConv", "CallConv")
      .value("CCallConv", LLVMCallConv::LLVMCCallConv)
      .value("FastCallConv", LLVMCallConv::LLVMFastCallConv)
      .value("ColdCallConv", LLVMCallConv::LLVMColdCallConv)
      .value("GHCCallConv", LLVMCallConv::LLVMGHCCallConv)
      .value("HiPECallConv", LLVMCallConv::LLVMHiPECallConv)
      .value("AnyRegCallConv", LLVMCallConv::LLVMAnyRegCallConv)
      .value("PreserveMostCallConv", LLVMCallConv::LLVMPreserveMostCallConv)
      .value("PreserveAllCallConv", LLVMCallConv::LLVMPreserveAllCallConv)
      .value("SwiftCallConv", LLVMCallConv::LLVMSwiftCallConv)
      .value("CXXFASTTLSCallConv", LLVMCallConv::LLVMCXXFASTTLSCallConv)
      .value("X86StdcallCallConv", LLVMCallConv::LLVMX86StdcallCallConv)
      .value("X86FastcallCallConv", LLVMCallConv::LLVMX86FastcallCallConv)
      .value("ARMAPCSCallConv", LLVMCallConv::LLVMARMAPCSCallConv)
      .value("ARMAAPCSCallConv", LLVMCallConv::LLVMARMAAPCSCallConv)
      .value("ARMAAPCSVFPCallConv", LLVMCallConv::LLVMARMAAPCSVFPCallConv)
      .value("MSP430INTRCallConv", LLVMCallConv::LLVMMSP430INTRCallConv)
      .value("X86ThisCallCallConv", LLVMCallConv::LLVMX86ThisCallCallConv)
      .value("PTXKernelCallConv", LLVMCallConv::LLVMPTXKernelCallConv)
      .value("PTXDeviceCallConv", LLVMCallConv::LLVMPTXDeviceCallConv)
      .value("SPIRFUNCCallConv", LLVMCallConv::LLVMSPIRFUNCCallConv)
      .value("SPIRKERNELCallConv", LLVMCallConv::LLVMSPIRKERNELCallConv)
      .value("IntelOCLBICallConv", LLVMCallConv::LLVMIntelOCLBICallConv)
      .value("X8664SysVCallConv", LLVMCallConv::LLVMX8664SysVCallConv)
      .value("Win64CallConv", LLVMCallConv::LLVMWin64CallConv)
      .value("X86VectorCallCallConv", LLVMCallConv::LLVMX86VectorCallCallConv)
      .value("HHVMCallConv", LLVMCallConv::LLVMHHVMCallConv)
      .value("HHVMCCallConv", LLVMCallConv::LLVMHHVMCCallConv)
      .value("X86INTRCallConv", LLVMCallConv::LLVMX86INTRCallConv)
      .value("AVRINTRCallConv", LLVMCallConv::LLVMAVRINTRCallConv)
      .value("AVRSIGNALCallConv", LLVMCallConv::LLVMAVRSIGNALCallConv)
      .value("AVRBUILTINCallConv", LLVMCallConv::LLVMAVRBUILTINCallConv)
      .value("AMDGPUVSCallConv", LLVMCallConv::LLVMAMDGPUVSCallConv)
      .value("AMDGPUGSCallConv", LLVMCallConv::LLVMAMDGPUGSCallConv)
      .value("AMDGPUPSCallConv", LLVMCallConv::LLVMAMDGPUPSCallConv)
      .value("AMDGPUCSCallConv", LLVMCallConv::LLVMAMDGPUCSCallConv)
      .value("AMDGPUKERNELCallConv", LLVMCallConv::LLVMAMDGPUKERNELCallConv)
      .value("X86RegCallCallConv", LLVMCallConv::LLVMX86RegCallCallConv)
      .value("AMDGPUHSCallConv", LLVMCallConv::LLVMAMDGPUHSCallConv)
      .value("MSP430BUILTINCallConv", LLVMCallConv::LLVMMSP430BUILTINCallConv)
      .value("AMDGPULSCallConv", LLVMCallConv::LLVMAMDGPULSCallConv)
      .value("AMDGPUESCallConv", LLVMCallConv::LLVMAMDGPUESCallConv);

  nb::enum_<LLVMValueKind>(m, "ValueKind", "ValueKind")
      .value("ArgumentValueKind", LLVMValueKind::LLVMArgumentValueKind)
      .value("BasicBlockValueKind", LLVMValueKind::LLVMBasicBlockValueKind)
      .value("MemoryUseValueKind", LLVMValueKind::LLVMMemoryUseValueKind)
      .value("MemoryDefValueKind", LLVMValueKind::LLVMMemoryDefValueKind)
      .value("MemoryPhiValueKind", LLVMValueKind::LLVMMemoryPhiValueKind)
      .value("FunctionValueKind", LLVMValueKind::LLVMFunctionValueKind)
      .value("GlobalAliasValueKind", LLVMValueKind::LLVMGlobalAliasValueKind)
      .value("GlobalIFuncValueKind", LLVMValueKind::LLVMGlobalIFuncValueKind)
      .value("GlobalVariableValueKind", LLVMValueKind::LLVMGlobalVariableValueKind)
      .value("BlockAddressValueKind", LLVMValueKind::LLVMBlockAddressValueKind)
      .value("ConstantExprValueKind", LLVMValueKind::LLVMConstantExprValueKind)
      .value("ConstantArrayValueKind", LLVMValueKind::LLVMConstantArrayValueKind)
      .value("ConstantStructValueKind", LLVMValueKind::LLVMConstantStructValueKind)
      .value("ConstantVectorValueKind", LLVMValueKind::LLVMConstantVectorValueKind)
      .value("UndefValueValueKind", LLVMValueKind::LLVMUndefValueValueKind)
      .value("ConstantAggregateZeroValueKind", LLVMValueKind::LLVMConstantAggregateZeroValueKind)
      .value("ConstantDataArrayValueKind", LLVMValueKind::LLVMConstantDataArrayValueKind)
      .value("ConstantDataVectorValueKind", LLVMValueKind::LLVMConstantDataVectorValueKind)
      .value("ConstantIntValueKind", LLVMValueKind::LLVMConstantIntValueKind)
      .value("ConstantFPValueKind", LLVMValueKind::LLVMConstantFPValueKind)
      .value("ConstantPointerNullValueKind", LLVMValueKind::LLVMConstantPointerNullValueKind)
      .value("ConstantTokenNoneValueKind", LLVMValueKind::LLVMConstantTokenNoneValueKind)
      .value("MetadataAsValueValueKind", LLVMValueKind::LLVMMetadataAsValueValueKind)
      .value("InlineAsmValueKind", LLVMValueKind::LLVMInlineAsmValueKind)
      .value("InstructionValueKind", LLVMValueKind::LLVMInstructionValueKind)
      .value("PoisonValueValueKind", LLVMValueKind::LLVMPoisonValueValueKind)
      .value("ConstantTargetNoneValueKind", LLVMValueKind::LLVMConstantTargetNoneValueKind);

  nb::enum_<LLVMIntPredicate>(m, "IntPredicate", "IntPredicate")
      .value("IntEQ", LLVMIntPredicate::LLVMIntEQ, "equal")
      .value("IntNE", LLVMIntPredicate::LLVMIntNE, "not equal")
     .value("IntUGT", LLVMIntPredicate::LLVMIntUGT, "unsigned greater than")
     .value("IntUGE", LLVMIntPredicate::LLVMIntUGE, "unsigned greater or equal")
     .value("IntULT", LLVMIntPredicate::LLVMIntULT, "unsigned less than")
     .value("IntULE", LLVMIntPredicate::LLVMIntULE, "unsigned less or equal")
      .value("IntSGT", LLVMIntPredicate::LLVMIntSGT, "signed greater than")
      .value("IntSGE", LLVMIntPredicate::LLVMIntSGE, "signed greater or equal")
      .value("IntSLT", LLVMIntPredicate::LLVMIntSLT, "signed less than")
      .value("IntSLE", LLVMIntPredicate::LLVMIntSLE, "signed less or equal");

  nb::enum_<LLVMRealPredicate>(m, "RealPredicate", "RealPredicate")
      .value("RealPredicateFalse", LLVMRealPredicate::LLVMRealPredicateFalse, "Always false (always folded)")
      .value("RealOEQ", LLVMRealPredicate::LLVMRealOEQ, "True if ordered and equal")
      .value("RealOGT", LLVMRealPredicate::LLVMRealOGT, "True if ordered and greater than")
      .value("RealOGE", LLVMRealPredicate::LLVMRealOGE, "True if ordered and greater than or equal")
      .value("RealOLT", LLVMRealPredicate::LLVMRealOLT, "True if ordered and less than")
      .value("RealOLE", LLVMRealPredicate::LLVMRealOLE, "True if ordered and less than or equal")
      .value("RealONE", LLVMRealPredicate::LLVMRealONE, "True if ordered and operands are unequal")
      .value("RealORD", LLVMRealPredicate::LLVMRealORD, "True if ordered (no nans)")
      .value("RealUNO", LLVMRealPredicate::LLVMRealUNO, "True if unordered: isnan(X) | isnan(Y)")
      .value("RealUEQ", LLVMRealPredicate::LLVMRealUEQ, "True if unordered or equal")
      .value("RealUGT", LLVMRealPredicate::LLVMRealUGT, "True if unordered or greater than")
      .value("RealUGE", LLVMRealPredicate::LLVMRealUGE, "True if unordered, greater than, or equal")
      .value("RealULT", LLVMRealPredicate::LLVMRealULT, "True if unordered or less than")
      .value("RealULE", LLVMRealPredicate::LLVMRealULE, "True if unordered, less than, or equal")
      .value("RealUNE", LLVMRealPredicate::LLVMRealUNE, "True if unordered or not equal")
      .value("RealPredicateTrue", LLVMRealPredicate::LLVMRealPredicateTrue, "Always true (always folded)");

  nb::enum_<LLVMLandingPadClauseTy>(m, "LandingPadClauseTy", "LandingPadClauseTy")
      .value("LandingPadCatch", LLVMLandingPadClauseTy::LLVMLandingPadCatch, "A catch clause")
      .value("LandingPadFilter", LLVMLandingPadClauseTy::LLVMLandingPadFilter, "A filter clause");

  nb::enum_<LLVMThreadLocalMode>(m, "ThreadLocalMode", "ThreadLocalMode")
      .value("NotThreadLocal", LLVMThreadLocalMode::LLVMNotThreadLocal)
      .value("GeneralDynamicTLSModel", LLVMThreadLocalMode::LLVMGeneralDynamicTLSModel)
      .value("LocalDynamicTLSModel", LLVMThreadLocalMode::LLVMLocalDynamicTLSModel)
      .value("InitialExecTLSModel", LLVMThreadLocalMode::LLVMInitialExecTLSModel)
      .value("LocalExecTLSModel", LLVMThreadLocalMode::LLVMLocalExecTLSModel);

  nb::enum_<LLVMAtomicOrdering>(m, "AtomicOrdering", "AtomicOrdering")
      .value("AtomicOrderingNotAtomic", LLVMAtomicOrdering::LLVMAtomicOrderingNotAtomic, "A load or store which is not atomic")
      .value("AtomicOrderingUnordered", LLVMAtomicOrdering::LLVMAtomicOrderingUnordered, "Lowest level of atomicity, guarantees somewhat sane results, lock free.")
      .value("AtomicOrderingMonotonic", LLVMAtomicOrdering::LLVMAtomicOrderingMonotonic, "guarantees that if you take all the operations affecting a specific address, a consistent ordering exists")
      .value("AtomicOrderingAcquire", LLVMAtomicOrdering::LLVMAtomicOrderingAcquire, "Acquire provides a barrier of the sort necessary to acquire a lock to access other memory with normal loads and stores.")
      .value("AtomicOrderingRelease", LLVMAtomicOrdering::LLVMAtomicOrderingRelease, "Release is similar to Acquire, but with a barrier of the sort necessary to release a lock.")
      .value("AtomicOrderingAcquireRelease", LLVMAtomicOrdering::LLVMAtomicOrderingAcquireRelease, "provides both an Acquire and a Release barrier (for fences and operations which both read and write memory).")
      .value("AtomicOrderingSequentiallyConsistent", LLVMAtomicOrdering::LLVMAtomicOrderingSequentiallyConsistent, "provides Acquire semantics for loads and Release semantics for stores. Additionally, it guarantees that a total ordering exists between all SequentiallyConsistent operations.");

  nb::enum_<LLVMAtomicRMWBinOp>(m, "LLVMAtomicRMWBinOp", "LLVMAtomicRMWBinOp")
      .value("LLVMAtomicRMWBinOpXchg", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpXchg, "Set the new value and return the one old")
      .value("LLVMAtomicRMWBinOpAdd", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpAdd, "Add a value and return the old one")
      .value("LLVMAtomicRMWBinOpSub", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpSub, "Subtract a value and return the old one")
      .value("LLVMAtomicRMWBinOpAnd", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpAnd, "And a value and return the old one")
      .value("LLVMAtomicRMWBinOpNand", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpNand, "Not-And a value and return the old one")
      .value("LLVMAtomicRMWBinOpOr", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpOr, "OR a value and return the old one")
      .value("LLVMAtomicRMWBinOpXor", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpXor, "Xor a value and return the old one")
      .value("LLVMAtomicRMWBinOpMax", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpMax, "Sets the value if it's greater than the original using a signed comparison and return the old one")
      .value("LLVMAtomicRMWBinOpMin", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpMin, "Sets the value if it's Smaller than the original using a signed comparison and return the old one")
      .value("LLVMAtomicRMWBinOpUMax", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpUMax, "Sets the value if it's greater than the original using an unsigned comparison and return the old one")
      .value("LLVMAtomicRMWBinOpUMin", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpUMin, "Sets the value if it's greater than the original using an unsigned comparison and return the old one")
      .value("LLVMAtomicRMWBinOpFAdd", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpFAdd, "Add a floating point value and return the old one")
      .value("LLVMAtomicRMWBinOpFSub", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpFSub, "Subtract a floating point value and return the old one")
      .value("LLVMAtomicRMWBinOpFMax", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpFMax, "Sets the value if it's greater than the original using an floating point comparison and return the old one")
      .value("LLVMAtomicRMWBinOpFMin", LLVMAtomicRMWBinOp::LLVMAtomicRMWBinOpFMin, "Sets the value if it's smaller than the original using an floating point comparison and return the old one");

    
  nb::class_<PyModule>(m, "Module", "Module")
      .def(nb::init<const std::string &>(), "id"_a)
      .def_prop_rw("id",
                   [](PyModule &m) { return m.getModuleIdentifier(); },
                   [](PyModule &m, const std::string &id)
                     { m.setModuleIdentifier(id); },
                   nb::for_getter(nb::sig("def id(self, /) -> int")),
                   nb::for_setter(nb::sig("def id(self, id: str, /) -> None")),
                   nb::for_getter("Get the module identifier.\nOrigin Function: LLVMSetModuleIdentifier."),
                   nb::for_setter("Set the module identifier.\nOrigin Function: LLVMGetModuleIdentifier."));
}
