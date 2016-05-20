#!/bin/sh

count=10
telnetTestFile="telnet_test_cmd"
serveraddress="localhost"
outfile="result_log"

cmd="GET / HTTP/1.0\r\n\r\n"

rm -fr $telnetTestFile
rm -fr $outfile

mknod $telnetTestFile p
touch $outfile

exec 7<>$outfile
exec 8<>$telnetTestFile


while [ $count > 0 ];
do
	telnet $serveraddress 80 <&8 >&7 &
	sleep 1;echo $cmd >> $telnetTestFile
	tail -f $outfile
	( (count--))
done

