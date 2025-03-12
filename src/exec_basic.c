#include "exec_basic.h"




void exec_echo(char* str){

    char *args[]={"sh","-c",str,NULL};

    execvp(args[0],args);

}

void execute_basic_cmd(char *org_string,char **input,int size){

    input[size]=NULL;
    int id=fork();
    if(id==0){
    
        if(strcmp(input[0],"echo")==0){
            char *str=strdup(org_string);
            exec_echo(str);
        }
        if(execvp(input[0],input) == -1){
            printf("ERROR : %s is not a valid command\n",input[0]);
            exit(0);
        }
        exit(1);
    }else{
        wait(NULL);
        return;
    }
}

