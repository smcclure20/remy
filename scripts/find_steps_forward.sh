#!/bin/bash

num=0
for i in /home/sarah_mcclure/remy/results/cfg0808-fct.*; do
    newnum=$(python3 inspect-whiskers.py $i)
    if [ $num -lt $newnum ]
    then 
	    num=$newnum
	    echo $i
	    echo $num
    fi
done
