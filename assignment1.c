#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

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

int main(int argc, char **argv)
{
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

    while (1)
    {


    }

    return 0;
}