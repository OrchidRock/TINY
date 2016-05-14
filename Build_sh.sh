#!/bin/sh
file=$1
buildfiledir="bin/"
otherobject="transaction.c"
if [ -d $buildfiledir ];then
	echo
else
	mkdir $buildfiledir
fi

if [ "$2" = "-d" ];then
	isdebug="-DDEBUG"
else
	isdebug=""
fi

if [ -f $file ];then
	gcc -c $isdebug $otherobject 
	gcc -c $isdebug $file
	gcc -o ${file%.c*} ${file%.c*}.o ${otherobject%.c*}.o -L./lib -lcsapp -lpthread
else
	echo "$file NOT exists"
	exit 1
fi

# remove $1.out file
rm ${file%.c*}.o
rm ${otherobject%.c*}.o


# move execable file 
mv ${file%.c*} $buildfiledir

exit 0
