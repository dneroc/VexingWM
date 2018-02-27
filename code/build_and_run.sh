#!/bin/bash

set -e


gcc -std=c++1y -lX11 -Wall -g wm.cpp 


XEPHYR=$(whereis -b Xephyr | cut -f2 -d' ')
xinit ./xinitrc -- \
    "$XEPHYR" \
        :89 \
        -screen 800x600 \
        -host-cursor
