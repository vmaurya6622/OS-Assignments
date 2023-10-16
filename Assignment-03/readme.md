file names along with the Fibonacci number range they calculate :-
they calculate this in a for loop and print the nth fibonacci number to the console :

-->   1.out : 1-20 \n
-->   2.out : 21-30 \n
-->   3.out : 31-35
-->   4.out : 36-40
-->   5.out : 41-45


_**General Instructions**_ :-

1. In the terminal based on UNIX run "gcc main.c -o main.out && ./main.out" without quotes to run the code.
2. submit the input files to save them in the queue.
3. you will be provided with a pid after you submit the program which you can use to track the process.
3. you can also add the process priority as eg. "submit ./1.out 2" this "2" can be any integral number being (1,2,3,4)
4. **Assumptions** we have assumed that 1 has the highest priority and 4 has the lowest priority.
5. you can use commands like
            "run"     : to start scheduling and start executing the processes with respect to the priority levels.
            "show"    : to show the queue contents.
            "exit"    : to exit from the simpleshell and simplescheduler.
            "history" : to print the history again to the console.
            "set"     : to set a new time slice to the program being nprocs same.
6. after finishing round-robin scheduling history will be printed to the console containing 
    **file_name, pid, execution time, waiting time, priority**.

                                                                *********
