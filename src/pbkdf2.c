/*
 * Copyright (C) 2015 Gabriel G. Garcia <gabriel.antunes.garcia@gmail.com>
 *                    Pedro H. Penna <pedrohenriquepenna@gmail.com>
 * 
 * Rainbow Magic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Rainbow Magic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <stdlib.h>
#include <string.h>

#include "rainbow-magic.h"

#define IC 4096
#define KeyLength 32
#define SHASIZE 20
#define MAX_MESSAGE_LENGTH 4096

#define ipad 0x36
#define opad 0x5c
#define b 64

void hmac(unsigned char *key, unsigned char key_length, unsigned char *data, unsigned char data_length, unsigned char *digest)
{
    unsigned char k0[b];
    unsigned char k0xorIpad[b];
    unsigned char step7data[b];
    unsigned char step5data[MAX_MESSAGE_LENGTH+128];
    unsigned char step8data[b+20];
    int i;

    memset(k0, 0, b);

    if (key_length != b)
    {
        if (key_length > b)      
        {
            digest = sha1(key, key_length);
            memcpy(k0, digest, 20);
        }
        else if (key_length < b)
        {
            memcpy(k0, key, key_length);
        }
    }
    else
    {
        memcpy(k0, key, b);
    }

    for (i=0; i<b; i++)
        k0xorIpad[i] = k0[i] ^ ipad;

    for (i=0; i<b; i++)
        step5data[i] = k0xorIpad[i];

    for (i=0;i<data_length;i++)
        step5data[i+b] = data[i];

    digest = sha1(step5data, data_length+b);

    for (i=0; i<b; i++)
        step7data[i] = k0[i] ^ opad;

    for (i=0;i<b;i++)
        step8data[i] = step7data[i];

    for (i=0;i<20;i++)
        step8data[i+b] = digest[i];

    digest = sha1(step8data, b+20);
}

unsigned char *pbkdf2(unsigned char *password, unsigned char passwordLength)
{
  unsigned char *key = malloc(32);

  unsigned char finalsum[SHASIZE];
  unsigned char finalsum2[SHASIZE];
  unsigned char digest[SHASIZE];
  unsigned char digest2[SHASIZE];

  unsigned char conc[saltlen + 5];
  memcpy(conc, salt, saltlen);
  conc[saltlen+0] = 0;
  conc[saltlen+1] = 0;
  conc[saltlen+2] = 0;
  conc[saltlen+3] = 1;

  hmac(password, passwordLength, conc, saltlen+4, digest);
  memcpy(finalsum, digest, SHASIZE);

  conc[saltlen+3] = 2;

  hmac(password, passwordLength, conc, saltlen+4, digest2);
  memcpy(finalsum2, digest2, SHASIZE);

  for (int j = 0; j < IC-1; j++)
  {
    hmac(password, passwordLength, digest, SHASIZE, digest);
    hmac(password, passwordLength, digest2, SHASIZE, digest2);

    for (int i = 0; i < SHASIZE; i++) {
      finalsum[i] ^= digest[i];
      finalsum2[i] ^= digest2[i];
    }
  }
  
  memcpy(key, finalsum, 20);
  memcpy(key+20, finalsum2, 12);

  return key;
}
