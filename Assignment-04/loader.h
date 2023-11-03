/*
 * No changes are allowed to this file
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/wait.h>
#include <time.h>
#include <stdint.h>
#include <setjmp.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#include <elf.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <math.h>

void load_and_run_elf(char** exe);
void loader_cleanup();
