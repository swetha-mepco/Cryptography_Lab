//Vigenere Cipher
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void vigenere_cipher(char text[], char key[], char mode[]) {
    char result[1000];
    int text_len = strlen(text);
    int key_len = strlen(key);

    int j = 0;

    for (int i = 0; i < text_len; i++) {
        if (isalpha(text[i])) {
            char t = toupper(text[i]);
            char k = toupper(key[j % key_len]);

            int char_val = t - 'A';
            int key_val = k - 'A';
            int new_val;

            if (strcmp(mode, "decrypt") == 0)
                new_val = (char_val - key_val + 26) % 26;
            else
                new_val = (char_val + key_val) % 26;

            result[j] = new_val + 'A';
            j++;
        }
    }
    result[j] = '\0';

    printf("\n--- Vigenere Cipher Result (%s) ---\n", mode);
    printf("%s\n", result);
}

int main() {
    char text[1000], key[100], mode[10];

    printf("Enter text: ");
    scanf("%s", text);

    printf("Enter key: ");
    scanf("%s", key);

    printf("Type 'encrypt' or 'decrypt': ");
    scanf("%s", mode);

    vigenere_cipher(text, key, mode);

    return 0;
}
