#include <stdio.h>
#define SQUARE(x) ((x) * (x))

enum {
    NUM_ROWS = 4, 
    NUM_COLS = 10, 
    LABEL_LEN = 10
};

const double EGGS = 1.25;

int next_id(void) {
    static int id = 0;

    return ++id;
}


int main(void) {
    printf("Squeare %d:\n", SQUARE(3 + 1));

    int arr[NUM_ROWS];
    printf("Array of %d rows created (%zu bytes)\n", NUM_ROWS, sizeof(arr));

    printf("ID: %d\n", next_id());
    printf("ID: %d\n", next_id());
    printf("ID: %d\n", next_id());

    {
        unsigned char block = 99;

        printf("Block var: %d\n", block);
        // unaccessible outside the block
    }

    return 0;
}