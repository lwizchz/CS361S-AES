#!/bin/bash

keysize="128"
mode="encrypt"
key="tests/key0"
inputfile="tests/plaintext0"
outputfile="tests/ciphertext0"

cmd="./aes --keysize $keysize --mode $mode --keyfile $key --inputfile $inputfile --outputfile $outputfile"
echo $cmd
$cmd
