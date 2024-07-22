#ifndef LLVMPYM_UTILS_H
#define LLVMPYM_UTILS_H

#define UNWRAP_VECTOR_WRAPPER_CLASS(RAW_TYPE, WRAPPER, RAW, SIZE) \
  std::vector<RAW_TYPE> RAW; \
  RAW.reserve(SIZE); \
  for (const auto& w : WRAPPER) { \
    RAW.push_back(w.get()); \
  }

#define WRAP_VECTOR_FROM_DEST(TYPE, NUMBER, RES, DEST) \
  std::vector<TYPE> RES; \
  RES.reserve(NUMBER); \
  for (unsigned i = 0; i < NUMBER; i++) { \
    RES.push_back(TYPE(DEST[i])); \
  }




#endif
