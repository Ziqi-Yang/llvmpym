#include "Linker.h"

#include <nanobind/nanobind.h>
#include <llvm-c/Linker.h>
#include "types_priv.h"

namespace nb = nanobind;
using namespace nb::literals;

void populateLinker(nanobind::module_ &m) {
  nb::enum_<LLVMLinkerMode>(m, "LinkerMode" "LinkerMode")
      // LLVMLinkerPreserveSource_Removed is deprecated
      .value("DestroySource", LLVMLinkerMode::LLVMLinkerDestroySource,
             "This is the default behavior.");

  // TODO customize (if error, throw an runtime error containing the reason. Just
  // like what llvmlite does)
  m.def("link_module",
        [](PymModule dest, PymModule src) {
          return LLVMLinkModules2(dest.get(), src.get()) != 0;
        },
        "dest"_a, "src"_a,
        "Links the source module into the destination module. The source module is"
        "destroyed.\n"
        "The return value is true if an error occurred, false otherwise.\n"
        "Use the diagnostic handler to get any diagnostic message.");
}


