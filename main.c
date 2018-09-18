#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes.h"

int main(int argc, char** argv) {
    Options opt = handleArgs(argc, argv);

    State** state_array = readStates(opt.inputfile);

    encrypt(opt.keysize, state_array);
    printStates(state_array);

    writeStates(opt.outputfile, state_array);

    decrypt(opt.keysize, state_array);
    printStates(state_array);

    return 0;
}
