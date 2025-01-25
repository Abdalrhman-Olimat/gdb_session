#include <stdio.h>
#include <time.h>

// Function to perform computation
unsigned long long compute(unsigned long long n) {
    unsigned long long sum = 0;
    for (unsigned long long i = 0; i < n; i++) {
        sum += i * i;
    }
    return sum;
}

int main() {
    clock_t start, end;
    double cpu_time_used;
    unsigned long long result;

    // Measure time taken by compute() function
    start = clock();
    result = compute(10000000000ULL);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Print result and time taken
    printf("Result: %lld\n", result);
    printf("Time: %f seconds\n", cpu_time_used);

    return 0;
}

