#include <stdio.h>
#include <time.h>

// Function to perform computation
int compute(int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += i * i;
    }
    return sum;
}

int main() {
    clock_t start, end;
    double cpu_time_used;
    int result;

    // Measure time taken by compute() function
    start = clock();
    result = compute(100000000);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Print result and time taken
    printf("Result: %d\n", result);
    printf("Time: %f seconds\n", cpu_time_used);

    return 0;
}

