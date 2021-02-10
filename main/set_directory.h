//
// Gets the HOME directory and sets it. Checks if it has been correctly set.
// Created by Regina Gyimesi on 09/02/2021.
//

// Gets and sets directory to HOME, if home directory is null then exists.
int set_directory() {
    char *homedir = getenv("HOME");
    int setdir = chdir(homedir);

    if (homedir != NULL) {
        printf("Home directory in environment");
        printf("%s\n", setdir);
    }

    else {
        printf("Failed\n");
        return ERROR;
    }

    // Checks is the current working directory has been correctly set.
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
        return TRUE;
    }
    else {
        perror("getcwd() error");
        return ERROR;
    }
    return TRUE;
}

