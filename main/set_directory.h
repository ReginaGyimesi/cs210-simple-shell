//
// Gets the HOME directory and sets it. Checks if it has been correctly set.
// Created by Regina Gyimesi on 09/02/2021.
//

#ifndef CS210_SIMPLE_SHELL_SET_DIRECTORY_H
#define CS210_SIMPLE_SHELL_SET_DIRECTORY_H

#endif //CS210_SIMPLE_SHELL_APPLYCOMMAND_H

// gets and sets directory to HOME, if home directory is null then exists
int set_directory() {
   if (chdir(getenv("HOME")) != 0) {
            perror("Failed");
        }

    // checks if current directory is set to home
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }
    return 0;
}
