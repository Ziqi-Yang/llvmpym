#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <vector>
#include <llvm-c/Support.h>

#include <iostream>

namespace nb = nanobind;
using namespace nb::literals;


void populateSupport(nb::module_ &m) {
  
  m.def("load_library_permanently",
        [](const char *filename) {
          return LLVMLoadLibraryPermanently(filename) != 0;
        },
        "filename"_a,
        "This function permanently loads the dynamic library at the given path."
        "It is safe to call this function multiple times for the same library.");

  m.def("parse_command_line_options",
        [](const std::vector<const char*> &args, const char *overview) {
          LLVMParseCommandLineOptions(args.size(), args.data(), overview);
        },
        "args"_a, "overview"_a,
        "This function parses the given arguments using the LLVM command line parser."
        "Note that the only stable thing about this function is its signature; you"
        "cannot rely on any particular set of command line arguments being interpreted"
        "the same way across LLVM versions.");
  

  // FIXME
  // can work, but the returned type is an capsule object, which can only be
  // passed into binding functions, so if we want to do operation based on
  // the returned address like `static_cast`, then we need to define a function
  // for it
  m.def("search_for_address_of_symbol",
        [](const char *symbolName) {
          return LLVMSearchForAddressOfSymbol(symbolName);
        },
        "name"_a,
        "This function will search through all previously loaded dynamic"
        "libraries for the symbol symbolName. If it is found, the address of"
        "that symbol is returned. If not, null is returned.");

  // seems to work correctly
  // test example:
  // 
  // for static void *x;
  //
  //  [](nb::any a) {
  //    x = &a;
  //  }

  // and then 
  //  std::cout << x << std::endl;
  //  std::cout << *(static_cast<int *>(x)) << std::endl;
  //
  // 0x7fffffff7d20
  // 1
  m.def("add_symbol",
        [](const char *symbolName, nb::any symbolValue) {
          return LLVMAddSymbol(symbolName, &symbolValue);
        },
        "symbol_name"_a, "symbol_value"_a,
        "This functions permanently adds the symbol symbolName with the"
        "value symbolValue.  These symbols are searched before any"
        "libraries.");
}
