#include <stdio.h>
#include "06_calc.h"

int main(void) {
    printf("=== Calculator ===\n");
    printf("add(10, 3) = %d\n", add(10, 3));
    printf("subtract(10, 3) = %d\n", subtract(10, 3));
    printf("multiply(10, 3) = %d\n", multiply(10, 3));
    printf("divide(10, 3) = %d\n", divide(10, 3));
    printf("divide(10, 0) = %d\n", divide(10, 0));
    printf("power(2, 8) = %d\n", power(2, 8));
    printf("power(3, 0) = %d\n", power(3, 0));
    printf("power(2, -1) = %d\n", power(2, -1));

    /* abs_value(5); -- linker error: static functions are file-scoped, invisible outside 06_calc.c */

    return 0;
}