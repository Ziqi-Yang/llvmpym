#include "iterator.h"
#include "../types_priv.h"

namespace nb = nanobind;
using namespace nb::literals;


void bindIterators(nb::module_ &m) {
  BIND_ITERATOR_CLASS(PymUseIterator, "UseIterator")
  // BIND_ITERATOR_CLASS(PymBasicBlockIterator, "BasicBlockIterator")
  // BIND_ITERATOR_CLASS(PymArgumentIterator, "ArgumentIterator")
  BIND_ITERATOR_CLASS(PymInstructionIterator, "InstructionIterator")
  BIND_ITERATOR_CLASS(PymGlobalVariableIterator, "GlobalVariableIterator")
  BIND_ITERATOR_CLASS(PymGlobalIFuncIterator, "GlobalIFuncIterator")
  BIND_ITERATOR_CLASS(PymGlobalAliasIterator, "GlobalAliasIterator")
  BIND_ITERATOR_CLASS(PymNamedMDNodeIterator, "NamedMDNodeIterator")
  BIND_ITERATOR_CLASS(PymFunctionIterator, "FunctionIterator")
}


