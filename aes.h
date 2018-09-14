#ifndef AES_H
#define AES_H 1

#define NUM_COL 4 // Nb
#define NUM_WORDS_128 4 // Nk for 128bit
#define NUM_WORDS_256 8 // Nk for 256bit
#define NUM_ROUNDS_128 10 // Nr for 128bit
#define NUM_ROUNDS_256 14 // Nr for 256bit

typedef enum {
    KEYSIZE_128,
    KEYSIZE_256
} E_KEYSIZE;
typedef enum  {
    MODE_ENCRYPT,
    MODE_DECRYPT
} E_MODE;

typedef struct {
    E_KEYSIZE keysize;
    E_MODE mode;

    char* keyfile;
    char* inputfile;
    char* outputfile;
} Options;

typedef struct {
    unsigned char byte[4][NUM_COL];
} State;

Options handleArgs(int, char**);

void subBytes(State*);
void shiftRows(State*);
void mixColumns(State*);
void addRoundKey(State*);

//void rotWord(State*);
//void subWord(State*);

void invSubBytes(State*);
void invShiftRows(State*);
void invMixColumns(State*);

#endif
