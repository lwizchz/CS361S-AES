#!/bin/bash

keysize="128"
mode="encrypt"
key="tests/key1"
inputfile="tests/plaintext1"
outputfile="tests/ciphertext1"

cmd="./aes --keysize $keysize --mode $mode --keyfile $key --inputfile $inputfile --outputfile $outputfile"
echo $cmd
$cmd
