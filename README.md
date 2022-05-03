# OSAssignment
Write a C program that behaves like a shell: can provide basic information and functionalities via built-in commands.

In order to achieve this purpose, the following should be considered:

1\. A prompt functionality that allows for users to input their commands, one line at time.

2\. Synchronous command execution, the shell waits until each command is finished.

3\. The basic built-in shell commands:

   **- help** displays a help message.

   **- exit** exits the shell.

   **- cd \<dir\>** changes the current directory.

   **- pwd** prints the current directory.

   **- type \<entry\>** displays the type of the given entry (absolute or relative path), e.g. regular file, symbolic link, directory.

   **- create \<type\> \<name\> \[\<target\] \[\<dir\>\]** creates a new entry of the given <type>, where <type> may have one of the following values: 

**-f** to create a regular file,

**-l** to create a symbolic link, in which case the <target> has to also be provided

**-d** to create a directory.

- if **\<dir\>** is also provided and it refers to a valid directory, then the new entry will be created in the given directory.

4\. Support for executing UNIX commands, such as ls or cat, by implementing the following options:

- run **\<command\> \[\<arg1\> \<arg2\> … \]** will execute the given **\<command\>** with an arbitrary number of arguments (depending on the command).

- **status** will display the exit code (exit status) of the previously executed command.

5\. Support for connecting two commands using pipes (e.g. **ls | grep**). This should work as an argument for the **run** command (from point 4, e.g.  **run cat somefile.txt | wc -l**). Each command should be run in a different process.

Your shell will implement the builtin commands by using system calls or library functions that achieve the required purpose. External commands will be executed in child processes.

