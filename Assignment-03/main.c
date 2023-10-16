#include "header.h"

// Assumption :- "1" means highest priority and "4" means the lowest priority.

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// Queue for the processes to run.
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

struct QNode
{
    pid_t key;
    int priority;
    char file_name[20];
    struct QNode *next;
    double execution_time;
    double waiting_time;
    double temp_wait;
};

struct Queue
{
    struct QNode *front, *rear;
};

struct QNode *newNode(pid_t k, char *name, int pr, double exec_time, double wait_time, double temp_wait)
{
    struct QNode *temp = (struct QNode *)malloc(sizeof(struct QNode));
    temp->key = k;
    strcpy(temp->file_name, name);
    temp->execution_time = exec_time;
    temp->waiting_time = wait_time;
    temp->next = NULL;
    temp->priority = pr;
    temp->temp_wait = temp_wait;
    return temp;
}

struct Queue *createQueue()
{
    struct Queue *q = (struct Queue *)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

void enQueue(struct Queue *q, pid_t k, char *name, int priority, double exec_time, double wait_time, double temp_wait)
{
    struct QNode *temp = newNode(k, name, priority, exec_time, wait_time, temp_wait);
    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
}

void deQueue(struct Queue *q)
{
    if (q->front == NULL || q->front->next == NULL)
    {
        return;
    }

    struct QNode *temp = q->front;

    q->front = q->front->next;
    q->rear->next = temp;
    q->rear = temp;
    q->rear->next = NULL;
}

void remove_process(struct Queue *q)
{
    if (q->front == NULL)
    {
        return;
    }
    else if (q->front->next == NULL)
    {
        struct QNode *temp = q->front;
        q->front = q->rear = NULL;
        free(temp);
    }
    else
    {
        struct QNode *temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
}

void display(struct Queue *queue)
{
    struct QNode *temp = queue->front;
    int i = 1;
    printf("\nQueue: ");
    printf("\nS.no  PID  File Name  Priority  Execution_time  Waiting_time  temp_wait \n ");
    while (temp != NULL)
    {
        printf("(%d). %d, %s, %d, %f, %f, %f\n", i, temp->key, temp->file_name, temp->priority, temp->execution_time, temp->waiting_time, temp->temp_wait);
        temp = temp->next;
        i++;
    }
    printf("\n");
}

void historyprinter(struct Queue *h)
{
    struct QNode *temp = h->front;
    int i = 1;
    printf("History: ");
    printf("\n+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=++=+=+=+=+=+=+=+=+=+=+\n");
    printf("S.no  PID  File Name  Priority  Execution_time  Waiting_time  Instance_wait\n");
    while (temp != NULL)
    {
        printf("(%d).  %d   %s      %d         %.3f       %.3f         %.3f\n", i, temp->key, temp->file_name, temp->priority, temp->execution_time, temp->waiting_time, temp->temp_wait);
        temp = temp->next;
        i++;
    }
    printf("\n+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=++=+=+=+=+=+=+=+=+=+=+\n");
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// Advanced Queue to prioritize the queue.
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

int NCPU;
float TSLICE;
volatile sig_atomic_t signal_received = 0;
struct Queue *q;
struct Queue *waiting_q;
struct Queue *h;

void enqueue_priority_start(struct Queue *q, pid_t k, char *name, int priority, double exec_time, double wait_time, double temp_wait)
{
    struct QNode *temp = newNode(k, name, priority, exec_time, wait_time, temp_wait);
    if (priority > 4)
    {
        printf("\nERROR :- Invalid priority (%d): valid range is [1-4].\n", priority);
        return;
    }
    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        return;
    }

    struct QNode *nex = q->front;
    struct QNode *prev = q->front;

    while (prev != NULL)
    {
        if (priority == 4)
        {
            if (nex != NULL)
            {
                if (nex->priority >= 4 && q->front == nex)
                {
                    q->front = temp;
                    q->front->next = nex;
                    break;
                }
                else if (prev->priority <= 3 && nex->priority >= 4)
                {
                    prev->next = temp;
                    temp->next = nex;
                    break;
                }
            }

            else if (prev->priority <= 3 && nex == NULL)
            {
                prev->next = temp;
                temp->next = nex;
                break;
            }
        }
        else if (priority == 3)
        {
            if (nex != NULL)
            {
                if (nex->priority >= 3 && q->front == nex)
                {
                    q->front = temp;
                    q->front->next = nex;
                    break;
                }
                else if (prev->priority <= 2 && nex->priority >= 3)
                {
                    prev->next = temp;
                    temp->next = nex;
                    break;
                }
            }

            else if (prev->priority <= 2 && nex == NULL)
            {
                prev->next = temp;
                temp->next = nex;
                break;
            }
        }
        else if (priority == 2)
        {
            if (nex != NULL)
            {
                if (nex->priority >= 2 && q->front == nex)
                {
                    q->front = temp;
                    q->front->next = nex;
                    break;
                }
                else if (prev->priority <= 1 && nex->priority >= 2)
                {
                    prev->next = temp;
                    temp->next = nex;
                    break;
                }
            }
            else if (prev->priority <= 1 && nex == NULL)
            {
                prev->next = temp;
                temp->next = nex;
                break;
            }
        }
        else if (priority == 1)
        {
            q->front = temp;
            q->front->next = prev;
            break;
        }
        else
        {
            printf("Invalid priority (%d): valid range is (1-4)\n", priority);
            break;
        }

        prev = nex;
        nex = nex->next;
    }
}

void enqueue_priority_end(struct Queue *q, pid_t k, char *name, int priority, double exec_time, double wait_time, double temp_wait)
{
    struct QNode *temp = newNode(k, name, priority, exec_time, wait_time, temp_wait);
    if (priority > 4)
    {
        printf("\nERROR :- Invalid priority (%d): valid range is [1-4].\n", priority);
        return;
    }

    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        return;
    }

    struct QNode *nex = q->front;
    struct QNode *prev = q->front;

    while (prev != NULL)
    {
        if (priority == 4)
        {
            if (nex == NULL)
            {
                prev->next = temp;
                temp->next = nex;
                break;
            }
        }
        else if (priority == 3)
        {
            if (nex != NULL)
            {
                if (nex->priority >= 4 && q->front == nex)
                {
                    q->front = temp;
                    q->front->next = nex;
                    break;
                }
                else if (prev->priority <= 3 && nex->priority >= 4)
                {
                    prev->next = temp;
                    temp->next = nex;
                    break;
                }
            }

            else if (prev->priority <= 3 && nex == NULL)
            {
                prev->next = temp;
                temp->next = nex;
                break;
            }
        }
        else if (priority == 2)
        {
            if (nex != NULL)
            {
                if (nex->priority >= 3 && q->front == nex)
                {
                    q->front = temp;
                    q->front->next = nex;
                    break;
                }
                else if (prev->priority <= 2 && nex->priority >= 3)
                {
                    prev->next = temp;
                    temp->next = nex;
                    break;
                }
            }
            else if (prev->priority <= 2 && nex == NULL)
            {
                prev->next = temp;
                temp->next = nex;
                break;
            }
        }
        else if (priority == 1)
        {
            if (nex != NULL)
            {
                if (nex->priority >= 2 && q->front == nex)
                {
                    q->front = temp;
                    q->front->next = nex;
                    break;
                }
                else if (prev->priority <= 1 && nex->priority >= 2)
                {
                    prev->next = temp;
                    temp->next = nex;
                    break;
                }
            }
            else if (prev->priority <= 1 && nex == NULL)
            {
                prev->next = temp;
                temp->next = nex;
                break;
            }
        }
        else
        {
            printf("\nERROR :- Invalid priority (%d): valid range is [1-4].\n", priority);
            break;
        }

        prev = nex;
        nex = nex->next;
    }
}

void dequeue_priority(struct Queue *q)
{
    if (q->front == NULL || q->front->next == NULL)
    {
        // printf("No process remaning to be executed");
        return;
    }

    struct QNode *temp = q->front;

    char *fname = q->front->file_name;
    pid_t tmppid = q->front->key;
    int priority = q->front->priority;
    double exec_time = q->front->execution_time;
    double wait_time = q->front->waiting_time;
    double temp_wait = q->front->temp_wait;

    if (priority < 4)
    {
        priority++;
    }

    enqueue_priority_end(q, tmppid, fname, priority, exec_time, wait_time, temp_wait);

    q->front = q->front->next;
    free(temp);
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// Schedular and RoundRoubin scheduling algorithm.
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

void RoundRobin()
{
    printf("Schedular started running...\n");
    display(q);
    historyprinter(h);
    double t1 = 0.0;
    double t2 = 0.0;

    while (q->front != NULL)
    {
        int index_start = 0;
        struct QNode *temp_start = q->front;

        while (index_start < NCPU && temp_start != NULL)
        {
            if (kill(temp_start->key, SIGCONT) == 0)
            {
                printf("Child process (PID: %d) has been resumed.\n", temp_start->key);
            }
            else
            {
                perror("kill");
            }
            temp_start = temp_start->next;
            index_start++;
        }

        usleep(TSLICE);
        int index = 0;
        t1 = t2;
        t2 += (TSLICE / 1000.0);

        while (index < index_start && q->front != NULL)
        {

            struct QNode *temp = q->front;
            int status;
            if (waitpid(temp->key, &status, WNOHANG) != 0)
            {
                printf("Child process (PID: %d) has terminated.\n", temp->key);
                temp->execution_time += (TSLICE / 1000.0);
                temp->waiting_time += (t1 - temp->temp_wait);
                temp->temp_wait = t2;
                enQueue(h, temp->key, temp->file_name, temp->priority, temp->execution_time, temp->waiting_time, temp->temp_wait);
                remove_process(q);
            }
            else
            {
                if (kill(temp->key, SIGSTOP) == 0)
                {
                    printf("Child process (PID: %d) has been stopped.\n", temp->key);
                    temp->execution_time += (TSLICE / 1000.0);
                    temp->waiting_time += (t1 - temp->temp_wait);
                    temp->temp_wait = t2;
                }
                else
                {
                    perror("kill");
                    printf("(PID: %d) has error in stopping.\n", temp->key);
                }
                dequeue_priority(q);
            }

            display(q);
            index++;
        }

        while (waiting_q->front != NULL)
        {
            printf("Adding process of waiting queue in the main queue...\n");
            pid_t pid = waiting_q->front->key;
            char *fname = waiting_q->front->file_name;
            int priority = waiting_q->front->priority;
            double exec_time = waiting_q->front->execution_time;
            double wait_time = waiting_q->front->waiting_time;
            double temp_wait = waiting_q->front->temp_wait;
            enqueue_priority_start(q, pid, fname, priority, exec_time, wait_time, temp_wait);
            remove_process(waiting_q);
            printf("Process added successfully in the main queue...\n");
        }
    }
    printf("\n-----> Schedular Executed all processes successfully <-----\n");
    // historyprinter(h);
    // exit(0);
    //     printf("\nNow Printing History!\n");
    //     printf("+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=++=+=+=+=+=+=+=+=+=+\n");
    //     historyprinter(h);
    //     printf("+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=++=+=+=+=+=+=+=+=+=+");
}

void *Schedular(void *arg)
{
    struct Queue *q = (struct Queue *)arg;

    while (1)
    {
        // printf("Entered...\n");
        if (signal_received)
        {
            struct QNode *node = q->front;
            if (node != NULL)
            {
                RoundRobin();
            }
            else
            {
                printf("No process remaining in the queue...\n");
            }
            signal_received = 0;
        }
        // printf("Finished...\n");
        sleep(5);
    }
    printf("Thread has been executed successfully...\n");
    return NULL;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// Signal Handler.
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

void my_handler(int signum)
{
    if (signum == SIGUSR1)
    {
        printf("\nBackground schedular received SIGUSR1 signal.\n");
        signal_received = 1;
    }

    else if (signum == SIGUSR2)
    {
        printf("\n-----> Waiting for the Schedular to finish all jobs <-----\n");
        if (q->front != NULL)
        {
            signal_received = 1;
        }

        while (1)
        {
            if (!signal_received)
            {
                break;
            }
        }
        historyprinter(h);
        exit(0);
    }
    else
    {
        char buff1[100] = "\n\n(\\/)\n(..)\n/> Program Terminated by the user\n\n";
        write(STDOUT_FILENO, buff1, 100);
        printf("\n-----> Waiting for the Schedular to finish all jobs <-----\n");

        if (q->front != NULL)
        {
            signal_received = 1;
        }

        while (1)
        {
            if (!signal_received)
            {
                break;
            }
        }
        historyprinter(h);
        exit(0);
    }
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// Main program to run simpleshell and schedular.
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

int main(int argc, char *argv[])
{
    struct sigaction sig;
    memset(&sig, 0, sizeof(sig));
    sig.sa_handler = my_handler;
    sigaction(SIGINT, &sig, NULL);
    sigaction(SIGUSR1, &sig, NULL);
    sigaction(SIGUSR2, &sig, NULL);

    q = createQueue();
    waiting_q = createQueue();
    h = createQueue();

    char perm_directory[4096];
    char temp_perm_directory[4096];
    getcwd(perm_directory, 4096);
    getcwd(temp_perm_directory, 4096);
    printf("Shell Has Been Started\n");
    printf("Shell Name: k@li\n");
    printf("Shell Author: Vishal(2022580) & Subham(2022510)\n\n");
    bool flag = true;

    NCPU = atoi(argv[1]);
    TSLICE = atof(argv[2]);
    TSLICE *= 1000.0;

    pthread_t tid;
    int sending_background = pthread_create(&tid, NULL, Schedular, &q);

    if (sending_background)
    {
        printf("Error creating thread; return code: %d\n", sending_background);
        return 1;
    }
    printf("\nSchedular Has Been Started\n\n");

    while (flag)
    {
        char curr_work_dir[4096];
        getcwd(curr_work_dir, 4096);
        strcat(perm_directory, "/");
        printf("K@li $-> ");
        size_t size = 1024;
        char arr_fgets[4096];
        fgets(arr_fgets, 4096, stdin);
        char *delin_ch = " \n";
        char *token_ch;
        char **given_input = (char **)malloc(4096 * sizeof(char *));
        token_ch = strtok(arr_fgets, delin_ch);
        int i = 0;
        given_input[i] = token_ch;
        while (token_ch != NULL)
        {
            token_ch = strtok(NULL, delin_ch);
            i++;
            given_input[i] = token_ch;
        }
        int arg_count = 0;
        while (given_input[arg_count] != NULL)
        {
            arg_count++;
        }
        if (arg_count == 0)
        {
            continue;
        }

        if (strcmp(given_input[0], "submit") == 0)
        {
            pid_t new_process = fork();

            // Storing the pid and file name in the process table i.e, queue named q

            if (arg_count == 2)
            {
                if (signal_received == 0)
                {
                    enqueue_priority_start(q, new_process, given_input[1], 1, 0.0, 0.0, 0.0);
                }
                else
                {
                    enQueue(waiting_q, new_process, given_input[1], 1, 0.0, 0.0, 0.0);
                }
            }
            else
            {
                if (signal_received == 0)
                {
                    enqueue_priority_start(q, new_process, given_input[1], atoi(given_input[2]), 0.0, 0.0, 0.0);
                }
                else
                {
                    enQueue(waiting_q, new_process, given_input[1], atoi(given_input[2]), 0.0, 0.0, 0.0);
                }
            }

            // Starting the execution of the executable file in child process
            // Stopping the execution of the file using parent process

            if (new_process == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            else if (new_process == 0)
            {
                execlp(given_input[1], given_input[1], NULL);
                perror("execlp");
            }
            else
            {
                if (kill(new_process, SIGSTOP) == 0)
                {
                    printf("Process (PID: %d) has been halted and added at the end of queue.\n", new_process);
                }
                else
                {
                    perror("kill");
                }
            }
        }

        else if (strcmp(given_input[0], "run") == 0)
        {
            // Sending signal to the schedular running in background...
            if (pthread_kill(tid, SIGUSR1) != 0)
            {
                perror("Signal sending failed");
                exit(1);
            }
        }

        else if (strcmp(given_input[0], "exit") == 0)
        {
            if (kill(getpid(), SIGUSR2) == 0)
            {
                printf("SIGUSR2 signal sent to PID %d.\n", getpid());
            }
            else
            {
                perror("kill");
            }
        }

        else if (strcmp(given_input[0], "show") == 0)
        {
            // To display all the process stored in the process table which are in ready state
            display(q);
        }

        else if (strcmp(given_input[0], "set") == 0)
        {
            // To set/change the NCPU and TSLICE value of the schedular
            printf("Enter the new TSLICE :- ");
            scanf("%f", &TSLICE);
            printf("NCPU: %d, TSLICE: %f\n", NCPU, TSLICE);
        }

        else if (strcmp(given_input[0], "history") == 0)
        {
            // To print the history
            printf("\nPrinting History....\n");
            historyprinter(h);
        }
    }
}
