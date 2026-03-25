/*
 * Lesson 13: void* and Generic Programming in C
 *
 * void* is C's "any pointer" — a raw memory address with no type info.
 * It's how C achieves generic programming: functions that work on any type.
 *
 * Think of it as: TypeScript's `unknown` (not `any`) — you MUST cast
 * before you can use it, because the compiler has no idea what's there.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ─── 1. void* basics: the typeless pointer ──────────────────────── */

static void basics(void) {
    printf("═══ 1. void* Basics ═══\n\n");

    int x = 42;
    float f = 3.14f;
    char c = 'A';

    /*
     * Any pointer can be assigned to void* WITHOUT a cast.
     * This is implicit in C (but NOT in C++ — C++ requires a cast).
     */
    void *vp;

    vp = &x;   /* int*   → void*  (implicit) */
    vp = &f;   /* float* → void*  (implicit) */
    vp = &c;   /* char*  → void*  (implicit) */

    /*
     * Going the other way: void* → typed pointer, also no cast needed in C.
     * But you SHOULD cast for clarity and C++ compatibility.
     */
    int *ip = &x;
    void *generic = ip;       /* int* → void* */
    int *back = generic;      /* void* → int* (implicit in C) */
    printf("Round-trip: %d\n", *back);

    /* void* has the same size as any other pointer — it's just an address */
    printf("sizeof(void*) = %zu\n", sizeof(void *));
    printf("sizeof(int*)  = %zu\n", sizeof(int *));
    printf("sizeof(char*) = %zu\n\n", sizeof(char *));

    /*
     * TWO things you CANNOT do with void*:
     *
     * 1. Dereference it:  *vp  ← ERROR: compiler doesn't know the size
     * 2. Arithmetic:      vp+1 ← ERROR: what's the stride?
     *
     * int* knows each element is 4 bytes, so p+1 means +4 bytes.
     * void* has no type info, so the compiler can't compute a stride.
     * (GCC extends this to treat void* arithmetic as stride-1, but
     *  that's non-standard and -pedantic will warn about it.)
     */
    (void)vp; /* suppress unused warning */
}

/* ─── 2. Generic swap: works on any type ─────────────────────────── */

/*
 * This is the classic generic C function. Compare to:
 *   TypeScript:  function swap<T>(a: T, b: T): void
 *   C:           void swap(void *a, void *b, size_t size)
 *
 * In C, you lose the type parameter <T> but gain a size parameter.
 * The caller must tell you HOW BIG each element is.
 */
static void generic_swap(void *a, void *b, size_t size) {
    unsigned char temp[size]; /* VLA as temp buffer (C99) */
    memcpy(temp, a, size);   /* temp = *a */
    memcpy(a, b, size);      /* *a = *b   */
    memcpy(b, temp, size);   /* *b = temp */
}

static void swap_demo(void) {
    printf("═══ 2. Generic Swap ═══\n\n");

    /* Swap ints */
    int a = 10, b = 20;
    printf("Before: a=%d, b=%d\n", a, b);
    generic_swap(&a, &b, sizeof(int));
    printf("After:  a=%d, b=%d\n\n", a, b);

    /* Same function swaps doubles */
    double x = 1.11, y = 9.99;
    printf("Before: x=%.2f, y=%.2f\n", x, y);
    generic_swap(&x, &y, sizeof(double));
    printf("After:  x=%.2f, y=%.2f\n\n", x, y);

    /* Same function swaps strings (well, char* pointers) */
    char *s1 = "hello";
    char *s2 = "world";
    printf("Before: s1=%s, s2=%s\n", s1, s2);
    generic_swap(&s1, &s2, sizeof(char *)); /* swap the pointers themselves */
    printf("After:  s1=%s, s2=%s\n\n", s1, s2);
}

/* ─── 3. memcpy, memset, memmove: the generic memory trio ────────── */

static void memory_functions(void) {
    printf("═══ 3. Generic Memory Functions ═══\n\n");

    /*
     * These are declared in <string.h> with void* signatures:
     *   void *memcpy(void *dest, const void *src, size_t n);
     *   void *memmove(void *dest, const void *src, size_t n);
     *   void *memset(void *s, int c, size_t n);
     *   int   memcmp(const void *s1, const void *s2, size_t n);
     *
     * They work on raw bytes — they don't care about the type.
     */

    /* memset: fill memory with a byte value */
    int arr[5];
    memset(arr, 0, sizeof(arr));  /* zero out entire array */
    printf("memset to 0: ");
    for (int i = 0; i < 5; i++) printf("%d ", arr[i]);
    printf("\n");

    /* memcpy: copy bytes (src and dest must NOT overlap) */
    int src[] = {10, 20, 30, 40, 50};
    int dst[5];
    memcpy(dst, src, sizeof(src));
    printf("memcpy'd:    ");
    for (int i = 0; i < 5; i++) printf("%d ", dst[i]);
    printf("\n");

    /* memmove: like memcpy but handles overlapping regions */
    int data[] = {1, 2, 3, 4, 5, 6};
    /* Shift elements right by 2: copy [1,2,3,4] to positions [2..5] */
    memmove(&data[2], &data[0], 4 * sizeof(int));
    printf("memmove'd:   ");
    for (int i = 0; i < 6; i++) printf("%d ", data[i]);
    printf("\n");

    /* memcmp: compare raw bytes (returns 0 if equal, like strcmp) */
    int a[] = {1, 2, 3};
    int b[] = {1, 2, 3};
    int c[] = {1, 2, 4};
    printf("memcmp a==b: %d\n", memcmp(a, b, sizeof(a)));
    printf("memcmp a==c: %d\n\n", memcmp(a, c, sizeof(a)));
}

/* ─── 4. qsort: the canonical generic C function ─────────────────── */

/*
 * qsort from <stdlib.h>:
 *   void qsort(void *base, size_t nmemb, size_t size,
 *              int (*compar)(const void *, const void *));
 *
 * It sorts ANY array. You provide:
 *   - base:  pointer to first element (as void*)
 *   - nmemb: number of elements
 *   - size:  size of each element in bytes
 *   - compar: a comparison function that receives void* pointers
 *
 * The comparator must cast void* back to the actual type.
 * This is where type safety breaks — if you cast wrong, it's UB.
 */

/* Comparator for ints: cast void* → const int*, then dereference */
static int compare_ints(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    /*
     * Don't just return ia - ib! That overflows for large values.
     * Example: INT_MAX - (-1) = INT_MIN (overflow, wrong sign).
     * Use the safe three-way comparison pattern instead:
     */
    return (ia > ib) - (ia < ib);
}

/* Comparator for strings (char* elements in an array of char*) */
static int compare_strings(const void *a, const void *b) {
    /*
     * qsort passes pointers TO the array elements.
     * Each element is a char*, so qsort passes char**.
     * We receive void* (which is actually char**), and must
     * dereference once to get the char* we want to compare.
     */
    const char *sa = *(const char **)a;
    const char *sb = *(const char **)b;
    return strcmp(sa, sb);
}

static void print_int_array(const int *arr, size_t len) {
    for (size_t i = 0; i < len; i++) printf("%d ", arr[i]);
    printf("\n");
}

static void qsort_demo(void) {
    printf("═══ 4. qsort — Generic Sorting ═══\n\n");

    /* Sort ints */
    int nums[] = {42, 7, 13, 99, 1, 28};
    size_t n = sizeof(nums) / sizeof(nums[0]);
    printf("Before: ");
    print_int_array(nums, n);
    qsort(nums, n, sizeof(int), compare_ints);
    printf("After:  ");
    print_int_array(nums, n);
    printf("\n");

    /* Sort strings */
    const char *langs[] = {"Rust", "C", "Go", "Zig", "Ada"};
    size_t nlangs = sizeof(langs) / sizeof(langs[0]);
    printf("Before: ");
    for (size_t i = 0; i < nlangs; i++) printf("%s ", langs[i]);
    printf("\n");

    qsort(langs, nlangs, sizeof(char *), compare_strings);

    printf("After:  ");
    for (size_t i = 0; i < nlangs; i++) printf("%s ", langs[i]);
    printf("\n\n");
}

/* ─── 5. Generic linear search ───────────────────────────────────── */

/*
 * A generic find function — like Array.prototype.find() in JS,
 * but operating on raw memory with void*.
 */
static void *generic_find(void *base, size_t nmemb, size_t size,
                          int (*match)(const void *elem, const void *target),
                          const void *target) {
    /*
     * We cast base to unsigned char* so we can do byte-level arithmetic.
     * Incrementing by `size` bytes moves to the next element.
     * We can't do arithmetic on void* (no stride), but unsigned char*
     * has a stride of 1 byte, so +size moves exactly size bytes.
     */
    unsigned char *p = base;
    for (size_t i = 0; i < nmemb; i++) {
        if (match(p + i * size, target))
            return p + i * size;  /* return void* to the matching element */
    }
    return NULL;
}

static int match_int(const void *elem, const void *target) {
    return *(const int *)elem == *(const int *)target;
}

static int match_str_prefix(const void *elem, const void *target) {
    const char *s = *(const char **)elem;
    const char *prefix = (const char *)target;
    return strncmp(s, prefix, strlen(prefix)) == 0;
}

static void search_demo(void) {
    printf("═══ 5. Generic Search ═══\n\n");

    int nums[] = {10, 20, 30, 40, 50};
    int key = 30;
    int *found = generic_find(nums, 5, sizeof(int), match_int, &key);
    if (found)
        printf("Found %d at index %ld\n", *found, found - nums);
    else
        printf("%d not found\n", key);

    key = 99;
    found = generic_find(nums, 5, sizeof(int), match_int, &key);
    if (found)
        printf("Found %d\n", *found);
    else
        printf("%d not found\n", key);

    /* Search strings by prefix */
    const char *words[] = {"apple", "banana", "cherry", "date"};
    const char *prefix = "ch";
    const char **match = generic_find(words, 4, sizeof(char *),
                                      match_str_prefix, prefix);
    if (match)
        printf("Prefix '%s' matched: %s\n\n", prefix, *match);
}

/* ─── 6. The void* callback pattern (user_data) ─────────────────── */

/*
 * In JS you'd pass a closure that captures variables from outer scope.
 * C has no closures. Instead, the convention is:
 *   - Accept a void *user_data parameter
 *   - Pass it through to the callback
 *   - The callback casts it back to whatever type it needs
 *
 * This is used everywhere: pthreads, GTK, libuv, SQLite callbacks...
 */

typedef void (*event_handler)(const char *event, void *user_data);

static void on_click(const char *event, void *user_data) {
    int *click_count = user_data;
    (*click_count)++;
    printf("  [%s] click count: %d\n", event, *click_count);
}

typedef struct {
    char name[32];
    int level;
} logger_t;

static void on_log(const char *event, void *user_data) {
    logger_t *log = user_data;
    printf("  [%s] logger '%s' (level %d)\n", event, log->name, log->level);
}

static void emit_event(const char *event, event_handler handler,
                       void *user_data) {
    handler(event, user_data);
}

static void callback_demo(void) {
    printf("═══ 6. Callback + void* user_data Pattern ═══\n\n");

    /* Same emit_event, different callbacks and data types */
    int clicks = 0;
    emit_event("button_click", on_click, &clicks);
    emit_event("button_click", on_click, &clicks);
    emit_event("button_click", on_click, &clicks);

    logger_t my_logger = {.name = "app", .level = 3};
    emit_event("startup", on_log, &my_logger);
    printf("\n");
}

/* ─── 7. Type safety: what void* gives up ────────────────────────── */

static void safety_demo(void) {
    printf("═══ 7. Type Safety Tradeoffs ═══\n\n");

    /*
     * void* throws away ALL type information at compile time.
     * The compiler cannot catch these bugs — they compile fine:
     */
    int x = 42;
    void *vp = &x;

    /* BUG: treating an int* as a double* — compiles, runs, garbage result */
    double *wrong = vp;
    printf("int 42 read as double: %f  (garbage!)\n", *wrong);

    /*
     * With typed pointers, the compiler WOULD warn:
     *   int *ip = &x;
     *   double *dp = ip;  // warning: incompatible pointer types
     *
     * But void* is an escape hatch — no warnings at all.
     *
     * Rule of thumb:
     *   - Use void* in "framework" code (generic containers, callbacks)
     *   - Use typed pointers everywhere else
     *   - Document what type void* actually points to
     *   - Cast as close to the API boundary as possible
     */

    /*
     * A common bug with qsort: wrong comparator for the type.
     * If you use compare_ints on an array of doubles, it compiles
     * fine but gives nonsense results. The compiler can't help.
     */
    printf("Lesson: void* is powerful but removes compile-time safety.\n");
    printf("Use it at API boundaries, cast early, document types.\n\n");
}

/* ─── main ───────────────────────────────────────────────────────── */

int main(void) {
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║  Lesson 13: void* and Generic Programming   ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");

    basics();
    swap_demo();
    memory_functions();
    qsort_demo();
    search_demo();
    callback_demo();
    safety_demo();

    return EXIT_SUCCESS;
}
