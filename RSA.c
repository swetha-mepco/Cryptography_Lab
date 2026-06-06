//RSA
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// --- MATH FUNCTIONS ---

int isPrime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return 0;
    return 1;
}

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int findE(int phi) {
    for (int i = 2; i < phi; i++)
        if (gcd(i, phi) == 1) return i;
    return 2;
}

int modInverse(int e, int phi) {
    for (int i = 1; i < phi; i++) {
        // Use long long to prevent overflow during (e * i) calculation
        if (((long long)e * i) % phi == 1) return i;
    }
    return 1;
}

int modPow(int base, int exp, int mod) {
    long long res = 1;
    long long b = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * b) % mod;
        b = (b * b) % mod;
        exp /= 2;
    }
    return (int)res;
}

// --- MAIN LOGIC ---

int main() {
    int p, q, action;
    char msg[100];

    printf("Enter p and q (Prime numbers): ");
    scanf("%d %d", &p, &q);

    if (!isPrime(p) || !isPrime(q)) {
        printf("ERROR: p or q is NOT prime.\n");
        return 1;
    }

    int n = p * q;
    int phi = (p - 1) * (q - 1);
    int e = findE(phi);
    int d = modInverse(e, phi);

    printf("Public Key (e, n): (%d, %d)\n", e, n);
    printf("Private Key (d, n): (%d, %d)\n", d, n);

    printf("\nEnter message: ");
    scanf("%s", msg);
    printf("Choose Action (1 for Encrypt, 2 for Decrypt): ");
    scanf("%d", &action);

    // Check if input message is purely numeric
    int isNumber = 1;
    for (int i = 0; i < strlen(msg); i++) {
        if (!isdigit(msg[i])) {
            isNumber = 0;
            break;
        }
    }

    if (isNumber) {
        int m;
        sscanf(msg, "%d", &m);
        if (action == 1)
            printf("Ciphertext (Number): %d\n", modPow(m, e, n));
        else
            printf("Plaintext (Number): %d\n", modPow(m, d, n));
    }
    else {
        printf("Result: ");
        for (int i = 0; i < strlen(msg); i++) {
            int m = (int)msg[i];
            int result;
            if (action == 1)
                result = modPow(m, e, n);
            else
                result = modPow(m, d, n);
           
            printf("%c", (char)result);
        }
        printf("\n");
    }

    return 0;
}
