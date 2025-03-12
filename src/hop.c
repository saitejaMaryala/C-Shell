#include "hop.h"

void hop(char *path, char *home)
{

    // char *newdir = (char*)malloc(PATH_MAX * sizeof(char));

    // get_curr(newdir);
    
    if (strcmp(path, "~") == 0)
    {
        if(chdir(home) == -1){
            printf("Error cannot find the directory!!\n");
            return;
        }
        set_cwd(home);
    }
    else if (strcmp(path, "-")==0)
    {
        char *one=get_old_cwd();
        if(chdir(one) == -1){
            printf("Error cannot find the directory!!\n");
            return;
        }
        set_cwd(one);
    }
    else
    {
        if(chdir(path)==-1){
            printf("Error cannot find the directory!!\n");
            return;
        }
        set_cwd(path);
    }

    char curr[PATH_MAX];
    if (getcwd(curr, sizeof(curr)) != NULL)
    {
        printf("\n%s\n", curr);
    }
    else
    {
        perror("Error in getting current working directory!!");
        return;
    }

    
}
