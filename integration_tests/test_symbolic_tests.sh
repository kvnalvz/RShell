#!/bin/bash

BASEDIR=$(dirname $0)
FILE="${BASEDIR}/test_literal_bracket.txt"

$BASEDIR/../rshell < $FILE
