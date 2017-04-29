#!/bin/bash

# This script tests whether or not the img2icon compiler i present.
# If it is not, it will check for Qt dev. env. and try to download and build it!
MKFILE_TEMPLATE=icons.mk.tmp
GIT_REPO="https://github.com/getopenmono/img2icon.git"

if hash img2icon 2>/dev/null; then
	echo "OK: img2icon compiler is present in PATH"
	exit 0
fi

echo "ISSUE: img2icon compiler does not exists in PATH"
if hash qmake; then
	echo "Qt dev tools are installed, will try to build img2icon..."

	MKFILE=${MKFILE_TEMPLATE%.*}
	if [[ $# > 0 ]]; then
		MKFILE=$1
	else
		if [[ ! -f $MKFILE && -f $MKFILE_TEMPLATE ]]; then
			echo "Creating $MKFILE from template..."
			cp $MKFILE_TEMPLATE $MKFILE
		fi
	fi

	if [[ ! -f $MKFILE ]]; then
		echo "Cannot find icons makefile at $MKFILE"
		exit 1
	fi

	DIST=../dist
	if [[ $# > 1 ]]; then
		DIST=$2
	fi
	if [[ ! -d $DIST ]]; then
		mkdir -p $DIST
	fi

	if [[ ! -d img2icon ]]; then
		if ! git clone $GIT_REPO; then
			exit 1
		fi
	fi
	PWD=`pwd`
	cd img2icon && \
		qmake && \
		make && \
		cd .. && \
		sed -i bak "s@IMGICON=.*img2icon@IMGICON=$PWD/img2icon/img2icon@" $MKFILE && \
		sed -i bak "s@DIST=../dist@DIST=$DIST@" $MKFILE && \
		echo "SUCCESS: You can run icons makefile" && \
		exit 0

	exit 1
fi

echo "ERROR: No img2icon present and no QT build environment!"
exit 1