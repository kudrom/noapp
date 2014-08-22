#!/usr/bin/env bash
if [ $# -ne 2 ]
then
    echo "Usage: split_utterances <raw_data> <dest_dir>"
    exit -1
fi

in="$PWD/$1"
in_length=$in".length"
acc=0
dir=$2
number=(first second third fourth fifth sixth seventh eighth ninth tenth eleventh twelfth thirteenth fourteenth fifteenth sixteenth seventeenth eigtheenth nineteenth)

mkdir $dir 2> /dev/null
cd $dir

i=0
while read linea
do
    out="${number[$i]}_utt"
    echo "dd bs=1 count=$linea skip=$acc if=$in of=$out"
    dd bs=1 count=$linea skip=$acc if=$in of=$out

    i=$((i + 1))
    acc=$(($acc + $linea))
done < $in_length
