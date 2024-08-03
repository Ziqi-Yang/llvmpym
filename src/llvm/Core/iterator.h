#ifndef ITERATOR_H
#define ITERATOR_H

#include <nanobind/nanobind.h>

#define BIND_ITERATOR_CLASS(ClassName, PythonClassName) \
  nanobind::class_<ClassName>(m, PythonClassName, PythonClassName) \
      .def("__iter__", [](ClassName &self) { return self; }) \
      .def("__next__", &ClassName::next);


void bindIterators(nanobind::module_ &m);


#endif
