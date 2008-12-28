#!/bin/bash

if [ "z$1" == "z" ]; then
    echo "no default upload, sorry"
    exit 1;
fi

if [ "$1" == "receiver" ]; then
  arch=atmega16
fi
if [ "$1" == "sensor" ]; then
  arch=t44
fi
if [ "$1" == "accmain" ]; then
  arch=atmega8
fi

if [ "z$arch" == "z" ]; then
    echo "unknown upload -- aborting"
    exit 1;
fi

#sudo avrdude -c bsd -p $arch -E noreset -U flash:w:$1.hex
avrdude -c usbasp -p $arch -U flash:w:$1.hex
