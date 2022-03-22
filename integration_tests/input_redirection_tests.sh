#!/bin/bash

BASEDIR=$(dirname $0)
FILE="${BASEDIR}/input_redirection.txt"

$BASEDIR/../rshell < $FILE

