
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


typedef enum{
	LBTY_OK = 0,
	LBTY_ErrorInvalidInput,
    LBTY_ErrorNullPointer,
	LBTY_NOK

}tenu_ErrorStatus;

#define TIME_OUT                             1000
#define ZERO                                 0
#define ONE                                  1
#define NULL                                 0


#endif
