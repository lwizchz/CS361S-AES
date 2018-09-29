#!/bin/bash

GREENCOLOR="\x1B[32m"
CLEARCOLOR="\x1B[0m \n"
LVL1="*"
LVL2="├"

mkdir -p tests/rand
# Generate Keys
head -c 16 < /dev/urandom > tests/rand/key128
head -c 32 < /dev/urandom > tests/rand/key256

# Key 128 tests
keysize="128"
key="tests/rand/key128"
printf "$GREENCOLOR *** Keysize $keysize Random Tests *** $CLEARCOLOR"

# Less than equal to block
printf "$GREENCOLOR $LVL1   Less than or equal to a block tests $CLEARCOLOR"
for i in `seq 1 16`;
do
  head -c $i < /dev/urandom > tests/rand/input$keysize-$i
  plainfile=tests/rand/input$keysize-$i
  cipherfile=tests/rand/encrypted$keysize-$i
  decryptfile=tests/rand/decrypted$keysize-$i

  cmd_encrypt="./aes --keysize $keysize --mode encrypt --keyfile $key --inputfile $plainfile --outputfile $cipherfile"
  cmd_decrypt="./aes --keysize $keysize --mode decrypt --keyfile $key --inputfile $cipherfile --outputfile $decryptfile"

  # echo $cmd_encrypt
  $cmd_encrypt

  # echo $cmd_decrypt
  $cmd_decrypt

  echo " $LVL2 Testing filesize $i; Resulting diff:"
  diff $plainfile $decryptfile
done

# block + partial block
printf "$GREENCOLOR $LVL1   More than one block tests $CLEARCOLOR"
for i in `seq 321 336`;
do
  head -c $i < /dev/urandom > tests/rand/input$keysize-$i
  plainfile=tests/rand/input$keysize-$i
  cipherfile=tests/rand/encrypted$keysize-$i
  decryptfile=tests/rand/decrypted$keysize-$i

  cmd_encrypt="./aes --keysize $keysize --mode encrypt --keyfile $key --inputfile $plainfile --outputfile $cipherfile"
  cmd_decrypt="./aes --keysize $keysize --mode decrypt --keyfile $key --inputfile $cipherfile --outputfile $decryptfile"

  # echo $cmd_encrypt
  $cmd_encrypt

  # echo $cmd_decrypt
  $cmd_decrypt

  echo " $LVL2 Testing filesize $i; Resulting diff:"
  diff $plainfile $decryptfile
done

# Key 256 tests
keysize="256"
key="tests/rand/key256"
printf "\n$GREENCOLOR *** Keysize $keysize Random Tests *** $CLEARCOLOR"

# Less than equal to block
printf "$GREENCOLOR $LVL1   Less than or equal to a block tests $CLEARCOLOR"
for i in `seq 1 16`;
do
  head -c $i < /dev/urandom > tests/rand/input$keysize-$i
  plainfile=tests/rand/input$keysize-$i
  cipherfile=tests/rand/encrypted$keysize-$i
  decryptfile=tests/rand/decrypted$keysize-$i

  cmd_encrypt="./aes --keysize $keysize --mode encrypt --keyfile $key --inputfile $plainfile --outputfile $cipherfile"
  cmd_decrypt="./aes --keysize $keysize --mode decrypt --keyfile $key --inputfile $cipherfile --outputfile $decryptfile"

  # echo $cmd_encrypt
  $cmd_encrypt

  # echo $cmd_decrypt
  $cmd_decrypt

  echo " $LVL2 Testing filesize $i; Resulting diff:"
  diff $plainfile $decryptfile
done

# block + partial block
printf "$GREENCOLOR $LVL1   More than one block tests $CLEARCOLOR"
for i in `seq 321 336`;
do
  plainfile=tests/rand/input$keysize-$i
  # echo "Make $plainfile"
  head -c $i < /dev/urandom > $plainfile
  cipherfile=tests/rand/encrypted$keysize-$i
  decryptfile=tests/rand/decrypted$keysize-$i

  cmd_encrypt="./aes --keysize $keysize --mode encrypt --keyfile $key --inputfile $plainfile --outputfile $cipherfile"
  cmd_decrypt="./aes --keysize $keysize --mode decrypt --keyfile $key --inputfile $cipherfile --outputfile $decryptfile"

  # echo $cmd_encrypt
  $cmd_encrypt

  # echo $cmd_decrypt
  $cmd_decrypt

  printf " $LVL2 Testing filesize $i; Resulting diff: \n"
  diff $plainfile $decryptfile
done
