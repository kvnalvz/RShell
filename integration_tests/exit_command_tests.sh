#!/bin/bash

BASEDIR=$(dirname $0)
FILE="${BASEDIR}/exit_command.txt"

$BASEDIR/../rshell < $FILE
