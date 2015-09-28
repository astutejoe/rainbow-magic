#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#define IC 4096
#define KeyLength 32
#define SHASIZE 20
#define MAX_MESSAGE_LENGTH 4096

#define ipad 0x36
#define opad 0x5c
#define b 64

unsigned char *salt = "test";
unsigned char saltLen = 4;
	
int print_hex(unsigned char *buf, int len)
{
    for(int i=0;i<len;i++)
        printf("%02x",buf[i]);

    printf("\n");

    return(0);
}

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
            SHA1(key, key_length, digest);
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

    SHA1(step5data, data_length+b, digest);

    for (i=0; i<b; i++)
        step7data[i] = k0[i] ^ opad;

    for (i=0;i<b;i++)
        step8data[i] = step7data[i];

    for (i=0;i<20;i++)
        step8data[i+b] = digest[i];

    SHA1(step8data, b+20, digest);
}

char* PBKDF2(unsigned char *password, unsigned char passwordLength)
{
  unsigned char *key = malloc(32);

  unsigned char finalsum[SHASIZE];
  unsigned char digest[SHASIZE];

  unsigned char conc[saltLen + 5];
  memcpy(conc, salt, saltLen);
  conc[saltLen+0] = 0;
  conc[saltLen+1] = 0;
  conc[saltLen+2] = 0;
  conc[saltLen+3] = 1;

  hmac(password, passwordLength, conc, saltLen+4, digest);

  memcpy(finalsum, digest, SHASIZE);

  for (int j = 0; j < IC-1; j++)
  {
    hmac(password, passwordLength, digest, SHASIZE, digest);
    for (int i = 0; i < SHASIZE; i++) {
      finalsum[i] ^= digest[i];
    }
  }
  memcpy(key, finalsum, 20);

  conc[saltLen+3] = 2;
  hmac(password, passwordLength, conc, saltLen+4, digest);

  memcpy(finalsum, digest, SHASIZE);

  for (int j = 0; j < IC-1; j++)
  {
    hmac(password, passwordLength, digest, SHASIZE, digest); 
    for (int i = 0; i < SHASIZE; i++) {
      finalsum[i] ^= digest[i];
    }
  }

  memcpy(key+20, finalsum, 12);

  return key;
}

int main()
{
  unsigned char *pass = "123456";
  unsigned char *output;
  #pragma omp parallel for
  for (int i = 0; i < 999; i++) {
    output = PBKDF2(pass, 6);
  }

  print_hex(output, 32);

  return 0;
}
