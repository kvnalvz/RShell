#!/bin/bash

BASEDIR=$(dirname $0)
FILE="${BASEDIR}/input_single_command.txt"

$BASEDIR/../rshell < $FILE
