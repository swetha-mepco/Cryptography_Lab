//Hill Cipher
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int N; // Matrix size (e.g., 2 for 2x2, 3 for 3x3)

// --- MATH UTILITIES ---

int mod26(int a) {
    return (a % 26 + 26) % 26;
}

int modInverse(int a) {
    a = mod26(a);
    for (int x = 1; x < 26; x++) {
        if (mod26(a * x) == 1) return x;
    }
    return -1; // Not invertible
}

// Function to get the minor of a matrix (used for determinant and cofactor)
void getMinor(int m[10][10], int temp[10][10], int p, int q, int n) {
    int i = 0, j = 0;
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (row != p && col != q) {
                temp[i][j++] = m[row][col];
                if (j == n - 1) { j = 0; i++; }
            }
        }
    }
}

int determinant(int m[10][10], int n) {
    if (n == 1) return m[0][0];
    if (n == 2) return (m[0][0] * m[1][1]) - (m[0][1] * m[1][0]);

    int det = 0, sign = 1;
    int temp[10][10];
    for (int f = 0; f < n; f++) {
        getMinor(m, temp, 0, f, n);
        det += sign * m[0][f] * determinant(temp, n - 1);
        sign = -sign;
    }
    return det;
}

// --- CORE HILL CIPHER LOGIC ---

void encrypt(char *text, int key[10][10]) {
    int len = strlen(text);
    printf("Ciphertext: ");
    for (int i = 0; i < len; i += N) {
        for (int row = 0; row < N; row++) {
            int sum = 0;
            for (int col = 0; col < N; col++) {
                sum += (text[i + col] - 'a') * key[col][row];
            }
            printf("%c", mod26(sum) + 'A');
        }
    }
    printf("\n");
}

void decrypt(char *text, int key[10][10]) {
    int det = mod26(determinant(key, N));
    int invDet = modInverse(det);
    if (invDet == -1) { printf("Matrix not invertible!\n"); return; }

    // Find Adjoint Matrix (Transpose of Cofactor Matrix)
    int adj[10][10], invKey[10][10], temp[10][10];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            getMinor(key, temp, i, j, N);
            int sign = ((i + j) % 2 == 0) ? 1 : -1;
            adj[j][i] = mod26(sign * determinant(temp, N - 1)); // Transposed here
            invKey[j][i] = mod26(adj[j][i] * invDet);
        }
    }

    printf("Decrypted: ");
    for (int i = 0; i < strlen(text); i += N) {
        for (int row = 0; row < N; row++) {
            int sum = 0;
            for (int col = 0; col < N; col++) {
                sum += (tolower(text[i + col]) - 'a') * invKey[col][row];
            }
            printf("%c", mod26(sum) + 'a');
        }
    }
    printf("\n");
}

int main() {
    char text[100], keyStr[100];
    int keyMatrix[10][10];

    printf("Enter matrix size (N): ");
    scanf("%d", &N);
    printf("Enter Key String (%d chars): ", N * N);
    scanf("%s", keyStr);

    // Fill Key Matrix
    for (int i = 0, k = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            keyMatrix[i][j] = tolower(keyStr[k++]) - 'a';

    printf("Enter Plaintext: ");
    scanf("%s", text);

    // Padding with 'x'
    while (strlen(text) % N != 0) strcat(text, "x");

    encrypt(text, keyMatrix);

    char cipher[100];
    printf("\nEnter Ciphertext to decrypt: ");
    scanf("%s", cipher);
    decrypt(cipher, keyMatrix);

    return 0;
}
