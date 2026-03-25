/*
 * Lesson 14: Command-Line Arguments — argc & argv
 *
 * Every C program's main() can receive arguments from the shell.
 * This is how grep, gcc, git, and every Unix tool gets its input.
 *
 * The full signature:  int main(int argc, char *argv[])
 *
 *   argc  — "argument count", always >= 1
 *   argv  — "argument vector", array of C strings (char pointers)
 *   argv[0] = the program name (or path)
 *   argv[1..argc-1] = the actual arguments
 *   argv[argc] = NULL  (guaranteed by the C standard)
 *
 * In JS you'd use process.argv — same idea, but argv[0] in Node is
 * the node binary itself, argv[1] is the script. In C, argv[0] is
 * whatever was typed to run the program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Section 1: Basics — printing all arguments                         */
/* ------------------------------------------------------------------ */

static void demo_print_args(int argc, char *argv[]) {
    printf("=== Section 1: Printing All Arguments ===\n\n");

    printf("argc = %d\n", argc);
    printf("Program name (argv[0]): \"%s\"\n\n", argv[0]);

    for (int i = 0; i < argc; i++) {
        printf("  argv[%d] = \"%s\"\n", i, argv[i]);
    }

    /*
     * argv[argc] is guaranteed to be NULL by the C standard (C11 §5.1.2.2.1).
     * This means you can also loop with a NULL sentinel:
     */
    printf("\nSame thing with NULL-sentinel loop:\n");
    for (char **p = argv; *p != NULL; p++) {
        printf("  \"%s\"\n", *p);
    }

    /*
     * Why does this work? argv is really:
     *
     *   char *argv[] = { "program", "arg1", "arg2", ..., NULL };
     *
     * It's a NULL-terminated array of NULL-terminated strings.
     * Two levels of sentinel: each string ends with '\0',
     * and the array itself ends with a NULL pointer.
     */

    printf("\nProof that argv[argc] is NULL: %p\n",
           (void *)argv[argc]);

    printf("\n");
}

/* ------------------------------------------------------------------ */
/* Section 2: Converting string arguments to numbers                  */
/* ------------------------------------------------------------------ */

/*
 * argv entries are ALWAYS strings (char *). To use them as numbers,
 * you must convert. Two approaches:
 *
 * 1. atoi() / atof()  — simple but DANGEROUS
 *    - atoi("hello") returns 0  — no way to tell "0" from garbage
 *    - atoi("99999999999") overflows silently (undefined behavior)
 *    - atof() has the same problems for floats
 *
 * 2. strtol() / strtod()  — robust, use these
 *    - Reports where parsing stopped (endptr)
 *    - Sets errno on overflow/underflow
 *    - Lets you specify the base (10, 16, 0 for auto-detect)
 */

static void demo_number_conversion(void) {
    printf("=== Section 2: String-to-Number Conversion ===\n\n");

    /* --- atoi: the footgun --- */
    printf("atoi(\"42\")      = %d\n", atoi("42"));
    printf("atoi(\"hello\")   = %d  (silent garbage!)\n", atoi("hello"));
    printf("atoi(\"42abc\")   = %d  (stops at non-digit, no warning)\n",
           atoi("42abc"));

    /* --- strtol: the right way --- */
    printf("\nstrtol examples:\n");

    const char *inputs[] = { "42", "hello", "42abc", "0xFF",
                             "99999999999999999999", NULL };

    for (const char **s = inputs; *s != NULL; s++) {
        char *endptr;
        errno = 0;
        long val = strtol(*s, &endptr, 0);  /* base 0 = auto-detect */

        printf("  strtol(\"%s\", base=0): ", *s);

        if (endptr == *s) {
            printf("FAILED — no digits found\n");
        } else if (*endptr != '\0') {
            printf("%ld (partial — stopped at '%c')\n", val, *endptr);
        } else if (errno == ERANGE) {
            printf("OVERFLOW/UNDERFLOW (errno=ERANGE)\n");
        } else {
            printf("%ld (OK)\n", val);
        }
    }

    /*
     * strtol signature:
     *   long strtol(const char *str, char **endptr, int base);
     *
     * endptr is an out-parameter (pointer to pointer!) — it gets set
     * to point at the first character that wasn't part of the number.
     *
     *   "42abc"
     *    ^  ^
     *    |  endptr points here
     *    str
     *
     * If endptr == str, nothing was parsed.
     * If *endptr == '\0', the entire string was consumed.
     *
     * base = 0 means auto-detect:
     *   "0x" prefix → hex, "0" prefix → octal, otherwise decimal
     */

    printf("\n");
}

/* ------------------------------------------------------------------ */
/* Section 3: Common patterns — option parsing                        */
/* ------------------------------------------------------------------ */

/*
 * Real programs need to handle flags like:
 *   ./program --verbose --output file.txt -n 10
 *
 * For serious flag parsing, use getopt() or getopt_long() (POSIX).
 * But understanding the manual approach teaches you how argv works.
 */

static void demo_manual_parsing(int argc, char *argv[]) {
    printf("=== Section 3: Manual Argument Parsing ===\n\n");

    /* Simulated argv for demo purposes */
    (void)argc;  /* suppress unused warning */
    (void)argv;

    char *fake_argv[] = {
        "./mytool",
        "--verbose",
        "--output",
        "results.txt",
        "-n",
        "42",
        "input1.c",
        "input2.c",
        NULL
    };
    int fake_argc = 8;

    int verbose = 0;
    const char *output_file = NULL;
    int count = -1;

    int i = 1;  /* skip argv[0] */
    while (i < fake_argc) {
        if (strcmp(fake_argv[i], "--verbose") == 0 ||
            strcmp(fake_argv[i], "-v") == 0) {
            verbose = 1;
            i++;
        } else if (strcmp(fake_argv[i], "--output") == 0 ||
                   strcmp(fake_argv[i], "-o") == 0) {
            if (i + 1 >= fake_argc) {
                fprintf(stderr, "Error: %s requires an argument\n",
                        fake_argv[i]);
                return;
            }
            output_file = fake_argv[i + 1];
            i += 2;  /* consume flag AND its value */
        } else if (strcmp(fake_argv[i], "-n") == 0) {
            if (i + 1 >= fake_argc) {
                fprintf(stderr, "Error: -n requires a number\n");
                return;
            }
            char *endptr;
            errno = 0;
            long val = strtol(fake_argv[i + 1], &endptr, 10);
            if (*endptr != '\0' || errno == ERANGE) {
                fprintf(stderr, "Error: invalid number '%s'\n",
                        fake_argv[i + 1]);
                return;
            }
            count = (int)val;
            i += 2;
        } else if (fake_argv[i][0] == '-') {
            fprintf(stderr, "Unknown option: %s\n", fake_argv[i]);
            return;
        } else {
            break;  /* positional arguments start here */
        }
    }

    printf("Parsed options:\n");
    printf("  verbose     = %s\n", verbose ? "true" : "false");
    printf("  output_file = %s\n", output_file ? output_file : "(none)");
    printf("  count       = %d\n", count);
    printf("  positional args:\n");
    for (; i < fake_argc; i++) {
        printf("    \"%s\"\n", fake_argv[i]);
    }

    printf("\n");
}

/* ------------------------------------------------------------------ */
/* Section 4: A practical mini-tool — sum calculator                  */
/* ------------------------------------------------------------------ */

/*
 * This is what a real tiny CLI tool looks like.
 * Usage: ./14_argc_argv_demo sum 10 20 30
 * Output: sum = 60
 */

static int cmd_sum(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s sum <num1> [num2] [num3] ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    long total = 0;

    for (int i = 2; i < argc; i++) {
        char *endptr;
        errno = 0;
        long val = strtol(argv[i], &endptr, 10);

        if (*endptr != '\0' || endptr == argv[i]) {
            fprintf(stderr, "Error: '%s' is not a valid number\n", argv[i]);
            return EXIT_FAILURE;
        }
        if (errno == ERANGE) {
            fprintf(stderr, "Error: '%s' is out of range\n", argv[i]);
            return EXIT_FAILURE;
        }
        total += val;
    }

    printf("sum = %ld\n", total);
    return EXIT_SUCCESS;
}

/* ------------------------------------------------------------------ */
/* Section 5: argv memory layout — where do the strings live?         */
/* ------------------------------------------------------------------ */

static void demo_memory_layout(int argc, char *argv[]) {
    printf("=== Section 5: argv Memory Layout ===\n\n");

    /*
     * The argv strings are set up by the OS before main() runs.
     * On most systems, they live in a special area at the top of
     * the process's stack (above main's frame).
     *
     * Layout in memory:
     *
     *   High addresses (top of stack)
     *   ┌─────────────────────────┐
     *   │ "input2.c\0"            │  actual string data
     *   │ "input1.c\0"            │  (contiguous block)
     *   │ "./program\0"           │
     *   ├─────────────────────────┤
     *   │ NULL                    │  argv[argc]
     *   │ ptr → "input2.c"       │  argv[2]
     *   │ ptr → "input1.c"       │  argv[1]
     *   │ ptr → "./program"      │  argv[0]
     *   ├─────────────────────────┤
     *   │ argc = 3                │
     *   ├─────────────────────────┤
     *   │ main()'s stack frame    │
     *   └─────────────────────────┘
     *   Low addresses (stack grows down)
     *
     * Key points:
     * - argv is a POINTER TO the first element of that pointer array
     * - The string data is usually contiguous (but don't rely on it)
     * - The C standard says argv strings ARE modifiable (unlike string
     *   literals in .rodata). Some programs exploit this to change
     *   their name in `ps` output.
     */

    printf("Address of argv array itself: %p\n", (void *)argv);
    printf("Addresses of each string:\n");
    for (int i = 0; i < argc; i++) {
        printf("  argv[%d] at %p → \"%s\" (str at %p, len=%zu)\n",
               i, (void *)&argv[i], argv[i], (void *)argv[i],
               strlen(argv[i]));
    }

    /* Show that strings are often contiguous */
    if (argc >= 2) {
        ptrdiff_t gap = argv[1] - (argv[0] + strlen(argv[0]) + 1);
        printf("\nGap between end of argv[0] and start of argv[1]: %td bytes\n",
               gap);
        if (gap == 0) {
            printf("  → Strings are contiguous (typical on Linux/macOS)\n");
        }
    }

    /*
     * argv strings are modifiable (C11 §5.1.2.2.1):
     * "The parameters argc and argv and the strings pointed to by
     *  the argv array shall be modifiable by the program"
     *
     * This is different from string literals ("hello") which are NOT
     * modifiable. The OS copies command-line text to writable memory.
     */
    if (argc > 0) {
        printf("\nModifying argv[0]... (legal, unlike string literals)\n");
        char original = argv[0][0];
        argv[0][0] = 'X';
        printf("  argv[0] is now: \"%s\"\n", argv[0]);
        argv[0][0] = original;  /* restore */
    }

    printf("\n");
}

/* ------------------------------------------------------------------ */
/* Section 6: envp — the third argument to main()                     */
/* ------------------------------------------------------------------ */

/*
 * There's a common (but non-standard) extension: main can take a
 * third parameter — the environment:
 *
 *   int main(int argc, char *argv[], char *envp[])
 *
 * envp is a NULL-terminated array of "KEY=VALUE" strings, just like
 * argv but for environment variables.
 *
 * The portable alternative is extern char **environ or getenv():
 */

static void demo_environment(void) {
    printf("=== Section 6: Environment Variables ===\n\n");

    /* getenv() — the standard, portable way */
    const char *home = getenv("HOME");
    const char *path = getenv("PATH");
    const char *user = getenv("USER");

    printf("HOME = %s\n", home ? home : "(not set)");
    printf("USER = %s\n", user ? user : "(not set)");
    printf("PATH = %.60s%s\n",
           path ? path : "(not set)",
           (path && strlen(path) > 60) ? "..." : "");

    /*
     * getenv() returns a pointer to the environment's internal storage.
     * Do NOT free() it. Do NOT modify it. Copy if you need to keep it.
     *
     * For setting env vars from C: setenv(), unsetenv(), putenv()
     * (POSIX, not ISO C).
     */

    printf("\n");
}

/* ------------------------------------------------------------------ */
/* main — subcommand dispatch pattern                                 */
/* ------------------------------------------------------------------ */

/*
 * Many real tools (git, docker, cargo) use subcommands:
 *   git commit -m "msg"
 *   docker run -it ubuntu
 *
 * The pattern: argv[1] is the subcommand name, the rest are its args.
 */

static void print_usage(const char *progname) {
    fprintf(stderr, "Usage: %s <command>\n", progname);
    fprintf(stderr, "\nCommands:\n");
    fprintf(stderr, "  demo    Run all demo sections\n");
    fprintf(stderr, "  sum     Sum numbers: %s sum 10 20 30\n", progname);
    fprintf(stderr, "  help    Show this message\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        /* No subcommand — run all demos */
        demo_print_args(argc, argv);
        demo_number_conversion();
        demo_manual_parsing(argc, argv);
        demo_memory_layout(argc, argv);
        demo_environment();

        printf("=== Try These Commands ===\n\n");
        printf("  %s sum 10 20 30 40\n", argv[0]);
        printf("  %s help\n", argv[0]);
        printf("  %s demo extra args here\n\n", argv[0]);
        return EXIT_SUCCESS;
    }

    /* Subcommand dispatch */
    if (strcmp(argv[1], "demo") == 0) {
        demo_print_args(argc, argv);
        demo_number_conversion();
        demo_manual_parsing(argc, argv);
        demo_memory_layout(argc, argv);
        demo_environment();
    } else if (strcmp(argv[1], "sum") == 0) {
        return cmd_sum(argc, argv);
    } else if (strcmp(argv[1], "help") == 0 ||
               strcmp(argv[1], "--help") == 0 ||
               strcmp(argv[1], "-h") == 0) {
        print_usage(argv[0]);
    } else {
        fprintf(stderr, "Unknown command: %s\n\n", argv[1]);
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
