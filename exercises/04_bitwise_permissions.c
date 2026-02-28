#include <stdio.h>

enum {
    READ = 4,
    WRITE = 2,
    EXECUTE = 1
};

void print_permissions(unsigned char perms) {
    perms & READ ? putchar('r') : putchar('-');
    perms & WRITE ? putchar('w') : putchar('-');
    perms & EXECUTE ? putchar('x') : putchar('-');
    putchar('\n');
}

unsigned char set_permission(unsigned char perms, unsigned char flag) {
    perms = perms | flag;

    return perms;
}

unsigned char clear_permission(unsigned char perms, unsigned char flag) {
    perms = perms & ~flag;

    return perms;
}

int has_permission(unsigned char perms, unsigned char flag) {
    return perms & flag ? 1 : 0;
}

void print_binary(unsigned char val) {
    for (int i = 7; i >= 0; i--) {
        putchar((val & (1 << i)) ? '1' : '0');
    }

    putchar('\n');
}

int main(void) {
    unsigned char perms = 0;

    print_permissions(perms);

    perms = set_permission(perms, READ);

    print_permissions(perms);

    perms = set_permission(perms, WRITE);

    print_permissions(perms);

    perms = set_permission(perms, EXECUTE);

    print_permissions(perms);

    perms = clear_permission(perms, WRITE);

    print_permissions(perms);

    printf("Combined Permission: %d\n", READ | EXECUTE);

    print_binary(perms);
    
    return 0;
}