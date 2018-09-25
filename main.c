#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes.h"

int main(int argc, char** argv) {
    Options opt = handleArgs(argc, argv);

    size_t state_bytes;
    State** state_array = readStates(opt.inputfile, &state_bytes);
    if (opt.is_verbose) {
        printStates(state_array);
    }

    KeySchedule schedule = generateKeySchedule(opt.keyfile, opt.keysize);

    switch (opt.mode) {
        case MODE_ENCRYPT: {
            state_array = encrypt(opt.keysize, state_array, &state_bytes, schedule);
            break;
        }
        case MODE_DECRYPT: {
            state_array = decrypt(opt.keysize, state_array, &state_bytes, schedule);
            break;
        }
        default: {
            fprintf(stderr, "Invalid mode: %d", opt.mode);
            return 1;
        }
    }

    if (opt.is_verbose) {
        printStates(state_array);
    }
    writeStates(opt.outputfile, state_array, state_bytes);

    freeStates(state_array);
    state_array = NULL;

    free(schedule);

    return 0;
}
