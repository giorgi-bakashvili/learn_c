# C Learning Progress

## Current Position
- **Phase**: Phase 1 — Foundations
- **Current Topic**: Variables, constants, #define vs const
- **Last Session**: 2026-02-27

## Completed Topics
| # | Topic | Date | Notes |
|---|-------|------|-------|
| 1 | Compilation model (preprocessor → compiler → assembler → linker) vs JS JIT | 2026-02-26 | Walked through all 4 stages with real output: -E, -S, -c, linking. Compared to V8 JIT pipeline. |
| 2 | Types: int, char, float, double, long, unsigned, sizeof | 2026-02-26 | Sizes, ranges, format specifiers, char as int, float precision, limits.h, sizeof. Overflow exercise completed. |
| 3 | Variables, constants, #define vs const | 2026-02-27 | Covered storage duration (auto/static/global), scope, #define vs const vs enum tradeoffs, preprocessor macro dangers, uninitialized variable UB. |

## Exercises
| # | Exercise | File | Status | Notes |
|---|----------|------|--------|-------|
| 1 | Explore the Build Pipeline | exercises/01_build_pipeline.c | completed | Passed. Ran all 4 stages. Minor style issue: space between # and directive. Didn't use -Wall -Wextra -Werror flags initially. |
| 2 | Type Overflow Explorer | exercises/02_overflow.c | completed | All 4 cases correct. Correctly identified UB vs defined wrapping. Explored all 4 build stages again with new code. Hit linker EISDIR error (tried -o build when build/ was a directory). |
| 3 | Constants & Variable Lifetime | exercises/03_constants_exercise.c | completed | All logic correct. SQUARE macro properly parenthesized, static next_id works, enum array sizing correct, block scoping demonstrated. Minor issues: typo in output, unused const, UPPER_CASE naming on a const variable (should be snake_case). |

## Mistakes & Weak Areas
- 2026-02-26: Used `# include` / `# define` with space after `#`. Technically valid but non-standard style. Clarified convention is `#include` / `#define`.
- 2026-02-26: Compiled without `-Wall -Wextra -Werror` flags. Explained that compiler flags are the C equivalent of a linter.
- 2026-02-27: Used UPPER_SNAKE_CASE for a `const` variable (`EGGS`). In C convention, UPPER_CASE is for macros and enum constants; `const` vars should be snake_case since they're still variables (with addresses), not true compile-time constants.

## Areas to Revisit
- [ ] Always use `-Wall -Wextra -Werror -g` when compiling

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
