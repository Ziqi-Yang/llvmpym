#include "iterator.h"
#include "../types_priv.h"

namespace nb = nanobind;
using namespace nb::literals;


void bindIterators(nb::module_ &m) {
  BIND_ITERATOR_CLASS(PyUseIterator, "UseIterator")
  // BIND_ITERATOR_CLASS(PyBasicBlockIterator, "BasicBlockIterator")
  // BIND_ITERATOR_CLASS(PyArgumentIterator, "ArgumentIterator")
  BIND_ITERATOR_CLASS(PyInstructionIterator, "InstructionIterator")
  BIND_ITERATOR_CLASS(PyGlobalVariableIterator, "GlobalVariableIterator")
  BIND_ITERATOR_CLASS(PyGlobalIFuncIterator, "GlobalIFuncIterator")
  BIND_ITERATOR_CLASS(PyGlobalAliasIterator, "GlobalAliasIterator")
  BIND_ITERATOR_CLASS(PyNamedMDNodeIterator, "NamedMDNodeIterator")
  BIND_ITERATOR_CLASS(PyFunctionIterator, "FunctionIterator")
}


