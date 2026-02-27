#include <stdio.h>

/* --- #define constants (preprocessor text replacement) --- */
#define MAX_ITEMS 5
#define PI 3.14159265358979
#define GREETING "Hello from a macro!"

/* --- enum constants (true compile-time integer constants) --- */
enum { BUFFER_SIZE = 128, EXIT_OK = 0 };

/* --- Global variable (lives in data segment, entire program lifetime) --- */
int call_count = 0;

void demo_static(void) {
    static int persistent = 0;
    int ephemeral = 0;

    persistent++;
    ephemeral++;
    call_count++;

    printf("  persistent = %d, ephemeral = %d, global call_count = %d\n",
           persistent, ephemeral, call_count);
}

int main(void) {
    /* --- const variables (read-only, but NOT compile-time constants) --- */
    const int max_score = 100;
    const double gravity = 9.81;

    printf("=== #define constants ===\n");
    printf("MAX_ITEMS = %d\n", MAX_ITEMS);
    printf("PI = %.15f\n", PI);
    printf("GREETING = %s\n", GREETING);

    printf("\n=== enum constants ===\n");
    printf("BUFFER_SIZE = %d\n", BUFFER_SIZE);

    printf("\n=== const variables ===\n");
    printf("max_score = %d (address: %p)\n", max_score, (void *)&max_score);
    printf("gravity = %.2f\n", gravity);

    printf("\n=== Array sized with enum vs #define ===\n");
    int items[MAX_ITEMS];
    char buffer[BUFFER_SIZE];
    printf("items: %zu bytes (%d ints)\n", sizeof(items), MAX_ITEMS);
    printf("buffer: %zu bytes\n", sizeof(buffer));

    printf("\n=== static vs automatic vs global ===\n");
    for (int i = 0; i < 3; i++) {
        printf("Call %d:\n", i + 1);
        demo_static();
    }

    printf("\n=== Uninitialized variable (DANGER!) ===\n");
    /*
     * int garbage;
     * printf("%d\n", garbage);  // UB! Reads whatever bits were on the stack.
     *
     * With -Wuninitialized (part of -Wall), the compiler catches this
     * and -Werror turns it into a hard error. This is your safety net.
     * In JS, 'let x' gives you undefined. In C, you get random bits.
     */
    printf("(skipped — compiler correctly rejects reading uninitialized vars)\n");

    printf("\n=== Block scoping ===\n");
    {
        int scoped = 42;
        printf("Inside block: scoped = %d\n", scoped);
    }
    /* scoped is dead here — accessing it would be a compile error */

    return EXIT_OK;
}
