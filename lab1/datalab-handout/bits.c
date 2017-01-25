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
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

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

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

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
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
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
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  // x AND y is the same as NOT x or NOT y, NOTed.
  return ~(~x|~y);
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  // take x and shift it to the right 8 * n
  // then AND it with 0x0000 0000 1111 1111 to get from word
  int temp = (x >> (n << 3)) & 0xFF;
  return temp;
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  // shift the value x n times
  int shift = x >> n;
  
  // replace missing bits what what they are supposed to be
  int temp = ~0 << (32 + ~n) << 1; //shift 1 positions left
  temp = ~temp; //flip

  // keep appropriate bits
  temp = temp & shift;
  return temp;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  //Build mask to help manage bits 
  int temp = ((0x11 | (0x11 << 8)) | ((0x11 | (0x11 << 8)) << 16));

  // initialize the sum with by masking the argument,
  // start shifting through x using mask
  int sum = x & temp;
  sum += x >> 1 & temp;
  sum += x >> 2 & temp;
  sum += x >> 3 & temp;

  sum = sum + (sum >> 16); //combined bottom/top sum
  
  // split lower bits
  temp = 0xF | (0xF << 8);
  //Mask over the sum
  sum = (sum & temp) + ((sum >> 4) & temp);
  //Sum it
  sum = (sum + (sum >> 8)) & 0x3F;

  return sum;
}

/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  int c = (x >> 16) | x;
  c = (c >> 8) | c;
  c = (c >> 4) | c;
  c = (c >> 2) | c;
  c = (c >> 1) | c;
  c = (~c & 1);
  return c;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1 << 31;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  //get the greatest to compare
  int temp = x >> 31;
  // Try temp w/ x and find out if it fits in n bits
  // ans will be true if it doesn't fit so needs to be
  // NOTed
  int ans = (((~x & temp) + (x & ~temp)) >> (n + ~0));
  return !ans; 
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
  int e = (x >> 31) & ((1 << n) + ~0);
  return (x + e) >> n;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x + 1;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  int ans = ((x>>0x1f)&1)^(!!x);
  return ans;
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int difference, negate, temp;
  difference = y + (~x + 1);
  negate = x ^ y;
  temp = ~negate;
 
  negate = negate & x;
  difference = ~difference;

  temp = temp & difference;
  temp = temp | negate;
  temp = temp & (1 << 31);

  temp = !!temp; 

  return temp;
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
  int temp, temp2, temp3, temp4, temp5;
 
  //Get the most significant bit
  x = x | (x >> 1);
  x = x | (x >> 2);
  x = x | (x >> 4);
  x = x | (x >> 8);
  x = x | (x >> 16);

  temp = 0x55 | (0x55 << 8); 
  //printf("%d", temp);
  temp = temp | (temp << 16);

  temp2 = 0x33 | (0x33 << 8);
  temp2 = temp2 | (temp2 << 16);

  temp3 = 0x0f | (0x0f << 8);
  temp3 = temp3 | (temp3 << 16);

  temp4 = 0xff | (0xff << 16);
  temp5 = 0xff | (0xff << 8);

  x = (x & temp) + ((x >> 1) & temp);
  x = (x & temp2) + ((x >> 2) & temp2);
  x = (x & temp3) + ((x >> 4) & temp3);
  x = (x & temp4) + ((x >> 8) & temp4);
  x = (x & temp5) + ((x >> 16) & temp5);

  x = x + ~0;
  return x;
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
  int nan = 0x000000FF << 23;
  int fraction = 0x7FFFFF & uf;
 
  if((nan & uf) == nan && fraction)
    return uf;

  return uf ^ (1 << 31);
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  unsigned exponent, temp, sign;
  int true = 1; //dummy for while loop

  if(x) {
    //Check if value is less than 0 to extablish what the exponent is
    if(x < 0) {
      sign = -x;
      exponent = 0xCF800000;
    }
    //Set the exponent to default
    else {
      sign = x;
      exponent = 0x4F800000;
    }
    
    //cycle through exponent
    while(true) {
      temp = sign;
      sign <<= 1;
      exponent -= 0x800000;
      if(temp & 0x80000000)
        break;
    }

    //Set apropriate exponent
    exponent = exponent + (sign >> 9);
    if((sign & 0x3FF) == 0x300) 
      exponent = exponent + 1;
    else if((sign & 0x1FF) > 0x100)
      exponent = exponent + 1;
    
    return exponent;
  }
  //x is 0
  return 0;
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
  unsigned sign = uf & 0x80000000, no_sign = uf << 1, answer = 0;

  //Setting up the answer with signs
  //Check if unsigned is too big
  if(0xFF000000 <= no_sign)
    answer = uf;
  //Check if unsigned fits
  else if(0x01000000 > no_sign)
    answer = sign | no_sign;
  else if(0xFE000000 > no_sign)
    answer = uf + 0x800000;
  else
    answer = sign | 0x7F800000;

  return answer;
}
