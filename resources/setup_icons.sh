#!/bin/bash

# This script tests whether or not the img2icon compiler i present.
# If it is not, it will check for Qt dev. env. and try to download and build it!

GIT_REPO="https://github.com/getopenmono/img2icon.git"

if hash img2icon; then
	echo "OK: img2icon compiler is present in PATH"
	exit 0
fi

echo "ISSUE: img2icon compiler does not exists in PATH"
if hash qmake; then
	echo "Qt dev tools are installed, will try to build img2icon..."
	
	MKFILE=icons.mk
	DIST=../dist
	if [[ $# > 0 ]]; then
		MKFILE=$1
	fi
	
	if [[ ! -f $MKFILE ]]; then
		echo "Cannot find icons makefile at $MKFILE"
		exit 1
	fi
	
	if [[ $# > 1 ]]; then
		DIST=$2
	fi
	
	if [[ -d $DIST ]]; then
		echo "ERROR: Destination dir does not exist: $DIST"
		exit 1
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
	sed --in-place=bak "s@IMGICON=img2icon@IMGICON=$PWD/img2icon/img2icon@" $MKFILE && \
	sed --in-place=bak "s@DIST=../dist@DIST=$DIST@" $MKFILE && \
	echo "SUCCESS: You can run icons makefile" && \
	exit 0
	exit 1
fi

echo "ERROR: No img2icon present and no QT build environment!"
exit 1