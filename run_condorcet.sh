#!/bin/bash

degree=5

rm -r in_*
rm -r ou_*
rm DBMain.out

gcc -Wall DBMain.c -o DBMain.out

./DBMain.out $degree 2 111 -1 1 0 238
echo "Completed phase one."

./DBMain.out $degree 2 111 -1 2 0 238
echo "Completed phase two."