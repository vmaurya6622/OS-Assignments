#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

void cat_function(int arg_count, const char *curr_work_dir, const char *given_input)
{
    if (arg_count != 2)
    {
        printf("Usage: cat <filename>\n");
    }
    else
    {
        char file_path[4096];
        snprintf(file_path, sizeof(file_path), "%s/%s", curr_work_dir, given_input);
        FILE *file = fopen(file_path, "r");
        if (file == NULL)
        {
            perror("fopen");
            printf("cat : Error : Unable to open file '%s'\n", given_input);
        }
        else
        {
            char line[4096];
            while (fgets(line, sizeof(line), file))
            {
                printf("%s", line);
            }
            fclose(file);
            printf("\n");
        }
    }
}

int main()
{
    char perm_directory[4096];
    char temp_perm_directory[4096];
    getcwd(perm_directory, 4096);
    getcwd(temp_perm_directory, 4096);

    printf("Shell Has Been Started\n");
    printf("Shell Name: k@li\n");
    printf("Shell Author: Vishal(2022580) & Subham(2022510)\n");

    while (1)
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
        else
        {
            if (strcmp(given_input[0], "echo") == 0)
            {
                for (int i = 1; i < arg_count; i++)
                {
                    printf("%s ", given_input[i]);
                }
                printf("\n");
            }
            else if (strcmp(given_input[0], "cat") == 0)
            {
                if (arg_count != 2)
                {
                    printf("Usage: cat <filename>\n");
                }
                else
                {
                    cat_function(arg_count, curr_work_dir, given_input[1]);
                }
            }
            else if (strcmp(given_input[0], "exit") == 0)
            {
                printf("Exited Successfully!");
                exit(0);
            }
            // else if(strcmp(given_input[0]==""))
            else
            {
                printf("Command not supported yet or not a command!\n");
                continue;
            }
        }
    }
    return 0;
}
