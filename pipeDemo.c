#include <fcntl.h>                              //
#include <stdio.h>                              //
#include <stdlib.h>                             //
#include <string.h>                             //
#include <sys/types.h>                          //
#include <sys/wait.h>                           //
#include <sys/stat.h>                           //
#include <termios.h>                            //
#include <unistd.h>                             //
                                                 //
#define INPUT_END 1                              // INPUT_END means where the pipe takes input
#define OUTPUT_END 0                             // OUTPUT_END means where the pipe produces output
                                                 //
int main(int argc, char* argv[])                 //
{                                                //
    pid_t pid1;                                  // [STDIN -> terminal_input, STDOUT -> terminal_output]                       (of the parent process)
    pid_t pid2;                                  //
    int fd[2];                                   //
                                                 //
    pipe(fd);                                    // [STDIN -> terminal_input, STDOUT -> terminal_output, fd[0] -> pipe_input, fd[1] -> pipe_output]
    pid1 = fork();                               //
                                                 //
    if(pid1==0)                                  //
    {                                            // I am going to be the wc process (i.e. taking input from the pipe)
        close(fd[INPUT_END]);                    // [STDIN -> terminal_input, STDOUT -> terminal_output, fd[1] -> pipe_output] (of the WC process)
        dup2(fd[OUTPUT_END], STDIN_FILENO);      // [STDIN -> pipe_output, STDOUT -> terminal_output, fd[1] -> pipe_output]    (of the WC process)
        close(fd[OUTPUT_END]);                   // [STDIN -> pipe_output, STDOUT -> terminal_output]                          (of the WC process)
        execlp("wc", "wc", "-l",(char*) NULL);   //
    }                                            //
    else                                         //
    {                                            //
        pid2=fork();                             //
                                                 //
        if(pid2==0)                              //
        {                                        // I am going to be the ls process (i.e. producing output to the pipe)
            close(fd[OUTPUT_END]);               // [STDIN -> terminal_input, STDOUT -> terminal_output, fd[0] -> pipe_input] (of the ls process)
            dup2(fd[INPUT_END], STDOUT_FILENO);  // [STDIN -> terminal_input, STDOUT -> pipe_input, fd[0] -> pipe_input]      (of the ls process)
            close(fd[INPUT_END]);                // [STDIN -> terminal_input, STDOUT -> pipe_input]                           (of the ls process)
            execlp("ls","ls","-l",(char*) NULL); //
        }                                        //
                                                 //
        close(fd[OUTPUT_END]);                   // [STDIN -> terminal_input, STDOUT -> terminal_output, fd[0] -> pipe_input] (of the parent process)
        close(fd[INPUT_END]);                    // [STDIN -> terminal_input, STDOUT -> terminal_output]                      (of the parent process)
        waitpid(-1, NULL, 0);                    // As the parent process - we wait for a process to die (-1) means I don't care which one - it could be either ls or wc
        waitpid(-1, NULL, 0);                    // As the parent process - we wait for the another process to die.
                                                 // At this point we can safely assume both process are completed
    }                                            //
}    