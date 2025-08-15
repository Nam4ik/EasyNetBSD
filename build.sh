#!/bin/sh 

echo "Checking requirements."

FILES_TO_CHECK="sys/types.h sys/disk.h sys/disklabel.h fcntl.h sys/ioctl.h unistd.h"


for file in $FILES_TO_CHECK; do
    if [ -f "/usr/include/$file" ]; then
        echo "OK: founded $file"
    else
        echo "ERROR: not founded $file"
    fi
done

if pkg_info -E 'gtk3*' >/dev/null 2>&1; then
    echo "OK: GTK3 installed"
else
    echo "ERROR: GTK3 not installed"
fi

if ! command -v pkg_info >/dev/null 2>&1; then
    echo "ERROR: pkg_info tool not founded. Is pkg_install installed?"
    exit 1
fi

echo "Check ended. Preparing to build."
