/* 
 * SHA-1 hash in C
 * 
 * Copyright (c) 2014 Project Nayuki
 * http://www.nayuki.io/page/fast-sha1-hash-implementation-in-x86-assembly
 * 
 * (MIT License)
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 */

#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "rainbow-magic.h"

void sha1_engine(uint32_t state[5], const uint8_t block[64]) {
	#define SCHEDULE(i)  \
		temp = schedule[(i - 3) & 0xf] ^ schedule[(i - 8) & 0xf] ^ schedule[(i - 14) & 0xf] ^ schedule[(i - 16) & 0xf];  \
		schedule[i & 0xf] = temp << 1 | temp >> 31;
	
	#define LOADSCHEDULE(i)  \
		schedule[i] =                           \
			  (uint32_t)block[(i << 2) + 0] << 24  \
			| (uint32_t)block[(i << 2) + 1] << 16  \
			| (uint32_t)block[(i << 2) + 2] <<  8  \
			| (uint32_t)block[(i << 2) + 3];
	
	#define ROUND0a(a, b, c, d, e, i)  LOADSCHEDULE(i)  ROUNDTAIL(a, b, e, ((b & c) | (~b & d))         , i, 0x5A827999)
	#define ROUND0b(a, b, c, d, e, i)  SCHEDULE(i)      ROUNDTAIL(a, b, e, ((b & c) | (~b & d))         , i, 0x5A827999)
	#define ROUND1(a, b, c, d, e, i)   SCHEDULE(i)      ROUNDTAIL(a, b, e, (b ^ c ^ d)                  , i, 0x6ED9EBA1)
	#define ROUND2(a, b, c, d, e, i)   SCHEDULE(i)      ROUNDTAIL(a, b, e, ((b & c) ^ (b & d) ^ (c & d)), i, 0x8F1BBCDC)
	#define ROUND3(a, b, c, d, e, i)   SCHEDULE(i)      ROUNDTAIL(a, b, e, (b ^ c ^ d)                  , i, 0xCA62C1D6)
	
	#define ROUNDTAIL(a, b, e, f, i, k)  \
		e += (a << 5 | a >> 27) + f + UINT32_C(k) + schedule[i & 0xF];  \
		b = b << 30 | b >> 2;
	
	uint32_t a = state[0];
	uint32_t b = state[1];
	uint32_t c = state[2];
	uint32_t d = state[3];
	uint32_t e = state[4];
	
	uint32_t schedule[16];
	uint32_t temp;
	ROUND0a(a, b, c, d, e,  0)
	ROUND0a(e, a, b, c, d,  1)
	ROUND0a(d, e, a, b, c,  2)
	ROUND0a(c, d, e, a, b,  3)
	ROUND0a(b, c, d, e, a,  4)
	ROUND0a(a, b, c, d, e,  5)
	ROUND0a(e, a, b, c, d,  6)
	ROUND0a(d, e, a, b, c,  7)
	ROUND0a(c, d, e, a, b,  8)
	ROUND0a(b, c, d, e, a,  9)
	ROUND0a(a, b, c, d, e, 10)
	ROUND0a(e, a, b, c, d, 11)
	ROUND0a(d, e, a, b, c, 12)
	ROUND0a(c, d, e, a, b, 13)
	ROUND0a(b, c, d, e, a, 14)
	ROUND0a(a, b, c, d, e, 15)
	ROUND0b(e, a, b, c, d, 16)
	ROUND0b(d, e, a, b, c, 17)
	ROUND0b(c, d, e, a, b, 18)
	ROUND0b(b, c, d, e, a, 19)
	ROUND1(a, b, c, d, e, 20)
	ROUND1(e, a, b, c, d, 21)
	ROUND1(d, e, a, b, c, 22)
	ROUND1(c, d, e, a, b, 23)
	ROUND1(b, c, d, e, a, 24)
	ROUND1(a, b, c, d, e, 25)
	ROUND1(e, a, b, c, d, 26)
	ROUND1(d, e, a, b, c, 27)
	ROUND1(c, d, e, a, b, 28)
	ROUND1(b, c, d, e, a, 29)
	ROUND1(a, b, c, d, e, 30)
	ROUND1(e, a, b, c, d, 31)
	ROUND1(d, e, a, b, c, 32)
	ROUND1(c, d, e, a, b, 33)
	ROUND1(b, c, d, e, a, 34)
	ROUND1(a, b, c, d, e, 35)
	ROUND1(e, a, b, c, d, 36)
	ROUND1(d, e, a, b, c, 37)
	ROUND1(c, d, e, a, b, 38)
	ROUND1(b, c, d, e, a, 39)
	ROUND2(a, b, c, d, e, 40)
	ROUND2(e, a, b, c, d, 41)
	ROUND2(d, e, a, b, c, 42)
	ROUND2(c, d, e, a, b, 43)
	ROUND2(b, c, d, e, a, 44)
	ROUND2(a, b, c, d, e, 45)
	ROUND2(e, a, b, c, d, 46)
	ROUND2(d, e, a, b, c, 47)
	ROUND2(c, d, e, a, b, 48)
	ROUND2(b, c, d, e, a, 49)
	ROUND2(a, b, c, d, e, 50)
	ROUND2(e, a, b, c, d, 51)
	ROUND2(d, e, a, b, c, 52)
	ROUND2(c, d, e, a, b, 53)
	ROUND2(b, c, d, e, a, 54)
	ROUND2(a, b, c, d, e, 55)
	ROUND2(e, a, b, c, d, 56)
	ROUND2(d, e, a, b, c, 57)
	ROUND2(c, d, e, a, b, 58)
	ROUND2(b, c, d, e, a, 59)
	ROUND3(a, b, c, d, e, 60)
	ROUND3(e, a, b, c, d, 61)
	ROUND3(d, e, a, b, c, 62)
	ROUND3(c, d, e, a, b, 63)
	ROUND3(b, c, d, e, a, 64)
	ROUND3(a, b, c, d, e, 65)
	ROUND3(e, a, b, c, d, 66)
	ROUND3(d, e, a, b, c, 67)
	ROUND3(c, d, e, a, b, 68)
	ROUND3(b, c, d, e, a, 69)
	ROUND3(a, b, c, d, e, 70)
	ROUND3(e, a, b, c, d, 71)
	ROUND3(d, e, a, b, c, 72)
	ROUND3(c, d, e, a, b, 73)
	ROUND3(b, c, d, e, a, 74)
	ROUND3(a, b, c, d, e, 75)
	ROUND3(e, a, b, c, d, 76)
	ROUND3(d, e, a, b, c, 77)
	ROUND3(c, d, e, a, b, 78)
	ROUND3(b, c, d, e, a, 79)
	
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;
}

unsigned char* sha1(unsigned char* in, unsigned int length)
{
	unsigned char* digest = smalloc(20);

	unsigned long originalLength = length << 3;

	unsigned char* tmp = smalloc(length+1);
	memcpy(tmp, in, length);
	tmp[length] = 0x80;
	in = smalloc(length+1);
	memcpy(in, tmp, length+1);

	length++;

	if (((56 - length) & 63) & 63 != 0)
	{
		unsigned int tmplength = length + ((56 - length) & 63) & 63;
		unsigned char* tmp = smalloc(tmplength);
		memcpy(tmp, in, length);

		memset(tmp + length, 0, ((56 - length) & 63) & 63);

		in = tmp;
		length = tmplength;
	}

	tmp = smalloc(length+8);
	memcpy(tmp, in, length);

	for(int i = 7; i >= 0; i--){
		tmp[length+(7-i)] = (originalLength>>(8*i)) & 0xff;
	}

	in = smalloc(length+8);
	memcpy(in, tmp, length+8);

	length += 8;

	unsigned int chunks = length & ~63;

	uint32_t state[5];
	state[0] = 0x67452301;
	state[1] = 0xefcdab89;
	state[2] = 0x98badcfe;
	state[3] = 0x10325476;
	state[4] = 0xc3d2e1f0;

	for (int i = 0; i < chunks; i++) {
		uint8_t block[64];
		memcpy(block, (in + 64*i), 64);
		sha1_engine(state, block);
	}

    digest[0] = state[0] >> 24;
    digest[1] = state[0] >> 16;
    digest[2] = state[0] >> 8;
    digest[3] = state[0];
    digest[4] = state[1] >> 24;
    digest[5] = state[1] >> 16;
    digest[6] = state[1] >> 8;
    digest[7] = state[1];
    digest[8] = state[2] >> 24;
    digest[9] = state[2] >> 16;
    digest[10] = state[2] >> 8;
    digest[11] = state[2];
    digest[12] = state[3] >> 24;
    digest[13] = state[3] >> 16;
    digest[14] = state[3] >> 8;
    digest[15] = state[3];
    digest[16] = state[4] >> 24;
    digest[17] = state[4] >> 16;
    digest[18] = state[4] >> 8;
    digest[19] = state[4];

//	printf("%d\n%d\n%d\n%d\n%d\n", state[0], state[1], state[2], state[3], state[4]);

	return digest;
}
