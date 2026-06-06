//CMAC
#include <stdio.h>
#include <string.h>

typedef unsigned char byte;

// ---------------- AES (SIMPLIFIED) ----------------
byte S_BOX[2][16] = {
    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0}
};

void to_hex(byte *block) {
    for (int i = 0; i < 16; i++) printf("%02x ", block[i]);
    printf("\n");
}

void xor_bytes(byte *res, byte *a, byte *b) {
    for (int i = 0; i < 16; i++) res[i] = a[i] ^ b[i];
}

void sub_bytes(byte *state) {
    for (int i = 0; i < 16; i++) {
        state[i] = S_BOX[state[i] % 2][state[i] % 16];
    }
}

void shift_rows(byte *state) {
    byte temp = state[0];
    for (int i = 0; i < 15; i++) state[i] = state[i + 1];
    state[15] = temp;
}

void simple_aes_encrypt(byte *block, byte *key) {
    xor_bytes(block, block, key); // Initial AddRoundKey
    // printf("AES Start: "); to_hex(block);

    for (int r = 0; r < 3; r++) {
        sub_bytes(block);
        shift_rows(block);
        xor_bytes(block, block, key);
        // printf("Round %d: ", r + 1); to_hex(block);
    }
}

// ---------------- CMAC ----------------
void left_shift(byte *block) {
    byte first_byte = block[0];
    for (int i = 0; i < 15; i++) block[i] = block[i + 1];
    block[15] = 0;
    if (first_byte & 0x80) block[15] ^= 0x87;
}

void generate_subkeys(byte *key, byte *K1, byte *K2) {
    byte L[16] = {0};
    printf("Generating Subkeys...\n");

    simple_aes_encrypt(L, key);
    printf("L: "); to_hex(L);

    memcpy(K1, L, 16);
    left_shift(K1);
    printf("K1: "); to_hex(K1);

    memcpy(K2, K1, 16);
    left_shift(K2);
    printf("K2: "); to_hex(K2);
}

void cmac(char *message, byte *key) {
    int msg_len = strlen(message);
    int num_blocks = (msg_len + 15) / 16;
    if (msg_len == 0) num_blocks = 1; // At least one block

    byte K1[16], K2[16];
    generate_subkeys(key, K1, K2);

    byte blocks[num_blocks][16];
    memset(blocks, 0, sizeof(blocks));

    // Fill blocks
    for (int i = 0; i < msg_len; i++) {
        blocks[i / 16][i % 16] = (byte)message[i];
    }

    int is_complete = (msg_len > 0 && msg_len % 16 == 0);
    byte last_block[16];
    memcpy(last_block, blocks[num_blocks - 1], 16);

    if (is_complete) {
        xor_bytes(last_block, last_block, K1);
        printf("Last block complete -> XOR with K1\n");
    } else {
        // Pad manually (0x80 followed by 0x00s)
        if (msg_len % 16 != 0 || msg_len == 0) {
            last_block[msg_len % 16] = 0x80;
        }
        xor_bytes(last_block, last_block, K2);
        printf("Last block padded -> XOR with K2\n");
    }
    printf("Last Block: "); to_hex(last_block);

    byte X[16] = {0};
    for (int i = 0; i < num_blocks - 1; i++) {
        xor_bytes(X, X, blocks[i]);
        simple_aes_encrypt(X, key);
        printf("X after Block %d: ", i + 1); to_hex(X);
    }

    // Final block
    xor_bytes(X, X, last_block);
    simple_aes_encrypt(X, key);

    printf("\nFinal CMAC Tag: "); to_hex(X);
}

int main() {
    char message[256];
    char key_input[256];
    byte key[16] = {0};

    printf("Enter message: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = 0;

    printf("Enter key (up to 16 chars): ");
    fgets(key_input, sizeof(key_input), stdin);
    key_input[strcspn(key_input, "\n")] = 0;

    // Prepare key (ljust 16 logic)
    for (int i = 0; i < 16; i++) {
        if (i < strlen(key_input)) key[i] = (byte)key_input[i];
        else key[i] = ' '; // Padding with spaces like ljust
    }

    cmac(message, key);

    return 0;
}

