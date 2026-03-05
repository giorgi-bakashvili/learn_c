#include <stdio.h>
#define MAX_STUDENTS 20
#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))

int grades[] = {85, 92, 67, 74, 95, 58, 83, 90};

int find_min(int grades[], int size) {
    int min = grades[0];

    for(int i = 0; i < size; i++) {
        if(grades[i] < min) {
            min = grades[i];
        }
    }

    return min;
}

int find_max(int grades[], int size) {
    int max = grades[0];

    for(int i = 0; i < size; i++) {
        if(grades[i] > max) {
            max = grades[i];
        }
    }

    return max;
}

float compute_average(int grades[], int size) {
    float sum = 0.0;
    float total = size;

    for (int i = 0; i < size; i++) {
        float grade = grades[i];

        sum += grade;
    }

    return sum / total;
}

int count_above(int grades[], int size, int threshold) {
    int count = 0;

    for (int i = 0; i < size; i++) {
        if (grades[i] > threshold) {
            count++;
        }
    }

    return count;
}

void curve_grades(int grades[], int size, int boost) {
    for (int i = 0; i < size; i++) {
        grades[i] = grades[i] + boost;
    }
}

int main(void) {
    int size = ARRAY_LEN(grades);

    printf("  === Student Grades ===\n");
    for (int i = 0; i < size; i++) {
        printf("grades[%d] = %d\n", i, grades[i]);
    }

    printf("=== Statistics ===\n");
    printf("Min: %d\n", find_min(grades, size));

    printf("Max: %d\n", find_max(grades, size));

    printf("Average: %.1f\n", compute_average(grades, size));

    printf("Above average: %d\n", count_above(grades, size, compute_average(grades, size)));

    printf("  === After +5 Curve ===\n");
    curve_grades(grades, size, 5);
    for (int i = 0; i < size; i++) {
        printf("grades[%d] = %d\n", i, grades[i]);
    }

    printf("  === Updated Statistics ===\n");
    int min = find_min(grades, size);
    printf("Min: %d\n", min);

    int max = find_max(grades, size);
    printf("Max: %d\n", max);

    float average = compute_average(grades, size);
    printf("Average: %.1f\n", average);

    int above_average = count_above(grades, size, average);
    printf("Above average: %d\n", above_average);
}