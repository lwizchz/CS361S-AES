#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes.h"

int main(int argc, char** argv) {
    Options opt = handleArgs(argc, argv);

    State* state_ptr = malloc(sizeof(State));
    memset(state_ptr->byte, 0, sizeof(State));

    readStates(opt.inputfile);

    
    

    //fclose(fptr);

    return 0;
}


