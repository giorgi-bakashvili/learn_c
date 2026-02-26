#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void) {
    printf("=== Integer Type Sizes ===\n");
    printf("char:        %zu byte(s)\n", sizeof(char));
    printf("short:       %zu byte(s)\n", sizeof(short));
    printf("int:         %zu byte(s)\n", sizeof(int));
    printf("long:        %zu byte(s)\n", sizeof(long));
    printf("long long:   %zu byte(s)\n", sizeof(long long));

    printf("\n=== Floating-Point Type Sizes ===\n");
    printf("float:       %zu byte(s)\n", sizeof(float));
    printf("double:      %zu byte(s)\n", sizeof(double));
    printf("long double: %zu byte(s)\n", sizeof(long double));

    printf("\n=== Signed Integer Ranges (from limits.h) ===\n");
    printf("char:        %d to %d\n", CHAR_MIN, CHAR_MAX);
    printf("short:       %d to %d\n", SHRT_MIN, SHRT_MAX);
    printf("int:         %d to %d\n", INT_MIN, INT_MAX);
    printf("long:        %ld to %ld\n", LONG_MIN, LONG_MAX);
    printf("long long:   %lld to %lld\n", LLONG_MIN, LLONG_MAX);

    printf("\n=== Unsigned Integer Ranges ===\n");
    printf("unsigned char:      0 to %u\n", UCHAR_MAX);
    printf("unsigned short:     0 to %u\n", USHRT_MAX);
    printf("unsigned int:       0 to %u\n", UINT_MAX);
    printf("unsigned long:      0 to %lu\n", ULONG_MAX);
    printf("unsigned long long: 0 to %llu\n", ULLONG_MAX);

    printf("\n=== Floating-Point Precision (from float.h) ===\n");
    printf("float  decimal digits: %d\n", FLT_DIG);
    printf("double decimal digits: %d\n", DBL_DIG);

    printf("\n=== char is just a number ===\n");
    char letter = 'A';
    printf("'A' stored as: %d\n", letter);
    printf("'A' + 1 = %d, which is '%c'\n", letter + 1, letter + 1);
    printf("'0' stored as: %d\n", '0');
    printf("'9' - '0' = %d  (digit extraction trick)\n", '9' - '0');

    printf("\n=== The 0.1 + 0.2 problem ===\n");
    float f = 0.1f + 0.2f;
    double d = 0.1 + 0.2;
    printf("float:  0.1 + 0.2 = %.20f\n", f);
    printf("double: 0.1 + 0.2 = %.20lf\n", d);
    printf("Are they 0.3? float: %s, double: %s\n",
           (f == 0.3f) ? "yes" : "NO",
           (d == 0.3)  ? "yes" : "NO");

    printf("\n=== sizeof on variables (not just types) ===\n");
    int x = 42;
    double pi = 3.14159;
    printf("sizeof(x)  = %zu  (x is an int)\n", sizeof(x));
    printf("sizeof(pi) = %zu  (pi is a double)\n", sizeof(pi));

    return 0;
}
