#!/bin/sh
ROOT_PATH="/home/mimose/programs/TINY"
file=$1
otherfile=$2
buildfiledir="$ROOT_PATH/bin/"
if [ -d $buildfiledir ];then
	echo
else
	mkdir $buildfiledir
fi

if [ "$3" = "-d" ];then
	isdebug="-DDEBUG"
else
	isdebug=""
fi

if [ -f $file ];then
	gcc -Wall -c $isdebug $otherfile -I$ROOT_PATH/lib
	gcc -Wall -c $isdebug $file -I$ROOT_PATH/lib
	gcc -Wall -o ${file%.c*} ${file%.c*}.o ${otherfile%.c*}.o $ROOT_PATH/lib/libcsapp.so
else
	echo "$file NOT exists"
	exit 1
fi

# remove $1.out file
rm ${file%.c*}.o
rm ${otherfile%.c*}.o


# move execable file 
mv ${file%.c*} $buildfiledir

exit 0
