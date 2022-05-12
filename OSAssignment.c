#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/sysmacros.h>
#include <stdint.h>
#include <dirent.h>

#define INPUT_END 1                             
#define OUTPUT_END 0

typedef struct arguments
{
    int noOfArgs;
    char arguments[22][100];
}ARGUMENTS;

ARGUMENTS populateArguments(char *s)
{
    ARGUMENTS args;
    char *p;
    int counter = 0;
    p = strtok(s, " ");
    while(p)
    {
        strcpy(args.arguments[counter++], p);
        p = strtok(NULL, " ");
    }
    args.noOfArgs = counter;

    return args;
}

void myCD(char *dir)
{   
    char s[100];
    printf("%s\n", getcwd(s, 100));
    
    if (chdir(dir) == -1)
    {
        perror("That directory doesn't exist");
    }
    else
    {
        printf("%s\n", getcwd(s, 100));
    }
}

void myPWD()
{
    char s[100];
    printf("%s\n", getcwd(s, 100));
}

void printHelp()
{
    printf("- help displays a help message.\n- exit exits the shell.\n");

    printf("- cd <dir> changes the current directory.\n");

    printf("- pwd prints the current directory.\n");

    printf("- type <entry> displays the type of the given entry (absolute or relative path), e.g. regular file, symbolic link, directory.\n");

    printf("- create <type> <name> [<target>] [<dir>] creates a new entry of the given <type>, where <type> may have one of the following values: \n");

    printf("    -f to create a regular file,\n");

    printf("    -l to create a symbolic link, in which case the <target> has to also be provided\n");

    printf("    -d to create a directory.\n");

    printf("    - if <dir> is also provided and it refers to a valid directory, then the new entry will be created in the given directory.\n");

    printf("- run <command> [<arg1> <arg2> … ] will execute the given <command> with an arbitrary number of arguments (depending on the command).\n");
    printf("    - status will display the exit code (exit status) of the previously executed command.\n");
    printf("    -it can connect 2 commands via a pipe\n");
}

void fileType(char *entry)
{
    struct stat status;
    if(lstat(entry, &status) == -1){
        perror("The file cannot be found!\n");
    }
    else{
    switch (status.st_mode & S_IFMT) {
           case S_IFBLK:  printf("block device\n");            break;
           case S_IFCHR:  printf("character device\n");        break;
           case S_IFDIR:  printf("directory\n");               break;
           case S_IFIFO:  printf("FIFO/pipe\n");               break;
           case S_IFLNK:  printf("symnolic link\n");                 break;
           case S_IFREG:  printf("regular file\n");            break;
           case S_IFSOCK: printf("socket\n");                  break;
           default:       printf("unknown?\n");                break;
           }
    }      
}

void createStuff(ARGUMENTS args)
{
    if(strcmp(args.arguments[1], "-f") == 0)
    {
        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        int fd;
        if (args.noOfArgs == 3)
        {
            fd = creat(args.arguments[2], mode);
        }
        else
        {
            if(args.arguments[3][strlen(args.arguments[3]) -1] != '/')
                strcat(args.arguments[3], "/");
            fd = creat(strcat(args.arguments[3], args.arguments[2]), mode);
        }
        if (fd == -1)
        {
            perror("File could not be created!");
        }
        
    }
    else if (strcmp(args.arguments[1], "-l") == 0)
    {
     //   symlink(strcat(strcat(),args.arguments[2], args.arguments[3]);
    }
    else if (strcmp(args.arguments[1], "-d") == 0)
    {
        int creator;
        if (args.noOfArgs == 3)
        {
            creator = mkdir(args.arguments[2], 0777);
        }
        else
        {
            if(args.arguments[3][strlen(args.arguments[3]) -1] != '/')
                strcat(args.arguments[3], "/");
            creator = mkdir(strcat(args.arguments[3], args.arguments[2]), 0777);
        }
        if (creator == -1)
        {
            perror("Directory could not be created!");
        }
    } 
}

void myRun(ARGUMENTS args)
{   

    pid_t pid1;
    pid_t pid2;                                  
    int fd[2];
    char **cmd1 = malloc(0);
    char **cmd2 = malloc(0);
    int isPipe = 0;
    int pipeIndex;

    int j1 = 0;
    for (int i = 1; i < args.noOfArgs && isPipe != 1; i++)
    {
        if( strcmp(args.arguments[i], "|") == 0)
        {
            pipeIndex = i;
            isPipe = 1;
        }
        else
        {
            j1++;
            cmd1 = realloc(cmd1, j1 * sizeof (char*));
            cmd1[j1 - 1] = malloc(100 * sizeof(char));
            strcpy(cmd1[j1 - 1], args.arguments[i]);
        }
        
    }
    
    int j2 = 0;
    if(isPipe == 1){
        for (int i = pipeIndex + 1; i < args.noOfArgs; i++){
            j2++;
            cmd2 = realloc(cmd2, j2 * sizeof (char*));
            cmd2[j2 - 1] = malloc(100 * sizeof(char));
            strcpy(cmd2[j2-1], args.arguments[i]);
        }
    }

    if(isPipe == 1)
    {
        pipe(fd);                                  
        pid1 = fork();                               
        if(pid1 < 0)
        {
            printf("Problems while creating the fork!\n");
        }
        else                                            
            if(pid1==0)                                  
            {                                            
                close(fd[INPUT_END]);                    
                dup2(fd[OUTPUT_END], STDIN_FILENO);      
                close(fd[OUTPUT_END]);                   
                execvp(cmd2[0], cmd2); 
                printf("The command could not be run!\n");  
            }                                            
            else                                         
            {                                            
                pid2=fork();                             
                if(pid2 < 0)
                {
                    printf("Problems while creating the fork!\n");
                }
                else                                        
                    if(pid2==0)                              
                    {                                        
                        close(fd[OUTPUT_END]);               
                        dup2(fd[INPUT_END], STDOUT_FILENO);  
                        close(fd[INPUT_END]);                
                        execvp(cmd1[0], cmd1);
                        printf("The command could not be run!\n");
                    }                                        
                                                        
                close(fd[OUTPUT_END]);                  
                close(fd[INPUT_END]);                    
                int status1, status2 = 0;
                waitpid(pid1, &status1, 0);
                printf("Status for the 1st command: %d\n", status1);               
                waitpid(pid2, &status2, 0);
                printf("Status for the 2nd command: %d\n", status2);     
            }
    }
    else
    {
        pid1 = fork(); 
        if(pid1 < 0)
        {
            printf("Problems while creating the fork!\n");
        }
        if (pid1 == 0)
        {
            execvp(cmd1[0], cmd1);
            printf("The command could not be run!\n");
        }
        int status1 = 0;
        waitpid(pid1, &status1, 0);
        printf("Status for the command: %d\n", status1);
    }

    for (int i = 0; i < j1; i++)
        free(cmd1[i]);
    free(cmd1);
    for (int i = 0; i < j2; i++)
        free(cmd2[i]);
    free(cmd2);
}

int main(int argc, char **argv)
{
    char s[100];
    ARGUMENTS args;

    while (1)
    {
        printf("Please provide an option:\n");
        fgets(s, 100, stdin);
        s[strlen(s) - 1] = '\0';
        args = populateArguments(s);
        if (strcmp(args.arguments[0], "help") == 0)
        {
            printHelp();
        }
        else if (strcmp(args.arguments[0], "exit") == 0) 
        {
            exit(0);
        }
        else if (strcmp(args.arguments[0], "pwd") == 0)
        {
            myPWD();
        }
        else if (strcmp( args.arguments[0], "cd") == 0)
        {
            if (args.noOfArgs < 2)
            {
                printf("Please provide arguments!\n");
            }
            else
            {
                myCD(args.arguments[1]);
            }
        }
        else if (strcmp( args.arguments[0], "type") == 0)
        {
            if (args.noOfArgs < 2)
            {
                printf("Please provide arguments!\n");
            }
            else
            {
                fileType(args.arguments[1]);
            }
        }
        else if (strcmp( args.arguments[0], "create") == 0)
        {
            if (args.noOfArgs < 2)
            {
                printf("Please provide arguments!\n");
            }
            else if (strcmp( args.arguments[1], "-l") == 0)
            {
                if (args.noOfArgs < 3)
                {
                    printf("Please provide arguments!\n");
                }
            }
            else if( !(strcmp( args.arguments[1], "-f") == 0 || strcmp( args.arguments[1], "-l") == 0 || strcmp( args.arguments[1], "-d") == 0))
            {
                printf("THE COMMAND IS UNKOWN!\n");
            }
            else
            {
                createStuff(args);
            }
        }
        else if (strcmp (args.arguments[0], "run") == 0)
        {
            if (args.noOfArgs < 2)
            {
                printf("Please provide arguments!\n");
            }
            else
            {
                myRun(args);   
            }
        }
        else
        {
            printf("COMMAND UNKNOWN! Type 'help' in order to see the commands!\n");
        }
    }

    return 0;
}