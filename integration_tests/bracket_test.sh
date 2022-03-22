#!/bin/bash

BASEDIR=$(dirname $0)
FILE="${BASEDIR}/test_bracket.txt"

$BASEDIR/../rshell < $FILE
