#include "prompt.h"


static char *cwd=NULL;
static char *old_cwd=NULL;

static int time_by_pro=0;
static char* name_of_pro=NULL;
static int call=0;


char *get_old_cwd(){

    return old_cwd;
    
}

char *get_cwd(){
    return cwd;
}

void get_path_of_dir(char *name,char *dir_name){
    char curr[1000];
    if(getcwd(curr,1000)==NULL){
        printf("Error while fetching current directory");
        return ;
    }

    chdir(name);

    if(getcwd(dir_name,1000)==NULL){
        printf("Error while fetching current directory");
        return ;
    }


    chdir(curr);

}
void set_cwd(char *path_name){

    char path[4096];

    get_path_of_dir(path_name,path);

    if(cwd==NULL){
        cwd = strdup(path);
        return;
    }

    if(strcmp(path,cwd) != 0){
        if(old_cwd!=NULL){
            free(old_cwd);
        }

        old_cwd=strdup(cwd);
        free(cwd);
        cwd=strdup(path);
    }
    return;

}

void get_curr(char *path){
    if(getcwd(path,1000)==NULL){
        printf("Error while fetching current directory");
        return ;
    }
    set_cwd(path);

}

void time_taken(){

    blue();

    if(time_by_pro>=2){
        printf("%s %ds>",name_of_pro,time_by_pro);
        // printf("%ds>",time_by_pro);
    }
    reset();


    time_by_pro=0;

    name_of_pro=NULL;

    call=0;
    
}


void set_time_name(int time_f, char *name_f) {
   
    time_by_pro += time_f;

    char int_str[12]; // Enough to hold any 32-bit integer including negative sign and null terminator

   
    sprintf(int_str, "%d", time_f);

    
    size_t new_len = strlen(name_f) + strlen(int_str) + 2; // +1 for possible ":" +1 for null terminator

    // Reallocate memory for name_of_pro
    if (call == 0) {
        name_of_pro = malloc(new_len);
        if (name_of_pro == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        strcpy(name_of_pro, name_f);
    } else {
        name_of_pro = realloc(name_of_pro, strlen(name_of_pro) + new_len);
        if (name_of_pro == NULL) {
            perror("realloc failed");
            exit(EXIT_FAILURE);
        }
        strcat(name_of_pro, ":");
        strcat(name_of_pro,name_f);
    }

    // Concatenate the integer string to the name
    strcat(name_of_pro, int_str);

    call++;
}

