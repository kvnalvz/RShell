#!/bin/bash

BASEDIR=$(dirname $0)
FILE="${BASEDIR}/paren_input.txt"

$BASEDIR/../rshell < $FILE
