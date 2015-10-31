#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>
#include "mpc.h"

int main(int argc, char** argv) {
    
    printf("Lispy version 0.0.0.0.1\n");
    printf("Press Ctrl+c to exit\n");

    while (1) {
        char* input = readline("lispy> ");
        add_history(input);

        // Echo
        printf("YO: %s\n", input);

        free(input);
    }

    return 0;
}
