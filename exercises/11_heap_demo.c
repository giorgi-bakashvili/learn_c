/*
 * Lesson 11: Heap Allocation — malloc, calloc, realloc, free
 *
 * In C, memory comes from two regions:
 *   - The STACK: automatic, scoped, fast, limited size (~8 MB default)
 *   - The HEAP:  manual, global lifetime, slower, virtually unlimited
 *
 * JS equivalent: every object/array you create in JS lives on V8's heap.
 * The difference is V8 calls free() for you (garbage collection). In C, YOU do it.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ──────────────────────────────────────────────
 * PART 1: Stack vs Heap — where things live
 * ────────────────────────────────────────────── */

static void stack_vs_heap(void) {
    printf("=== PART 1: Stack vs Heap ===\n\n");

    /* Stack allocation: variable lives inside the function's stack frame.
     * Created when function is called, destroyed when it returns.
     * Size must be known at compile time (in standard C89). */
    int stack_var = 42;

    /* Heap allocation: you ask the OS for memory at runtime.
     * Lives until YOU free it. Size can be determined at runtime. */
    int *heap_var = malloc(sizeof(*heap_var));
    if (heap_var == NULL) {
        fprintf(stderr, "malloc failed\n");
        return;
    }
    *heap_var = 42;

    printf("  stack_var lives at: %p  (high address, stack grows down)\n",
           (void *)&stack_var);
    printf("  heap_var  lives at: %p  (low address, heap grows up)\n",
           (void *)heap_var);
    printf("  heap_var pointer itself lives at: %p  (on the stack!)\n",
           (void *)&heap_var);
    printf("\n  Key insight: the POINTER is on the stack,\n"
           "  but the DATA it points to is on the heap.\n\n");

    /*
     * Memory map (simplified):
     *
     *  HIGH ADDRESSES
     *  ┌──────────────────┐
     *  │      Stack       │  ← stack_var, heap_var (the pointer)
     *  │    (grows ↓)     │
     *  ├──────────────────┤
     *  │                  │
     *  │   (free space)   │
     *  │                  │
     *  ├──────────────────┤
     *  │      Heap        │  ← *heap_var (the data)
     *  │    (grows ↑)     │
     *  ├──────────────────┤
     *  │   .bss / .data   │  ← globals, statics
     *  ├──────────────────┤
     *  │     .text        │  ← your compiled code
     *  └──────────────────┘
     *  LOW ADDRESSES
     */

    free(heap_var);
}


/* ──────────────────────────────────────────────
 * PART 2: malloc — allocate raw bytes
 * ────────────────────────────────────────────── */

static void demo_malloc(void) {
    printf("=== PART 2: malloc ===\n\n");

    /*
     * void *malloc(size_t size);
     *
     * - Allocates `size` bytes of UNINITIALIZED memory on the heap
     * - Returns a void* pointer to the start (or NULL on failure)
     * - You MUST cast or assign to the right pointer type
     *   (in C, void* implicitly converts — no cast needed, unlike C++)
     * - The memory contains GARBAGE — whatever was there before
     */

    /* Allocate space for 5 ints */
    int *nums = malloc(5 * sizeof(*nums));
    if (nums == NULL) {
        fprintf(stderr, "malloc failed\n");
        return;
    }

    /* DANGER: memory is uninitialized — contains garbage */
    printf("  Uninitialized heap memory (garbage values):\n  ");
    for (int i = 0; i < 5; i++) {
        printf("  nums[%d] = %d", i, nums[i]);
    }
    printf("\n\n");

    /* Initialize it ourselves */
    for (int i = 0; i < 5; i++) {
        nums[i] = (i + 1) * 10;
    }

    printf("  After initialization:\n  ");
    for (int i = 0; i < 5; i++) {
        printf("  nums[%d] = %d", i, nums[i]);
    }
    printf("\n\n");

    /* sizeof on a heap pointer gives pointer size, NOT array size */
    printf("  sizeof(nums) = %zu  (size of the pointer, NOT the array!)\n",
           sizeof(nums));
    printf("  This is why you must track array length separately.\n");
    printf("  In JS, array.length is free. In C, YOU remember the size.\n\n");

    free(nums);
}


/* ──────────────────────────────────────────────
 * PART 3: calloc — allocate AND zero-initialize
 * ────────────────────────────────────────────── */

static void demo_calloc(void) {
    printf("=== PART 3: calloc ===\n\n");

    /*
     * void *calloc(size_t count, size_t size);
     *
     * - Allocates space for `count` elements of `size` bytes each
     * - ZEROS all bytes (unlike malloc)
     * - Returns NULL on failure
     * - Slightly slower than malloc (has to zero memory)
     * - Bonus: catches overflow — if count * size overflows, returns NULL
     *   (malloc(count * size) would silently wrap and allocate too little)
     */

    int *nums = calloc(5, sizeof(*nums));
    if (nums == NULL) {
        fprintf(stderr, "calloc failed\n");
        return;
    }

    printf("  calloc zeroes memory:\n  ");
    for (int i = 0; i < 5; i++) {
        printf("  nums[%d] = %d", i, nums[i]);
    }
    printf("\n");
    printf("  All zeros — safe to read immediately.\n\n");

    /* When to use calloc vs malloc:
     *   - calloc: when you want zeroed memory, or need overflow protection
     *   - malloc: when you'll immediately overwrite all values anyway
     *             (no point zeroing memory you're about to fill)
     */

    free(nums);
}


/* ──────────────────────────────────────────────
 * PART 4: realloc — resize an existing allocation
 * ────────────────────────────────────────────── */

static void demo_realloc(void) {
    printf("=== PART 4: realloc ===\n\n");

    /*
     * void *realloc(void *ptr, size_t new_size);
     *
     * - Resizes the allocation at `ptr` to `new_size` bytes
     * - May MOVE the data to a new location (if it can't grow in-place)
     * - If moved, old data is copied and old block is freed automatically
     * - New bytes (if growing) are UNINITIALIZED
     * - Returns NULL on failure — BUT the old block is still valid!
     *
     * Special cases:
     *   realloc(NULL, size)  →  same as malloc(size)
     *   realloc(ptr, 0)     →  implementation-defined (avoid this)
     */

    /* Start with 3 ints */
    int *arr = malloc(3 * sizeof(*arr));
    if (arr == NULL) {
        fprintf(stderr, "malloc failed\n");
        return;
    }
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;

    printf("  Before realloc (%p):\n  ", (void *)arr);
    for (int i = 0; i < 3; i++) {
        printf("  arr[%d] = %d", i, arr[i]);
    }
    printf("\n");

    /* Grow to 6 ints */
    int *tmp = realloc(arr, 6 * sizeof(*arr));
    if (tmp == NULL) {
        fprintf(stderr, "realloc failed\n");
        free(arr);  /* old pointer is still valid on realloc failure */
        return;
    }
    arr = tmp;  /* might be a new address */

    printf("  After realloc  (%p):\n  ", (void *)arr);
    for (int i = 0; i < 3; i++) {
        printf("  arr[%d] = %d", i, arr[i]);
    }
    printf("  (old data preserved)\n");

    /* Initialize the new slots */
    arr[3] = 40;
    arr[4] = 50;
    arr[5] = 60;

    printf("  Full array:\n  ");
    for (int i = 0; i < 6; i++) {
        printf("  arr[%d] = %d", i, arr[i]);
    }
    printf("\n\n");

    /*
     * THE CLASSIC realloc BUG:
     *
     *   arr = realloc(arr, new_size);   // WRONG!
     *
     * If realloc returns NULL, you just overwrote `arr` with NULL.
     * Now you've lost the pointer to the original memory → MEMORY LEAK.
     * Always use a temporary pointer:
     *
     *   int *tmp = realloc(arr, new_size);
     *   if (tmp == NULL) { handle error; free(arr); return; }
     *   arr = tmp;
     */

    printf("  realloc pattern — this is how JS Array.push() works under the hood.\n");
    printf("  V8 allocates backing storage, and reallocs when you exceed capacity.\n\n");

    free(arr);
}


/* ──────────────────────────────────────────────
 * PART 5: free — return memory to the allocator
 * ────────────────────────────────────────────── */

static void demo_free(void) {
    printf("=== PART 5: free ===\n\n");

    /*
     * void free(void *ptr);
     *
     * - Returns the block at `ptr` to the allocator
     * - ptr MUST have come from malloc/calloc/realloc (or be NULL)
     * - free(NULL) is a no-op (safe to call)
     * - After free, the pointer is DANGLING — do not use it
     */

    int *p = malloc(sizeof(*p));
    if (p == NULL) {
        fprintf(stderr, "malloc failed\n");
        return;
    }
    *p = 99;
    printf("  *p = %d (before free)\n", *p);

    free(p);
    /* p is now DANGLING — it still holds the old address,
     * but that memory no longer belongs to us.
     * Accessing *p here is UNDEFINED BEHAVIOR. */

    /* Defensive pattern: set pointer to NULL after free */
    p = NULL;
    /* Now any accidental *p will segfault (crash) immediately
     * rather than silently reading corrupted data. */

    printf("  After free: p set to NULL (defensive pattern)\n");
    printf("  free(NULL) is safe: ");
    free(p);  /* no-op */
    printf("called free(NULL) — no crash.\n\n");
}


/* ──────────────────────────────────────────────
 * PART 6: Practical pattern — dynamic array (like JS Array)
 * ────────────────────────────────────────────── */

typedef struct {
    int *data;
    size_t length;
    size_t capacity;
} IntArray;

static IntArray int_array_create(size_t initial_cap) {
    IntArray a;
    a.data = malloc(initial_cap * sizeof(*a.data));
    if (a.data == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }
    a.length = 0;
    a.capacity = initial_cap;
    return a;
}

static void int_array_push(IntArray *a, int value) {
    if (a->length == a->capacity) {
        size_t new_cap = a->capacity * 2;  /* double capacity (same as V8) */
        int *tmp = realloc(a->data, new_cap * sizeof(*a->data));
        if (tmp == NULL) {
            fprintf(stderr, "realloc failed\n");
            free(a->data);
            exit(EXIT_FAILURE);
        }
        a->data = tmp;
        a->capacity = new_cap;
        printf("    [grew capacity: %zu → %zu]\n", a->capacity / 2, new_cap);
    }
    a->data[a->length] = value;
    a->length++;
}

static void int_array_destroy(IntArray *a) {
    free(a->data);
    a->data = NULL;
    a->length = 0;
    a->capacity = 0;
}

static void demo_dynamic_array(void) {
    printf("=== PART 6: Dynamic Array (JS Array under the hood) ===\n\n");

    IntArray arr = int_array_create(2);
    printf("  Created with capacity 2\n");

    for (int i = 1; i <= 10; i++) {
        printf("  push(%d)", i);
        int_array_push(&arr, i);
        printf("  → length=%zu, capacity=%zu\n", arr.length, arr.capacity);
    }

    printf("\n  Final array: [");
    for (size_t i = 0; i < arr.length; i++) {
        if (i > 0) printf(", ");
        printf("%d", arr.data[i]);
    }
    printf("]\n\n");

    int_array_destroy(&arr);
    printf("  Destroyed — memory freed.\n");
    printf("  In JS this is just: const arr = []; arr.push(1, 2, ...)\n");
    printf("  V8 does exactly what we just did, invisibly.\n\n");
}


/* ──────────────────────────────────────────────
 * PART 7: The danger zone — what NOT to do
 * ────────────────────────────────────────────── */

static void demo_dangers(void) {
    printf("=== PART 7: Common Heap Bugs ===\n\n");

    /* Bug 1: Memory leak — allocate but never free */
    printf("  Bug 1: MEMORY LEAK\n");
    printf("  int *leaked = malloc(100);\n");
    printf("  // ... forgot to free(leaked)\n");
    printf("  → Memory stays allocated until the process exits.\n");
    printf("  → In a long-running program (server), this adds up.\n");
    printf("  → JS equivalent: impossible — GC handles it.\n\n");

    /* Bug 2: Use after free */
    printf("  Bug 2: USE AFTER FREE\n");
    printf("  int *p = malloc(sizeof(int));\n");
    printf("  *p = 42;\n");
    printf("  free(p);\n");
    printf("  printf(\"%%d\", *p);  // UNDEFINED BEHAVIOR\n");
    printf("  → Might print 42, might print garbage, might crash.\n");
    printf("  → The memory might be reused for something else.\n\n");

    /* Bug 3: Double free */
    printf("  Bug 3: DOUBLE FREE\n");
    printf("  int *p = malloc(sizeof(int));\n");
    printf("  free(p);\n");
    printf("  free(p);  // UNDEFINED BEHAVIOR — heap corruption\n");
    printf("  → Can corrupt the allocator's internal data structures.\n");
    printf("  → Fix: set p = NULL after free (free(NULL) is safe).\n\n");

    /* Bug 4: Freeing stack memory */
    printf("  Bug 4: FREEING STACK MEMORY\n");
    printf("  int x = 5;\n");
    printf("  free(&x);  // UNDEFINED BEHAVIOR — x is on the stack\n");
    printf("  → Only free what came from malloc/calloc/realloc.\n\n");

    /* Bug 5: Returning stack pointer (dangling pointer) */
    printf("  Bug 5: RETURNING STACK POINTER\n");
    printf("  int *bad_func(void) {\n");
    printf("      int local = 42;\n");
    printf("      return &local;  // local dies when function returns!\n");
    printf("  }\n");
    printf("  → Caller gets a dangling pointer to dead stack frame.\n");
    printf("  → Fix: malloc inside the function, caller must free.\n\n");
}


int main(void) {
    stack_vs_heap();
    printf("\n");
    demo_malloc();
    printf("\n");
    demo_calloc();
    printf("\n");
    demo_realloc();
    printf("\n");
    demo_free();
    printf("\n");
    demo_dynamic_array();
    printf("\n");
    demo_dangers();

    printf("=== Lesson complete ===\n");
    printf("Key takeaway: In C, every malloc needs a free.\n");
    printf("Ownership rule: whoever mallocs is responsible for freeing.\n");
    return EXIT_SUCCESS;
}
