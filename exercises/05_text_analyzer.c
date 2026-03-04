
#include <stdio.h>

int main(void) {
    char* text = "Hello, World! 123 ... zzzzz OK?";
    int vowels = 0;
    int digits = 0;
    int spaces = 0;
    int other = 0;
    int length = 0;
    int current_run = 0;
    char current_char = '\0';
    int max_run = 0;
    char max_char = '\0';

    for (int i = 0; text[i] != '\0'; i++) {
        length++;

        switch (text[i]) {
            case 'a':
            case 'A':
            case 'e':
            case 'E':
            case 'i':
            case 'I':
            case 'o':
            case 'O':
            case 'u':
            case 'U':
                vowels++;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                digits++;
                break;
            case ' ':
                spaces++;
                break;
            default:
                other++;
                break;
        }

        if (text[i] == current_char) {
            current_run++;
        } else {
            current_run = 1;
            current_char = text[i];
        }

        if (current_run > max_run) {
            max_run = current_run;
            max_char = current_char;    
        }
    }

    
    printf("=== Text Analysis ===\n");
    printf("String: %s\n", text);
    printf("Length: %d\n", length);
    printf("Vowels: %d\n", vowels);
    printf("Digits: %d\n", digits);
    printf("Spaces: %d\n", spaces);
    printf("Other: %d\n", other);
    printf("Longest run: %d x %c\n", max_run, max_char);

    return 0;
}