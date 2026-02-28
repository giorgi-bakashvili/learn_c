#include <stdio.h>

void print_binary(unsigned char val) {
    for (int i = 7; i >= 0; i--) {
        putchar((val & (1 << i)) ? '1' : '0');
    }
}

void show_operation(const char *name, unsigned char a, unsigned char b,
                    unsigned char result) {
    printf("  ");
    print_binary(a);
    printf("  (%3d)\n", a);
    printf("  ");
    print_binary(b);
    printf("  (%3d)  %s\n", b, name);
    printf("  ────────\n");
    printf("  ");
    print_binary(result);
    printf("  (%3d)  ← result\n\n", result);
}

int main(void) {
    unsigned char a = 42;   /* 00101010 */
    unsigned char b = 15;   /* 00001111 */

    /* ─── OPERATOR 1: AND (&) ─── */
    printf("══════════════════════════════════════\n");
    printf("  OPERATOR: AND  (&)\n");
    printf("  Rule: output bit is 1 ONLY if BOTH\n");
    printf("        input bits are 1\n");
    printf("══════════════════════════════════════\n\n");
    printf("  42 & 15:\n");
    show_operation("& (AND)", a, b, a & b);
    printf("  Think of it as: \"which bits do they\n");
    printf("  have in COMMON?\"\n\n");

    /* ─── OPERATOR 2: OR (|) ─── */
    printf("══════════════════════════════════════\n");
    printf("  OPERATOR: OR  (|)\n");
    printf("  Rule: output bit is 1 if EITHER\n");
    printf("        input bit is 1 (or both)\n");
    printf("══════════════════════════════════════\n\n");
    printf("  42 | 15:\n");
    show_operation("| (OR)", a, b, a | b);
    printf("  Think of it as: \"combine all the\n");
    printf("  bits that are ON in either number\"\n\n");

    /* ─── OPERATOR 3: XOR (^) ─── */
    printf("══════════════════════════════════════\n");
    printf("  OPERATOR: XOR  (^)\n");
    printf("  Rule: output bit is 1 if the bits\n");
    printf("        are DIFFERENT\n");
    printf("══════════════════════════════════════\n\n");
    printf("  42 ^ 15:\n");
    show_operation("^ (XOR)", a, b, a ^ b);
    printf("  Think of it as: \"where do they\n");
    printf("  DIFFER?\"\n\n");

    /* ─── OPERATOR 4: NOT (~) ─── */
    printf("══════════════════════════════════════\n");
    printf("  OPERATOR: NOT  (~)\n");
    printf("  Rule: flip EVERY bit\n");
    printf("        (0 becomes 1, 1 becomes 0)\n");
    printf("══════════════════════════════════════\n\n");
    printf("  ~42:\n");
    printf("  ");
    print_binary(a);
    printf("  (%3d)  original\n", a);
    printf("  ────────\n");
    unsigned char not_a = ~a;
    printf("  ");
    print_binary(not_a);
    printf("  (%3d)  ← flipped\n\n", not_a);
    printf("  Notice: 42 + 213 = 255 (all bits)\n\n");

    /* ─── OPERATOR 5: LEFT SHIFT (<<) ─── */
    printf("══════════════════════════════════════\n");
    printf("  OPERATOR: LEFT SHIFT  (<<)\n");
    printf("  Rule: slide all bits to the left,\n");
    printf("        filling right side with 0s\n");
    printf("  Effect: multiplies by 2 each shift\n");
    printf("══════════════════════════════════════\n\n");
    unsigned char val = 5;  /* 00000101 */
    printf("  Starting with 5:\n");
    printf("  ");
    print_binary(val);
    printf("  (%3d)  original\n\n", val);

    for (int i = 1; i <= 4; i++) {
        unsigned char shifted = val << i;
        printf("  5 << %d = ", i);
        print_binary(shifted);
        printf("  (%3d)", shifted);
        printf("  →  5 × %d = %d\n", 1 << i, 5 * (1 << i));
    }
    printf("\n  Each left shift = multiply by 2!\n\n");

    /* ─── OPERATOR 6: RIGHT SHIFT (>>) ─── */
    printf("══════════════════════════════════════\n");
    printf("  OPERATOR: RIGHT SHIFT  (>>)\n");
    printf("  Rule: slide all bits to the right,\n");
    printf("        filling left side with 0s\n");
    printf("  Effect: divides by 2 each shift\n");
    printf("          (integer division, truncates)\n");
    printf("══════════════════════════════════════\n\n");
    val = 200;  /* 11001000 */
    printf("  Starting with 200:\n");
    printf("  ");
    print_binary(val);
    printf("  (%3d)  original\n\n", val);

    for (int i = 1; i <= 4; i++) {
        unsigned char shifted = val >> i;
        printf("  200 >> %d = ", i);
        print_binary(shifted);
        printf("  (%3d)", shifted);
        printf("  →  200 / %d = %d\n", 1 << i, 200 / (1 << i));
    }
    printf("\n  Each right shift = divide by 2!\n");

    return 0;
}
