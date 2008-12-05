#!/bin/bash

if [ "z$1" == "z" ]; then
   name=test
else
   name=$1
fi

arch=t44
if [ "$1" == "receiver" ]; then
  arch=atmega16
fi

sudo avrdude -c bsd -p $arch -E noreset -U flash:w:$name.hex
