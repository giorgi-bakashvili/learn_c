#include <stdio.h>
#include <string.h>

int main(void) {
    printf("=== String Memory Layout ===\n");
    char greeting[] = "Hello";
    printf("greeting = \"%s\"\n", greeting);
    printf("sizeof(greeting) = %zu (includes \\0)\n", sizeof(greeting));
    printf("strlen(greeting)  = %zu (excludes \\0)\n", strlen(greeting));

    printf("\nByte-by-byte:\n");
    for (size_t i = 0; i <= strlen(greeting); i++) {
        printf("  [%zu] = '%c' (%d)\n", i, greeting[i] ? greeting[i] : '0',
               greeting[i]);
    }

    printf("\n=== Array vs Pointer ===\n");
    char arr[] = "Mutable";
    const char *ptr = "Immutable";
    arr[0] = 'm';
    printf("arr after modification: \"%s\"\n", arr);
    printf("ptr points to: \"%s\"\n", ptr);
    printf("sizeof(arr) = %zu (array size)\n", sizeof(arr));
    printf("sizeof(ptr) = %zu (pointer size)\n", sizeof(ptr));

    printf("\n=== String Comparison ===\n");
    const char *a = "apple";
    const char *b = "apple";
    const char *c = "banana";
    printf("strcmp(\"%s\", \"%s\") = %d (0 means equal)\n", a, b, strcmp(a, b));
    printf("strcmp(\"%s\", \"%s\") = %d (negative: a < b)\n", a, c, strcmp(a, c));
    printf("a == b pointer comparison: %s (unreliable!)\n",
           a == b ? "true" : "false");

    printf("\n=== Safe String Building ===\n");
    char buf[20];
    const char *name = "World";
    int written = snprintf(buf, sizeof(buf), "Hello, %s!", name);
    printf("buf = \"%s\"\n", buf);
    printf("snprintf returned %d (chars that would be written)\n", written);

    char tiny[8];
    written = snprintf(tiny, sizeof(tiny), "Hello, %s!", name);
    printf("tiny = \"%s\" (truncated, buffer too small)\n", tiny);
    printf("snprintf returned %d (wanted %d, had %zu)\n",
           written, written, sizeof(tiny));

    printf("\n=== Pointer Iteration ===\n");
    const char *msg = "C strings";
    printf("Chars: ");
    for (const char *p = msg; *p; p++) {
        printf("'%c' ", *p);
    }
    printf("\n");

    printf("\n=== Useful String Functions ===\n");
    const char *haystack = "Learn C programming";
    const char *found = strstr(haystack, "C pro");
    if (found) {
        printf("strstr found \"C pro\" at position %ld\n", found - haystack);
    }

    const char *ch = strchr(haystack, 'C');
    if (ch) {
        printf("strchr found 'C' at position %ld\n", ch - haystack);
        printf("From that point: \"%s\"\n", ch);
    }

    return 0;
}
