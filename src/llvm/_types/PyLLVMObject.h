#ifndef PYLLVMOBJECT_H
#define PYLLVMOBJECT_H

#include <iostream>
#include <memory>

template <typename Derived, typename UnderlyingType>
class PyLLVMObject {
public:
  virtual ~PyLLVMObject() = default;

  UnderlyingType get() const {
    return const_cast<const Derived*>(static_cast<const Derived*>(this))->get();
  }

  bool __bool__() const {
    UnderlyingType raw = static_cast<UnderlyingType>(get());
    if (!raw) return false;
    return true;
  }

  // `__equal__` and `__hash__` works well on pointer type UnderlyingType
  bool __equal__(const PyLLVMObject& other) const {
    return this->get() == other.get();
  }

  std::size_t __hash__() const {
    return std::hash<UnderlyingType>{}(this->get());
  }
};


#endif
