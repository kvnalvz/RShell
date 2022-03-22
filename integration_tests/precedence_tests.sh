#!/bin/bash

BASEDIR=$(dirname $0)
FILE="${BASEDIR}/precedence_tests.txt"

$BASEDIR/../rshell < $FILE

