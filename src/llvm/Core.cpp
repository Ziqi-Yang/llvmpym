#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <llvm-c/Core.h>

namespace nb = nanobind;
using namespace nb::literals;

class PyModule {
public:
  explicit PyModule(const std::string &id) {
    module = LLVMModuleCreateWithName(id.c_str());
    if (!module) {
      throw std::runtime_error("Failed to create LLVM module");
    }
  }

  ~PyModule() {
    if (module) 
      LLVMDisposeModule(module);
  }

  // Delete copy constructor and copy assignment
  PyModule(const PyModule &) = delete;
  PyModule &operator=(const PyModule &) = delete;

  // Move constructor
  PyModule(PyModule &&other) noexcept : module(other.module) {
    other.module = nullptr;
  }
	
	// Move assignment
  PyModule &operator=(PyModule &&other) noexcept {
    if (this != &other) {
      if (module) {
        LLVMDisposeModule(module);
      }
      module = other.module;
      other.module = nullptr;
    }
    return *this;
  }

  LLVMModuleRef get() {
    return module;
  }

  void setModuleIdentifier(const std::string &identifier) {
    LLVMSetModuleIdentifier(module, identifier.c_str(), identifier.size());
  }

  std::string getModuleIdentifier() const {
    size_t len;
    const char *identifier = LLVMGetModuleIdentifier(module, &len);
    return std::string(identifier, len);
  }

private:
  LLVMModuleRef module;
};


void populateCore(nb::module_ &m) {
  nb::class_<PyModule>(m, "Module")
      .def(nb::init<const std::string &>(), "id"_a)
      .def_prop_rw("id",
                   [](PyModule &m) { return m.getModuleIdentifier(); },
                   [](PyModule &m, const std::string &id)
                     { m.setModuleIdentifier(id); },
                   nb::for_getter(nb::sig("def id(self, /) -> int")),
                   nb::for_setter(nb::sig("def id(self, id: str, /) -> None")),
                   nb::for_getter("Get the module identifier"),
                   nb::for_setter("Set the module identifier"));
}
