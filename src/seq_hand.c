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
