#include <stdio.h>
#include <stddef.h>

int main(void) {
    /* === Basic pointer declaration and dereferencing === */
    int x = 42;
    int *p = &x;

    printf("=== Basics ===\n");
    printf("x         = %d\n", x);
    printf("&x        = %p\n", (void *)&x);
    printf("p         = %p\n", (void *)p);
    printf("*p        = %d\n", *p);

    /* Modify x through the pointer */
    *p = 99;
    printf("After *p = 99:\n");
    printf("x         = %d\n", x);
    printf("*p        = %d\n", *p);

    /* === Pointer sizes are always the same === */
    printf("\n=== Pointer Sizes ===\n");
    printf("sizeof(int *)    = %zu\n", sizeof(int *));
    printf("sizeof(char *)   = %zu\n", sizeof(char *));
    printf("sizeof(double *) = %zu\n", sizeof(double *));
    printf("sizeof(void *)   = %zu\n", sizeof(void *));

    /* === Pointer arithmetic with arrays === */
    int arr[] = {10, 20, 30, 40, 50};
    int *ap = arr;

    printf("\n=== Pointer Arithmetic ===\n");
    printf("arr lives at: %p\n", (void *)arr);

    for (int i = 0; i < 5; i++) {
        printf("ap + %d = %p  ->  value = %d  (offset = %td bytes)\n",
               i,
               (void *)(ap + i),
               *(ap + i),
               (char *)(ap + i) - (char *)ap);
    }

    /* Incrementing a pointer */
    printf("\n=== Pointer Increment ===\n");
    int *walker = arr;
    printf("*walker = %d  (at %p)\n", *walker, (void *)walker);
    walker++;
    printf("After walker++:\n");
    printf("*walker = %d  (at %p)\n", *walker, (void *)walker);
    walker += 2;
    printf("After walker += 2:\n");
    printf("*walker = %d  (at %p)\n", *walker, (void *)walker);

    /* Pointer subtraction */
    printf("\n=== Pointer Subtraction ===\n");
    int *start = &arr[0];
    int *end = &arr[4];
    ptrdiff_t diff = end - start;
    printf("end - start = %td elements\n", diff);
    printf("Byte distance = %td\n", (char *)end - (char *)start);

    /* === NULL pointer === */
    printf("\n=== NULL ===\n");
    int *np = NULL;
    printf("np = %p\n", (void *)np);
    if (np != NULL) {
        printf("*np = %d\n", *np);
    } else {
        printf("np is NULL — not safe to dereference\n");
    }

    /* === Array indexing is pointer arithmetic === */
    printf("\n=== arr[i] == *(arr + i) ===\n");
    for (int i = 0; i < 5; i++) {
        printf("arr[%d] = %d  |  *(arr + %d) = %d  |  %d[arr] = %d\n",
               i, arr[i],
               i, *(arr + i),
               i, i[arr]);
    }

    return 0;
}
