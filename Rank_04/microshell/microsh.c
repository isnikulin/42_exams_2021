#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

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
		*start = new;
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
	write(STDERR, s, ft_strlen(s));
	write(STDERR, "\n", 1);
	if (flag_exit)
		exit(EXIT_FAILURE);
}

void exit_2_messages(char *s, char *s2, bool flag_exit) {
	write(STDERR, s, ft_strlen(s));
	write(STDERR, s2, ft_strlen(s2));
	write(STDERR, "\n", 1);
	if (flag_exit)
		exit(EXIT_FAILURE);
}

/*parser*/
int get_size_of_args(char **argv) {
	int i = 0;

	while (argv[i] && strcmp(argv[i], PIPE) != 0 &&
		   strcmp(argv[i], BREAK) != 0) {
		i++;
	}
	return i;
}

int get_type(char *s) {
	int i = TYPE_END;
	if (s && strcmp(s, PIPE) == 0) {
		i = TYPE_PIPE;
	} else if (s && strcmp(s, BREAK) == 0) {
		i = TYPE_BREAK;
	}
	return i;
}

int parse_args(t_cmd **start, char **argv) {
	int size = get_size_of_args(argv);
	t_cmd *tmp = (t_cmd *) malloc(sizeof(t_cmd));

	if (!tmp) {
		exit_message(ERR_FATAL, true);
	}
	tmp->argv = (char **) malloc(sizeof(char *) * (size + 1));
	if (!tmp->argv) {
		exit_message(ERR_FATAL, true);
	}
	tmp->argv[size] = NULL;
	tmp->prev = NULL;
	tmp->next = NULL;
	tmp->size = size;
	tmp->type = get_type(argv[size]);
	while (--size >= 0) {
		tmp->argv[size] = ft_strdup(argv[size]);
	}
	tmp->cmd = tmp->argv[0];
	ft_lstaddback(start, tmp);
	return (tmp->size);
}

/*exec*/
void exec_cmd(t_cmd *cmd, char *env[]) {
	bool pipes = false;
	int status = 0;

	if (cmd->type == TYPE_PIPE || (cmd->prev && cmd->prev->type == TYPE_PIPE)) {
		pipes = true;
		pipe(cmd->fd);
	}

	pid_t pid = fork();
	if (pid < 0) {
		exit_message(ERR_FATAL, true);
	} else if (pid == 0) {    //child
		if (cmd->type == TYPE_PIPE && dup2(cmd->fd[STDOUT], STDOUT) < 0) {
			exit_message(ERR_FATAL, true);
		}
		if (cmd->prev && cmd->prev->type == TYPE_PIPE &&
			dup2(cmd->prev->fd[STDIN], STDIN) < 0) {
			exit_message(ERR_FATAL, true);
		}
		if (execve(cmd->cmd, cmd->argv, env) < 0) {
			exit_2_messages(ERR_EXEC, cmd->cmd, true);
		}
		exit(EXIT_SUCCESS);
	} else {    //parent
		waitpid(pid, &status, 0);
		if (pipes) {
			close(cmd->fd[STDOUT]);
			if (!cmd->next && cmd->type == TYPE_BREAK) {
				close(cmd->fd[STDIN]);
			}
			if (cmd->prev && cmd->prev->type == TYPE_PIPE) {
				close(cmd->prev->fd[STDIN]);
			}
		}
	}
}

void exec_cmds(t_cmd *start, char *env[]) {
	t_cmd *tmp = start;
	while (tmp) {
		if (strcmp(tmp->cmd, "cd") == 0) {
			if (tmp->size != 2) {
				exit_message(ERR_CD_ARG, false);
			} else if (chdir(tmp->argv[1]) < 0) {
				exit_2_messages(ERR_CD_DIR, tmp->argv[1], false);
			}
		} else {
			exec_cmd(tmp, env);
		}
		tmp = tmp->next;
	}
}

/* main */
void free_all(t_cmd *ptr) {
	t_cmd *tmp = ptr;

	while (tmp) {
		ptr = tmp;
		if (tmp->argv) {
			for (int i = 0; i < tmp->size; i++) {
				free(tmp->argv[i]);
			}
			free(tmp->argv);
		}
		tmp = ptr->next;
		free(ptr);
	}
	ptr = NULL;
}

int main(int argc, char *argv[], char *env[]) {
	if (argc <= 1)
		return (EXIT_SUCCESS);
	int i = 1;
	t_cmd *start = NULL;

	while (argv[i]) {
		if (strcmp(argv[i], ";") == 0) {
			i++;
			continue;
		} else {
			i += parse_args(&start, &argv[i]);
		}
		if (!argv[i]) {
			break;
		} else {
			i++;
		}
	}
	if (start) {
		exec_cmds(start, env);
	}
	free_all(start);
	return (EXIT_SUCCESS);
}
