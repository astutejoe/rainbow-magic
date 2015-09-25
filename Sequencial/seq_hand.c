#include <openssl/evp.h>
#include <openssl/sha.h>
#include <omp.h>
#include <string.h>

#define IC 4096
#define KeyLength 32
#define SHASIZE 20

unsigned char *salt = "test";
unsigned char saltLen = 4;

unsigned char* itoa(int val){
	
	static unsigned char buf[32] = {0};
	
	int i = 30;
	
	for(; val && i ; --i, val /= 10)
		buf[i] = "0123456789abcdef"[val % 10];
	
	return &buf[i+1];
}
	
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
/*   $output = "";
    for($i = 1; $i <= 2; $i++) {
        // $i encoded as 4 bytes, big endian.
        $last = $salt . pack("N", $i);
        // first iteration
        $last = $xorsum = hash_hmac($algorithm, $last, $password, true);
        // perform the other $count - 1 iterations
        for ($j = 1; $j < $count; $j++) {
            $xorsum ^= ($last = hash_hmac($algorithm, $last, $password, true));
        }
        $output .= $xorsum;
    }*/
  unsigned char finalsum[SHASIZE];
  unsigned char finalsum2[SHASIZE];

  unsigned char digest[SHASIZE];

  unsigned char conc[saltLen + 5];
  memcpy(conc, salt, saltLen);
  memcpy(conc + saltLen, "0001", 4);
  hmac_sha1(password, passwordLength, conc, saltLen + 32, digest);

  memcpy(finalsum, digest, SHASIZE);

  for (int j = 0; j < IC-1; j++)
  {
    unsigned char tmp[SHASIZE];
    memcpy(tmp, digest, SHASIZE);
    hmac_sha1(password, passwordLength, tmp, SHASIZE, digest);
    for (int i = 0; i < 20; i++) {
      finalsum[i] ^= digest[i];
    }
  }

  unsigned char digest2[SHASIZE];

  memset(conc, 0, saltLen + 5);
  memcpy(conc, salt, saltLen);
  memcpy(conc + saltLen, "0002", 4);
  hmac_sha1(password, passwordLength, conc, saltLen + 32, digest2);

  memcpy(finalsum2, digest2, SHASIZE);

  for (int j = 0; j < IC-1; j++)
  {
    unsigned char tmp[SHASIZE];
    memcpy(tmp, digest2, SHASIZE);
    hmac_sha1(password, passwordLength, tmp, SHASIZE, digest2);
    for (int i = 0; i < 20; i++) {
      finalsum2[i] ^= digest2[i];
    }
  }

  memcpy(key, finalsum, 20);
  memcpy(key+20, finalsum2, 12);

  return key;

}

int main()
{
  unsigned char *pass = "123456";

  #pragma omp parallel for
  for (int i = 0; i < 999; i++) {
    unsigned char *output = PBKDF2(pass, 6);
  }

  //print_hex(output, 32);

  return 0;
}