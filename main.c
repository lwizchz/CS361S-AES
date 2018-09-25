#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes.h"

int main(int argc, char** argv) {
    Options opt = handleArgs(argc, argv);

    State** state_array = readStates(opt.inputfile);
    if (opt.is_verbose) {
        printStates(state_array);
    }

    KeySchedule schedule = generateKeySchedule(opt.keyfile, opt.keysize);

    switch (opt.mode) {
        case MODE_ENCRYPT: {
            encrypt(opt.keysize, state_array, schedule);
            if (opt.is_verbose) {
                printStates(state_array);
            }
            writeStates(opt.outputfile, state_array);
            break;
        }
        case MODE_DECRYPT: {
            decrypt(opt.keysize, state_array, schedule);
            if (opt.is_verbose) {
                printStates(state_array);
            }
            writeStates(opt.outputfile, state_array);
            break;
        }
        default: {
            fprintf(stderr, "Invalid mode: ", opt.mode);
            return 1;
        }
    }

    freeStates(state_array);
    state_array = NULL;

    free(schedule);

    return 0;
}
