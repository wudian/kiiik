#ifndef COMMON_MATH_H_
#define COMMON_MATH_H_

#include "Baselib/Global.h"

namespace baselib {

bool BASELIB_API PriceGreatThan(double val1, double val2);
bool BASELIB_API PriceInRange(double val, double val1, double val2);
bool BASELIB_API PriceEqual(double val1, double val2);
bool BASELIB_API PriceUnEqual(double val1, double val2);
///成交额.
bool BASELIB_API DoubleUnEqual(double val1, double val2);

bool BASELIB_API FloatUnEqual(float val1, float val2);

template <typename T>
bool UnEqual(T t1, T t2);

template bool BASELIB_API UnEqual(signed char, signed char);
template bool BASELIB_API UnEqual(int, int);
template<> bool BASELIB_API UnEqual(float, float); // 加<>表示需要重新实现.
template<> bool BASELIB_API UnEqual(double, double);

}


#endif