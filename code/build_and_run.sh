#!/bin/bash

set -e


g++ -std=c++1y -Wall -g wm.cpp -lX11


XEPHYR=$(whereis -b Xephyr | cut -f2 -d' ')
xinit ./xinitrc -- \
    "$XEPHYR" \
        :89 \
        -screen 800x600 \
        -host-cursor
