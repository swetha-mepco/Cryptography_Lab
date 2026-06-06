//Primitive Root
#include <stdio.h>
#include <math.h>

// --- HELPER FUNCTIONS (from euler.js logic) ---

long long modPow(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

int gcd(int a, int b) {
    while (b) {
        a %= b;
        int temp = a;
        a = b;
        b = temp;
    }
    return a;
}

int isPrime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int eulerTotient(int n) {
    int res = n;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0) n /= i;
            res = res * (i - 1) / i;
        }
    }
    if (n > 1) res = res * (n - 1) / n;
    return res;
}

// --- PRIMITIVE ROOT LOGIC ---

// Returns the number of unique prime factors and fills the 'factors' array
int getPrimeFactors(int n, int factors[]) {
    int count = 0;
    int temp = n;
    if (temp % 2 == 0) {
        factors[count++] = 2;
        while (temp % 2 == 0) temp /= 2;
    }
    for (int i = 3; i * i <= temp; i += 2) {
        if (temp % i == 0) {
            factors[count++] = i;
            while (temp % i == 0) temp /= i;
        }
    }
    if (temp > 2) factors[count++] = temp;
    return count;
}

// Checks if g is a primitive root of n
int isPrimitiveRoot(int g, int n) {
    int phi = eulerTotient(n);
    int factors[32]; // Max unique prime factors for int is small
    int count = getPrimeFactors(phi, factors);

    for (int i = 0; i < count; i++) {
        int exp = phi / factors[i];
        long long val = modPow(g, exp, n);
       
        // Logical check: if g^(phi/factor) mod n == 1, it's not a primitive root
        if (val == 1) return 0;
    }
    return 1;
}

void findPrimitiveRoots(int n) {
    if (!isPrime(n)) {
        printf("Number %d is not prime. Primitive root logic for primes only here.\n", n);
        return;
    }

    printf("Primitive roots of %d: ", n);
    int found = 0;
    for (int g = 2; g < n; g++) {
        if (gcd(g, n) == 1) {
            if (isPrimitiveRoot(g, n)) {
                printf("%d ", g);
                found = 1;
            }
        }
    }
    if (!found) printf("None");
    printf("\n");
}

int main() {
    int n;
    printf("Enter a prime number n: ");
    scanf("%d", &n);

    findPrimitiveRoots(n);

    return 0;
}
