/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */
#include <stdio.h>
#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */    
int bitXor(int x, int y) {
    // x^y=(x & ~y) | (~x & y) 没有或门就用与门和非门构造一个 x|y = ~(~x&~y)
    return ~(~((~x)&y)&~(x&(~y)));
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
    // 返回4字节中最小的整数
    return 1<<31;
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
	// 返回是不是最大的整数
	// 先整出最大的整数，然后异或取反
	int y = ~(1<<31);
  	return !(x^y);
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
	// 所有奇数位先通过与拿出来，然后异或取反
	// 异或取反可以判断两个数是否相等
	int allOddOne = 0xAA;
	allOddOne = (allOddOne << 8) + allOddOne;
	allOddOne = (allOddOne << 16) + allOddOne;
	// printf("allOddOne = %x\n", allOddOne);
	int oddBitOut = x & allOddOne;
	int y = allOddOne;
  	return !(oddBitOut^y);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
	// 取反加一，但是会在tmin()处溢出，但是别无选择
  	return ~x+1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
	// 重点在判断0x??????3(0-9)
	int eight = 0x8;
	int nine = 0x9;
	int s0 = x&0x0F;
	// printf("%x\n", ~0x3F);
  	return (!(x&(~0x3F)))	// 前26个bit如果是1直接false，
			& (!((x&0x30)^0x30))	// 第27，28个bit必须是1
			& (!(x&0x08)	// 最后四个bit，第一个bit不是1则直接true
			| (!(s0^eight)) 
			| (!(s0^nine)));
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
	// !!x在x非0的情况下等于1，否则为0.但是是个bool表达式，和0或一下不改变值但是改变类型，减少warning
	// 核心在如果是1就把它变成-1（0xFFFFFFFF）和相应的与一下就可以做到选择的效果
  	return (((~(0x0|(!!x)))+1)&y) 
	  		| (((~(0x0|(!x)))+1)&z);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
	// 先根据正负判断一次，然后同号再判断一次
	int tmin = 1<<31;
	int ff = ~0x0;
	int pos_x = !(x & tmin);
	int pos_y = !(y & tmin);
	int not_pos_y = !pos_y;
	int not_pos_x = !pos_x;

	x = (pos_x+ff)^x;	// 负数取反，正数不变
	y = (pos_y+ff)^y;	// 负数取反，正数不变
	int s = !((~x+y)&tmin);
  	return ((not_pos_x | pos_y)) & (
		  		(not_pos_x & pos_y) | 	// 一正一负
				(s^not_pos_y) | // 小于
	  			(!(x^y))
			  );
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
	// 利用只有0和0xFFFFFFFF异或最高位才有1(还有0x80000000)，右移31位情况下才是1，否则都是0（负数正书分开讨论）
	// 然后-1 再求相反数就是结果
	int tmin = 1<<31;
	int negative_x = ~x+1;
	int isMinInt = !(x^tmin);
	return 	((x^tmin)>>31) & ((negative_x^tmin)>>31) & (!isMinInt);
}

/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
	int y = x;
	int ff = ~0x0;								// 1
	int positive = !(x>>31);					// 2
	y = (positive+ff)^x;						// 负数取反 2

	// 分成4块，然后求每块内的最大偏移
	int x0 = y & 0xFF;							// 1
	int x1 = (y >> 8) & 0xFF;					// 2
	int x2 = (y >> 16) & 0xFF;					// 2
	int x3 = y >> 24;							// 2
	int base = 0;
	int z = x0;
	int a1 = !x1 + ff;	int not_a1 = ~a1;					// 3
	int a2 = !x2 + ff;	int not_a2 = ~a2;					// 3
	int a3 = !x3 + ff;	int not_a3 = ~a3;					// 3
	base = 	a1&8;											// 1
	z = 	(a1&x1) | (not_a1&z);							// 3
	base =	(a2&16) | (not_a2&base);						// 3
	z = 	(a2&x2) | (not_a2&z);							// 3
	base = 	(a3&24) | (not_a3&base);						// 3
	z = 	(a3&x3) | (not_a3&z);							// 3
	int second_base = 0;

	// 再分成两块 2*4
	int y0 = z & 0x0F; int y1 = (z & 0xF0)>>4;				// 3
	z = y0;
	int b1 = !y1 + ff;										// 2
	second_base = b1 & 4;									// 1
	z = (b1 & y1) | ((~b1) & z);							// 4
	int s1 = !(z&0x2)+ff; 	int s2 = !(z&0x4)+ff; 	int s3 = !(z&0x8)+ff;		// 9														// 3
	int third_base = 0;

	third_base = ((!(z&0x1)+ff)&1);							// 4	
	third_base = (s1&2) | ((~s1)&third_base);				// 4
	third_base = (s2&3) | ((~s2)&third_base);				// 4
	third_base = (s3&4) | ((~s3)&third_base);				// 4

	return third_base+second_base+base+1;					// 3
															// total = 75
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
	int tmin = 1<<31;
	int pos = uf&tmin;
	int exponent = (uf>>23)&0xFF;	// 指数部分的绝对值
	int decimal = uf&(~(((0xFF<<8)+0x80)<<16));	// 你问这是什么，这其实就是0x007FFFFF，为什么这么麻烦，不允许用4字节常数就离谱，我凑给你看
	int exponent_equal_zero = exponent == 0;
	int isZero = exponent_equal_zero && decimal == 0;
	if (isZero || exponent == 0xFF) {
		return uf;	// 如果是NaN和无穷，直接返回本身
	}
	int highest_bit_in_decimal = (decimal&(1<<22));
	if (!highest_bit_in_decimal && decimal != 0 && exponent_equal_zero) { // 如果是非规格化数且乘以2之后还是非规格化数，那就小数位左移
		decimal = decimal << 1;
	} else {	// 否则改动指数位
		if (highest_bit_in_decimal && exponent_equal_zero) {  // 如果之前是非规格化，现在是规格化，那就得规格化一下
			decimal = decimal << 1;
		}
		exponent++;
	}
	
	unsigned res = (exponent<<23) | pos | decimal;
  	return res;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
	int tmin = 1<<31;
	int sign = uf&tmin;
	int exponent = (uf>>23)& 0xFF;	// 指数部分的绝对值
	// exponent -= 126;
	int fraction;	// 你问这是什么，这其实就是0x007FFFFF，为什么这么麻烦，不允许用4字节常数就离谱，我凑给你看
	fraction = uf & (~(0xFF<<24));
	int isZero = !exponent && !fraction;

	if (isZero || exponent<126) {
		return 0;
	}
	if (exponent > 157 || (exponent == 157 && !sign)) {
		return tmin;	// 超出int范围
	}
	int res = fraction;
	// printf("uf = %x, sign = %d, exponent = %d, fraction = %x, isZero = %d\n", uf, sign, exponent, fraction, isZero);
	if (exponent > 150) {
		res = fraction << (exponent-150);
	} else {
		res = fraction >> (150-exponent);
	}
	res = sign ? -res : res;
	return res;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
	int inf = 0xFF << 23;
	if (x>128) {
		return inf;
	}
	if (x < 0-126) {
		return 0;
	}
	int exponent = (x+127) << 23;
    return exponent;
}
