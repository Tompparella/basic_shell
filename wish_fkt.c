#include <wish.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

/*
Copyright: Tommi Kunnari 6.4.2021

This is the file where most of the programs' functionality comes from.
All of the functions used in the main file are defined here.
*/

void print_error(char err_msg[]) {
	char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));
	write(STDERR_FILENO, err_msg, strlen(err_msg));
}

void print_prompt() {						// Prints the shell prompt
	printf("wish> ");
}

int cd(char *path) { 						// Homebrewn cd command using chdir.
	return chdir(path);
}

void super_advanced_exit_tool_5000() { 				// The amazing exit function.
//	printf("You have called the 'super advanced exit tool 5000'.\nThis is a tool of dark origin that's built by a grand IT-wizard and possesses unimaginable power.\nBeware, for the program is about to be terminated in a manner most foul...\n");
//	sleep(1);
	exit(0);
}

void execute(char *path[], char command[], char *params[]) {	// Executes a system program based on path variables and the parsed command.
	char cmd[100];
	int x = 0;
	while (path[x] != NULL) {       			// Check all of the path variables.
	        strcpy(cmd, path[x]);
	        strcat(cmd, command);

	        if (execv(cmd, params) == -1) { 		// Attempt to execute the command with a path. If it fails, try with the next path variable.
	                x++;
	                continue;
	        }
	        break;
	}
	char err_msg[] = "The command was not found on current path. Please check input or choose a different path.\n";
	print_error(err_msg);					// Prints the user an error in case the command wasn't found in any paths.
}

void execute_p(char *path[], char command[], char *params[]) {	// A function that executes two commands parallel to eachother.

        char *params1[50];
        int x = 0;
	pid_t pid;

	int determined = 0;
	int end = 0;

	while (1) {
		pid = fork();

		if (pid == 0) {					// If we're a child process and parameters are parsed, execute command 2.
			if (determined == 1) {
//				printf("Params: "); print_array(params1);
	                        execute(path, params1[0], params1);
			} else return;

		} else if (pid > 0) {				// If we're the parent process and the parameters aren't parsed, parse the commands. If parameters are parsed, execute command 1.
			if (end == 1) {
				return;
			}

			int y = 0;

		        while (strcmp(params[x], "&") != 0) {
//				printf("%s\n", params[x]);
		                params1[y] = params[x];
		                x++; y++;
			        if (params[x] == NULL) {
                                	end = 1;
                                	break;
				}

		        }
		        params1[y] = NULL;
			x++;
			determined = 1;

		} else {					// Error handling in case there's a problem with the fork.
			char err_msg[] = "Error with fork.\n";
			print_error(err_msg);
			return;
		}
	}
}

void execute_r(char *path[], char command[], char *params[]) {	// Function that executes the parsed command, but redirects its output to a file defined by the user.

        char cmd[100];
	char *new_params[50];
        int x = 0;
	int index;

	while (strcmp(params[x], ">") != 0) {
		new_params[x] = params[x];
		x++;
	}

	new_params[x] = NULL;
	index = x+1;
								// Open the user-defined file as writeable, and if the file doesn't exist, create the file. In case of errors, present them.
	int file = open(params[index], O_WRONLY | O_CREAT, 0777);

	if (file == -1) { 
		char err_msg[] = "Error while opening the file. Redirection failed.\n";
		print_error(err_msg);
		return;
	}

	int stdout_save = dup(STDOUT_FILENO);			// Save the original stdout (commandline) to a token.

	dup2(file, STDOUT_FILENO);				// Define the file as the new stdout.
	close(file);

	x = 0;

        while (path[x] != NULL) {       			// Check all of the path variables.
                strcpy(cmd, path[x]);
                strcat(cmd, command);
                if (execv(cmd, new_params) == -1) { 		// Attempt to execute the command with a path.
			x++;
                        continue;
                }
		dup2(stdout_save, STDOUT_FILENO); 		// Return output to original stdout (commandline).
                return;
        }							// Print errors.
        char err_msg[] = "The command was not found on current path. Please check input or choose a different path.\n";
	print_error(err_msg);
}


void set_path(char *path[], char *params[]) { 			// A prebuilt function that allows the user to define different paths. Without parameters, it prints the current working paths.
	int i = 1;
	int y = 0;
	while (params[i] != NULL) {
		path[y] = params[i];
		i++; y++;
	}
	if (y == 0) {
		print_array(path);
		return;
	}
	path[y] = NULL;
}

int check_cmd(char *params[], char *cmds[]) {			// Check the input for built-in commands (cd, path, exit), and for redirections or parallelisations (>, &).
	int i = 0;

	while (cmds[i] != NULL) {				// If system commands are found, return 0.
//		printf("%s and %s\n", cmds[i], params[0]);
		if (!strcmp(cmds[i], params[0])) {
//			printf("%s and %s are the same.\n", cmds[i], params[0]);
			return 0;
		}
		i++;
	}
	i = 0;
	while (params[i] != NULL) {				// If redirection is wanted, return 2, and for parallelisation return 3.
		if (!strcmp(params[i], ">")) {
			return 2;
	        } else if (!strcmp(params[i], "&")) {
			return 3;
	        }
		i++;
	}
	return 1;						// If nothing special is found, return 1.
}

void print_array(char *array[]) { 				// A helper function used to print arrays (used mostly for development purposes).
	int i = 0;
	while (array[i] != NULL) {
		printf("%s ", array[i++]);
	}
	printf("\n");
}
								// Parses the given command line argument.
void parse_cmd(char command[], char *params[], char filename[]) {
	char prompt[1024];
	int no = 0, x = 0;
	char *parsed[100], *pch;
	size_t size = 1024;
	char *p;

	p = prompt;

	if (!filename) { 					// If there's no batch file defined, take user input.
		getline(&p, &size, stdin);
	} else {
//		printf("Batch:%s\n", filename); 		// ... otherwise take input from the batch file.
		FILE *file;
		file = fopen(filename,"r");
		if (file == NULL) {
			char err_msg[] = "No file with that name found.\n";
			print_error(err_msg);
			exit(1);
		} else {
			while (getline(&p, &size, file) != -1) {
				fputs(p, stdout);
			}
			fclose(file);
		}
	}
	if (no == 1) {						// No need to parse if there's no arguments.
		return;
	}

	pch = strtok(prompt, " \n");

	while (pch != NULL) {					// Parse the line as tokens until there's no more tokens left (ie. the line has ended.)
		parsed[x++] = strdup(pch);
		pch = strtok(NULL, " \n");
	}
	strcpy(command,parsed[0]); 				// Command is the first given argument.

	for (int d = 0; d < x; d++) {				// Parse the parameters.
		params[d] = parsed[d];
	}
	params[x] = NULL;					// End the parameter array with a null value for ease-of-access.
}
