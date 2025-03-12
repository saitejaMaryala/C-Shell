#include "seek.h"


void seek(char **cmds, int size, char *home)
{

    int e_flag = 0, d_flag = 0, f_flag = 0;

    int index = 1;

    while (index < size)
    {
        char *cmd = cmds[index];

        int cmd_len = strlen(cmd);

        if (cmd[0] == '-' && cmd_len > 1)
        {

            for (int i = 1; i < cmd_len; i++)
            {
                if (cmd[i] == 'd')
                    d_flag = 1;

                else if (cmd[i] == 'e')
                    e_flag = 1;

                else if (cmd[i] == 'f')
                    f_flag = 1;

                else
                {
                    printf("Invalid flas!!\n");
                    return;
                }
            }
        }
        else
        {
            break;
        }
        index++;
    }

    if (d_flag == 1 && f_flag == 1)
    {
        printf("Invalid flags!!\n");
        return;
    }

    if (index >= size)
    {
        printf("NO Name given for search!!!\n");
        return;
    }

    if ((size - index) > 2)
    {
        printf("Too many arguments are provided!!!\n");
        return;
    }

    char *name = cmds[index++];

    remove_extension(name);

    char *name_dir = index < size ? cmds[index] : NULL;

    char path[4096];

    if (name_dir == NULL)
    {
        if(getcwd(path,4096)==NULL){
        printf("Error while fetching current directory");
        return ;
    }
    }
    else if (strcmp(name_dir, "~") == 0)
    {
        strcpy(path, home);
    }
    else
    {
        strcpy(path, name_dir);
    }

    int got_one=0;

    if (!e_flag)
    {
        
        seek_fd(f_flag, d_flag, e_flag,name, path,path,&got_one);

    }else{
        int count=count_name(d_flag,f_flag,name,path);

        if(count > 1 &&(d_flag || f_flag)){
            printf("Multiple files with same name\n");
            return;
        }else{
            seek_fd(f_flag, d_flag, e_flag,name, path,path,&got_one);
        }

    }

    if(!got_one){
        printf("NO Match found!!\n");
    }
}

void remove_extension(char *filename) {
    char *dot = strrchr(filename,'.');
    if (dot != NULL) {
        *dot = '\0';
    }
}

int alphasort(const struct dirent **a, const struct dirent **b) {
    return strcasecmp((*a)->d_name, (*b)->d_name);
}



void get_rel_path(const char *filepath, const char *s_path, char *relpath) {
    if (strncmp(filepath, s_path, strlen(s_path)) == 0) {
        snprintf(relpath, 4096, ".%s", filepath + strlen(s_path));
    } else {
        snprintf(relpath, 4096, "%s", filepath);
    }
}


void seek_fd(int f_flag, int d_flag, int e_flag,char* name, char *path,char *s_path,int *got_one)
{


    int find_all = !f_flag && !d_flag;
    struct dirent **fileinfo;
    struct stat statbuf;

    int n = scandir(path, &fileinfo, NULL, alphasort);

    for (int i = 0; i < n; i++)
    {

        if (strcmp(fileinfo[i]->d_name, ".") == 0 || strcmp(fileinfo[i]->d_name, "..") == 0)
            continue;

        char filepath[4096] = {};

        char* item_name = strdup(fileinfo[i]->d_name);

        strcat(filepath, path);
        strcat(filepath, "/");
        strcat(filepath, fileinfo[i]->d_name);

        char relpath[4096];
        get_rel_path(filepath,s_path,relpath);

        if (lstat(filepath, &statbuf) == -1) {
            perror("lstat");
            continue;
        }

        if(S_ISDIR(statbuf.st_mode)){

            if((d_flag || find_all) &&(strncmp(name,item_name,strlen(name))==0)){

                *got_one=1;
                blue();

                printf("%s\n",relpath);

                reset();

                if(e_flag && d_flag){
                    if(access(filepath,X_OK)!=0){
                        printf("Missing permission for directory %s\n",item_name);
                        return;
                    }

                    char curr_path[4096];
                    realpath(filepath,curr_path);

                    if(chdir(curr_path)){
                        perror("Error while changing the directoyr!!\n");
                        return;
                    }
                    else{
                        set_cwd(curr_path);
                    }
                }
            }
            seek_fd(f_flag, d_flag, e_flag,name, filepath,s_path,got_one);

        }

        else if(S_ISREG(statbuf.st_mode)){
            if((f_flag || find_all) && (strncmp(name,item_name,strlen(name))==0)){

                *got_one=1;
                green();

                printf("%s\n",relpath);

                reset();


                if(e_flag && f_flag){

                    printf("\n");
                    if(access(filepath,R_OK) != 0){
                        printf("Missing permission for reading the file %s\n",item_name);
                        return;
                    }

                    FILE *rdfile=fopen(filepath,"r");

                    char line[4096];

                    while (fgets(line,4096,rdfile))
                    {
                        printf("%s",line);
                    }
                    printf("\n");
                    fclose(rdfile);
                    
                }

            }

        }

    }
}



int count_name(int d_flag,int f_flag,char* name,char* path){

    int cnt=0;
    int find_all = !f_flag && !d_flag;
    struct dirent **fileinfo;
    struct stat statbuf;

    int n = scandir(path, &fileinfo, NULL, alphasort);

    for (int i = 0; i < n; i++)
    {

        if (strcmp(fileinfo[i]->d_name, ".") == 0 || strcmp(fileinfo[i]->d_name, "..") == 0)
            continue;

        char filepath[4096] = {};

        char* item_name = strdup(fileinfo[i]->d_name);

        strcat(filepath, path);
        strcat(filepath, "/");
        strcat(filepath, fileinfo[i]->d_name);


        if (lstat(filepath, &statbuf) == -1) {
            perror("lstat");
            continue;
        }

        if(S_ISDIR(statbuf.st_mode)){

            if((d_flag || find_all) &&(strcmp(name,item_name)==0)){

                cnt++;
                // printf("%s\n",filepath);
               
            }

            cnt=cnt+count_name(d_flag,f_flag,name,filepath);
            
        }

        else if(S_ISREG(statbuf.st_mode)){
            if((f_flag || find_all) && (strncmp(name,item_name,strlen(name))==0)){

                cnt++;
                // printf("%s\n",filepath);

            }

        }

    }
    return cnt;
}