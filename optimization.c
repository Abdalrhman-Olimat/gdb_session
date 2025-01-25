#include <stdio.h>
#include <time.h>

// Function to perform computation
unsigend long long compute(unsigend long long n) {
    unsigend long long sum = 0;
    for (unsigend long long i = 0; i < n; i++) {
        sum += i * i;
    }
    return sum;
}

int main() {
    clock_t start, end;
    double cpu_time_used;
    unsigend long long result;

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

