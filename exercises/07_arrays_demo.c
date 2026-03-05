#include <stdio.h>

#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))

void print_array(int arr[], int size) {
    /*
     * sizeof(arr) here would give sizeof(int*) = 8, NOT the array size.
     * clang's -Wsizeof-array-argument catches this mistake at compile time.
     * That's why we always pass size as a separate parameter.
     */
    int *ptr = arr;
    printf("  sizeof(ptr) inside function: %zu (pointer, not array!)\n",
           sizeof(ptr));

    for (int i = 0; i < size; i++) {
        printf("  arr[%d] = %d  (at address %p)\n", i, arr[i], (void *)&arr[i]);
    }
}

void zero_fill(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = 0;
    }
}

int main(void) {
    printf("=== Array Basics ===\n");
    int nums[5] = {10, 20, 30, 40, 50};
    printf("sizeof(nums) = %zu bytes\n", sizeof(nums));
    printf("sizeof(nums[0]) = %zu bytes\n", sizeof(nums[0]));
    printf("ARRAY_LEN(nums) = %zu\n", ARRAY_LEN(nums));

    printf("\n=== Memory Layout (addresses are contiguous) ===\n");
    print_array(nums, 5);
    printf("  Notice: each address differs by %zu bytes (sizeof(int))\n",
           sizeof(int));

    printf("\n=== Partial Initialization ===\n");
    int partial[6] = {100, 200};
    for (size_t i = 0; i < ARRAY_LEN(partial); i++) {
        printf("  partial[%zu] = %d\n", i, partial[i]);
    }

    printf("\n=== Array Decay ===\n");
    int *p = nums;
    printf("  nums decays to: %p\n", (void *)nums);
    printf("  &nums[0]      : %p\n", (void *)&nums[0]);
    printf("  p              : %p\n", (void *)p);
    printf("  p[2] = %d, nums[2] = %d (same element)\n", p[2], nums[2]);

    printf("\n=== Modifying Through Function ===\n");
    printf("  Before zero_fill: ");
    for (size_t i = 0; i < ARRAY_LEN(nums); i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");
    zero_fill(nums, 5);
    printf("  After zero_fill:  ");
    for (size_t i = 0; i < ARRAY_LEN(nums); i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");

    printf("\n=== 2D Array ===\n");
    int matrix[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };
    printf("  matrix[0][0] at %p\n", (void *)&matrix[0][0]);
    printf("  matrix[0][2] at %p\n", (void *)&matrix[0][2]);
    printf("  matrix[1][0] at %p  (right after matrix[0][2])\n",
           (void *)&matrix[1][0]);
    printf("  Total size: %zu bytes (%zu ints)\n",
           sizeof(matrix), sizeof(matrix) / sizeof(int));

    return 0;
}
