#include <stdio.h>
#include <limits.h>

int main(void) {
    unsigned char uc = 255;

    printf("uc: %u\n", uc);

    uc++;

    printf("uc: %u\n", uc);
    // will wrap around to 0

    signed char sc = 127;

    printf("sc: %d\n", sc);

    sc++;

    printf("sc: %d\n", sc);
    // undefined behavior

    unsigned int ui = 0;

    ui--;

    printf("ui: %u\n", ui);
    // will wrap around to UINT_MAX

    int mi = INT_MAX;

    mi++;

    printf("mi %d:\n", mi);
    // undefined behavior

    return 0;
}