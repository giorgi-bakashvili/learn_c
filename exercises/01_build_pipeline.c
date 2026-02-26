#include <stdio.h>
#define MAX_COUNT 3

int main(void) {
    for (int i = 0; i < MAX_COUNT; i++) {
        printf("Step %d of %d\n", i + 1, MAX_COUNT);
    }

    return 0;
}