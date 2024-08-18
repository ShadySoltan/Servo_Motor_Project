/*
 * std_types.h
 *
 *  Created on: Feb 10, 2024
 *      Author: shady
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

#define SetBit(Reg,BitNo) (Reg |= 1<<BitNo)
#define ClearBit(Reg,BitNo) (Reg &= ~(1<<BitNo))
#define ToggleBit(Reg,BitNo) (Reg ^= 1<<BitNo)
#define GetBit(Reg,BitNo) (Reg>>BitNo & 1)

//Main Types
typedef unsigned char uint8;
typedef signed char sint8;
typedef unsigned short uint16;
typedef signed short sint16;
typedef unsigned long uint32;
typedef signed long sint32;
typedef unsigned long long uint64;
typedef signed long long sint64;
typedef float float32;
typedef double double64;

//Boolean Values
#ifndef FALSE
#define FALSE (0u)
#endif
#ifndef TRUE
#define TRUE (1u)
#endif

//Logic Values
#define LOGIC_HIGH (1u)
#define LOGIC_LOW (0u)

//Null Pointer
#define NULL_PTR ((void*)0)

//Boolean Data Type
typedef uint8 boolean;

#endif /* STD_TYPES_H_ */
