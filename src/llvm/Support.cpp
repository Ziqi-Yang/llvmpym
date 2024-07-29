#include <nanobind/nanobind.h>

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

  // m.def("parse_command_line_options",
  //       [](std::vector<std::string> &args, const char *overview) {
  //         return LLVMParseCommandLineOptions(args.size(), args.data(), )
  //       })
}
