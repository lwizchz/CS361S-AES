#ifndef AES_C
#define AES_C 1

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "aes.h"

const unsigned char aes_sbox[16][16] = {
    /*       0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f   */
    /* 0 */ {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
    /* 1 */ {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
    /* 2 */ {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
    /* 3 */ {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
    /* 4 */ {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
    /* 5 */ {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
    /* 6 */ {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
    /* 7 */ {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
    /* 8 */ {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
    /* 9 */ {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
    /* a */ {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
    /* b */ {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
    /* c */ {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
    /* d */ {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
    /* e */ {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
    /* f */ {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
};
const unsigned char aes_inv_sbox[16][16] = {
    /*       0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f   */
    /* 0 */ {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb},
    /* 1 */ {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb},
    /* 2 */ {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e},
    /* 3 */ {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25},
    /* 4 */ {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92},
    /* 5 */ {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84},
    /* 6 */ {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06},
    /* 7 */ {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b},
    /* 8 */ {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73},
    /* 9 */ {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e},
    /* a */ {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b},
    /* b */ {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4},
    /* c */ {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f},
    /* d */ {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef},
    /* e */ {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61},
    /* f */ {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d}
};
const short aes_irreducible = 0x011b; // The irreducible polynomial m(x) = x^8 + x^4 + x^3 + x + 1

void testSBox() {
    unsigned char v_0 = '\0';
    do {
        int r_0 = v_0 >> 4;
        int c_0 = v_0 & 0xf;
        unsigned char v_1 = aes_sbox[r_0][c_0];
        int r_1 = v_1 >> 4;
        int c_1 = v_1 & 0xf;
        unsigned char v_2 = aes_inv_sbox[r_1][c_1];

        if (v_0 != v_2) {
            fprintf(stderr, "AES sbox failed for 0x%02x : produced 0x%02x through 0x%02x\n", v_0, v_2, v_1);
            fprintf(stderr, "v_0 indices: 0x%02x 0x%02x\n", r_0, c_0);
            fprintf(stderr, "v_1 indices: 0x%02x 0x%02x\n", r_1, c_1);
            exit(1);
        }

        v_0++;
    } while (v_0 != 0xff);
}

const struct option long_opts[] = {
    {"keysize", required_argument, NULL, 's'},
    {"keyfile", required_argument, NULL, 'k'},
    {"inputfile", required_argument, NULL, 'i'},
    {"outputfile", required_argument, NULL, 'o'},
    {"mode", required_argument, NULL, 'm'},
    {"verbose", no_argument, NULL, 'v'},
    {NULL, 0, NULL, 0}
};

void exitError(char* str) {
    fprintf(stderr, str, "");
    exit(1);
}
Options handleArgs(int argc, char** argv) {
    testSBox();

    Options opt = {
        KEYSIZE_128,
        MODE_ENCRYPT,
        false,
        NULL, NULL, NULL
    };

    char c;
    while ((c = getopt_long(argc, argv, "s:k:i:o:m:v", long_opts, NULL)) != -1) {
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
            case 'v': {
                opt.is_verbose = true;
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

State** readStates(char* filename) {
    // find size of file and check if it exists
    size_t file_size_bytes = findSize(filename);
    // determine how many state arrays are needed
    int arrays_needed = floor((double) file_size_bytes / BLOCK_SIZE) + 1;

    State** state_array = malloc(sizeof(State*) * (arrays_needed + 1));
    state_array[arrays_needed] = NULL; // Set sentinel value

    FILE* fptr = fopen(filename, "rb");

    int rem = file_size_bytes % BLOCK_SIZE;

    for (int a = 0; a < arrays_needed; a++) {
        State* current_state = malloc(sizeof(State));
        state_array[a] = current_state;

        for (int c = 0; c < NUM_COL; c++) {
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
        state_array[arrays_needed - 1]->byte[3][3] = BLOCK_SIZE - rem;
    }
    else {
        state_array[arrays_needed - 1]->byte[3][3] = BLOCK_SIZE;
    }

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
            for (int c = 0; c < NUM_COL; c++) {
                printf("%02x ", (*state_array)->byte[r][c]);
            }
            printf("\n");
        }
        printf("\n");
        state_array++;
    }
}

size_t writeStates(const char* filename, State** state_array) {
    FILE* fh = fopen(filename, "wb");
    if (fh == NULL) {
        exitError("Error opening output file\n");
    }

    State* state = *state_array;
    size_t bytes = 0;
    while (state) {
        for (int c=0; c<NUM_COL; c++) {
            for (int r=0; r<4; r++) {
                bytes += fwrite(&state->byte[r][c], sizeof(char), 1, fh);
            }
        }
        state = *(++state_array);
    }

    fclose(fh);

    return bytes;
}

void freeStates(State** state_array) {
    for (int i=0; state_array[i]; i++) {
        free(state_array[i]);
    }
    free(state_array);
}

void subBytes(State* state) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < NUM_COL; c++) {
            char sbox_col_idx = (state->byte[r][c] & 0x0f);
            char sbox_row_idx = (state->byte[r][c] & 0xf0) >> 4;
            state->byte[r][c] = aes_sbox[sbox_row_idx][sbox_col_idx];
        }
    }
}
// All row shifts are left cyclically
void shiftRows(State* state) {
    // Do nothing to Row 0

    // Shift Row 1 by 1
    char s1_0 = state->byte[1][0];
    for (int c = 0; c < 3; c++) {
        state->byte[1][c] = state->byte[1][c + 1];
    }
    state->byte[1][3] = s1_0;

    // Shift Row 2 by 2
    char s2_0 = state->byte[2][0];
    char s2_1 = state->byte[2][1];
    state->byte[2][0] = state->byte[2][2];
    state->byte[2][1] = state->byte[2][3];
    state->byte[2][2] = s2_0;
    state->byte[2][3] = s2_1;

    // Shift Row 3 by 3
    char s3_0 = state->byte[3][0];
    char s3_1 = state->byte[3][1];
    char s3_2 = state->byte[3][2];
    state->byte[3][0] = state->byte[3][3];
    state->byte[3][1] = s3_0;
    state->byte[3][2] = s3_1;
    state->byte[3][3] = s3_2;
}
unsigned char aes_add(unsigned char a, unsigned char b) {
    return a ^ b;
}
unsigned char xtime(unsigned char a, int x) {
    if (x == 0) {
        return a;
    }

    // Conditional add of the irreducible polynomial 0x011b to reduce a*0x02
    if (a & 0x80) {
        a = aes_add(a<<1, aes_irreducible);
    } else {
        a <<= 1;
    }
    return xtime(a, x-1);
}
unsigned char aes_mult(unsigned char a, unsigned char b) {
    if ((a == 0)||(b == 0)) {
        return 0;
    }

    unsigned char r = 0;
    for (int i=0; i<8; i++) {
        // Conditionally add the multiplication by 0x02*i
        if (b & 1<<i) {
            r = aes_add(r, xtime(a, i));
        }
    }

    return r;
}
void mixColumns(State* state) {
    for (int c=0; c<NUM_COL; c++) {
        // Store bytes for computation
        const unsigned char s0 = state->byte[0][c];
        const unsigned char s1 = state->byte[1][c];
        const unsigned char s2 = state->byte[2][c];
        const unsigned char s3 = state->byte[3][c];

        // Compute bytes according to the AES specification
        state->byte[0][c] = aes_add(aes_add(aes_mult(0x02, s0), aes_mult(0x03, s1)), aes_add(s2, s3));
        state->byte[1][c] = aes_add(aes_add(s0, aes_mult(0x02, s1)), aes_add(aes_mult(0x03, s2), s3));
        state->byte[2][c] = aes_add(aes_add(s0, s1), aes_add(aes_mult(0x02, s2), aes_mult(0x03, s3)));
        state->byte[3][c] = aes_add(aes_add(aes_mult(0x03, s0), s1), aes_add(s2, aes_mult(0x02, s3)));
    }
}


/*
 * addRoundkey: XOR the state with a 128-bit round key
 * derived from the original key K by a recursive process.
 *
 * The input text (state) is represented as a 4 x 4 array of bytes.
 * The key is represented as a 4 x n array of bytes,
 * where n depends on the key size (10 or 14)
 */
void addRoundKey(State* state, Word* partialSchedule) {

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < NUM_COL; c++) {
            state->byte[r][c] = aes_add(state->byte[r][c], partialSchedule[c].byte[r][0]);
        }
    }
}



// Generate Key schedule
KeySchedule generateKeySchedule(const char* keyfile, E_KEYSIZE keysize) {
    // rawKey = Read in key into an array with (keysize/4) Word s
    Key rawKey = readKey(keyfile, keysize);

    return keyExpansion(rawKey, keysize);

    // Key Expansion
}
// Key Expansion
KeySchedule keyExpansion(Key rawKey, E_KEYSIZE keysize) {

    int numWords = (keysize == KEYSIZE_128) ? NUM_WORDS_128 : NUM_WORDS_256;

    int numRounds = (keysize == KEYSIZE_128) ? NUM_ROUNDS_128 : NUM_ROUNDS_256;

    // schedule = malloc KeySchedule with (numRounds+1) Word s
    KeySchedule schedule = (Word *)malloc(sizeof(Word)*NUM_COL*(numRounds+1));

    Word temp;
    int i = 0;
    while (i < numWords) {
        copyWord(&schedule[i], &rawKey[i]);
        i = i+1;
    }
    i = numWords; // should be the case but reassurance
    while (i < NUM_COL*(numRounds+1)) {
        copyWord(&temp, &schedule[i-1]);
        if (i % numWords == 0) {
            Word rconTmp = rcon(i/numWords);
            Word xorTmp = xorWord(subWord(rotWord(&temp)), &rconTmp);
            copyWord(&temp, &xorTmp);
        } else if (numWords > 6 && i % numWords == 4) {
            copyWord(&temp, subWord(&temp));
        }
        Word xorTmp = xorWord(&schedule[i-numWords], &temp);
        copyWord(&schedule[i], &xorTmp);
        i = i+1;
    }

    return schedule;
}

// assumes memory is setup
void copyWord(Word* to, const Word* from) {
    to->byte[0][0] = from->byte[0][0];
    to->byte[1][0] = from->byte[1][0];
    to->byte[2][0] = from->byte[2][0];
    to->byte[3][0] = from->byte[3][0];
}

Word xorWord(Word* left, Word* right) {
    Word a;
    a.byte[0][0] = left->byte[0][0] ^ right->byte[0][0];
    a.byte[1][0] = left->byte[1][0] ^ right->byte[1][0];
    a.byte[2][0] = left->byte[2][0] ^ right->byte[2][0];
    a.byte[3][0] = left->byte[3][0] ^ right->byte[3][0];
    return a;
}

/*
Nk = key length (4 or 8)
Nb = Block Size  (4)
Nr = Num Rounds (10 or 14)
KeyExpansion(byte key[4*Nk], word w[Nb*(Nr+1)], Nk) begin
    word temp
    i=0
    while (i < Nk)
        w[i] = word(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3])
        i = i+1
    end while
    i = Nk
    while (i < Nb * (Nr+1)]
        temp = w[i-1]
        if (i mod Nk = 0)
            temp = SubWord(RotWord(temp)) xor Rcon[i/Nk]
        else if (Nk > 6 and i mod Nk = 4)
             temp = SubWord(temp)
        end if
        w[i] = w[i-Nk] xor temp
        i=i+1
    end while
end
*/

//void rotWord(Word* word) {a,b,c,d -> b,c,d,a}
Word* rotWord(Word* word) {
    unsigned char temp = word->byte[0][0];
    word->byte[0][0] = word->byte[1][0];
    word->byte[1][0] = word->byte[2][0];
    word->byte[2][0] = word->byte[3][0];
    word->byte[3][0] = temp;
    return word;
}


//void subWord(Word* word) {}
Word* subWord(Word* word) {
    int c = 0;
    for (int r = 0; r < 4; r++) {
        char sbox_col_idx = (word->byte[r][c] & 0x0f);
        char sbox_row_idx = (word->byte[r][c] & 0xf0) >> 4;
        word->byte[r][c] = aes_sbox[sbox_row_idx][sbox_col_idx];
    }
    return word;
}

Word rcon(int i) {
    assert(i != 0);
    assert(!(i > 10));
    unsigned char Rcon[10] = {
    //  1       2       3       4       5       6       7       8       9       10
        0x01,   0x02,   0x04,   0x08,   0x10,   0x20,   0x40,   0x80,   0x1b,   0x36
    };
    Word w = {0};
    w.byte[0][0] = Rcon[i-1];
    return w;
}


Key readKey(const char* filename, E_KEYSIZE keysize) {
    // find size of file and check if it exists
    FILE* fptr = fopen(filename, "rb");
    if (fptr == NULL) {
        exitError("File Not Found!\n");
    }

    int bytes_needed = (keysize == KEYSIZE_128) ? 16 : 32;
    int words_needed = bytes_needed/4;
    Key key = malloc(bytes_needed);

    for (int i = 0; i < words_needed; i++) {
        for (int c = 0; c < 4; c++) {
            fread(&key[i].byte[c][0], sizeof(char), 1, fptr);
        }
    }
    

    fclose(fptr);

    return key;
}


void invSubBytes(State* state) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < NUM_COL; c++) {
            char inv_sbox_col_idx = (state->byte[r][c] & 0x0f);
            char inv_sbox_row_idx = (state->byte[r][c] & 0xf0) >> 4;
            state->byte[r][c] = aes_inv_sbox[inv_sbox_row_idx][inv_sbox_col_idx];
        }
    }
}
// All row shifts are right cyclically
void invShiftRows(State* state) {
    // Do nothing to Row 0

    // Shift Row 1 by 1
    char s1_3 = state->byte[1][3];
    for (int c = 3; c > 0; c--) {
        state->byte[1][c] = state->byte[1][c - 1];
    }
    state->byte[1][0] = s1_3;

    // Shift Row 2 by 2
    char s2_0 = state->byte[2][0];
    char s2_1 = state->byte[2][1];
    state->byte[2][0] = state->byte[2][2];
    state->byte[2][1] = state->byte[2][3];
    state->byte[2][2] = s2_0;
    state->byte[2][3] = s2_1;

    // Shift Row 3 by 3
    char s3_0 = state->byte[3][0];
    for (int c = 0; c < 3; c++) {
        state->byte[3][c] = state->byte[3][c + 1];
    }
    state->byte[3][3] = s3_0;
}
void invMixColumns(State* state) {
    for (int c=0; c<NUM_COL; c++) {
        // Store bytes for computation
        const unsigned char s0 = state->byte[0][c];
        const unsigned char s1 = state->byte[1][c];
        const unsigned char s2 = state->byte[2][c];
        const unsigned char s3 = state->byte[3][c];

        // Compute bytes according to the AES specification
        state->byte[0][c] = aes_add(aes_add(aes_mult(0x0e, s0), aes_mult(0x0b, s1)), aes_add(aes_mult(0x0d, s2), aes_mult(0x09, s3)));
        state->byte[1][c] = aes_add(aes_add(aes_mult(0x09, s0), aes_mult(0x0e, s1)), aes_add(aes_mult(0x0b, s2), aes_mult(0x0d, s3)));
        state->byte[2][c] = aes_add(aes_add(aes_mult(0x0d, s0), aes_mult(0x09, s1)), aes_add(aes_mult(0x0e, s2), aes_mult(0x0b, s3)));
        state->byte[3][c] = aes_add(aes_add(aes_mult(0x0b, s0), aes_mult(0x0d, s1)), aes_add(aes_mult(0x09, s2), aes_mult(0x0e, s3)));
    }
}

void encrypt(E_KEYSIZE keysize, State** state_array, KeySchedule words) {
    const int Nr = (keysize == KEYSIZE_128) ? NUM_ROUNDS_128 : NUM_ROUNDS_256;
    while (state_array && *state_array) {
        State* state = *state_array;

        addRoundKey(state, &words[0]);

        for (int r=1; r<=Nr-1; r++) {
            subBytes(state);
            shiftRows(state);
            mixColumns(state);
            addRoundKey(state, &words[r*NUM_COL]);
        }

        subBytes(state);
        shiftRows(state);
        addRoundKey(state, &words[Nr*NUM_COL]);

        state_array++;
    }
}
void decrypt(E_KEYSIZE keysize, State** state_array, KeySchedule words) {
    const int Nr = (keysize == KEYSIZE_128) ? NUM_ROUNDS_128 : NUM_ROUNDS_256;
    while (state_array && *state_array) {
        State* state = *state_array;

        addRoundKey(state, &words[Nr*NUM_COL]);

        for (int r=Nr-1; r>0; r--) {
            invShiftRows(state);
            invSubBytes(state);
            addRoundKey(state, &words[r*NUM_COL]);
            invMixColumns(state);
        }

        invShiftRows(state);
        invSubBytes(state);
        addRoundKey(state, &words[0]);

        state_array++;
    }
}


#endif
