#!/bin/bash

BASEDIR=$(dirname $0)
FILE="${BASEDIR}/input_multiple.txt"

$BASEDIR/../rshell < $FILE

