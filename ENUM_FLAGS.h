#pragma once

#ifndef _ENUM_FLAGS
#define _ENUM_FLAGS

#define baseType(T) std::underlying_type_t<T>
#define baseCast(T,X) static_cast<std::underlying_type_t<T>>(X)

#define ENUM_FLAG_OPERATOR(T,X) inline T operator X (T lhs, T rhs) { return (T) (static_cast<std::underlying_type_t <T>>(lhs) X static_cast<std::underlying_type_t <T>>(rhs)); } 
#define ENUM_FLAG_OP_EQUALS(T, X) inline T& operator X= (T& lhs, T rhs) { lhs = (T) (baseCast(T,lhs) X baseCast(T,rhs)); return lhs; }
#define ENUM_FLAGS(T) \
enum class T; \
inline T operator ~ (T t) { return (T) (~static_cast<std::underlying_type_t <T>>(t)); } \
ENUM_FLAG_OPERATOR(T,|) \
ENUM_FLAG_OPERATOR(T,^) \
ENUM_FLAG_OPERATOR(T,&) \
ENUM_FLAG_OP_EQUALS(T,|) \
ENUM_FLAG_OP_EQUALS(T,^) \
ENUM_FLAG_OP_EQUALS(T,&) \
enum class T


#endif // !_ENUM_FLAGS

