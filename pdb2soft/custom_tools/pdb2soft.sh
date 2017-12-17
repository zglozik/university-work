#!/bin/sh

DATA=$1
if [ ! -r "$DATA" ]; then
	echo "DATA file is absent, sorry" >&2
	exit 1
fi

INPUT=`awk '/FILE_NAME/ { print $2 }' $DATA`
if [ ! -r "$INPUT" ]; then
	echo "specified file name can't be read, sorry"
	exit 1
fi

SPHERE_HORIZONTAL=`awk '/SPHERE_HORIZONTAL/ { print $2 }' $DATA`
SPHERE_VERTICAL=`awk '/SPHERE_VERTICAL/ { print $2 }' $DATA`
ROLL_LONGITUDE=`awk '/ROLL_LONGITUDE/ { print $2 }' $DATA`
ROLL_LATITUDE_STEP=`awk '/ROLL_LATITUDE_STEP/ { print $2 }' $DATA`
ROLL_LATITUDE_BASE=`awk '/ROLL_LATITUDE_BASE/ { print $2 }' $DATA`

pdb2soft -v "$SPHERE_VERTICAL" -h "$SPHERE_HORIZONTAL" \
	-b "$ROLL_LATITUDE_BASE" -l "$ROLL_LONGITUDE" -r "$ROLL_LATITUDE_STEP"\
	$INPUT 
rm $DATA
