//Fermat
#include <stdio.h>

// Modular exponentiation
long long modular_exponentiation(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;

    while (exp > 0) {
        if (exp % 2 == 1)
            res = (res * base) % mod;

        base = (base * base) % mod;
        exp = exp / 2;
    }
    return res;
}

void fermat_test() {
    long long p, a;

    printf("--- FERMAT'S PRIMALITY TEST ---\n");
    printf("Enter p (number to test): ");
    scanf("%lld", &p);

    printf("Enter a (base): ");
    scanf("%lld", &a);

    if (p <= 1) {
        printf("%lld is not prime.\n", p);
        return;
    }

    long long result = modular_exponentiation(a, p - 1, p);

    printf("\nCalculation: %lld^(%lld-1) mod %lld\n", a, p, p);
    printf("Result: %lld\n", result);

    if (result == 1)
        printf("Conclusion: %lld is PROBABLY PRIME.\n", p);
    else
        printf("Conclusion: %lld is COMPOSITE.\n", p);
}

int main() {
    fermat_test();
    return 0;
}
