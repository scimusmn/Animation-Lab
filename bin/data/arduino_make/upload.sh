#!/bin/sh
for i in /dev/tty.usb*; do
		# if the file is there
	filename=${i}
	#echo exporting $i to makefile
	PORT=$i
	export PORT
		#cp -p $i /etc/$filename
done

BASEDIR=$(dirname $0)

cd $BASEDIR

echo Compiling... 1>progress

../Tools/make upload