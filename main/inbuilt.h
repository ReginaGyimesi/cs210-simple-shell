//
// Created by bilko on 2021. 02. 11..
//

#ifndef CS210_SIMPLE_SHELL_INBUILT_H
#define CS210_SIMPLE_SHELL_INBUILT_H

#endif //CS210_SIMPLE_SHELL_INBUILT_H

/*
*	header for our inbuilt,non-linux commands:cd,exit,getpath,setpath
*	further dev: Create your inbuilt function somewhere here, then add it in the apply command.h
*	you have to add its keyword to the inbuilt array AND you have to store its pointer in the function
*	pointers array at the corresponding index
*/


#include <dirent.h>
#include <errno.h>

int change_directory(char** tokens){
    if(tokens == NULL || *tokens == NULL || tokens[0] == NULL){
        perror("NULL pointer error in cd command\n");
        return ERROR;
    }

    if(tokens[1] == NULL){ // no parameter were given, set to user's home directory
        if(chdir(getenv("HOME"))!=0){
            perror("Failed to change HOME");
            return ERROR;
        }
        return TRUE;
    }
    else{               // some parameter were given, execute cd with that param
        if(chdir(tokens[1]) != 0){  // disregarding other params, only using the next param after cd
            perror("Failed to change");
            return ERROR;
        }
    }

    return TRUE;

}

int exit1()
{
    return 0;
}
int getpath(char** tokens)
{
    if(tokens[1] == NULL){
        const char *s = getenv("PATH");
        printf("PATH is currently set to :%s\n", (s != NULL) ? s : "getenv returned NULL");
        return 1;
    }
    else
        printf("Invalid arguments after getpath\n");


}
int setpath(char** tokens)
{
    if(tokens[2]!=NULL)
    {
        printf("Too many arguments for setpath\n");
        return -1
    }
    char * isDir;
    char path[100];
    const char delim[2]=":";
    isDir=strtok(tokens[1],delim);
    strcpy(path,isDir);
    while(isDir!=NULL)
    {

        //printf("These are the tokens:%s\n",isDir);

        DIR* dir=opendir(isDir);
        if(dir)
        {
            //printf("Directory exists\n");
        }
        else if(ENOENT==errno)
        {
            printf("The directory %s is not a valid PATH\n",isDir);
            return -1;
        }





        isDir=strtok(NULL,delim);
        if(isDir!=NULL)
        {
            strcat(path,delim);
            strcat(path,isDir);

        }



    }

    //printf("This is the path:%s\n",path);






    setenv("PATH",path,1);

    free(isDir);



    return 1;


}