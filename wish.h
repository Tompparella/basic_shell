/*
Copyright: Tommi Kunnari 6.4.2021

This file helps importing all the necessary functions we've defined in wish_fkt.c
to the main file.
*/
void print_prompt(void);
void parse_cmd(char command[], char *params[], char filename[]);
int cd(char *path);
void print_array(char *array[]);
void super_advanced_exit_tool_5000();
void set_path(char *path[], char *params[]);
int check_cmd(char *params[], char *cmds[]);
void execute(char *path[], char command[], char *params[]);
void execute_r(char *path[], char command[], char *params[]);
void execute_p(char *path[], char command[], char *params[]);
void print_error(char err_msg[]);
