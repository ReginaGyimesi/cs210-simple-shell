//
// Created by Zoltan Kiss on 27/01/2021.
//

#ifndef CS210_SIMPLE_SHELL_MAIN_H
#define CS210_SIMPLE_SHELL_MAIN_H

#endif //CS210_SIMPLE_SHELL_MAIN_H

#define TRUE 1
#define FALSE 0
#define ERROR -1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "set_directory.h"
#include "change_directory.h"
#include "tokenise.h"
#include "applycommand.h"
#include "prompt.h"
#include "history_handler.h"

typedef struct HISTORY {
	int number;
	char** command;
} History