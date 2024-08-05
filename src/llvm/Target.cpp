#include "Target.h"

#include <llvm-c/Target.h>
#include <nanobind/nanobind.h>
#include "types_priv.h"

namespace nb = nanobind;
using namespace nb::literals;

void populateTarget(nb::module_ &m) {
  auto TargetDataClass = nb::class_<PymTargetData,
                                  PymLLVMObject<PymTargetData, LLVMTargetDataRef>>
                         (m, "TargetData", "TargetData");
}

