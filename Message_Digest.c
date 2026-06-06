//Message Digest --> MD5
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

// --- CONSTANTS ---
const int SHIFT[] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

uint32_t K[64];

// Equivalent to Integer.rotateLeft
uint32_t rotateLeft(uint32_t x, int n) {
    return (x << n) | (x >> (32 - n));
}

// Initialize K table exactly like Java constructor
void initK() {
    for (int i = 0; i < 64; i++) {
        K[i] = (uint32_t)(pow(2, 32) * fabs(sin(i + 1)));
    }
}

// --- CORE PROCESS ---
void md5Process(const char* message) {
    int originalLength = strlen(message);
   
    // Calculate padding (identical to Java logic)
    int paddingLength = (56 - (originalLength + 1) % 64 + 64) % 64;
    int totalLength = originalLength + 1 + paddingLength + 8;
   
    uint8_t* padded = (uint8_t*)calloc(totalLength, 1);
    memcpy(padded, message, originalLength);
   
    padded[originalLength] = 0x80; // Add 1 bit (0x80 byte)
   
    // Add original length in bits as a 64-bit little-endian
    uint64_t bitLength = (uint64_t)originalLength * 8;
    for (int i = 0; i < 8; i++) {
        padded[totalLength - 8 + i] = (uint8_t)(bitLength >> (8 * i));
    }

    // Initialize registers
    uint32_t A = 0x67452301;
    uint32_t B = 0xefcdab89;
    uint32_t C = 0x98badcfe;
    uint32_t D = 0x10325476;

    // Process blocks of 64 bytes
    for (int i = 0; i < totalLength; i += 64) {
        uint32_t M[16];
        for (int j = 0; j < 16; j++) {
            int idx = i + j * 4;
            M[j] = (padded[idx]) |
                   (padded[idx + 1] << 8) |
                   (padded[idx + 2] << 16) |
                   (padded[idx + 3] << 24);
        }

        uint32_t a = A, b = B, c = C, d = D;

        for (int k = 0; k < 64; k++) {
            uint32_t F, g;
            if (k < 16) {
                F = (b & c) | ((~b) & d);
                g = k;
            } else if (k < 32) {
                F = (d & b) | ((~d) & c);
                g = (5 * k + 1) % 16;
            } else if (k < 48) {
                F = b ^ c ^ d;
                g = (3 * k + 5) % 16;
            } else {
                F = c ^ (b | (~d));
                g = (7 * k) % 16;
            }

            uint32_t temp = d;
            d = c;
            c = b;
            uint32_t sum = a + F + K[k] + M[g];
            uint32_t rotated = rotateLeft(sum, SHIFT[k]);
            b = b + rotated;
            a = temp;
        }

        A += a;
        B += b;
        C += c;
        D += d;
    }

    // Print result in Little-Endian Hex format (toHexLE in Java)
    uint32_t regs[] = {A, B, C, D};
    printf("MD5 Hash: ");
    for (int r = 0; r < 4; r++) {
        printf("%02x%02x%02x%02x",
                regs[r] & 0xFF,
               (regs[r] >> 8) & 0xFF,
               (regs[r] >> 16) & 0xFF,
               (regs[r] >> 24) & 0xFF);
    }
    printf("\n");

    free(padded);
}

int main() {
    initK();
    char input[256];
   
    printf("Enter message: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // Remove newline character

    md5Process(input);

    return 0;
}
