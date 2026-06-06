//Affine Cipher
#include <stdio.h>
#include <ctype.h>

// GCD function
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Modular inverse
int mod_inverse(int a, int m) {
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1)
            return x;
    }
    return -1;
}

void affine_cipher(char text[], int a, int b, char mode[]) {
    if (gcd(a, 26) != 1) {
        printf("Error: 'a' must be coprime with 26.\n");
        return;
    }

    printf("\n--- AFFINE %s ---\n", mode);

    int a_inv = mod_inverse(a, 26);

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char ch = toupper(text[i]);

            if (mode[0] == 'd') {  // decrypt
                int y = ch - 'A';
                int res = (a_inv * (y - b + 26)) % 26;
                printf("%c -> %c\n", ch, res + 'A');
            } else {  // encrypt
                int x = ch - 'A';
                int res = (a * x + b) % 26;
                printf("%c -> %c\n", ch, res + 'A');
            }
        }
    }
}

int main() {
    char text[1000], mode[10];
    int a, b;

    printf("Enter text: ");
    scanf("%s", text);

    printf("Enter key a (coprime with 26): ");
    scanf("%d", &a);

    printf("Enter key b: ");
    scanf("%d", &b);

    printf("Encrypt or Decrypt: ");
    scanf("%s", mode);

    affine_cipher(text, a, b, mode);

    return 0;
}

