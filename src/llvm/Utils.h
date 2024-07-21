#ifndef LLVMPYM_UTILS_H
#define LLVMPYM_UTILS_H

#define UNWRAP_VECTOR_WRAPPER_CLASS(WRAPPER, RAW, SIZE) \
  std::vector<LLVMTypeRef> RAW; \
  RAW.reserve(SIZE); \
  for (const auto& w : WRAPPER) { \
    RAW.push_back(w.get()); \
  }


#endif
