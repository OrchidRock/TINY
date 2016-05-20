#!/bin/sh

gcc -c csapp.c
gcc -c sbuf.c
ar crv libcsapp.a csapp.o sbuf.o


exit 0


