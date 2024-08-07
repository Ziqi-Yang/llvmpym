#include <nanobind/nanobind.h>
#include "llvm/Core.h"
#include "llvm/ErrorHandling.h"
#include "llvm/Utils.h"
#include "llvm/Support.h"
#include "llvm/types_priv.h"
#include "llvm/Analysis.h"
#include "llvm/Target.h"
#include "llvm/TargetMachine.h"
#include "llvm/Disassembler.h"
#include "llvm/BitReader.h"

namespace nb = nanobind;
using namespace nb::literals;


NB_MODULE(llvmpym_ext, m) {
  m.doc() = "LLVM Pymthon Native Extension";
  
  BIND_PYLLVMOBJECT();
  
  auto coreModule = m.def_submodule("core", "LLVM Core");
  populateCore(coreModule);

  auto errorHandlingModule = m.def_submodule("error_handling", "Error Handling");
  populateErrorHandling(errorHandlingModule);
  
  auto supportModule = m.def_submodule("support", "Support");
  populateSupport(supportModule);
   
  auto utilsModule = m.def_submodule("utils", "Utilities");
  populateUtils(utilsModule);

  auto analysisModule = m.def_submodule("analysis", "Analysis");
  populateAnalysis(analysisModule);
  
  auto targetModule = m.def_submodule("target", "Target");
  populateTarget(targetModule);
  
  auto targetMachineModule = m.def_submodule("target_machine", "Target Machine");
  populateTargetMachine(targetMachineModule);

  auto bitReaderModule = m.def_submodule("bit_reader", "bit_reader");
  populateBitReader(bitReaderModule);

  auto disassemblerModule = m.def_submodule("disassembler", "disassembler");
  populateDisassembler(disassemblerModule);

}
