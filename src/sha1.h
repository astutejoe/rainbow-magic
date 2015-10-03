/*
 * Based on the Mozilla SHA1 (see mozilla-sha1/sha1.h),
 * optimized to do word accesses rather than byte accesses,
 * and to avoid unnecessary copies into the context array.
 */

typedef struct {
	unsigned int H[5];
	unsigned int W[16];
	unsigned long long size;
} blk_SHA_CTX;

void sha1(const void*, unsigned long, unsigned char[20]);
void blk_SHA1_Update(blk_SHA_CTX *ctx, const void *dataIn, unsigned long len);
