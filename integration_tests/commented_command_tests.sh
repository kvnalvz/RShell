#!/bin/bash

BASEDIR=$(dirname $0)
FILE="${BASEDIR}/input_commented_command.txt"

$BASEDIR/../rshell < $FILE
