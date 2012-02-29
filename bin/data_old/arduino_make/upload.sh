#!/bin/sh
for i in /dev/tty.usb*; do
		# if the file is there
	filename=${i}
	#echo exporting $i to makefile
	PORT=$i
	export PORT
		#cp -p $i /etc/$filename
done

UPLOAD_RATE=57600
AVRDUDE_PROGRAMMER=stk500v1

if [ $1 == 'UNO' ]
then
  UPLOAD_RATE=115200
  AVRDUDE_PROGRAMMER=arduino
fi

export UPLOAD_RATE
export AVRDUDE_PROGRAMMER

BASEDIR=$(dirname $0)

cd $BASEDIR

echo Compiling... 1>progress

../Tools/make upload