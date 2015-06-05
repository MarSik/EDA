#!/bin/sh -x
function fix_name() {
MATCH=$1
REPLACE=$2
shift 2
sed -i -e "1,/RE/s/$MATCH/$REPLACE/" "$@" 
rename -v $MATCH $REPLACE "$@"
}

fix_name W7.62mm W300mil DIP*
fix_name W10.16mm W400mil DIP*
fix_name W15.24mm W600mil DIP*
fix_name W25.4mm W1000mil DIP*
fix_name W22.86mm W900mil DIP*

