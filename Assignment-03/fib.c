#include "header.h"
int fibonacci(int n) {
    if (n <= 1) {
        return n;
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}

int main() {
    printf("\nExecuting fib(41-45) fib4.out\n");
    int n=40;
    for(int i=36;i<=n;i++){
        printf("(%d)th Fibonacci number = %d\n", i, fibonacci(i));
    }

    // printf("Enter the value of n: ");
    // scanf("%d", &n);

    // if (n < 0) {
    //     printf("Please enter a non-negative integer.\n");
    // } else {
    //     printf("(%d)th Fibonacci number = %d\n", n, fibonacci(n));
    // }

    return 0;
}
