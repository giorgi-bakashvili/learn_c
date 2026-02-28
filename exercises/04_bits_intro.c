#include <stdio.h>

void print_binary(unsigned char val) {
    for (int i = 7; i >= 0; i--) {
        if (val & (1 << i)) {
            putchar('1');
        } else {
            putchar('0');
        }
    }
}

int main(void) {
    printf("=== How numbers look in binary ===\n\n");

    printf("Decimal → Binary\n");
    printf("─────────────────\n");

    unsigned char numbers[] = {0, 1, 2, 3, 4, 5, 7, 8, 10, 15, 42, 100, 127, 255};
    int count = sizeof(numbers) / sizeof(numbers[0]);

    for (int i = 0; i < count; i++) {
        printf("  %3d   →  ", numbers[i]);
        print_binary(numbers[i]);
        printf("\n");
    }

    printf("\n=== Powers of 2 — only ONE bit is set ===\n\n");
    for (int i = 0; i < 8; i++) {
        unsigned char power = 1 << i;
        printf("  2^%d = %3d  →  ", i, power);
        print_binary(power);
        printf("\n");
    }

    printf("\n=== Breaking down the number 42 ===\n\n");
    printf("  42 in binary: ");
    print_binary(42);
    printf("\n");
    printf("  Position:     76543210\n\n");
    printf("  Which bits are ON?\n");
    for (int i = 7; i >= 0; i--) {
        if (42 & (1 << i)) {
            printf("    bit %d is ON  → value %d\n", i, 1 << i);
        }
    }
    printf("    Total: 32 + 8 + 2 = 42  ✓\n");

    return 0;
}
