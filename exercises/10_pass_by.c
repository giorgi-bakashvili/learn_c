#include <stdio.h>

/*
 * Demo: Pass-by-value vs Pass-by-pointer
 *
 * In C, ALL function arguments are copies.
 * To modify the caller's variable, you must pass its address.
 */

/* ---- PART 1: Pass-by-value (the copy trap) ---- */

void try_to_double(int x) {
    printf("  inside try_to_double: x = %d (address: %p)\n", x, (void *)&x);
    x = x * 2;
    printf("  after doubling:       x = %d\n", x);
}

/* ---- PART 2: Pass-by-pointer (the fix) ---- */

void actually_double(int *p) {
    printf("  inside actually_double: p = %p, *p = %d\n", (void *)p, *p);
    *p = *p * 2;
    printf("  after doubling:         *p = %d\n", *p);
}

/* ---- PART 3: Swap — the classic pass-by-pointer exercise ---- */

void broken_swap(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
    /* swapped local copies — caller sees nothing */
}

void real_swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* ---- PART 4: Multiple return values via out-parameters ---- */

void divide(int dividend, int divisor, int *quotient, int *remainder) {
    *quotient = dividend / divisor;
    *remainder = dividend % divisor;
}

/* ---- PART 5: Arrays are already pointers (decay) ---- */

void zero_out(int *arr, int len) {
    for (int i = 0; i < len; i++) {
        arr[i] = 0;
    }
}

int main(void) {
    /* Part 1: pass-by-value */
    printf("=== Part 1: Pass-by-value ===\n");
    int num = 10;
    printf("before call: num = %d (address: %p)\n", num, (void *)&num);
    try_to_double(num);
    printf("after call:  num = %d  <-- unchanged!\n\n", num);

    /* Part 2: pass-by-pointer */
    printf("=== Part 2: Pass-by-pointer ===\n");
    num = 10;
    printf("before call: num = %d (address: %p)\n", num, (void *)&num);
    actually_double(&num);
    printf("after call:  num = %d  <-- changed!\n\n", num);

    /* Part 3: swap */
    printf("=== Part 3: Swap ===\n");
    int x = 42, y = 99;
    printf("before broken_swap: x=%d, y=%d\n", x, y);
    broken_swap(x, y);
    printf("after broken_swap:  x=%d, y=%d  <-- no change\n", x, y);

    printf("before real_swap:   x=%d, y=%d\n", x, y);
    real_swap(&x, &y);
    printf("after real_swap:    x=%d, y=%d  <-- swapped!\n\n", x, y);

    /* Part 4: multiple return values */
    printf("=== Part 4: Out-parameters ===\n");
    int q, r;
    divide(17, 5, &q, &r);
    printf("17 / 5 = %d remainder %d\n\n", q, r);

    /* Part 5: arrays decay to pointers */
    printf("=== Part 5: Array decay ===\n");
    int data[] = {10, 20, 30, 40, 50};
    int len = 5;
    printf("before zero_out: ");
    for (int i = 0; i < len; i++)
        printf("%d ", data[i]);
    printf("\n");

    zero_out(data, len);

    printf("after zero_out:  ");
    for (int i = 0; i < len; i++)
        printf("%d ", data[i]);
    printf("  <-- arrays are always 'by pointer'\n\n");

    /* Part 6: What the stack actually looks like */
    printf("=== Part 6: Stack addresses ===\n");
    int a = 1, b = 2, c = 3;
    printf("&a = %p\n", (void *)&a);
    printf("&b = %p\n", (void *)&b);
    printf("&c = %p\n", (void *)&c);
    printf("Notice: addresses decrease — stack grows downward on ARM64\n");

    return 0;
}
