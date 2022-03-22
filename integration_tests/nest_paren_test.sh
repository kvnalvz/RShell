#!/bin/bash

BASEDIR=$(dirname $0)
FILE="${BASEDIR}/nested_paren.txt"

$BASEDIR/../rshell < $FILE
