#ifndef MINIHSELL_H
#define MINIHSELL_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <time.h>
#include <signal.h>
#include <dirent.h>
#include <limits.h>
#include <termios.h>
#include "memory_management/cgc.h"
#include "execution/execution.h"

# define NONE -1
# define SECURE_PATH "PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:."
# define PARSE_ERROR "Parse error near: "
# define QUOTE_ERROR "Parse error: open quotes\n"
# define PARA_ERROR "Parse error: open parantheses\n"
# define SYNTAX_ERROR_STATUS 258
# define NUMERIC_ARG_STATUS 255
enum e_token
{
	WORD = 0,
	COMMAND = 0,
	WHITE_SPACE = ' ',
	NEW_LINE = '\n',
	QUOTE = '\'',
	DOUBLE_QUOTE = '\"',
	ENV = '$',
	PIPE_LINE = '|',
	REDIR_IN = '<',
	REDIR_OUT = '>',
	OPEN_PARANTHESE = '(',
	CLOSE_PARANTHESE = ')',
	STAR = '*',
	ENV_STAR,
	AND,
	OR,
	HERE_DOC,
	DREDIR_OUT,
};



enum e_state
{
	IN_DQUOTE,
	IN_QUOTE,
	IN_PARENTHESES,
	GENERAL,
} ;

enum e_treetypes
{
	OPERATOR,
	CONDITION
};

enum e_modes
{
	LEFT,
	RIGHT
};

enum e_wait_modes
{
	NO_WAIT,
	WAIT
};

enum e_malloc_types
{
	MANUAL,
	GC
};

enum e_error_codes
{
	PARSING_ERROR = 1,
	MALLOC_ERROR
};

enum e_bulitins
{
	ECHO_CMD,
	EXIT,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV_CMD,
};

enum e_replace_modes
{
	NO_REPLACE,
	ONLY_ENV,
	REPLACE_ALL,
};

enum e_star_modes
{
	SEEN,
	HIDDEN
};

typedef struct s_env
{
	char *value;
	bool before_joinable;
	bool after_joinable;
	int star_to_replace;
	bool envyable;
	struct s_env *next;
	struct s_env *prev;
} t_env;

typedef struct s_arg
{
	char *content;
	int to_replace;
	int join_count;
	int token;
	bool before_joinable;
	bool after_joinable;
	struct s_arg *next;
	struct s_arg *prev;
}	t_arg;

typedef struct s_lex
{
	char *content;
	int len;
	int to_replace;
	int	join_count;
	enum e_token token;
	int here_doc_fd; 
	struct s_lex *prev;
	struct s_lex *next;
} t_lex;

typedef struct s_redir
{
	int token;
	int to_replace;
	int here_doc_replacer;
	t_arg *redir_input;
	char *redir_string;
	char *here_doc_buffer;
	bool actual_here_doc;
	struct s_redir *next;
	struct s_redir *prev;
}	t_redir;


typedef struct s_cmd_arg
{
	t_arg *arg;
	struct s_cmd_arg *next;
	struct s_cmd_arg *prev;
} t_cmd_arg;

typedef struct s_middle
{
	int		token;
	char	*content;
	t_arg	*command;
	t_arg	*args;
	t_arg	*redirections;
	t_cmd_arg *cmd_arg;
	char	*redir_string;
	char	*here_doc_buffer;
	int		to_replace;
	int		builtin;
	struct	s_middle *next;
	struct	s_middle *prev;
}	t_middle;


typedef struct	s_middle_vars
{
	t_middle	*head;
	t_middle	*current;
	bool	in_command;
	t_arg	*command;
}	t_middle_vars;


typedef struct	s_bool_syntax
{
	bool	in_para;
	bool	in_quote;
	bool	in_dquote;
}	t_bool_syntax;

typedef struct s_treenode
{
	int		token;
	char	*content;
	char	*minimzed;
	t_arg	*args;
	t_arg	*command;
	t_cmd_arg *cmd_arg;
	int		to_replace;
	t_redir	*before_redir;
	t_redir	*after_redir;
	int		builtin;
	bool	is_a_directory;
	struct	s_treenode *left;
	struct	s_treenode *right;
} t_treenode;

typedef struct s_data t_data;
// LEXER
t_lex *tokenizer(char *input);
void quotes_handler(t_lex *lexed);
t_lex	*ft_lstnew_lex(char *content, int token, int len, int join_count);
t_lex	*ft_lstlast_lex(t_lex *lst);
t_lex	*ft_lstfirst_lex(t_lex *lst);
void	ft_lstadd_back_lex(t_lex **lst, t_lex *new);
void ft_lstiter_lex(t_lex *lex);
t_lex *lex_input_checker(t_lex *tokens);
const char* tokenToString(int t);
int open_checker(t_lex *token);
char **args_to_arr(t_arg *arg);
bool is_special(char c);
void fake_open(t_lex *lex, t_lex *final);
t_lex* skip_sp(t_lex *lex, int mode);

// MIDDLE MAN
t_middle	*ft_lstnew_middle(t_arg *command, t_cmd_arg *cmd_arg, int token);
void	ft_lstadd_back_middle(t_middle **lst, t_middle *new);
t_middle *make_middle(t_lex *lex);
void ft_lstiter_middle(t_middle *first);
t_middle	*ft_lstlast_middle(t_middle *lst);
t_arg *ft_lstnew_arg(t_lex *word);
t_arg *ft_lstlast_arg(t_arg *head);
t_arg *ft_lstfirst_arg(t_arg *tail);
void ft_lstaddback_arg(t_arg **head, t_arg *new);
int ft_lstsize_arg(t_arg *arg);
t_cmd_arg *ft_lstnew_cmd_arg(t_arg *arg);
void ft_lstaddback_cmd_arg(t_cmd_arg **lst, t_cmd_arg *new);
t_cmd_arg	*ft_lstlast_cmd_arg(t_cmd_arg *lst);
void ft_lstiter_arg(t_arg *arg);
void ft_lstiter_cmd_arg(t_cmd_arg *cmd_arg);
void valid_here_doc(t_middle *middle);
int after_env_star(char *str);

// ABSTRACT SYNTAX TREE
t_treenode		*new_treenode(t_middle *middled);
int counter(t_lex *current, char c);
t_treenode *ruined_tree(t_middle *middled);
t_treenode *parse_cmdline(t_middle **middled);
t_treenode *parse_pipeline(t_middle **middled);
t_redir *ft_lstnew_redir(t_middle *middled);
void ft_lstaddback_redir(t_redir **head, t_redir *new);
void ft_lstiter_redir(t_redir *first);
t_redir *ft_lstlast_redir(t_redir *first);
void print_ascii_tree(t_treenode *root, int level); // FOR DEBUGGING -- SHOULD BE REMOVED LATER

// UTILS
char	*ft_substr(const char *s, unsigned int start, size_t len, int mode);
char	*ft_strdup(const char *s1, int mode);
size_t	ft_strlen(const char *s);
void ft_putstr_fd(int fd, char *str);
char	*ft_strjoin(const char *s1, const char *s2, int mode);
int	ft_strcmp(const char *s1, const char *s2);
char	**ft_split(char const *s, char c, int mode);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char	*ft_strchr(const char *s, int c);
int	ft_atoi(const char *str, bool *exit_status);
char	*ft_itoa(int n, int mode);
size_t	ft_strcpy(char *dst, const char *src);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int	numlen(int n);
size_t ft_strncpy(char *dst, const char *src, size_t dstsize);
int	ft_isalpha(int c);
bool is_c_num(char c);
int ft_strcasecmp(const char *s1, const char *s2) ;
char *access_real_path(const char *path, const char *loop_for);

// EXPANDER
void prep_cmd_arg(t_cmd_arg **cmd_arg, t_env *env);
char *args_to_str(t_arg *args);
t_env *star_matching(char *to_match);
t_arg *arg_star_matching(char *to_match);
t_cmd_arg *cmd_arg_star_matching(char *to_match);
char *normalize_pattern(char *pattern);
char *get_real_env(char *value);
void expand_node(t_treenode *root, t_env **env);
t_lex *heredoc_tokenizer(char *input);

// BUILTINS
int cd(t_treenode *cd_root, t_env **env, t_data *data);
int pwd(t_data *data);
int env(t_env *env);
char *get_pwd(void); // GC does not free this!
int export(t_env **env, t_treenode *export_root);
int unset(t_env **env, t_treenode *unset_root);
int echo(t_treenode *echo_root);
int exit_cmd(t_treenode *root);
int export_core(t_env **env, char *exp_arg);
int exit_core(int status);

// ENV STUFF
bool is_env(char *str);
t_env *array_to_env(char **env);
void ft_lstiter_env(t_env *env); // DEBUG
void ft_lstclear_env(t_env *env);
t_env	*ft_lstnew_env(char *env, int mode);
void	ft_lstadd_back_env(t_env **lst, t_env *new);
t_env *get_env(t_env *env, char *str);
int ft_lstsize_env(t_env *env);
char **env_to_array(t_env *env);
int change_status(t_env **env, int new_status); // CAN FAIL
void sort_env_list(t_env *head);
void set_joinables(char *str, bool *before_joinable, bool *after_joinable);
t_env	*ft_lstlast_env(t_env *lst);
t_env *copy_env(t_env *env);

// STAR STUFF
char *no_stars(char *path);
bool is_a_directory(char *path);
bool is_path(char *str);
void expand_arg_as_star(t_arg **head);

#endif