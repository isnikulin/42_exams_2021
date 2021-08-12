#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ERR_FATAL "error: fatal"
#define ERR_CD_ARG "error: cd: bad arguments"
#define ERR_CD_DIR "error: cd: cannot change directory to "
#define ERR_EXEC "error: cannot execute "

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define BREAK ";"
#define PIPE "|"

#define TYPE_END 3
#define TYPE_PIPE 4
#define TYPE_BREAK 5

typedef struct s_cmd {
	int fd[2];
	int type;
	int size;
	char *cmd;
	char **argv;
	struct s_cmd *prev;
	struct s_cmd *next;
} t_cmd;


/*	UTILS	*/
int ft_strlen(char *s) {
	int i = 0;
	if (s) {
		while (s[i]) {
			i++;
		}
	}
	return i;
}

char *ft_strdup(char *s) {
	if (!s) {
		return NULL;
	}
	int size = ft_strlen(s);
	char *ret = (char *) malloc(sizeof(char) * size + 1);
	ret[size] = 0;
	while (--size >= 0) {
		ret[size] = s[size];
	}
	return (ret);
}

void ft_lstaddback(t_cmd **start, t_cmd *new) {
	t_cmd *tmp = NULL;
	if (!(*start)) {
		*start = new;    //?
	} else {
		tmp = *start;
		while (tmp->next) {
			tmp = tmp->next;
		}
		tmp->next = new;
		new->prev = tmp;
	}
}

/*	EXIT functions	*/
void exit_message(char *s, bool flag_exit) {
	write(2, s, ft_strlen(s));
	write(2, "\n", 1);
	if (flag_exit)
		exit(EXIT_FAILURE);
}

void exit_2_messages(char *s, char *s2, bool flag_exit) {
	write(2, s, ft_strlen(s));
	write(2, s2, ft_strlen(s2));
	write(2, "\n", 1);
	if (flag_exit)
		exit(EXIT_FAILURE);
}


/*	parser	*/
int get_size_of_argv(char *argv[]) {
	int i = 0;
	while (argv[i] && strcmp(argv[i], BREAK) != 0 &&
		   strcmp(argv[i], PIPE) != 0) {
		i++;
	}
	return i;
}

int get_type(char *s) {
	int ret = TYPE_END;

	if (s && strcmp(s, PIPE) == 0) {
		ret = TYPE_PIPE;
	} else if (s && strcmp(s, BREAK) == 0) {
		ret = TYPE_BREAK;
	}
	return ret;
}

int parse_args(t_cmd **start, char *argv[]) {
	int size = get_size_of_argv(argv);
	t_cmd *tmp = (t_cmd *) malloc(sizeof(t_cmd));

	if (!tmp) {
		exit_message(ERR_FATAL, true);
	}
	tmp->argv = (char **) malloc(sizeof(char *) * (size + 1));
	if (!tmp->argv) {
		exit_message(ERR_FATAL, true);
	}
	tmp->size = size;
	tmp->next = NULL;
	tmp->prev = NULL;
	tmp->argv[size] = NULL;
	while (--size >= 0) {
		tmp->argv[size] = ft_strdup(argv[size]);
	}
	tmp->cmd = tmp->argv[0];
	tmp->type = get_type(argv[tmp->size]);
	ft_lstaddback(start, tmp);
	return tmp->size;
}


/*exec*/
void exec_cmd(t_cmd *tmp, char *env[]) {
	bool flag_pipe = false;
	pid_t pid;
	int status = 0;

	if (tmp->type == TYPE_PIPE || (tmp->prev && tmp->prev->type == TYPE_PIPE)) {
		flag_pipe = true;
		if (pipe(tmp->fd) < 0) {
			exit_message(ERR_FATAL, true);
		}
	}

	pid = fork();
	if (pid < 0) {
		exit_message(ERR_FATAL, true);
	} else if (pid == 0) {    //child
		if (tmp->type == TYPE_PIPE) {
			dup2(tmp->fd[STDOUT], STDOUT);
		}
		if (tmp->prev && tmp->prev->type == TYPE_PIPE) {
			dup2(tmp->prev->fd[STDIN], STDIN);
		}
		if (execve(tmp->cmd, tmp->argv, env) < 0) {
			exit_2_messages(ERR_EXEC, tmp->cmd, true);
		}
		exit(EXIT_SUCCESS);
	} else {    //parent
		waitpid(pid, &status, 0);
		if (flag_pipe) {
			close(tmp->fd[STDOUT]);
			if (!tmp->next || tmp->type == TYPE_BREAK) {
				close(tmp->fd[STDIN]);
			}
			if (tmp->prev && tmp->prev->type == TYPE_PIPE) {
				close(tmp->prev->fd[STDIN]);
			}
		}
		/*if (tmp->prev && tmp->prev->type == TYPE_PIPE) {
			close(tmp->prev->fd[STDIN]);
		}*/
	}
}

void exec_cmds(t_cmd *start, char *env[]) {
	t_cmd *tmp = start;

	while (tmp) {
		if (strcmp(tmp->cmd, "cd") == 0) {
			if (tmp->size != 2) {
				exit_message(ERR_CD_ARG, false);
			}
			if (chdir(tmp->argv[1]) < 0) {
				exit_2_messages(ERR_CD_DIR, tmp->argv[1], false);
			}
		} else {
			exec_cmd(tmp, env);
		}
		tmp = tmp->next;
	}
}

/* main */
int main(int argc, char *argv[], char *env[]) {
	if (argc <= 1)
		return (EXIT_SUCCESS);
	(void) env;
	int i = 1;
	t_cmd *start = NULL;

	while (argv[i]) {
		if (strcmp(argv[i], ";") == 0) {
			i++;
			continue;
		} else {
			i += parse_args(&start, &argv[i]);    //parse_args
		}
		if (!argv[i]) {
			break;
		} else {
			i++;
		}
	}
//	printf("Parsed struct, cycle: %d:\n size = '%d', type = '%d', cmd: %s, argv:\n", i, start->size,start->type, start->cmd);
//	for (int j = 0; j < start->size; ++j) {
//		printf("'%s'\n", start->argv[j]);
//	}
	if (start) {
		exec_cmds(start, env);
	}
//	while (1);
	return (EXIT_SUCCESS);

}
