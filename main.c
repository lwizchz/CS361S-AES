#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes.h"

int main(int argc, char** argv) {
    Options opt = handleArgs(argc, argv);

    State** state_array = readStates(opt.inputfile);

    writeStates(opt.outputfile, state_array);

    return 0;
}
