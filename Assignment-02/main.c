#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>
// #include<sys/wait.h>
// #include<pthread.h>
#include<stdbool.h>


void cat_function(){
    
}


int main(){
    char perm_directory[4096];
    char temp_perm_directory[4096];
    getcwd(perm_directory, 4096);  
    getcwd(temp_perm_directory, 4096);
    // printf("%s\n",perm_directory);
    // printf("%s\n",temp_perm_directory);
    printf("Shell Has BeenStarted\n");
    printf("Shell Name: k@li\n");
    printf("Shell Author: Vishal(2022580) & Subham(2022510)\n");
    while(1){
        char cwd_curr[4096];
        getcwd(cwd_curr, 4096);
        strcat(perm_directory, "/");
        printf("K@li $-> ");

        size_t size = 1024;
        char arr_fgets[4096];
        fgets(arr_fgets, 4096, stdin);

        char *delim = " \n";
        char *token;
        char **input = (char**)malloc(4096 * sizeof(char*));
        token = strtok(arr_fgets, delim);
        int i = 0;
        input[i] = token;
        while(token != NULL){
            token = strtok(NULL, delim);
            i++;
            input[i] = token;
        }
        int arg_count = 0;

        while(input[arg_count] != NULL){
            arg_count++;
        }
        if(arg_count == 0){
            continue;
        }
        else{
            if(strcmp(input[0],"echo")==0){
                for(int i=1;i<arg_count;i++){
                    printf("%s ",input[i]);
                }
                printf("\n");
            }
            else if(strcmp (input[0],"cat")==0){
                printf("%s\n",input[1]);
                char *args[]={"cat",input[1],NULL};
                execvp("cat", args);
                // if(arg_count==0){
                //     printf("cat : Too many arguments to handle!\n");
                // }
                // if(arg_count>3){
                //     printf("cat : Too many arguments to handle!\n");
                // }
                // printf("Command Not Found.\n");
                perror("execvp");
                return 1;
            }
        }
    }
}