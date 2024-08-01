#include <nanobind/nanobind.h>
#include "Utils.h"
// #include "_types.h"

namespace nb = nanobind;
using namespace nb::literals;

// void parseAssembly(std::string iasm, PyContext &context) {
//   auto memBuf = LLVMCreateMemoryBufferWithMemoryRangeCopy(iasm.c_str(), iasm.size(), "");
// }

void populateUtils(nb::module_ &m) {
  // m.def("parse_assembly",
  //       )  
}
