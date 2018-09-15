#ifndef AES_C
#define AES_C 1

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <math.h>

#include "aes.h"

const struct option long_opts[] = {
    {"keysize", required_argument, NULL, 's'},
    {"keyfile", required_argument, NULL, 'k'},
    {"inputfile", required_argument, NULL, 'i'},
    {"outputfile", required_argument, NULL, 'o'},
    {"mode", required_argument, NULL, 'm'},
    {NULL, 0, NULL, 0}
};

void exitError(char* str) {
    fprintf(stderr, str, "");
    exit(1);
}
Options handleArgs(int argc, char** argv) {
    Options opt = {
        KEYSIZE_128,
        MODE_ENCRYPT,
        NULL, NULL, NULL
    };

    char c;
    while ((c = getopt_long(argc, argv, "s:k:i:o:m:", long_opts, NULL)) != -1) {
        switch (c) {
            case 's': {
                if (strcmp(optarg, "128") == 0) {
                    opt.keysize = KEYSIZE_128;
                } else if (strcmp(optarg, "256") == 0) {
                    opt.keysize = KEYSIZE_256;
                } else {
                    exitError("Invalid keysize\n");
                }
                break;
            }
            case 'k': {
                size_t kf_len = strlen(optarg);
                if (!kf_len) {
                    exitError("Invalid keyfile\n");
                }

                opt.keyfile = malloc(kf_len);
                strcpy(opt.keyfile, optarg);

                break;
            }
            case 'i': {
                size_t if_len = strlen(optarg);
                if (!if_len) {
                    exitError("Invalid inputfile\n");
                }

                opt.inputfile = malloc(if_len);
                strcpy(opt.inputfile, optarg);

                break;
            }
            case 'o': {
                size_t of_len = strlen(optarg);
                if (!of_len) {
                    exitError("Invalid outputfile\n");
                }

                opt.outputfile = malloc(of_len);
                strcpy(opt.outputfile, optarg);

                break;
            }
            case 'm': {
                if (strcmp(optarg, "encrypt") == 0) {
                    opt.mode = MODE_ENCRYPT;
                } else if (strcmp(optarg, "decrypt") == 0) {
                    opt.mode = MODE_DECRYPT;
                } else {
                    exitError("Invalid mode\n");
                }
                break;
            }
            default: {}
        }
    }

    if (opt.keyfile == NULL) {
        exitError("Please specify a keyfile\n");
    }
    if (opt.inputfile == NULL) {
        exitError("Please specify a inputfile\n");
    }
    if (opt.outputfile == NULL) {
        exitError("Please specify a outputfile\n");
    }

    return opt;
}

size_t writeStates(const char* filename, State** state_array) {
    FILE* fh = fopen(filename, "wb");
    if (fh == NULL) {
        exitError("Error opening output file\n");
    }

    State* state = *state_array;
    size_t bytes = 0;
    while (state) {
        for (int c=0; c<4; c++) {
            for (int r=0; r<4; r++) {
                bytes += fwrite(&state->byte[r][c], sizeof(char), 1, fh);
            }
        }
        state = *(++state_array);
    }

    fclose(fh);

    return bytes;
}

State** readStates(char* filename) {
    // find size of file and check if it exists
    size_t file_size_bytes = findSize(filename);
    // determine how many state arrays are needed
    int arrays_needed = ceil((double) file_size_bytes / BLOCK_SIZE);

    State** state_array = malloc(sizeof(State*) * (arrays_needed + 1));
    state_array[arrays_needed] = NULL; // Set sentinel value

    FILE* fptr = fopen(filename, "rb");

    int rem = file_size_bytes % BLOCK_SIZE;

    for (int a = 0; a < arrays_needed; a++) {
        State* current_state = malloc(sizeof(State));
        state_array[a] = current_state;

        for (int c = 0; c < 4; c++) {
            for (int r = 0; r < 4; r++) {
                fread(&current_state->byte[r][c], sizeof(char), 1, fptr);

                if (a == arrays_needed - 1) {
                    if (r + 4*c >= BLOCK_SIZE - rem) {
                        current_state->byte[r][c] = 0;
                    }
                }
            }
        }  
    }

    fclose(fptr);
    
    if (rem != 0) {
        printf("remainder not 0\n");
        state_array[arrays_needed - 1]->byte[3][3] = BLOCK_SIZE - rem;
    }

    printStates(state_array);

    printf("State array pointer location: %p\n", (void*) state_array);
    printf("File Size: %ld\n", findSize(filename));
    return state_array;
}
//Function findSize() taken from https://www.geeksforgeeks.org/c-program-find-size-file/
long int findSize(char* file_name) {
    // opening the file in read mode
    FILE* fp = fopen(file_name, "rb");

    // checking if the file exist or not
    if (fp == NULL) {
        exitError("File Not Found!\n");
    }

    fseek(fp, 0L, SEEK_END);

    // calculating the size of the file
    long int res = ftell(fp);

    // closing the file
    fclose(fp);

    return res;
}
void printStates(State** state_array) {
    while (state_array && *state_array) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                printf("%02x ", (*state_array)->byte[r][c]);
            }
            printf("\n");
        }
        printf("\n");
        state_array++;
    }
}

void subBytes(State* state) {}
void shiftRows(State* state) {}
void mixColumns(State* state) {}
void addRoundKey(State* state) {}

//void rotWord(State* state) {}
//void subWord(State* state) {}

void invSubBytes(State* state) {}
void invShiftRows(State* state) {}
void invMixColumns(State* state) {}


void encrypt(E_KEYSIZE keysize, State** state_array) {}
void decrypt(E_KEYSIZE keysize, State** state_array) {}

#endif
