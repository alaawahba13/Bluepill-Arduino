#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H
#include <stdbool.h>

#ifndef _Bool
#define _Bool unsigned char
#endif

typedef _Bool boolean;

#ifndef FALSE
#define FALSE (boolean)false;
#endif

#ifndef TRUE
#define TRUE (boolean)true;
#endif

typedef unsigned char uint8;
typedef signed char sint8;
typedef unsigned  short int uint16;
typedef signed  short int sint16;
typedef unsigned int uint32;
typedef signed int sint32;
typedef unsigned long long uint64;
typedef signed long long sint64;
typedef float f32;
typedef double f64;

typedef volatile uint8 vuint8;
typedef volatile sint8 vsint8;

typedef volatile uint16 vuint16;
typedef volatile sint16 vsint16;

typedef volatile uint32 vuint32;
typedef volatile sint32 vsint32;

typedef volatile uint64 vuint64;
typedef volatile sint64 vsint64;

#endif
