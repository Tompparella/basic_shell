#include <stdio.h>
#include <stdlib.h>
#include <wish.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>



/*
Copyright: Tommi Kunnari 6.4.2021

This is a simple command line interface (CLI, aka shell) program.
It was made as a part of a final project assignment submission for a bachelors'
course on OS- and systems programming. It contains the following built-in
shell commands:

cd - Allows the user to change the working directory.
path - Lets the user choose their path variables where system commands will be queried from.
exit - Exits the program.

It also includes some advanced functionality, such as redirecting output to files, and parallel
program execution using the '>' and '&' tags.
The program is divided into main-, function-, and header files (wish.c, wish_fkt.c, wish.h) and
includes a makefile for easy compiling (makefile).

This is the main function. Here we include the header file and
call functions from wish_fkt.c.
*/



int main (int args, char *argv[]){

	char command[100] = "", cmd[100], *params[50], *path[50], *cmds[5]; 			// Defining variables.
	cmds[0] = ""; cmds[1] = "cd"; cmds[2] = "path"; cmds[3] = "exit"; cmds[4] = NULL;
	pid_t pid;
	path[0] = "/bin/"; path[1] = NULL;

	if (args == 2) { 									// If a batch file is defined, read and execute it.
		parse_cmd(command, params, argv[1]);
                strcpy(cmd, path[0]);
                strcat(cmd, command);
                if (execv(cmd, params) == -1) {
			char err_msg[] = "Batch file invalid. Please check input and try again.\n";	// NOTE: The error messages are vague due to assignment restrictions.
			print_error(err_msg);
			exit(0);
		}
                return(0);

	} else if (args > 2) {
		char err_msg[] = "Batch commands only take one (1) argument.\n";
		print_error(err_msg);
		exit(0);
	} else { 										// If there's no batch file, enter interactive mode.

		while (strcmp(command, "exit")) {

			pid = fork();								/*												Execution is done via child processes, called with forks.												The parent program handles taking the input and parsing it.
												*/

			if (pid == 0) {
				switch(check_cmd(params, cmds)) {				/*
												The check_cmd function returns a value from 0-3.
												If the value is 1, execute the command normally.
												If it's 2, the user wants to redirect output to a file,
												so we'll call the specific execute function for that.
												If it's 3, the user wants parallel execution, so we'll
												call a function that parses the command and executes it as intended.
												*/
					case 1:
//						printf("Execute:\n");
						execute(path, command, params);
						break;
					case 2:
//						printf("Redirect:\n");
						execute_r(path, command, params);
						break;
					case 3:
//						printf("Parallel:\n");
						execute_p(path, command, params);
						wait(NULL);
						break;
				}
				return 0;

			} else if (pid > 0) {
				wait(NULL); 							// Pid is greater than 0, so we are the parent function. If there are child-processes, wait for them.
				print_prompt(); 						// Print the prompt
	                	parse_cmd(command, params, argv[1]); 				// Parse the command given
//				print_array(params);

				if (strcmp(command, "cd") == 0) { 				// We have to use our own cd- command due to it not being a system program.
	                                if (cd(params[1]) < 0) {
	                                        perror(params[1]);
					}
				} else if (strcmp(command, "path") == 0) { 			// Define the built in 'path' command, that allows the user to pick which system commands they want to use.
					set_path(path,params);
				}

			} else {								// Some error handling.
				char err_msg[] = "Error while processing the previous command.\n";
				print_error(err_msg);
			}
		}
	}
	super_advanced_exit_tool_5000(); 							// Our amazing exit tool.

}
