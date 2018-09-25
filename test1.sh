#!/bin/bash

keysize="128"
key="tests/key1"
plainfile="tests/plaintext1"
cipherfile="tests/ciphertext1"
decryptfile="tests/decrypted1"

cmd_encrypt="./aes --keysize $keysize --mode encrypt --keyfile $key --inputfile $plainfile --outputfile $cipherfile"
cmd_decrypt="./aes --keysize $keysize --mode decrypt --keyfile $key --inputfile $cipherfile --outputfile $decryptfile"

echo $cmd_encrypt
$cmd_encrypt

echo $cmd_decrypt
$cmd_decrypt
