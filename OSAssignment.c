#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

typedef struct arguments
{
    int noOfArgs;
    char arguments[10][100];
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
    args.noOfArgs = counter + 1;

    return args;
}

void myCD(char *dir)
{   
    char s[100];
    printf("%s\n", getcwd(s, 100));
    
    if (chdir(dir) == -1)
    {
        perror("That directory doesn't exist");
        exit(EXIT_FAILURE);
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
    printf("The manual for this script will be written ASAP!\n");
}

int main(int argc, char **argv)
{
    char s[100];
    ARGUMENTS args;

    /*
    if (argc < 2)
    {
        printf("Please provide arguments!\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp( argv[1], "pwd") == 0)
    {
        myPWD();
    }
    else if (strcmp( argv[1], "cd") == 0)
    {
        if (argc < 3)
        {
            printf("Please provide arguments!\n");
            exit(EXIT_FAILURE);
        }

        myCD(argv[2]);
        
    }else if (strcmp(argv[1], "type") == 0)
    {
        printf("TO BE IMPLEMENTED!\n");
    }
    else
    {
        printf("COMMAND UNKNOWN!\n");
    }
    */
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
        else if(strcmp(args.arguments[0], "pwd") == 0)
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
        else
        {
            printf("COMMAND UNKNOWN!\n");
        }
        
    }

    return 0;
}