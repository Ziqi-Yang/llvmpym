#ifndef LLVMPYM_CORE_UTILS_H
#define LLVMPYM_CORE_UTILS_H

#include <nanobind/nanobind.h>
#include <llvm-c/Core.h>
#include "../types_priv.h"
#include <fmt/core.h>
#include <string>


/**
 * Get a python side repr string for obj
 */
template <typename T>
inline const char *get_repr_str(T &&obj) {
  return nanobind::repr(nanobind::cast(obj)).c_str();
}

PymMetadataAsValue* getMoreSpcMetadataAsValue(LLVMValueRef raw);

inline std::string get_type_str(PymType &t) {
  char *str = LLVMPrintTypeToString(t.get());
  std::string res(str);
  LLVMDisposeMessage(str);
  return res;
}

inline std::string get_value_str(LLVMValueRef v) {
  auto rawStr = LLVMPrintValueToString(v);
  std::string str(rawStr);
  LLVMDisposeMessage(rawStr);
  return str;
}

inline std::string get_value_name(LLVMValueRef v) {
  size_t len;
  const char *str = LLVMGetValueName2(v, &len);
  return std::string(str, len);
}

inline std::string gen_value_repr(const char *typeName, PymValue &v) {
  LLVMValueRef raw = v.get();
  auto name = get_value_name(raw);
  return fmt::format("<{} name='{}'>", typeName, name);
}

PymInstruction* PymInstructionAuto(LLVMValueRef inst);

PymType* PymTypeAuto(LLVMTypeRef rawType);

PymValue* PymValueAuto(LLVMValueRef rawValue);

PymAttribute* PymAttributeAuto(LLVMAttributeRef rawValue);

PymModule parseIR(LLVMContextRef ctx, LLVMMemoryBufferRef memBuf);


#endif
