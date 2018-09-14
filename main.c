#include <stdio.h>

#include "aes.h"

int main(int argc, char** argv) {
    printf("Hello, AES!\n");

    Options opt = handleArgs(argc, argv);

    State state_ptr* = (State*) malloc(sizeof(State));
    
    FILE *fptr = NULL;

    if ((fptr = fopen(opt.inputfile, "rb")) == NULL)
    {
        printf("ERROR OPENING FILE");
        exit (1);
    }

    for (int c = 0; c < 4; c++)
    {
        for (int r = 0; r < 4; r++)
        {
            fread(*state_ptr[r][c], sizeof(char), 1, fptr);
        }
        
    }

    fclose(fptr);

    fprintf("First byte: %x\n", *state_ptr[0][0]);

}
