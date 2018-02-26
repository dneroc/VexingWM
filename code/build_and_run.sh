#!/bin/bash

set -e

#make 

XEPHYR=$(whereis -b Xephyr | cut -f2 -d' ')
xinit ./xinitrc -- \
    "$XEPHYR" \
        :89 \
        -screen 800x600 \
        -host-cursor
