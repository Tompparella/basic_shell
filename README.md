/*
Copyright: Tommi Kunnari 6.4.2021
  ***************************************
This is a simple command line interface (CLI, aka shell) program.
It was made as a part of a final project assignment submission for a bachelors'
course on OS- and systems programming. It contains the following built-in
shell commands:
  ***************************************
cd - Allows the user to change the working directory.
path - Lets the user choose their path variables where system commands will be queried from.
exit - Exits the program.
  ***************************************
It also includes some advanced functionality, such as redirecting output to files, and parallel
program execution using the '>' and '&' tags.
  ***************************************
The program is divided into main-, function-, and header files (wish.c, wish_fkt.c, wish.h) and
includes a makefile for easy compiling (makefile).
  ***************************************
This is the main function. Here we include the header file and
call functions from wish_fkt.c.
  ***************************************
To get started, simply navigate to the main folder and type 'make' to the commandline.
This will compile the program for you.
  ***************************************
The syntax of the shell is easy. To run it on a linux platform, type './wish'.
If you wish to run a batch file, type './wish batchfilename' (note that batch files can only run one command.
It's a bit of a gimmick so far).
In interactive mode the shell takes commands just like any other shell-program. For example,
'wish> ls -l' executes the ls function with an -l flag.
  ***************************************
Built in commands work as follows:
cd -    Takes one parameter, which is the directory the user wants to navigate to. That's it.
path -  With no arguments, prints the current path-variables. With arguments, such as 'path /bin/ /testfolder/'
        it adds both '/bin' and '/testfolder/' as path variables.
exit -  I don't think i need to explain this one.

*/
