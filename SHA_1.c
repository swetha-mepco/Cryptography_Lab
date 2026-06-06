//SHA 1
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define ROTL(x,n) ((x << n) | (x >> (32-n)))

// SHA1 function
void sha1(const char *msg) {

    uint64_t len = strlen(msg);
    uint64_t bitLen = len * 8;

    // Padding
    int newLen = len + 1;
    while (newLen % 64 != 56)
        newLen++;

    uint8_t *data = calloc(newLen + 8, 1);
    memcpy(data, msg, len);

    data[len] = 0x80; // append 1 bit

    // append original length (big endian)
    for (int i = 0; i < 8; i++)
        data[newLen + i] = (bitLen >> (56 - 8*i)) & 0xff;

    // Initial hash values
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;

    // Process each 512-bit block
    for (int i = 0; i < newLen + 8; i += 64) {

        uint32_t w[80];

        // Break into 16 words
        for (int j = 0; j < 16; j++) {
            w[j] = (data[i + j*4] << 24) |
                   (data[i + j*4 + 1] << 16) |
                   (data[i + j*4 + 2] << 8) |
                   (data[i + j*4 + 3]);
        }

        // Extend to 80 words
        for (int j = 16; j < 80; j++)
            w[j] = ROTL(w[j-3] ^ w[j-8] ^ w[j-14] ^ w[j-16], 1);

        uint32_t a=h0, b=h1, c=h2, d=h3, e=h4;

        // Main loop
        for (int j = 0; j < 80; j++) {

            uint32_t f, k;

            if (j < 20) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            }
            else if (j < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if (j < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

            uint32_t temp = ROTL(a,5) + f + e + k + w[j];
            e = d;
            d = c;
            c = ROTL(b,30);
            b = a;
            a = temp;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    free(data);

    printf("\nSHA-1 Hash:\n");
    printf("%08x %08x %08x %08x %08x\n", h0,h1,h2,h3,h4);
}

// Main
int main() {
    char msg[200];

    printf("Enter message: ");
    fgets(msg, sizeof(msg), stdin);
    msg[strcspn(msg,"\n")] = 0;

    sha1(msg);

    return 0;
}
