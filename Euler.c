//Euler
#include <stdio.h>
#include <math.h>

// 1. Function to calculate (base^exp) % mod
long long modPow(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp = exp / 2;
    }
    return result;
}

// 2. Function to find Greatest Common Divisor
int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

// 3. Function to check if a number is prime
int isPrime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// 4. Function to calculate Euler's Totient Phi(n)
// This implements the unique prime factor logic from your JS code
int eulerTotient(int n) {
    int result = n;
    int temp = n;
   
    // Check for prime factor 2
    if (temp % 2 == 0) {
        result = result * (2 - 1) / 2;
        while (temp % 2 == 0) temp /= 2;
    }
   
    // Check for odd prime factors
    for (int i = 3; i * i <= temp; i += 2) {
        if (temp % i == 0) {
            result = result * (i - 1) / i;
            while (temp % i == 0) temp /= i;
        }
    }
   
    // If temp is still > 2, then temp is the last prime factor
    if (temp > 2) {
        result = result * (temp - 1) / temp;
    }
   
    return result;
}

// 5. Compute Euler's Theorem result: a^phi(n) % n
void computeEuler(int a, int n) {
    int phi = eulerTotient(n);
    long long result = modPow(a, phi, n);
   
    printf("a: %d\n", a);
    printf("n: %d\n", n);
    printf("phi(n): %d\n", phi);
    printf("Result (a^phi %% n): %lld\n", result);
}

int main() {
    int a, n;

    printf("Enter base (a): ");
    scanf("%d", &a);
    printf("Enter modulus (n): ");
    scanf("%d", &n);

    if (gcd(a, n) != 1) {
        printf("Note: a and n are not co-prime. Euler's theorem may not apply.\n");
    }

    computeEuler(a, n);

    return 0;
}
