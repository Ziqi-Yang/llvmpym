#include <nanobind/nanobind.h>
#include <llvm-c/Core.h>
#include <nanobind/stl/string.h>
#include "Utils.h"
#include "_types.h"
#include "Core/utils.h"

namespace nb = nanobind;
using namespace nb::literals;

PyModule parseAssembly(std::string &iasm) {
  auto context = PyContext::getGlobalContext();
  auto memBuf = LLVMCreateMemoryBufferWithMemoryRangeCopy(iasm.c_str(), iasm.size(), "");
  return parseIR(context.get(), memBuf);
}


void populateUtils(nb::module_ &m) {
  m.def("parse_assembly", &parseAssembly, "isam"_a);
}
