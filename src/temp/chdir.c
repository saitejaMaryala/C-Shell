#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

char* curr_working_dir(){
    char* cwd = (char*)malloc(256 * sizeof(char)); // Allocate 256 bytes for the buffer
    if (cwd == NULL) {
        perror("malloc failed");
        return NULL;
    }

    if (getcwd(cwd, 256) == NULL) { // Pass the correct size of the buffer
        perror("getcwd failed");
        free(cwd);
        return NULL;
    }
    printf("from curr---->%s\n",cwd);
    return cwd;
    
}

int main() {
    char s[100];
    printf("%s\n", curr_working_dir()); // Get current working directory
    chdir(".."); // Change to parent directory
    printf("%s\n",curr_working_dir()); // Get updated working directory
    return 0;
}