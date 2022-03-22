#!/bin/bash

BASEDIR=$(dirname $0)
FILE="${BASEDIR}/output_redirection.txt"

$BASEDIR/../rshell < $FILE

