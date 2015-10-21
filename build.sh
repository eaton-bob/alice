#!/bin/sh

# zproject build is broken - let's ignore it

for PROG in comp ups; do
    gcc -lmlm -lczmq -lzmq src/${PROG}.c > ${PROG}
done

g++ -lmlm -lczmq -lzmq ui/ui.cc > ui
