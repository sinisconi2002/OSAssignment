#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main (int argc, char **argv)
{
    char s[100];

    if (argc < 2)
    {
        printf("Please provide arguments!\n");
        exit(EXIT_FAILURE);
    }
    
    printf("%s\n", getcwd(s, 100));
    
    if (chdir(argv[1]) == -1)
    {
        perror("That directory doesn't exist");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("%s\n", getcwd(s, 100));
    }
    return 0;
}