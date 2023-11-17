#include <iostream>
#include <list>
#include <functional>
#include <stdlib.h>
#include <pthread.h>
#include <cstring>
#include <chrono> //included extra can be removed used only for super precision of time
int user_main(int argc, char **argv);

struct ThreadData
{
    std::function<void(int)> lambda;
    int low;
    int high;
};

struct ThreadData2D
{
    std::function<void(int, int)> lambda;
    int low1;
    int high1;
    int low2;
    int high2;
};

static void *parallel_for_helper(void *arg)
{
    ThreadData *data = static_cast<ThreadData *>(arg);
    for (int i = data->low; i < data->high; ++i)
    {
        data->lambda(i);
    }
    return nullptr;
}

static void *parallel_for_helper_2d(void *arg)
{
    ThreadData2D *data = static_cast<ThreadData2D *>(arg);
    for (int i = data->low1; i < data->high1; ++i)
    {
        for (int j = data->low2; j < data->high2; ++j)
        {
            data->lambda(i, j);
        }
    }
    return nullptr;
}

void parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads)
{
    auto start = std::chrono::high_resolution_clock::now();
    std ::cout << "1D-Thread Creation Started\n";
    // Create threads
    pthread_t threads[numThreads];
    ThreadData data[numThreads];
    int chunk = (high - low)/numThreads;
    for (int i = 0; i < numThreads; ++i)
    {
        data[i].low=i*chunk; data[i].high=(i+1)*chunk;
        data[i].lambda = lambda;
        pthread_create(&threads[i], nullptr, parallel_for_helper,(void*)&data[i]);
    }

    // Join threads
    for (int i = 0; i < numThreads; ++i)
    {
        pthread_join(threads[i], nullptr);
    }

    std ::cout << "All 1D-Threads Completed\n";
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time for parallel_for(1D): " << duration.count() << " seconds\n";
}

/*Parallelizing for i*/
void parallel_for(int low1, int high1, int low2, int high2, std::function<void(int, int)> &&lambda, int numThreads)
{
    auto start = std::chrono::high_resolution_clock::now();
    std ::cout << "2D-Thread Creation Started\n";

    // Create threads
    pthread_t threads[numThreads];
    ThreadData2D data[numThreads];
    int chunk = (high1-low1)/numThreads;
    for (int i = 0; i < numThreads; ++i)
    {
        data[i].low1=i*chunk; data[i].high1=(i+1)*chunk;
        data[i].low2=low2; data[i].high2=high2;
        data[i].lambda = lambda;
        pthread_create(&threads[i], nullptr, parallel_for_helper_2d,(void*) &data[i]);
    }

    // Join threads
    for (int i = 0; i < numThreads; ++i)
    {
        pthread_join(threads[i], nullptr);
    }

    std ::cout << "All 2D-Threads Completed\n";
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time for parallel_for (2D): " << duration.count() << " seconds\n";
}

/*Parallelizing for j*/
// void parallel_for(int low1, int high1, int low2, int high2, std::function<void(int, int)> &&lambda, int numThreads)
// {
//     auto start = std::chrono::high_resolution_clock::now();
//     std ::cout << "2D-Thread Creation Started\n";

//     // Create threads
//     int loop_1 = (high1-low1);
//     pthread_t threads[numThreads * loop_1];
//     ThreadData2D data[numThreads * loop_1];
//     int chunk = (high2-low2)/numThreads;
//     int index = 0;
//     for (int i = 0; i < loop_1; ++i)
//     {
//       for (int j = 0 ; j < numThreads ; j++)
//       {
//           data[index].low1=i; data[index].high1=(i+1);
//           data[index].low2=j*chunk; data[index].high2=(j+1)*chunk;
//           data[index].lambda = lambda;
//           pthread_create(&threads[index], nullptr, parallel_for_helper_2d,(void*) &data[index]);
//           index++;
//       }
//     }

//     // Join threads
//     for (int i = 0; i < numThreads*loop_1; ++i)
//     {
//         pthread_join(threads[i], nullptr);
//     }

//     std ::cout << "All 2D-Threads Completed\n";
//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> duration = end - start;
//     std::cout << "Execution time for parallel_for (2D): " << duration.count() << " seconds\n";
// }

/* Demonstration on how to pass lambda as parameter.
 * "&&" means r-value reference. You may read about it online.
 */
void demonstration(std::function<void()> &&lambda)
{
    lambda();
}

int main(int argc, char **argv)
{
    /*
     * Declaration of a sample C++ lambda function
     * that captures variable 'x' by value and 'y'
     * by reference. Global variables are by default
     * captured by reference and are not to be supplied
     * in the capture list. Only local variables must be
     * explicity captured if they are used inside lambda.
     */
    int x = 5, y = 1;
    // std ::cout<<"Hello Hi";

    // Declaring a lambda expression that accepts void type parameter
    auto /*name*/ lambda1 = /*capture list*/ [/*by value*/ x, /*by reference*/ &y](void)
    {
        /* Any changes to 'x' will throw compilation error as x is captured by value */
        y = 5;
        std::cout << "====== Welcome to Assignment-" << y << " of the CSE231(A) ======\n";
        /* you can have any number of statements inside this lambda body */
    };
    // Executing the lambda function
    demonstration(lambda1); // the value of x is still 5, but the value of y is now 5

    int rc = user_main(argc, argv);

    auto /*name*/ lambda2 = [/*nothing captured*/]()
    {
        std::cout << "====== Hope you enjoyed CSE231(A) ======\n";
        /* you can have any number of statements inside this lambda body */
    };
    demonstration(lambda2);
    return rc;
}

#define main user_main
