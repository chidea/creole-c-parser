#!/bin/sh
test=../../bin/parsertest
for f in `ls *.txt`; do
  ${test} -o ${f/.txt/.html} $f
done
