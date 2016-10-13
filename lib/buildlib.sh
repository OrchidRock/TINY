#!/bin/sh

#gcc -c csapp.c
#gcc -c sbuf.c
#ar crv libcsapp.a csapp.o sbuf.o
gcc -shared -fPIC -o libcsapp.so csapp.c sbuf.c -lpthread
exit 0


