# C Learning Progress

## Current Position
- **Phase**: Phase 1 — Foundations
- **Current Topic**: Compilation model (preprocessor → compiler → assembler → linker)
- **Last Session**: 2026-02-26

## Completed Topics
| # | Topic | Date | Notes |
|---|-------|------|-------|
| 1 | Compilation model (preprocessor → compiler → assembler → linker) vs JS JIT | 2026-02-26 | Walked through all 4 stages with real output: -E, -S, -c, linking. Compared to V8 JIT pipeline. |

## Exercises
| # | Exercise | File | Status | Notes |
|---|----------|------|--------|-------|
| 1 | Explore the Build Pipeline | exercises/01_build_pipeline.c | completed | Passed. Ran all 4 stages. Minor style issue: space between # and directive. Didn't use -Wall -Wextra -Werror flags initially. |

## Mistakes & Weak Areas
- 2026-02-26: Used `# include` / `# define` with space after `#`. Technically valid but non-standard style. Clarified convention is `#include` / `#define`.
- 2026-02-26: Compiled without `-Wall -Wextra -Werror` flags. Explained that compiler flags are the C equivalent of a linter.

## Areas to Revisit
- [ ] Always use `-Wall -Wextra -Werror -g` when compiling

## Session Log
### 2026-02-26
- Covered: Compilation model — preprocessor, compiler, assembler, linker. All 4 stages demonstrated with 01_hello.c
- Exercises: "Explore the Build Pipeline" — completed successfully
- Key takeaways: Saw preprocessor expand 8 lines to 574, saw ARM64 assembly, understood .o vs executable size difference. Learned compiler flags are the C linter.
