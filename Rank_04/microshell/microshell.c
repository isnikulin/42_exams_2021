#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_FATAL "error: fatal\n"
#define ERROR_EXEC "error: cannot execute "
#define ERROR_CD_ARG "error: cd: bad arguments\n"
#define ERROR_CD_DIR "error: cd: cannot change directory to "

#define TYPE_END		3
#define TYPE_PIPE		4
#define TYPE_BREAK		5

typedef struct s_data {	//struct of input command
	char **argv;		//array of arguments
	int size;			//amount of arguments
	int type;			//type of command (?)
	int fd[2];
	struct s_data *prev;
	struct s_data *next;
}	t_data;

/*	utils	*/
int ft_strlen(char *s) {
	int i = 0;

	if (s) {
		while (s[i])
			i++;
	}
	return i;
}

char *ft_strdup(char *s) {

	if (!s) {
		return NULL;
	}
	int size = ft_strlen(s);
	char *new = (char *) malloc(sizeof(char) * size+1);
	if (!new) {return NULL;}
	new[size] = 0;
	while (--size >= 0) {
		new[size] = s[size];
	}
	return new;
}

void ft_lstaddback(t_data **start, t_data *new) {
	t_data *tmp;

	if (!(*start))
		*start = new;
	else
	{
		tmp = *start;
		while (tmp->next) {
			tmp = tmp->next;
		}
		tmp->next=new;
		new->prev=tmp;
	}
}

/* Exit functions */
void exit_fatal() {
	write(2, ERROR_FATAL, ft_strlen(ERROR_FATAL));
	exit(EXIT_FAILURE);
}
/*	parser */
/*
 * Count how many elements of argv are actual command/argument, before delimeters or end of array
 */
int size_argv(char **argv) {
	int i = 0;
	while (argv[i] && strcmp(argv[i], "|") != 0 && strcmp(argv[i], ";") != 0) {
		i++;
	}
	return i;
}

int check_type(char *s) {
	int ret = TYPE_END;
	if (!strcmp(s, ";")) {
		ret = TYPE_BREAK;
	} else if (!strcmp(s, "|")) {
		ret = TYPE_PIPE;
	}
	return ret;
}

/*
 * Parsing of argv, how many arguments per command?
 */
int parser_argv(t_data **ptr, char **argv) {
	int size = size_argv(argv);
	t_data *tmp = (t_data *)malloc(sizeof(t_data));	//new element of doubly linked list

	if(!tmp) {
		exit_fatal();
	} else if (!(tmp->argv = (char **) malloc(sizeof(char *) * (size + 1)))) {
		//allocated space for strings with command & argument
		exit_fatal();
	}
	tmp->size = size;
	tmp->next = NULL;
	tmp->prev = NULL;
	tmp->argv[size] = NULL;
	while (--size >= 0) {
		tmp->argv[size] = ft_strdup(argv[size]);	//dup all contents of the command
	}
	tmp->type = check_type(argv[tmp->size]); 		//check what type of command is next (?)
	ft_lstaddback(ptr, tmp);
	return (tmp->size);		//return amount of strings in array
}

int main(int argc, char *argv[], char *env[]) {
	t_data *ptr = NULL;
	int i = 1;
	(void )env;
	if (argc > 1) {
		while (argv[i]) {	//arg after execname
			if (!strcmp(";", argv[i])) {
				i++;
				continue;
			}
			i += parser_argv(&ptr, &argv[i]);
			if (!argv[i])
				break;
			else
				i++;
		}
	}
	while (1)
		i++;
	return (EXIT_SUCCESS);
}
