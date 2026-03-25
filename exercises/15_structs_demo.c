/*
 * Lesson 15: struct, typedef, and nested structs
 *
 * Phase 3 begins! Structs are C's mechanism for grouping related data
 * into a single type. If pointers gave you the power to reach into
 * memory, structs give you the power to SHAPE it.
 *
 * JS parallel: A struct is like a JS object literal — { x: 1, y: 2 } —
 * but the shape is fixed at compile time (can't add/remove fields),
 * every field has a declared type, and the compiler decides the exact
 * memory layout.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>

/* ================================================================== */
/* Section 1: Declaring and using a struct                             */
/* ================================================================== */

/*
 * A struct declaration creates a new TYPE.
 * It describes the layout: what fields exist and their types.
 *
 * In JS you'd write:   const point = { x: 3, y: 7 };
 * In C you declare the SHAPE first, then create instances of it.
 */

struct point {
    int x;
    int y;
};

/*
 * `struct point` is now a type, just like `int` or `char`.
 * The tag name `point` lives in the "struct namespace" — it doesn't
 * collide with variables or functions named `point`.
 */

static void demo_basics(void) {
    printf("=== Section 1: Struct Basics ===\n\n");

    /* Declaration + field-by-field initialization */
    struct point p1;
    p1.x = 10;
    p1.y = 20;
    printf("p1 = (%d, %d)\n", p1.x, p1.y);

    /* Initialization at declaration (positional — order matters) */
    struct point p2 = { 30, 40 };
    printf("p2 = (%d, %d)\n", p2.x, p2.y);

    /*
     * C99 designated initializers — name the fields explicitly.
     * Order doesn't matter. Unmentioned fields are zero-initialized.
     * This is the preferred style: it's self-documenting and resilient
     * to field reordering.
     */
    struct point p3 = { .y = 100, .x = 50 };
    printf("p3 = (%d, %d)\n", p3.x, p3.y);

    /* Partial initialization — .y is implicitly 0 */
    struct point p4 = { .x = 7 };
    printf("p4 = (%d, %d)  [.y defaulted to 0]\n", p4.x, p4.y);

    /* Zero-init the whole struct */
    struct point origin = { 0 };
    printf("origin = (%d, %d)\n", origin.x, origin.y);

    /*
     * WARNING: An uninitialized struct has GARBAGE in every field,
     * just like uninitialized int/char variables.
     *
     *   struct point garbage;
     *   printf("%d\n", garbage.x);  // UB — reading uninitialized memory
     *
     * Always initialize your structs.
     */

    /* Struct assignment copies ALL fields (value semantics) */
    struct point p5 = p2;
    p5.x = 999;
    printf("p5 = (%d, %d), p2 still = (%d, %d)  [independent copy]\n",
           p5.x, p5.y, p2.x, p2.y);

    /*
     * Unlike JS where `const copy = obj` creates a reference (same object),
     * C struct assignment is a DEEP COPY of the raw bytes. Modifying p5
     * doesn't affect p2. This is value semantics — like primitives in JS.
     *
     * Caveat: if a struct contains a POINTER, the pointer is copied but
     * not what it points to. That's a shallow copy, and both structs would
     * share the pointed-to data. More on this later.
     */

    printf("\n");
}

/* ================================================================== */
/* Section 2: sizeof, memory layout, and padding                      */
/* ================================================================== */

struct example_a {
    char c;     /* 1 byte */
    int i;      /* 4 bytes */
    char d;     /* 1 byte */
};

struct example_b {
    int i;      /* 4 bytes */
    char c;     /* 1 byte */
    char d;     /* 1 byte */
};

static void demo_layout(void) {
    printf("=== Section 2: Memory Layout & Padding ===\n\n");

    /*
     * The CPU reads memory most efficiently when data is ALIGNED —
     * an int at an address divisible by 4, a double at an address
     * divisible by 8, etc.
     *
     * The compiler inserts invisible PADDING bytes between fields
     * to maintain alignment. This means sizeof(struct) is often
     * LARGER than the sum of its fields.
     */

    printf("Sizes of individual types:\n");
    printf("  sizeof(char) = %zu\n", sizeof(char));
    printf("  sizeof(int)  = %zu\n", sizeof(int));

    printf("\nstruct example_a { char c; int i; char d; }:\n");
    printf("  Sum of fields: 1 + 4 + 1 = 6 bytes\n");
    printf("  sizeof(struct example_a) = %zu bytes\n", sizeof(struct example_a));

    /*
     * Layout of example_a in memory (on typical 64-bit system):
     *
     *   Offset  Field   Size    Notes
     *   ------  ------  ------  -----
     *     0     c       1       
     *     1     (pad)   3       padding to align i at offset 4
     *     4     i       4       aligned to 4-byte boundary
     *     8     d       1       
     *     9     (pad)   3       trailing padding for array alignment
     *   ------
     *   Total:  12 bytes  (not 6!)
     *
     * Trailing padding ensures that in an array of structs, every
     * element starts at a properly aligned address.
     */

    printf("\nstruct example_b { int i; char c; char d; }:\n");
    printf("  Sum of fields: 4 + 1 + 1 = 6 bytes\n");
    printf("  sizeof(struct example_b) = %zu bytes\n", sizeof(struct example_b));

    /*
     * Layout of example_b:
     *
     *   Offset  Field   Size    Notes
     *   ------  ------  ------  -----
     *     0     i       4       naturally aligned
     *     4     c       1       
     *     5     d       1       
     *     6     (pad)   2       trailing padding
     *   ------
     *   Total:  8 bytes  (instead of 12!)
     *
     * SAME fields, DIFFERENT order → 33% less memory.
     * Rule of thumb: order fields from largest to smallest
     * to minimize padding.
     */

    /* Prove the layout with offsetof */
    printf("\nField offsets (example_a):\n");
    printf("  c at offset %zu\n", offsetof(struct example_a, c));
    printf("  i at offset %zu\n", offsetof(struct example_a, i));
    printf("  d at offset %zu\n", offsetof(struct example_a, d));

    printf("\nField offsets (example_b):\n");
    printf("  i at offset %zu\n", offsetof(struct example_b, i));
    printf("  c at offset %zu\n", offsetof(struct example_b, c));
    printf("  d at offset %zu\n", offsetof(struct example_b, d));

    printf("\n");
}

/* ================================================================== */
/* Section 3: typedef — naming your types                             */
/* ================================================================== */

/*
 * Writing `struct point` everywhere gets verbose. `typedef` creates
 * an alias — a shorter name for an existing type.
 */

typedef unsigned long ulong;   /* simple alias */
typedef int error_code;        /* semantic alias — makes intent clear */

/*
 * The idiomatic pattern: combine struct definition with typedef.
 *
 * Syntax:  typedef struct tag_name { ... } AliasName;
 *
 * The tag_name is optional but useful for self-referential structs
 * (like linked list nodes that point to themselves).
 */

typedef struct vec2 {
    double x;
    double y;
} Vec2;

/*
 * Now you can use either `struct vec2` or `Vec2` — they're the same type.
 * Most C code uses the typedef name. Some projects (like the Linux kernel)
 * deliberately avoid typedefs for structs, arguing that `struct` makes it
 * clear the type has value semantics and may be expensive to copy.
 *
 * Convention: typedef names are often PascalCase or UPPER_CASE.
 * We'll use PascalCase (Vec2, Color, Rect) since it's visually distinct
 * from variables (snake_case) and macros (UPPER_CASE).
 */

/* typedef without a tag — anonymous struct */
typedef struct {
    uint8_t r, g, b, a;
} Color;

/*
 * Anonymous structs can't reference themselves (no tag to use),
 * so you can't build linked lists with them. Use tagged structs
 * when self-reference is needed.
 */

static void demo_typedef(void) {
    printf("=== Section 3: typedef ===\n\n");

    Vec2 velocity = { .x = 3.0, .y = 4.0 };
    printf("velocity = (%.1f, %.1f)\n", velocity.x, velocity.y);

    Color red = { .r = 255, .g = 0, .b = 0, .a = 255 };
    printf("red = rgba(%u, %u, %u, %u)\n", red.r, red.g, red.b, red.a);

    printf("sizeof(Vec2)  = %zu bytes\n", sizeof(Vec2));
    printf("sizeof(Color) = %zu bytes  [4 chars, no padding needed]\n",
           sizeof(Color));

    /* typedef for function clarity */
    error_code status = 0;
    printf("status (error_code) = %d\n", status);

    printf("\n");
}

/* ================================================================== */
/* Section 4: Structs as function arguments                            */
/* ================================================================== */

/*
 * CRITICAL CONCEPT: structs are passed BY VALUE in C.
 * The entire struct is COPIED onto the stack. For a 16-byte Vec2,
 * that's fine. For a 10KB struct, that's a disaster.
 *
 * This is exactly the same as passing an int — the function gets
 * its own copy and can't modify the caller's original.
 */

static double vec2_length(Vec2 v) {
    /* v is a copy — modifying v.x here wouldn't affect the caller */
    return sqrt(v.x * v.x + v.y * v.y);
}

/* To modify the caller's struct, pass a pointer (just like with int) */
static void vec2_scale(Vec2 *v, double factor) {
    v->x *= factor;
    v->y *= factor;
    /*
     * v->x is shorthand for (*v).x
     *
     * The -> operator: dereference the pointer AND access the field.
     * You'll use this constantly. It exists because (*v).x is ugly
     * and easy to get wrong (operator precedence: *v.x means *(v.x)).
     */
}

/* Return a struct by value — returns a copy */
static Vec2 vec2_add(Vec2 a, Vec2 b) {
    return (Vec2){ .x = a.x + b.x, .y = a.y + b.y };
    /*
     * (Vec2){ .x = ..., .y = ... } is a COMPOUND LITERAL (C99).
     * It creates a temporary Vec2 on the stack, which gets copied
     * to the caller as the return value. The compiler often optimizes
     * this away (RVO — Return Value Optimization).
     */
}

/* Common pattern: const pointer for read-only access */
static void vec2_print(const Vec2 *v) {
    printf("(%.2f, %.2f)", v->x, v->y);
}

static void demo_functions(void) {
    printf("=== Section 4: Structs & Functions ===\n\n");

    Vec2 pos = { .x = 3.0, .y = 4.0 };

    printf("pos = ");
    vec2_print(&pos);
    printf("\n");

    /* Pass by value — length gets a copy */
    printf("length = %.2f\n", vec2_length(pos));

    /* Pass by pointer — scale modifies original */
    vec2_scale(&pos, 2.0);
    printf("after scale(2.0): ");
    vec2_print(&pos);
    printf("\n");

    /* Return by value */
    Vec2 dir = { .x = 1.0, .y = 0.0 };
    Vec2 sum = vec2_add(pos, dir);
    printf("pos + dir = ");
    vec2_print(&sum);
    printf("\n");

    /*
     * When to pass by value vs pointer:
     *
     * By value:     small structs (≤ 16-32 bytes), when you need a copy
     * By pointer:   large structs, when function needs to modify original
     * By const ptr: large structs, read-only (best of both worlds)
     *
     * In JS everything that's not a primitive is by reference. In C you
     * choose explicitly — and that choice is visible in the API.
     */

    printf("\n");
}

/* ================================================================== */
/* Section 5: Nested structs                                           */
/* ================================================================== */

typedef struct {
    char name[64];
    char email[128];
} Contact;

typedef struct {
    char street[128];
    char city[64];
    char state[3];    /* 2-letter code + null terminator */
    int zip;
} Address;

typedef struct {
    int id;
    Contact contact;     /* struct inside a struct */
    Address address;
    double balance;
} Customer;

static void demo_nested(void) {
    printf("=== Section 5: Nested Structs ===\n\n");

    Customer c = {
        .id = 1001,
        .contact = {
            .name = "Alice Chen",
            .email = "alice@example.com"
        },
        .address = {
            .street = "123 Elm St",
            .city = "Portland",
            .state = "OR",
            .zip = 97201
        },
        .balance = 1250.75
    };

    /*
     * Access nested fields by chaining the dot operator.
     * This is exactly like JS: customer.contact.name
     */
    printf("Customer #%d\n", c.id);
    printf("  Name:    %s\n", c.contact.name);
    printf("  Email:   %s\n", c.contact.email);
    printf("  Address: %s, %s, %s %d\n",
           c.address.street, c.address.city,
           c.address.state, c.address.zip);
    printf("  Balance: $%.2f\n", c.balance);

    /* Through a pointer: -> then . for the nested access */
    Customer *cp = &c;
    printf("\nVia pointer:\n");
    printf("  cp->contact.name  = %s\n", cp->contact.name);
    printf("  cp->address.city  = %s\n", cp->address.city);

    /*
     * cp->contact.name means:
     *   1. cp->contact  — dereference cp, get the Contact sub-struct
     *   2. .name        — access the name field of that Contact
     *
     * You only use -> once (for the first dereference). After that,
     * you're accessing fields of a value, not a pointer, so use dot.
     *
     * If contact were a POINTER inside Customer:
     *   cp->contact->name   (two arrows, two dereferences)
     */

    printf("\nMemory layout of Customer:\n");
    printf("  sizeof(Customer) = %zu bytes\n", sizeof(Customer));
    printf("  sizeof(Contact)  = %zu bytes\n", sizeof(Contact));
    printf("  sizeof(Address)  = %zu bytes\n", sizeof(Address));
    printf("  offsets:\n");
    printf("    id       at %zu\n", offsetof(Customer, id));
    printf("    contact  at %zu\n", offsetof(Customer, contact));
    printf("    address  at %zu\n", offsetof(Customer, address));
    printf("    balance  at %zu\n", offsetof(Customer, balance));

    /*
     * The nested structs are EMBEDDED — Contact's bytes live inside
     * Customer, not as a pointer to somewhere else. This is like
     * having the JS object's fields inlined, not referenced.
     *
     * Memory:
     *   ┌─────────────────────────────────────────┐
     *   │ id (4) │ pad │ Contact (192) │           │
     *   │                │ Address (200) │          │
     *   │                               │ balance  │
     *   └─────────────────────────────────────────┘
     *
     * No pointers, no indirection, no extra allocations.
     * This is what makes C structs efficient — the data is FLAT.
     */

    printf("\n");
}

/* ================================================================== */
/* Section 6: Arrays of structs                                        */
/* ================================================================== */

typedef struct {
    char title[100];
    char author[50];
    int year;
    double price;
} Book;

static void print_book(const Book *b) {
    printf("  \"%s\" by %s (%d) — $%.2f\n",
           b->title, b->author, b->year, b->price);
}

static void demo_array_of_structs(void) {
    printf("=== Section 6: Arrays of Structs ===\n\n");

    Book library[] = {
        { .title = "The C Programming Language",
          .author = "Kernighan & Ritchie", .year = 1978, .price = 45.99 },
        { .title = "Expert C Programming",
          .author = "Peter van der Linden", .year = 1994, .price = 39.99 },
        { .title = "Modern C",
          .author = "Jens Gustedt", .year = 2019, .price = 0.00 },
    };
    size_t count = sizeof(library) / sizeof(library[0]);

    printf("Library (%zu books):\n", count);
    for (size_t i = 0; i < count; i++) {
        print_book(&library[i]);
    }

    /*
     * Memory layout of library[]:
     *
     *   library[0]           library[1]           library[2]
     *   ┌──────────────────┬──────────────────┬──────────────────┐
     *   │ title│author│... │ title│author│... │ title│author│... │
     *   └──────────────────┴──────────────────┴──────────────────┘
     *   ← sizeof(Book)    →← sizeof(Book)    →← sizeof(Book)    →
     *
     * Contiguous in memory. Pointer arithmetic works:
     *   &library[1] == &library[0] + 1
     *   (but + 1 moves by sizeof(Book) bytes, not 1 byte)
     */

    printf("\nEach Book is %zu bytes\n", sizeof(Book));
    printf("Entire array: %zu bytes (%zu × %zu)\n",
           sizeof(library), count, sizeof(Book));

    /* Find most expensive book using pointer iteration */
    const Book *most_expensive = &library[0];
    for (const Book *b = library + 1; b < library + count; b++) {
        if (b->price > most_expensive->price) {
            most_expensive = b;
        }
    }
    printf("\nMost expensive: \"%s\" ($%.2f)\n",
           most_expensive->title, most_expensive->price);

    printf("\n");
}

/* ================================================================== */
/* Section 7: Structs on the heap                                      */
/* ================================================================== */

typedef struct {
    char *name;       /* pointer to heap-allocated string */
    int *scores;      /* pointer to heap-allocated array */
    size_t num_scores;
} Student;

static Student *student_create(const char *name,
                               const int *scores, size_t n) {
    Student *s = malloc(sizeof(*s));
    if (s == NULL) return NULL;

    s->name = malloc(strlen(name) + 1);
    if (s->name == NULL) {
        free(s);
        return NULL;
    }
    strcpy(s->name, name);

    s->scores = malloc(n * sizeof(*s->scores));
    if (s->scores == NULL) {
        free(s->name);
        free(s);
        return NULL;
    }
    memcpy(s->scores, scores, n * sizeof(*s->scores));
    s->num_scores = n;

    return s;
}

static void student_destroy(Student *s) {
    if (s == NULL) return;
    free(s->name);
    free(s->scores);
    free(s);
}

static void student_print(const Student *s) {
    printf("  %s: [", s->name);
    for (size_t i = 0; i < s->num_scores; i++) {
        printf("%d%s", s->scores[i],
               i + 1 < s->num_scores ? ", " : "");
    }
    printf("]\n");
}

static void demo_heap_structs(void) {
    printf("=== Section 7: Structs on the Heap ===\n\n");

    /*
     * When structs contain dynamically-sized data (variable-length
     * strings, growable arrays), the struct itself may hold POINTERS
     * to heap memory rather than embedding the data inline.
     *
     * This introduces OWNERSHIP: the struct "owns" the memory its
     * pointers reference, and must free it when destroyed.
     *
     * Pattern: constructor allocates everything, destructor frees
     * everything. If any allocation fails in the constructor,
     * clean up what was already allocated and return NULL.
     *
     * This is the C equivalent of a class with a constructor and
     * destructor. No RAII, no garbage collector — you manage it.
     */

    int scores_a[] = { 95, 87, 92, 88 };
    int scores_b[] = { 78, 82, 90 };

    Student *alice = student_create("Alice", scores_a, 4);
    Student *bob   = student_create("Bob", scores_b, 3);

    if (alice == NULL || bob == NULL) {
        fprintf(stderr, "allocation failed\n");
        student_destroy(alice);
        student_destroy(bob);
        return;
    }

    student_print(alice);
    student_print(bob);

    /*
     * SHALLOW COPY TRAP:
     *
     *   Student copy = *alice;   // copies the struct
     *   // copy.name and alice->name point to the SAME heap memory!
     *   // Freeing one invalidates the other.
     *
     * To safely copy a struct with heap pointers, you need a DEEP COPY
     * function that allocates new memory for each pointer field.
     */

    printf("\nMemory map:\n");
    printf("  alice struct at %p (stack)\n", (void *)alice);
    printf("  alice->name  at %p (heap) → \"%s\"\n",
           (void *)alice->name, alice->name);
    printf("  alice->scores at %p (heap)\n", (void *)alice->scores);

    student_destroy(alice);
    student_destroy(bob);

    printf("\n");
}

/* ================================================================== */
/* Section 8: Self-referential structs (linked list preview)           */
/* ================================================================== */

/*
 * A struct can contain a pointer to its own type. This is how you
 * build linked lists, trees, graphs — any recursive data structure.
 *
 * You MUST use the struct tag for self-reference, because the
 * typedef alias isn't available inside the struct definition yet.
 */

typedef struct node {
    int data;
    struct node *next;  /* pointer to same type — self-reference */
    /* Node *next; would NOT work here — Node isn't defined yet */
} Node;

static void demo_self_ref(void) {
    printf("=== Section 8: Self-Referential Structs ===\n\n");

    /* Build a tiny linked list: 10 → 20 → 30 → NULL */
    Node n3 = { .data = 30, .next = NULL };
    Node n2 = { .data = 20, .next = &n3 };
    Node n1 = { .data = 10, .next = &n2 };

    printf("Linked list (stack-allocated): ");
    for (const Node *cur = &n1; cur != NULL; cur = cur->next) {
        printf("%d", cur->data);
        if (cur->next != NULL) printf(" → ");
    }
    printf(" → NULL\n");

    printf("sizeof(Node) = %zu bytes  [int + pointer + padding]\n",
           sizeof(Node));

    /*
     * Note: these nodes live on the stack, which is unusual.
     * Real linked lists use heap allocation so nodes can be created
     * and destroyed dynamically. We'll build a full heap-based
     * linked list in an upcoming lesson.
     */

    printf("\n");
}

/* ================================================================== */
/* main                                                                */
/* ================================================================== */

int main(void) {
    demo_basics();
    demo_layout();
    demo_typedef();
    demo_functions();
    demo_nested();
    demo_array_of_structs();
    demo_heap_structs();
    demo_self_ref();

    return EXIT_SUCCESS;
}
