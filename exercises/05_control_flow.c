/*
 * 05_control_flow.c — Control flow in C: if, switch, for, while, do-while, goto
 * Demonstrates C-specific behaviors and traps.
 */

#include <stdio.h>
#include <stdbool.h>

static void truthiness_demo(void) {
    printf("=== Truthiness in C ===\n");

    int zero = 0;
    int nonzero = 42;
    char *null_ptr = NULL;
    char *valid_ptr = "hello";

    printf("0 is %s\n", zero ? "truthy" : "falsy");
    printf("42 is %s\n", nonzero ? "truthy" : "falsy");
    printf("NULL is %s\n", null_ptr ? "truthy" : "falsy");
    printf("\"hello\" is %s\n", valid_ptr ? "truthy" : "falsy");
    printf("-1 is %s\n", -1 ? "truthy" : "falsy");
    printf("\n");
}

static void switch_fallthrough_demo(void) {
    printf("=== Switch Fallthrough ===\n");

    int x = 1;
    printf("x = %d, switch without break: ", x);
    switch (x) {
        case 1:
            printf("one ");
            /* intentional fallthrough to show the behavior */
            __attribute__((fallthrough));
        case 2:
            printf("two ");
            __attribute__((fallthrough));
        case 3:
            printf("three ");
            break;
        default:
            printf("other ");
            break;
    }
    printf("\n");

    printf("x = %d, switch WITH break: ", x);
    switch (x) {
        case 1:
            printf("one ");
            break;
        case 2:
            printf("two ");
            break;
        case 3:
            printf("three ");
            break;
        default:
            printf("other ");
            break;
    }
    printf("\n\n");
}

static void vowel_check(char c) {
    switch (c) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            printf("'%c' is a vowel\n", c);
            break;
        default:
            printf("'%c' is a consonant\n", c);
            break;
    }
}

static void loop_demo(void) {
    printf("=== Loop Variants ===\n");

    printf("for:      ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", i);
    }
    printf("\n");

    printf("while:    ");
    int j = 0;
    while (j < 5) {
        printf("%d ", j);
        j++;
    }
    printf("\n");

    printf("do-while: ");
    int k = 10;
    do {
        printf("%d ", k);
        k++;
    } while (k < 5);
    printf("(ran once even though 10 >= 5)\n");
    printf("\n");
}

static int fake_init_resource(int id, bool should_fail) {
    if (should_fail) {
        return -1;
    }
    printf("  Resource %d initialized\n", id);
    return 0;
}

static void fake_cleanup_resource(int id) {
    printf("  Resource %d cleaned up\n", id);
}

static int goto_cleanup_demo(bool fail_second) {
    printf("=== Goto Cleanup Pattern ===\n");

    if (fake_init_resource(1, false) != 0)
        goto fail_r1;

    if (fake_init_resource(2, fail_second) != 0)
        goto fail_r2;

    if (fake_init_resource(3, false) != 0)
        goto fail_r3;

    printf("  All resources ready — doing work...\n");

    fake_cleanup_resource(3);
fail_r3:
    fake_cleanup_resource(2);
fail_r2:
    fake_cleanup_resource(1);
fail_r1:
    return fail_second ? -1 : 0;
}

int main(void) {
    truthiness_demo();
    switch_fallthrough_demo();

    printf("=== Vowel Check (switch grouping) ===\n");
    vowel_check('a');
    vowel_check('z');
    printf("\n");

    loop_demo();

    printf("--- Success path ---\n");
    goto_cleanup_demo(false);
    printf("\n--- Failure path (resource 2 fails) ---\n");
    goto_cleanup_demo(true);
    printf("\n");

    return 0;
}
