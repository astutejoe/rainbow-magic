#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#define IC 4096
#define KeyLength 32
#define SHASIZE 20

unsigned char *salt = "test";
unsigned char saltLen = 4;
	
int print_hex(unsigned char *buf, int len)
{
    for(int i=0;i<len;i++)
        printf("%02x",buf[i]);

    printf("\n");

    return(0);
}

void hmac_sha1(const unsigned char *text, int text_len, const unsigned char *key, int key_len, unsigned char *digest)
{
  SHA_CTX context;
  unsigned char k_ipad[65];
  unsigned char k_opad[65];
  unsigned char tk[20];

  if (key_len > 64) {
    SHA_CTX      tctx;

    SHA1_Init(&tctx);
    SHA1_Update(&tctx, key, key_len);
    SHA1_Final(tk, &tctx);

    key = tk;
    key_len = 20;
  }

  memset( k_ipad, 0, sizeof(k_ipad));
  memset( k_opad, 0, sizeof(k_opad));
  memcpy( k_ipad, key, key_len);
  memcpy( k_opad, key, key_len);

  for (int i = 0; i < 64; i++) {
    k_ipad[i] ^= 0x36;
    k_opad[i] ^= 0x5c;
  }

  SHA1_Init(&context);
  SHA1_Update(&context, k_ipad, 64);
  SHA1_Update(&context, text, text_len);
  SHA1_Final(digest, &context);

  SHA1_Init(&context);
  SHA1_Update(&context, k_opad, 64);
  SHA1_Update(&context, digest, 20);
  SHA1_Final(digest, &context);
}

char* PBKDF2(unsigned char *password, unsigned char passwordLength)
{
  unsigned char *key = malloc(32);

  unsigned char finalsum[SHASIZE];

  unsigned char conc[saltLen + 5];
  memcpy(conc, salt, saltLen);
  conc[saltLen+0] = 0;
  conc[saltLen+1] = 0;
  conc[saltLen+2] = 0;
  conc[saltLen+3] = 1;

  unsigned char *digest = HMAC(EVP_sha1(), password, passwordLength, conc, saltLen+4, NULL, NULL);
  //print_hex(digest, SHASIZE);

  memcpy(finalsum, digest, SHASIZE);

  for (int j = 0; j < IC-1; j++)
  {
    //hmac_sha1(password, passwordLength, tmp, SHASIZE, digest);
    digest = HMAC(EVP_sha1(), password, passwordLength, digest, SHASIZE, NULL, NULL);  
    for (int i = 0; i < SHASIZE; i++) {
      finalsum[i] ^= digest[i];
    }
  }
  memcpy(key, finalsum, 20);

  conc[saltLen+3] = 2;
  //hmac_sha1(password, passwordLength, conc, saltLen + 4, digest2);
  digest = HMAC(EVP_sha1(), password, passwordLength, conc, saltLen+4, NULL, NULL);
  //print_hex(digest, SHASIZE);

  memcpy(finalsum, digest, SHASIZE);

  for (int j = 0; j < IC-1; j++)
  {
    //hmac_sha1(password, passwordLength, tmp, SHASIZE, digest);
    digest = HMAC(EVP_sha1(), password, passwordLength, digest, SHASIZE, NULL, NULL);  
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
  for (int i = 0; i < 99; i++) {
    output = PBKDF2(pass, 6);
  }
  //unsigned char *output = PBKDF2(pass, 6);
  print_hex(output, 32);

  return 0;
}