//Shift Cipher
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char text[100];
    int key, i;

    // 1. Input Text
    printf("Enter text: ");
    scanf("%s", text);

    // 2. Input Key and Validate
    printf("Enter key (0-25): ");
    scanf("%d", &key);

    if (key < 0 || key > 25) {
        printf("Error: Key must be between 0 and 25\n");
        return 1;
    }

    // --- ENCRYPTION ---
    printf("\nEncrypted Text: ");
    for (i = 0; i < strlen(text); i++) {
        char p = toupper(text[i]); // Convert to Uppercase
        if (p >= 'A' && p <= 'Z') {
            int modValue = (p - 'A' + key) % 26;
            printf("%c", modValue + 'A');
        }
    }

    // --- DECRYPTION ---
    printf("\nDecrypted Text: ");
    for (i = 0; i < strlen(text); i++) {
        char c = toupper(text[i]); // Original JS logic decrypts from the Cipher
        if (c >= 'A' && c <= 'Z') {
            // Note: We add 26 before the modulo to handle negative results
            int modValue = (c - 'A' - key + 26) % 26;
            printf("%c", modValue + 'a'); // JS code uses + 97 (lowercase)
        }
    }

    printf("\n");
    return 0;
}
