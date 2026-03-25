/*
 * Lesson 12: Pointer to Pointer & Arrays of Pointers
 *
 * A pointer holds an address. A pointer-to-pointer holds the address
 * of a pointer. This sounds abstract, but it solves two concrete problems:
 *   1. Letting a function change which address a caller's pointer holds
 *   2. Working with arrays of pointers (like an array of strings)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ──────────────────────────────────────────────
 * PART 1: What is a pointer to pointer?
 * ────────────────────────────────────────────── */

static void basics(void) {
    printf("=== PART 1: Pointer to Pointer Basics ===\n\n");

    int x = 42;
    int *p = &x;     /* p holds the address of x */
    int **pp = &p;   /* pp holds the address of p */

    /*
     * Memory picture:
     *
     *   pp ──→ p ──→ x
     *   [&p]   [&x]  [42]
     *
     *   pp  is an int** — "pointer to pointer to int"
     *   *pp is an int*  — dereferencing once gives us p
     *  **pp is an int   — dereferencing twice gives us x
     */

    printf("  x   = %d\n", x);
    printf("  *p  = %d   (one dereference: p → x)\n", *p);
    printf("  **pp = %d   (two dereferences: pp → p → x)\n", **pp);
    printf("\n");

    printf("  Addresses:\n");
    printf("    &x  = %p  (where x lives)\n", (void *)&x);
    printf("     p  = %p  (p stores &x — same address)\n", (void *)p);
    printf("    &p  = %p  (where p itself lives)\n", (void *)&p);
    printf("    pp  = %p  (pp stores &p — same address)\n", (void *)pp);
    printf("    &pp = %p  (where pp itself lives)\n", (void *)&pp);
    printf("\n");

    /* Modifying through double dereference */
    **pp = 99;
    printf("  After **pp = 99: x = %d\n\n", x);
}


/* ──────────────────────────────────────────────
 * PART 2: Why you need int** — modifying a caller's pointer
 * ────────────────────────────────────────────── */

/*
 * Remember: C is pass-by-value. If a function receives `int *p`,
 * it gets a COPY of the pointer. It can modify *p (the data),
 * but it can't change where p points for the caller.
 *
 * To change the caller's pointer itself, you need a pointer TO
 * that pointer: int **pp.
 *
 * JS parallel: this is like the difference between mutating an
 * object's property (obj.x = 5) vs reassigning the variable
 * (obj = newObj). In JS, you can't reassign a caller's variable
 * from inside a function either.
 */

/* BROKEN version: tries to malloc for the caller */
static void alloc_broken(int *out) {
    out = malloc(sizeof(*out));  /* changes the LOCAL copy of out */
    if (out != NULL) {
        *out = 42;
    }
    /* when this function returns, caller's pointer is unchanged */
    /* AND the malloc'd memory is leaked — nobody has the address */
}

/* FIXED version: takes int** so it can modify the caller's pointer */
static void alloc_fixed(int **out) {
    *out = malloc(sizeof(**out));  /* modifies caller's pointer */
    if (*out != NULL) {
        **out = 42;
    }
}

static void demo_modify_caller_pointer(void) {
    printf("=== PART 2: Modifying a Caller's Pointer ===\n\n");

    /* Broken version */
    int *p1 = NULL;
    alloc_broken(p1);
    printf("  After alloc_broken: p1 = %p  (still NULL — broken!)\n",
           (void *)p1);

    /* Fixed version */
    int *p2 = NULL;
    alloc_fixed(&p2);
    printf("  After alloc_fixed:  p2 = %p  (*p2 = %d)\n",
           (void *)p2, *p2);
    printf("\n");

    /*
     * The pattern:
     *   - To modify an int from a function:    pass int*  (pointer to int)
     *   - To modify an int* from a function:   pass int** (pointer to pointer)
     *   - To modify an int** from a function:  pass int*** (and so on...)
     *
     * Rule: to modify a T from a function, pass a T*.
     * If T is already int*, then T* is int**.
     */

    printf("  Pattern: to modify a T, pass T*.\n");
    printf("  int  → pass int*\n");
    printf("  int* → pass int**\n\n");

    free(p2);
}


/* ──────────────────────────────────────────────
 * PART 3: Arrays of pointers
 * ────────────────────────────────────────────── */

static void demo_array_of_pointers(void) {
    printf("=== PART 3: Arrays of Pointers ===\n\n");

    /*
     * An array of pointers: each element is a pointer to something.
     * Most common use: array of strings (char *words[]).
     */

    const char *fruits[] = {"apple", "banana", "cherry", "date"};
    size_t count = sizeof(fruits) / sizeof(fruits[0]);

    /*
     * Memory layout:
     *
     *   fruits (on stack):
     *   ┌──────────┬──────────┬──────────┬──────────┐
     *   │ fruits[0]│ fruits[1]│ fruits[2]│ fruits[3]│
     *   │  char*   │  char*   │  char*   │  char*   │
     *   └──┬───────┴──┬───────┴──┬───────┴──┬───────┘
     *      │          │          │          │
     *      ↓          ↓          ↓          ↓
     *   "apple\0" "banana\0" "cherry\0" "date\0"
     *   (all in .rodata — read-only string literals)
     *
     * Each element is 8 bytes (a pointer), so the array is 4 × 8 = 32 bytes.
     * The strings themselves are NOT in the array — only pointers to them.
     *
     * This is fundamentally different from a 2D char array:
     *   char fruits2[4][10] = {"apple", "banana", "cherry", "date"};
     * which stores all characters contiguously (4 × 10 = 40 bytes).
     */

    printf("  Array of string pointers:\n");
    printf("  sizeof(fruits) = %zu bytes (%zu pointers × %zu bytes each)\n",
           sizeof(fruits), count, sizeof(fruits[0]));
    for (size_t i = 0; i < count; i++) {
        printf("    fruits[%zu] = %p → \"%s\"\n",
               i, (void *)fruits[i], fruits[i]);
    }
    printf("\n");

    /* Array of int pointers — each pointing to a heap-allocated int */
    int *numbers[3];
    for (int i = 0; i < 3; i++) {
        numbers[i] = malloc(sizeof(*numbers[i]));
        if (numbers[i] == NULL) {
            fprintf(stderr, "malloc failed\n");
            for (int j = 0; j < i; j++) free(numbers[j]);
            return;
        }
        *numbers[i] = (i + 1) * 100;
    }

    printf("  Array of int pointers (heap-allocated values):\n");
    for (int i = 0; i < 3; i++) {
        printf("    numbers[%d] = %p → %d\n",
               i, (void *)numbers[i], *numbers[i]);
    }
    printf("\n");

    for (int i = 0; i < 3; i++) {
        free(numbers[i]);
    }
}


/* ──────────────────────────────────────────────
 * PART 4: Heap-allocated 2D array (jagged array)
 * ────────────────────────────────────────────── */

static void demo_jagged_array(void) {
    printf("=== PART 4: Heap-Allocated 2D Array (Jagged) ===\n\n");

    /*
     * A "2D array" on the heap is really an array of pointers,
     * where each pointer points to a separately allocated row.
     *
     * This is called a "jagged array" because rows can have
     * different lengths (unlike a true 2D array on the stack).
     *
     * JS equivalent: [[1,2,3], [4,5], [6,7,8,9]] — each sub-array
     * is a separate object on the heap. Exact same idea.
     */

    int rows = 3;
    int cols[] = {3, 2, 4};  /* different length per row */

    /* Step 1: allocate the array of row pointers */
    int **matrix = malloc((size_t)rows * sizeof(*matrix));
    if (matrix == NULL) {
        fprintf(stderr, "malloc failed\n");
        return;
    }

    /* Step 2: allocate each row */
    for (int r = 0; r < rows; r++) {
        matrix[r] = malloc((size_t)cols[r] * sizeof(*matrix[r]));
        if (matrix[r] == NULL) {
            fprintf(stderr, "malloc failed\n");
            for (int j = 0; j < r; j++) free(matrix[j]);
            free(matrix);
            return;
        }
        for (int c = 0; c < cols[r]; c++) {
            matrix[r][c] = r * 10 + c;
        }
    }

    /*
     * Memory layout:
     *
     *   matrix (int**) ──→ ┌────────────┐
     *                      │ matrix[0]  │──→ [0, 1, 2]       (3 ints)
     *                      │ matrix[1]  │──→ [10, 11]        (2 ints)
     *                      │ matrix[2]  │──→ [20, 21, 22, 23](4 ints)
     *                      └────────────┘
     *
     *   4 separate heap allocations total:
     *     1 for the pointer array, 3 for the rows.
     *   Each must be freed individually.
     */

    printf("  Jagged array (different row lengths):\n");
    for (int r = 0; r < rows; r++) {
        printf("    row %d (%p): [", r, (void *)matrix[r]);
        for (int c = 0; c < cols[r]; c++) {
            if (c > 0) printf(", ");
            printf("%d", matrix[r][c]);
        }
        printf("]\n");
    }
    printf("\n");

    /* Free in reverse order: rows first, then the pointer array */
    for (int r = 0; r < rows; r++) {
        free(matrix[r]);
    }
    free(matrix);

    printf("  Freed: 4 allocations (3 rows + 1 pointer array)\n");
    printf("  Rule: free in reverse order of allocation.\n\n");
}


/* ──────────────────────────────────────────────
 * PART 5: char** — the string array pattern
 * ────────────────────────────────────────────── */

static char **split_words(const char *sentence, size_t *out_count) {
    /* Count words first (simplified: split on spaces) */
    size_t word_count = 0;
    const char *s = sentence;
    while (*s) {
        while (*s == ' ') s++;
        if (*s) {
            word_count++;
            while (*s && *s != ' ') s++;
        }
    }

    /* Allocate array of string pointers */
    char **words = malloc(word_count * sizeof(*words));
    if (words == NULL) return NULL;

    /* Extract each word */
    s = sentence;
    size_t idx = 0;
    while (*s) {
        while (*s == ' ') s++;
        if (*s) {
            const char *start = s;
            while (*s && *s != ' ') s++;
            size_t len = (size_t)(s - start);

            words[idx] = malloc(len + 1);  /* +1 for null terminator */
            if (words[idx] == NULL) {
                for (size_t j = 0; j < idx; j++) free(words[j]);
                free(words);
                return NULL;
            }
            memcpy(words[idx], start, len);
            words[idx][len] = '\0';
            idx++;
        }
    }

    *out_count = word_count;
    return words;
}

static void demo_string_array(void) {
    printf("=== PART 5: char** — Dynamic String Arrays ===\n\n");

    const char *sentence = "hello world from C pointers";
    size_t count = 0;
    char **words = split_words(sentence, &count);
    if (words == NULL) {
        fprintf(stderr, "split_words failed\n");
        return;
    }

    /*
     * words is a char** — pointer to an array of char* pointers.
     * Each char* points to a heap-allocated copy of a word.
     *
     *   words (char**) ──→ ┌──────────┐
     *                      │ words[0] │──→ "hello\0"
     *                      │ words[1] │──→ "world\0"
     *                      │ words[2] │──→ "from\0"
     *                      │ words[3] │──→ "C\0"
     *                      │ words[4] │──→ "pointers\0"
     *                      └──────────┘
     *
     * This is EXACTLY the same structure as argv in main(int argc, char **argv).
     * argc tells you how many strings, argv is the array of string pointers.
     */

    printf("  Split \"%s\":\n", sentence);
    for (size_t i = 0; i < count; i++) {
        printf("    words[%zu] = \"%s\"\n", i, words[i]);
    }
    printf("\n");

    printf("  This char** pattern is how argc/argv works.\n");
    printf("  argc = %zu, argv = the array of string pointers.\n\n", count);

    /* Free: each string, then the array of pointers */
    for (size_t i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}


/* ──────────────────────────────────────────────
 * PART 6: Common patterns and gotchas
 * ────────────────────────────────────────────── */

static void demo_patterns(void) {
    printf("=== PART 6: Patterns & Gotchas ===\n\n");

    /* Pattern 1: Function that creates and returns a heap object */
    printf("  Pattern 1: \"Constructor\" via return\n");
    printf("    int *create_array(size_t n) {\n");
    printf("        return calloc(n, sizeof(int));\n");
    printf("    }\n");
    printf("    Caller gets ownership. Caller must free.\n\n");

    /* Pattern 2: Function that creates via out-parameter */
    printf("  Pattern 2: \"Constructor\" via out-parameter (int**)\n");
    printf("    int create_array(size_t n, int **out) {\n");
    printf("        *out = calloc(n, sizeof(int));\n");
    printf("        return (*out == NULL) ? -1 : 0;\n");
    printf("    }\n");
    printf("    Return value is error code. Common in C APIs.\n\n");

    /* Pattern 3: Function that frees and NULLs */
    printf("  Pattern 3: \"Destructor\" that NULLs the pointer\n");
    printf("    void destroy(int **pp) {\n");
    printf("        free(*pp);\n");
    printf("        *pp = NULL;\n");
    printf("    }\n");
    printf("    Takes int** so it can set caller's pointer to NULL.\n");
    printf("    Prevents use-after-free and double-free.\n\n");

    /* Gotcha: ** in declarations */
    printf("  Gotcha: declaring multiple pointers\n");
    printf("    int **a, *b, c;\n");
    printf("    a is int**, b is int*, c is plain int.\n");
    printf("    The * binds to the variable, not the type.\n");
    printf("    (Same trap as int *a, b — b is NOT a pointer)\n\n");
}


int main(void) {
    basics();
    printf("\n");
    demo_modify_caller_pointer();
    printf("\n");
    demo_array_of_pointers();
    printf("\n");
    demo_jagged_array();
    printf("\n");
    demo_string_array();
    printf("\n");
    demo_patterns();

    printf("=== Lesson complete ===\n");
    printf("Key takeaway: int** means \"I can change which address\n");
    printf("an int* points to.\" It's the tool for functions that\n");
    printf("allocate memory on behalf of the caller.\n");
    return EXIT_SUCCESS;
}
