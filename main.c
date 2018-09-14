#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes.h"

int main(int argc, char** argv) {
    Options opt = handleArgs(argc, argv);

    State* state_ptr = malloc(sizeof(State));
    memset(state_ptr->byte, 0, sizeof(State));

    FILE* fptr = NULL;
    if ((fptr = fopen(opt.inputfile, "rb")) == NULL) {
        printf("Error opening file");
        exit(1);
    }

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            fread(&state_ptr->byte[r][c], sizeof(char), 1, fptr);
            printf("%02x ", state_ptr->byte[r][c]);
        }
        printf("\n");
    }

    fclose(fptr);

    return 0;
}
