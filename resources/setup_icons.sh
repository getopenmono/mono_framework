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

	if [[ $# > 0 ]]; then
		MKFILE_TEMPLATE=$1
	fi
	
	if [[ ! -f $MKFILE_TEMPLATE ]]; then
		echo "Cannot find icons template makefile at $MKFILE_TEMPLATE"
		exit 1
	fi
	
	MKFILE=${MKFILE_TEMPLATE%.*}
	if [[ ! -f $MKFILE ]]; then
		echo "Creating $MKFILE from template..."
		cp $MKFILE_TEMPLATE $MKFILE
	fi

	DIST=../dist
	if [[ $# > 1 ]]; then
		DIST=$2
		echo "Using custom destination: $DIST"
	fi
	if [[ ! -d $DIST ]]; then
		mkdir -p $DIST
	fi

	if [[ ! -d img2icon ]]; then
		if ! git clone $GIT_REPO; then
			exit 1
		fi
	else
		echo "img2icon repo already exists, clean & update repo..."
		cd img2icon && \
		git reset --hard && \
		git clean -fXd && \
		git checkout && \
		git pull && \
		cd .. || exit 1
	fi
	
	PWD=`pwd`
	sed -ibak "s@IMGICON=.*img2icon@IMGICON=$PWD/img2icon/img2icon@" $MKFILE && \
	sed -ibak "s@DIST=../dist@DIST=$DIST@" $MKFILE
	if [ ! $? ]; then
		echo "ERROR: Failed to replace paths in icons makefile template!"
		exit 1
	fi
	
	if [[ $# > 2 && $3 == "win" ]]; then
		echo "Building for Windows with MSVC++ toolchain..."
		cd img2icon && \
			qmake -tp vc project.pro && \
			ls -l && \
			MSBuild.exe img2icon.vcxproj //p:Configuration=Release //p:Platform=x86 && \ 
			cd .. && \
			sed -ibak "s@IMGICON=.*img2icon@IMGICON=$PWD/img2icon/release/img2icon.exe@" $MKFILE && \
			echo "SUCCESS: You can run icons makefile" && \
			cat $MKFILE && \
			exit 0 || exit 1
	else
		cd img2icon && \
			qmake && \
			make && \
			cd .. && \
			echo "SUCCESS: You can run icons makefile" && \
			exit 0 || exit 1
	fi
	
	echo "ERROR: Compile error!"
	exit 1
fi

echo "ERROR: No img2icon present and no QT build environment!"
exit 1