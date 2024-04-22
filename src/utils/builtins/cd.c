#include "../../header/minishell.h"

void cd(const char *path){
    if (chdir(path) == -1) {
        printf("Impossible de changer de repertoire !\n");
    }
}
