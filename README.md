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

- if **<dir>** is also provided and it refers to a valid directory, then the new entry will be created in the given directory.
