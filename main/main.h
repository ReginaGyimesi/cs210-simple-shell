//
// Created by Zoltan Kiss on 27/01/2021.
//

#ifndef CS210_SIMPLE_SHELL_MAIN_H
#define CS210_SIMPLE_SHELL_MAIN_H
#define TRUE 1
#define FALSE 0
#define ERROR (-1)
#define HISTORY_SIZE 20
#define NUMBER_OF_DECIMALS 10
#define MAX_INPUT_LENGTH 514
#define COMMANDS_LENGTH 5
#define DELIMITER_SIZE 8
#define MAX_TOKENS 50

#define VERSION "Stage 5"

#endif //CS210_SIMPLE_SHELL_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>

#include "tokenise.h"
#include "inbuilt.h"
#include "history_handler.h"
#include "applycommand.h"
#include "set_directory.h"
#include "prompt.h"
