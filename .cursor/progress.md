# C Learning Progress

## Current Position
- **Phase**: Phase 2 — Pointers & Memory
- **Current Topic**: Pointer to pointer, arrays of pointers
- **Last Session**: 2026-03-23

## Completed Topics
| # | Topic | Date | Notes |
|---|-------|------|-------|
| 1 | Compilation model (preprocessor → compiler → assembler → linker) vs JS JIT | 2026-02-26 | Walked through all 4 stages with real output: -E, -S, -c, linking. Compared to V8 JIT pipeline. |
| 2 | Types: int, char, float, double, long, unsigned, sizeof | 2026-02-26 | Sizes, ranges, format specifiers, char as int, float precision, limits.h, sizeof. Overflow exercise completed. |
| 3 | Variables, constants, #define vs const | 2026-02-27 | Covered storage duration (auto/static/global), scope, #define vs const vs enum tradeoffs, preprocessor macro dangers, uninitialized variable UB. |
| 4 | Operators and expressions (including bitwise) | 2026-02-27 | Integer division truncation, integer promotions, signed/unsigned comparison trap, sequence points & UB, bitwise operators (& | ^ ~ << >>), bit manipulation patterns, operator precedence gotcha. |
| 5 | Control flow: if, switch, for, while, do-while, goto | 2026-03-04 | if truthiness, switch fallthrough/grouping, for C99 scoping, do-while, break/continue (no labels), goto cleanup pattern. Exercise completed with all fixes applied. |
| 6 | Functions, prototypes, header files | 2026-03-04 | Header guards, prototypes, #include "" vs <>, multi-file compilation, static functions for file-scoped visibility. |
| 7 | Arrays (stack-allocated, fixed-size) | 2026-03-04 | Declaration, initialization, memory layout, sizeof trick, ARRAY_LEN macro, array decay, passing to functions, 2D row-major. |
| 8 | Strings as char[] / char* with null terminator | 2026-03-04 | char[] vs char*, null terminator, .rodata segment, strlen vs sizeof, strcmp, snprintf, pointer iteration. Skipped exercise (too easy). |
| 9 | Pass-by-value vs pass-by-pointer | 2026-03-05 | Copy trap demo, address proof, swap pattern, out-parameters, array decay recap. Skipped exercise. |
| 10 | malloc, calloc, realloc, free — heap allocation | 2026-03-23 | Stack vs heap memory map, malloc (uninitialized), calloc (zeroed + overflow-safe), realloc (temp pointer pattern), free (NULL-after-free), dynamic array pattern, 5 common heap bugs. |
| 11 | Pointer to pointer, arrays of pointers | 2026-03-23 | int** basics, modifying caller's pointer (alloc_broken vs alloc_fixed), arrays of pointers, jagged heap 2D arrays, char** string arrays (argv preview), constructor/destructor patterns. |

## Exercises
| # | Exercise | File | Status | Notes |
|---|----------|------|--------|-------|
| 1 | Explore the Build Pipeline | exercises/01_build_pipeline.c | completed | Passed. Ran all 4 stages. Minor style issue: space between # and directive. Didn't use -Wall -Wextra -Werror flags initially. |
| 2 | Type Overflow Explorer | exercises/02_overflow.c | completed | All 4 cases correct. Correctly identified UB vs defined wrapping. Explored all 4 build stages again with new code. Hit linker EISDIR error (tried -o build when build/ was a directory). |
| 3 | Constants & Variable Lifetime | exercises/03_constants_exercise.c | completed | All logic correct. SQUARE macro properly parenthesized, static next_id works, enum array sizing correct, block scoping demonstrated. Minor issues: typo in output, unused const, UPPER_CASE naming on a const variable (should be snake_case). |
| 4 | Bitwise Permissions | exercises/04_bitwise_permissions.c | completed | All logic correct after fixing 4 bugs: unsigned loop var infinite loop, ignored return values (pass-by-value), XOR vs AND-NOT for clear, reversed print order. Fixed all on first attempt after review. |
| 5 | Text Analyzer | exercises/05_text_analyzer.c | completed | Control flow exercise: for, switch fallthrough, do-while, longest run tracking. All review issues fixed: const char *, * placement, do-while added. Clean compile, correct output. |
| 6 | Multi-File Calculator | exercises/06_calc.h, 06_calc.c, 06_main.c | completed | Header guard, prototypes, multi-file compilation, static function, divide-by-zero guard. Needed help with: trailing }; (JS habit), power off-by-one (result=base not 1), abs_value logic confusion (caused by ambiguous exercise spec), negative exponent early return. |
| 7 | Grade Statistics | exercises/07_grade_stats.c | completed | Arrays exercise: find_min, find_max, compute_average, count_above, curve_grades. Core logic correct. Skipped fixes (understood): global→local array, size==0 guard, cap at 100, float-to-int implicit conversion. |
| 8 | String Toolkit | exercises/08_string_toolkit.c | skipped | Student found it too easy, skipped exercise. Concepts understood from demo and Q&A. |
| 9 | Pointer Explorer | exercises/09_pointer_explorer.c | skipped | Skipped exercise. Concepts understood from demo and explanation. |
| 10 | Pointer Functions | exercises/10_pass_by_exercise.c | skipped | Skipped exercise. Concepts understood from demo. |

## Mistakes & Weak Areas
- 2026-03-04: Used global array instead of local — same principle as avoiding globals in JS. Prefer local variables passed to functions.
- 2026-03-04: Missing edge case guard for size==0 in find_min/find_max — accessing grades[0] with empty array is UB.
- 2026-03-04: Missing cap at 100 in curve_grades — spec required clamping, worked by coincidence with test data.
- 2026-03-04: Implicit float-to-int conversion when passing compute_average() result to count_above(int threshold) — silent truncation.
- 2026-02-27: Used `unsigned char` as loop variable in a `>= 0` condition — unsigned types can never be negative, causing infinite loop. Understood wrapping behavior after explanation.
- 2026-02-27: Ignored return values from functions expecting pass-by-value pattern. Forgot that C functions get copies of arguments. Fixed by capturing return values.
- 2026-02-27: Used XOR (`^`) instead of AND-NOT (`& ~`) for clearing a bit. XOR toggles, AND-NOT clears unconditionally. Corrected after explanation.
- 2026-02-26: Used `# include` / `# define` with space after `#`. Technically valid but non-standard style. Clarified convention is `#include` / `#define`.
- 2026-02-26: Compiled without `-Wall -Wextra -Werror` flags. Explained that compiler flags are the C equivalent of a linter.
- 2026-02-27: Used UPPER_SNAKE_CASE for a `const` variable (`EGGS`). In C convention, UPPER_CASE is for macros and enum constants; `const` vars should be snake_case since they're still variables (with addresses), not true compile-time constants.
- 2026-03-04: Trailing `};` after function bodies — JS habit from `const fn = () => { ... };`. In C, function definitions end with `}` only.
- 2026-03-04: Initialized `result = base` instead of `result = 1` in power loop — off-by-one multiplication. Pattern: accumulator loops should start at the identity element (1 for multiplication, 0 for addition).

## Areas to Revisit
- [ ] Always use `-Wall -Wextra -Werror -g` when compiling
- [ ] Never use unsigned types for loop variables that count down to 0
- [ ] Remember C is pass-by-value — capture return values or use pointers
- [ ] Always use `const char *` for string literals — they live in read-only memory
- [ ] Put `*` with the variable name in C: `char *p` not `char* p`

## Session Log
### 2026-02-26
- Covered: Compilation model — preprocessor, compiler, assembler, linker. All 4 stages demonstrated with 01_hello.c
- Exercises: "Explore the Build Pipeline" — completed successfully
- Key takeaways: Saw preprocessor expand 8 lines to 574, saw ARM64 assembly, understood .o vs executable size difference. Learned compiler flags are the C linter.

### 2026-02-26 (session 2)
- Covered: Types — int, char, short, long, long long, unsigned, float, double, sizeof. Explored sizes and ranges on ARM64 macOS. Covered format specifiers, char as integer, digit extraction trick, float comparison pitfalls, limits.h/float.h.
- Exercises: "Type Overflow Explorer" — assigned
- Key takeaways: long and long long both 8 bytes on ARM64, long double same as double on Apple Silicon, %zu for size_t, never compare floats with ==.

### 2026-02-27
- Covered: Variables (auto/static/global, storage duration, scope), constants (#define vs const vs enum), macro pitfalls, uninitialized variable UB
- Exercises: "Constants & Variable Lifetime" — assigned
- Key takeaways: const in C is NOT a compile-time constant (unlike JS const), enum gives true compile-time int constants, -Wuninitialized catches reading garbage
- Exercise completed: All logic correct on first attempt. Macro parenthesization, static locals, enum sizing all understood. Style note: const vars use snake_case, not UPPER_CASE.

### 2026-03-01
- Covered: Control flow — if (truthiness, assignment trap), switch (integer-only, fallthrough, grouping, __attribute__((fallthrough))), for (C99 scoping, infinite loop idiom), while, do-while (macro wrapper pattern), break/continue (no labels in C), goto (cleanup pattern from Linux kernel)
- Exercises: "Text Analyzer" — assigned
- Key takeaways: Understood all control flow constructs. Switch fallthrough grouping for vowels. Longest-run tracking pattern.

### 2026-03-04
- Covered: Text Analyzer review (approved), Functions / prototypes / header files
- Exercises: "Multi-File Calculator" — completed with assistance on power function
- Key takeaways: Header guards, prototypes, `#include ""` vs `<>`, multi-file compilation, `static` functions for file-scoped visibility. JS habit of `};` after functions flagged. Accumulator pattern (start at identity element) needs reinforcement.

### 2026-03-04 (session 2)
- Covered: Arrays — declaration, initialization, memory layout, no bounds checking, sizeof trick, array decay to pointer, passing to functions, ARRAY_LEN macro, 2D arrays row-major layout
- Exercises: "Grade Statistics" — completed (skipped fixes, understood issues)
- Key takeaways: Demonstrated contiguous addressing, sizeof inside vs outside function, decay equivalence (arr == &arr[0] == ptr), modification through function pointer, 2D flat memory layout. Compiler warning -Wsizeof-array-argument catches sizeof-on-parameter mistake.
- Covered: Strings — char[] vs char*, null terminator, string literal storage, strlen vs sizeof, strcmp (0=equal gotcha), snprintf for safe building, pointer iteration, strstr/strchr, string interning caveat
- Exercises: "String Toolkit" — assigned
- Key takeaways: No string type in C — just char arrays with \0 sentinel. const char* for literals. Never use gets/scanf for strings. snprintf for safe formatting.

### 2026-03-05
- Covered: Pointers — declaration, &, *, pointer sizes, NULL, pointer arithmetic (stride = sizeof(type)), pointer subtraction, array-pointer equivalence, dangerous patterns (uninitialized, dangling, out-of-bounds)
- Exercises: "Pointer Explorer" — skipped
- Key takeaways: Phase 2 begins. Demo showed arithmetic stride (4 bytes per int), byte-distance via char* cast, NULL as 0x0, arr[i] ≡ *(arr+i) ≡ i[arr].

### 2026-03-05 (session 2)
- Covered: Pass-by-value vs pass-by-pointer — copy trap demo, address proof, swap pattern, out-parameters for multiple returns, array decay recap, stack growth direction
- Exercises: "Pointer Functions" — skipped
- Key takeaways: ALL args are copies in C (no references), pass &var to let function modify it, out-parameters for multiple returns (like scanf), arrays always effectively by-pointer.

### 2026-03-23
- Covered: Heap allocation — malloc (uninitialized raw bytes), calloc (zeroed + overflow-safe), realloc (resize with temp-pointer pattern), free (NULL-after-free defensive pattern), stack vs heap memory model, dynamic array implementation (JS Array.push internals), 5 common heap bugs (leak, use-after-free, double-free, freeing stack, returning stack pointer)
- Exercises: none yet — recommended exercise for heap due to 3 skipped exercises in a row
- Key takeaways: The pointer is on the stack but the data is on the heap. sizeof on a heap pointer gives pointer size, not array size. realloc can move data. Ownership rule: whoever mallocs must free.

### 2026-03-23 (session 2)
- Covered: Pointer to pointer — int** basics (dereference chain pp→p→x), modifying caller's pointer (alloc_broken vs alloc_fixed), "to modify T pass T*" rule, arrays of pointers (char *[], int *[]), jagged heap 2D arrays (N+1 allocs/frees), char** string array pattern (split_words, argv preview), constructor/destructor patterns (return vs out-parameter vs free-and-NULL)
- Exercises: none assigned
- Key takeaways: int** is for letting functions change WHERE a caller's pointer points. Jagged arrays require freeing rows before the pointer array. char** is how argv works.
