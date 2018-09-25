# CS361S-AES
# Name 1: Andrew Q Langman
# Name 2: Will Kuglen
# Name 3: Luke Montalvo

AES Implementation:

- typedef Struct State: contains only a 4x4 char array. It is intended to hold
bytes from the inputfile, and the intermediary data during the algorithm's
execution.

- typedef Struct Word: contains only a 4x1 char array. It is intended to hold
a 32-byte word for manipulation during Key Expansion. This was created for ease
of conceptualizing the 32-byte word operations that take place during Key
Expansion since that process operates on columns.

- state_array: When inputfile is read in, readStates() creates an array of
pointers to 4x4 "state arrays", thus making state_array a double pointer.
state_array is then passed to the main encrypt() function which then can pass
that into the four sub-routines. In other words, state_array is the entry point
to accessing the state arrays during encryption and decryption. Each 4x4 state
array contains 16 bytes of data, one per entry in the array. When inputfile is
initially read into state arrays, it is read in column major order; i.e. the
first byte goes into [0][0], the second byte goes into [1][0], the third byte
goes into [2][0], the fifth byte goes into [0][1], etc.

- encrypt(): We essentially implemented the pseudocode for Cipher() in the NIST
(p. 15). This function calls on the four main sub-routines - addRoundKey(),
shiftRows(), mixColumns(), and subBytes() - in the same order as shown in the
pseudocode.

- rotWord(): Takes in a Word in form {ABCD} where A, B, C, D are bytes. Then, a
Word in the form {BCDA} is returned.

- subWord(): Takes in a Word in form {ABCD} where A, B, C, D are bytes. Each
byte is comprised of two nibbles X, Y in the form {XY}. Using simple bitwise
operations, X and Y are obtained from each byte. Then, nibble X is used as the
row index and nibble Y is used as the column index in order to index into the
pre-defined S-box, aes_sbox, to obtain a new byte. That new byte replaces the
byte {XY} in the original Word. Once all 4 bytes A, B, C, D perform this
substitution, the new Word is returned.

- Key Expansion: When the key is read in, it must be expanded into a Key
Schedule. The Key Schedule is a large array that has size n equal to the
(number of rounds Nr plus 1) times the keysize in bytes Nb * 4
[n = (Nb * 4)(Nr + 1)]. Columns in the Key Schedule are generated by two main
operations on Words (defined above) - rotWord() and subWord(). We then
implemented the pseudocode for KeyExpansion() from the NIST (p. 20).

- addRoundKey(): Takes in the state_array and "adds" it to Words (columns) from
the Key Schedule. This "addition" is really a bitwise XOR operation. This is
performed as follows: The State array passed to this function is in the form
{W0 W1 W2 W3} where W0, W1, W2, and W3 are Words (columns). Each Word is in
form {ABCD} where A, B, C, and D are bytes. Then, a word W is obtained from the
Key Schedule according to the formula W = (current_round*Nb) + WordNum where
WordNum = 0, 1, 2, or 3 for W0, W1, W2, or W3 respectively.

- shiftRows(): This function rotates bytes left in the state arrays cyclically.
The number of positions to be rotated corresponds to the row number; i.e.
entries in the 0th row move by 0 positions, entries in the 1st row move by 1
position, entries in the 2nd row move by 2 positions, and entries in the 3rd
row move by 3 positions.

- mixColumns(): This function modifies each column of the state array manually,
as if obtaining the result from the matrix multiply in the NIST (p. 18). The
algorithm makes use of 3 helper functions: aes_add(), xtime(), and aes_mult().
The first function, aes_add(), is implemented as a simple XOR operation as
specified in the NIST (p. 10). The second, xtime(), is a helper to aes_mult()
and allows recursive multiplication by x mod m(x) where m(x) is the irreducible
polynomial x^8 + x^4 + x^3 + x + 1. This multiplication is equivalent to a
multiplication by 2 followed by a conditional XOR with the irreducible if the
result would overflow the 8bit storage. The third function, aes_mult(), is used
to multiply two bytes a and b according to the NIST (p. 10-12). It uses xtime()
to multiply a by b_i*x^i where b_i is the i'th bit of b.

- subBytes(): This function substitutes the bytes from the state array with
bytes from the aes_sbox. aes_sbox is manually copied into our program file to
match the S-box shown in the NIST (p. 16) rather than being generated according
to the algorithm in the NIST (p. 15). The substitution scheme is as follows:
for each byte {XY} in the state arrays, index into the S-box using nibble X as
the row index and nibble Y as the column index to find the byte with which you
will replace. Each byte is read in as a char, and then a few simple bitwise
operations are used to obtain nibble X and nibble Y by themselves.

- decrypt(): Like encrypt(), we essentially copied the pseudocode for
InvCipher() in the NIST (p. 21). We chose not to implement the EqInvCipher()
defined on p. 25 of the NIST. This function takes in the key size (to determine
how many rounds to execute) and a pointer to the state_array. It then performs
the four main inverse sub-routines - invAddRoundKey(), invShiftRows(),
invMixColumns(), and invSubBytes() - in the order shown in the NIST for
InvCipher().

- invShiftRows(): This function rotates bytes right in the state arrays
cyclically. The number of positions to be rotated corresponds to the row
number; i.e. entries in the 0th row move by 0 positions, entries in the 1st row
move by 1 position, entries in the 2nd row move by 2 positions, and entries in
the 3rd row move by 3 positions.

- invSubBytes(): This function substitutes the bytes from the state array with
bytes from the inv_aes_sbox. inv_aes_sbox is manually copied into our program
file to match the Inverse S-box shown in the NIST (p. 22) rather than being
generated at runtime. The substitution scheme is as follows: for each byte {XY}
in the state arrays, index into the Inverse S-box using nibble X as the row
index and nibble Y as the column index to find the byte with which you will
replace. Each byte is read in as a char, and then a few simple bitwise
operations are used to obtain nibble X and nibble Y by themselves.

- invMixColumns(): This function is much the same as mixColumns(), the primary
difference being the matrix used in the equivalent operation. This matrix is
specified in the NIST (p. 23).
