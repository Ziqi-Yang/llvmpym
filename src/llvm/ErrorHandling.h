#ifndef LLVMPYM_ERRORHANLDING_H
#define LLVMPYM_ERRORHANLDING_H

#include <nanobind/nanobind.h>
namespace nb = nanobind;

void populateErrorHandling(nb::module_ &m);

#endif
