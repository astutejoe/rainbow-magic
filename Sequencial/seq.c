#include <openssl/evp.h>
#include <omp.h>

 #define IC 4096

char* itoa(int val){
	
	static char buf[32] = {0};
	
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

int main()
{
    char *pass = itoa(5478214);
    char *salt = "test";
    unsigned char buf[33];

    #pragma omp parallel for private(pass)
    for (int i = 0; i < 999; i++)
    {
    	pass = itoa(i);
    	PKCS5_PBKDF2_HMAC_SHA1(pass, 8, salt, 4, 4096, 32, buf);
    }

    return 0;
}