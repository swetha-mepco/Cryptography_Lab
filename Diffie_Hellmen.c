//Diffie Hellmen
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// --- MATH FUNCTIONS ---

int isPrime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return 0;
    return 1;
}

// Checks if g is a primitive root of p
int isPrimitiveRoot(int g, int p) {
    // We use a dynamic array to track visited values
    int *visited = (int *)calloc(p, sizeof(int));
    long long value = 1;

    for (int i = 0; i < p - 1; i++) {
        value = (value * g) % p;
        if (visited[value]) {
            free(visited);
            return 0; // Repeated value means it's not primitive
        }
        visited[value] = 1;
    }

    // Verify all values from 1 to p-1 were hit
    for (int i = 1; i < p; i++) {
        if (!visited[i]) {
            free(visited);
            return 0;
        }
    }

    free(visited);
    return 1;
}

// Standard Modular Exponentiation (Square and Multiply)
int modPow(int base, int exp, int mod) {
    long long result = 1;
    long long b = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * b) % mod;
        b = (b * b) % mod;
        exp /= 2;
    }
    return (int)result;
}

// Simplified version of the Java "Detailed" function
int fastExpoDetailed(int base, int exp, int mod, char* label) {
    int result = modPow(base, exp, mod);
    printf("%s: %d\n", label, result);
    return result;
}

// --- MAIN LOGIC ---

int main() {
    int q, alpha, xa, xb;

    // Input collection
    printf("Enter q (prime number): ");
    scanf("%d", &q);
    printf("Enter alpha (primitive root of q): ");
    scanf("%d", &alpha);
    printf("Enter Xa (Private key A): ");
    scanf("%d", &xa);
    printf("Enter Xb (Private key B): ");
    scanf("%d", &xb);

    // Validations
    if (q < 1 || alpha < 1 || xa < 1 || xb < 1) {
        printf("ERROR: All inputs must be >= 1\n");
        return 1;
    }
    if (!isPrime(q)) {
        printf("ERROR: q is NOT prime\n");
        return 1;
    }
    if (!isPrimitiveRoot(alpha, q)) {
        printf("ERROR: alpha is NOT a primitive root of q\n");
        return 1;
    }
    if (xa >= q || xb >= q) {
        printf("ERROR: xa and xb must be less than q\n");
        return 1;
    }

    // Step 1: Calculate Public Keys
    printf("\n--- Generating Public Keys ---\n");
    int ya = fastExpoDetailed(alpha, xa, q, "Ya (Public A)");
    int yb = fastExpoDetailed(alpha, xb, q, "Yb (Public B)");

    // Step 2: Calculate Shared Secret Keys
    printf("\n--- Generating Shared Keys ---\n");
    int ka = fastExpoDetailed(yb, xa, q, "Ka (Shared Secret A)");
    int kb = fastExpoDetailed(ya, xb, q, "Kb (Shared Secret B)");

    // Comparison
    if (ka == kb)
        printf("\nSUCCESS: Shared Key MATCHES (%d)\n", ka);
    else
        printf("\nFAILURE: Keys DO NOT MATCH\n");

    return 0;
}
