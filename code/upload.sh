#!/bin/bash

if [ "z$1" == "z" ]; then
   name=test
else
   name=$1
fi

sudo avrdude -c bsd -p t44 -U flash:w:$name.hex
