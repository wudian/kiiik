#ifndef BASE_LIB_DEFINE_H_
#define BASE_LIB_DEFINE_H_

#include <qstring>

#define SORT_CASE(VEC, column) \
case column: \
	std::sort(VEC.begin(), VEC.end(), Compare<column>); \
	break;

#define DESC_SORT_CASE(VEC, column) \
case column: \
	std::sort(VEC.begin(), VEC.end(), DescCompare<column>); \
	break;

//排序函数
#define FIELD_CMP_IMP(TYPE, column, field) template<> bool Compare<column>(const TYPE &lhs, const TYPE &rhs) { return lhs.field < rhs.field; }
#define FIELD_DESCMP_IMP(TYPE, column, field) template<> bool DescCompare<column>(const TYPE &lhs, const TYPE &rhs) { return lhs.field >= rhs.field; }

#define CASE_LABEL(i) case i: return label[i]


QString formatDate(long date);
QString formatTime(long time);

#endif