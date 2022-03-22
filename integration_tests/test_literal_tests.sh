#!/bin/bash

BASEDIR=$(dirname $0)
FILE="${BASEDIR}/test_literal.txt"

$BASEDIR/../rshell < $FILE
