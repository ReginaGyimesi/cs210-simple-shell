/**
 * Header for declaring methods and constants
 */

#define TRUE 1
#define FALSE 0
#define ERROR (-1)
#define HISTORY_SIZE 20
#define MAX_INPUT_LENGTH 514
#define COMMANDS_LENGTH 5
#define DELIMITER_SIZE 8
#define MAX_TOKENS 50
#define MAX_DEPTH 5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>

struct alias{
    char* key;
    char* value;
    struct alias *next;
};

typedef struct alias Alias;
typedef struct alias** AList;

int set_directory();
void make_history();
void make_aliases();
int load_history();
int load_aliases();
int print_prompt();
char* read_input();
int add_to_history(char* input);
char** tokenise_string(char* input);
int apply_command(char** tokens);
int save_history();
void free_history();
int save_aliases();
int free_aliases();

int change_directory(char** tokens);
int exit1();
int getpath(char** tokens);
int setpath(char** tokens);
int print_history(char** tokens);
char** check_history_type(char** tokens);

char** substituteAlias(char** tokens);
char** check_alias(char** tokens);
void free_tokens(char** tokens);
Alias* get_aliases();
char* detokenize(char** tokens, char* value);