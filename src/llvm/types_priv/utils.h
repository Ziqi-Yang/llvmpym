#ifndef LLVMPYM_TYPES_PRIV_UTILS_H
#define LLVMPYM_TYPES_PRIV_UTILS_H

#define SHARED_POINTER_DEF(UnderlyingPtrClassName, UnderlyingClassName) \
  std::shared_ptr<UnderlyingClassName> obj; \
\
  struct Deleter { \
    void operator()(UnderlyingPtrClassName obj) const; \
  }; \
\
  static std::shared_ptr<UnderlyingClassName> get_shared_obj(UnderlyingPtrClassName \
   obj); \
\
  static std::unordered_map<UnderlyingPtrClassName, \
                            std::weak_ptr<UnderlyingClassName>> obj_map; \
  static std::mutex map_mutex;



#define SHARED_POINTER_IMPL(ClassName, UnderlyingPtrClassName, UnderlyingClassName, DISPOSE_FUNC) \
  std::unordered_map<UnderlyingPtrClassName, \
    std::weak_ptr<UnderlyingClassName>> ClassName::obj_map; \
  std::mutex ClassName::map_mutex; \
  \
  void ClassName::Deleter::operator() \
  (UnderlyingPtrClassName obj) const { \
    if (obj) { \
      DISPOSE_FUNC(obj); \
  \
      std::lock_guard<std::mutex> lock(ClassName::map_mutex); \
      ClassName::obj_map.erase(obj); \
    } \
  } \
  \
  \
  std::shared_ptr<UnderlyingClassName> ClassName::get_shared_obj \
  (UnderlyingPtrClassName obj) { \
    std::lock_guard<std::mutex> lock(ClassName::map_mutex); \
    auto it = ClassName::obj_map.find(obj); \
  \
    if (it != ClassName::obj_map.end()) { \
      if (auto shared = it->second.lock()) { \
        return shared;  \
      } \
    } \
  \
    auto shared = std::shared_ptr<UnderlyingClassName>(obj, Deleter()); \
    ClassName::obj_map[obj] = shared; \
    return shared; \
  }





#endif
