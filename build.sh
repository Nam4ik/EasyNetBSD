#!/bin/sh 

echo "Checking requirements."

FILES_TO_CHECK="sys/types.h sys/disk.h sys/disklabel.h fcntl.h sys/ioctl.h unistd.h sys/dkio.h"

DEPS_SATISFIED=true 

for file in $FILES_TO_CHECK; do
    if [ -f "/usr/include/$file" ]; then
        echo "OK: founded $file"
    else
        echo "ERROR: not founded $file"
        DEPS_SATISFIED=false
    fi
done

if pkg-config --cflags --libs gtk+-3.0 >/dev/null 2>&1; then
    echo "OK: GTK3+ installed"
else
    echo "ERROR: GTK3+ not installed"
    DEPS_SATISFIED=false
fi

if cmake --version >/dev/null 2>&1; then
    echo "OK: CMake installed" 
else 
    echo "ERROR: CMake not installed"
    DEPS_SATISFIED=false
fi
#if ! command -v pkg_info >/dev/null 2>&1; then
#    echo "ERROR: pkg_info tool not founded. Is pkg_install installed?"
#    exit 1
#fi

if "$DEPS_SATISFIED" == "true"; then 
   echo "Check ended. Preparing to build."
   rm -rf build && mkdir build 
   cmake -S AutoNBSD/ -B build/
   make -f build/Makefile 
else 
   echo "You have unsatisfied requirements."
fi 


