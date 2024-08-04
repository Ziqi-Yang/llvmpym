#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <vector>
#include <llvm-c/Support.h>

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
  

  // FIXME seems not practical in python side
  // m.def("search_for_address_of_symbol",
  //       [](const char *symbolName) {
  //         return LLVMSearchForAddressOfSymbol(symbolName);
  //       },
  //       "name"_a,
  //       "This function will search through all previously loaded dynamic"
  //       "libraries for the symbol symbolName. If it is found, the address of"
  //       "that symbol is returned. If not, null is returned.");

  // m.def("add_symbol",
  //       [](const char *symbolName, nb::any symbolValue) {
  //         // the passed value is nb::any*, but not void *
  //         return LLVMAddSymbol(symbolName, &symbolValue);
  //       },
  //       "symbol_name"_a, "symbol_value"_a,
  //       "This functions permanently adds the symbol symbolName with the"
  //       "value symbolValue.  These symbols are searched before any"
  //       "libraries.");
}
