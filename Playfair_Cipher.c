//Playfair cipher
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char matrix[5][5];

// Helper: Find row and column of a letter
void findPos(char ch, int *r, int *c) {
    if (ch == 'J') ch = 'I';
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (matrix[i][j] == ch) { *r = i; *c = j; return; }
}

// Helper: Build the 5x5 Playfair grid
void generateMatrix(char *key) {
    int seen[26] = {0};
    seen['J' - 'A'] = 1; // Treat J as I
    int r = 0, c = 0;
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   
    // Combine key and alphabet logic
    char fullKey[150];
    sprintf(fullKey, "%s%s", key, alphabet);

    for (int i = 0; fullKey[i] != '\0'; i++) {
        char ch = toupper(fullKey[i]);
        if (isalpha(ch) && !seen[ch - 'A']) {
            seen[ch - 'A'] = 1;
            matrix[r][c++] = ch;
            if (c == 5) { r++; c = 0; }
        }
    }
}

void encrypt(char *text) {
    printf("Encrypted: ");
    for (int i = 0; i < strlen(text); i += 2) {
        int r1, c1, r2, c2;
        findPos(text[i], &r1, &c1);
        findPos(text[i+1], &r2, &c2);

        if (r1 == r2)      printf("%c%c", matrix[r1][(c1 + 1) % 5], matrix[r2][(c2 + 1) % 5]);
        else if (c1 == c2) printf("%c%c", matrix[(r1 + 1) % 5][c1], matrix[(r2 + 1) % 5][c2]);
        else               printf("%c%c", matrix[r1][c2], matrix[r2][c1]);
    }
    printf("\n");
}

void decrypt(char *text) {
    printf("Decrypted: ");
    for (int i = 0; i < strlen(text); i += 2) {
        int r1, c1, r2, c2;
        findPos(text[i], &r1, &c1);
        findPos(text[i+1], &r2, &c2);

        // We use +4 instead of -1 to avoid negative numbers in C modulo
        if (r1 == r2)      printf("%c%c", matrix[r1][(c1 + 4) % 5], matrix[r2][(c2 + 4) % 5]);
        else if (c1 == c2) printf("%c%c", matrix[(r1 + 4) % 5][c1], matrix[(r2 + 4) % 5][c2]);
        else               printf("%c%c", matrix[r1][c2], matrix[r2][c1]);
    }
    printf("\n");
}

int main() {
    char key[50], input[100], prepared[100];
   
    printf("Enter Key: ");
    scanf("%s", key);
    generateMatrix(key);

    printf("Enter Plaintext: ");
    scanf("%s", input);

    // Prepare text (handle duplicates and odd length)
    int p = 0;
    for (int i = 0; i < strlen(input); i++) {
        prepared[p++] = toupper(input[i]);
        if (input[i+1] == '\0') {
            prepared[p++] = 'X';
        } else if (toupper(input[i]) == toupper(input[i+1])) {
            prepared[p++] = 'X';
        } else {
            prepared[p++] = toupper(input[++i]);
        }
    }
    prepared[p] = '\0';

    encrypt(prepared);

    // To test decrypt, we can pass the prepared/encrypted style text
    char cipher[100];
    printf("\nEnter Ciphertext to decrypt: ");
    scanf("%s", cipher);
    decrypt(cipher);

    return 0;
}
