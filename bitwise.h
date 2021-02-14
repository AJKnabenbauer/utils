#pragma once

#include <stdint.h>

/* Create a bit mask for a given bit position*/
#define BIT_MASK(a)		(1ULL<<(a))

/* Create a bit mask for a given bit range. a= msb bit position, b = lsb bit position */
#define BIT_RANGE(a,b)	(((~0ULL)<<((a)+1)) ^ ((~0ULL)<<(b)))

/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a,b)		((a) |= BIT_MASK(b))
#define BIT_CLEAR(a,b)		((a) &= ~BIT_MASK(b))
#define BIT_FLIP(a,b)		((a) ^= BIT_MASK(b))
#define BIT_CHECK(a,b)		(!!((a) & BIT_MASK(b)))   // '!!' to make sure this returns 0 or 1
#define BIT_SET_VAL(a,b,c)	((c) ? BIT_SET(a,b) : BIT_CLEAR(a,b))

/* a=target variable, b=number of highest bit in range, c=number of lowest bit in range */
#define BIT_RANGE_SET(a,b,c)		((a) |= BIT_RANGE(b,c))
#define BIT_RANGE_CLEAR(a,b,c)		((a) &= ~BIT_RANGE(b,c))
#define BIT_RANGE_FLIP(a,b,c)		((a) ^= BIT_RANGE(b,c))
#define BIT_RANGE_GET(a,b,c)		(((a) & BIT_RANGE(b,c))>>(c))
#define BIT_RANGE_CHECK_ALL(a,b,c)	(!(~(a) & BIT_RANGE(b,c)))
#define BIT_RANGE_CHECK_ANY(a,b,c)	((a) & BIT_RANGE(b,c))
#define BIT_RANGE_SET_VAL(a,b,c,d)	((d) ? BIT_RANGE_SET(a,b,c) : BIT_RANGE_CLEAR(a,b,c))

/* x=target variable, y=mask */
#define BIT_MASK_SET(x,y)		((x) |= (y))
#define BIT_MASK_CLEAR(x,y)		((x) &= (~(y)))
#define BIT_MASK_FLIP(x,y)		((x) ^= (y))
#define BIT_MASK_CHECK_ALL(x,y)	(!(~(x) & (y)))
#define BIT_MASK_CHECK_ANY(x,y)	((x) & (y))

#define BYTE_MASK(pos)							(0xFFULL<<(pos*8))
#define BYTE_GET(d,n)							(((d)>>(n)) & BYTE_MASK(0))
#define BYTE_CONCAT2(b1,b0)						(((b1)<<8) | (b0))
#define BYTE_CONCAT4(b3,b2,b1,b0)				(((b3)<<24) | ((b2)<<16) | ((b1)<<8) | (b0))
#define BYTE_CONCAT8(b7,b6,b5,b4,b3,b2,b1,b0)	(((b7)<<56) | ((b6)<<48) | ((b5)<<40) | ((b4)<<32) | ((b3)<<24) | ((b2)<<16) | ((b1)<<8) | (b0))

#ifdef __cplusplus
extern "C" {
#endif
	 
	/* 
	Returns a byte whose individual bits have had their positions reversed
	Define BM_NO_CACHE_REVERSE to prevent a 256 byte look up table from being used
	*/
	uint8_t bitReverse(uint8_t d); 

	/* 
	Returns a char* to a c string containing the binary representation of d
	Define BM_NO_CACHE_CHAR to prevent a 2304 byte look up table from being used
	Note: Macro "bitString(d)" is defined to choose the ASCII or UNICODE version of this function automatically
	*/
	const char* bitStringA(uint8_t d); 

	/* 
	Returns a wide char* to a c string containing the binary representation of d
	Define BM_NO_CACHE_WCHAR to prevent a 4608 byte look up table from being used
	Note: Macro "bitString(d)" is defined to choose the ASCII or UNICODE version of this function automatically 
	*/
	const wchar_t* bitStringW(uint8_t d);

	#ifdef UNICODE
	#define bitString(d) bitStringW(d) /* Returns a pointer to a c string containing the binary representation of d */
	#else
	#define bitString(d) bitStringA(d) /* Returns a pointer to a c string containing the binary representation of d */
	#endif

#ifdef __cplusplus
}
#endif
