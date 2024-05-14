#pragma once
#include "iCVBase_inst_mgr.hpp"
#include "iCVFaceDetect_def.h"
#if __cplusplus >= 201402L // C++14 and beyond
// using std::enable_if_t;
using std::make_unique;
using make_unique_cust = std::make_unique;
#else
template <typename T, typename... Args>
std::unique_ptr<T> make_unique_cust(Args &&...args) {
    static_assert(!std::is_array<T>::value, "arrays not supported");
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif
std::unique_ptr<iCVBaseDef> creatorFaceDetectInst() {
    return make_unique_cust<iCVFaceDetectInst>();
}
