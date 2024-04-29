
#ifndef _STD_TYPES_H
#define _STD_TYPES_H

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned long  int u32;

typedef signed char s8;
typedef signed short int s16;
typedef signed long  int s32;

typedef float  f32;
typedef double f64;

typedef unsigned long long  u64;

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;
typedef signed char         sint8_t;
typedef signed short int    sint16_t;
typedef signed int          sint32_t;
typedef signed long long    sint64_t;
typedef float               flt32_t;
typedef double              flt64_t;


typedef enum{
	LBTY_OK = 0,
	LBTY_ErrorInvalidInput,
    LBTY_ErrorNullPointer,
	LBTY_Busy,
	LBTY_NOK

}tenu_ErrorStatus;

#define TIME_OUT                             1000
#define ZERO                                 0
#define ONE                                  1
#define NULL                                 0


#endif
