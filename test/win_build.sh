#!/bin/bash
GCC_ARM_WIN_URL="https://launchpad.net/gcc-arm-embedded/5.0/5-2015-q4-major/+download/gcc-arm-none-eabi-5_2-2015q4-20151219-win32.zip"
GCC_ARM_FILE_NAME="${GCC_ARM_WIN_URL##*/}"
GCC_ARM_DIR_NAME="${GCC_ARM_FILE_NAME%.*}"

function downloadGcc {
	FILE=$(basename $1)
	
	if [[ ! -f $FILE && ! -d $GCC_ARM_DIR_NAME ]]; then
		echo "Downloading GCC Embedded Arm..."
		curl -O -L $1
	else
		echo "Skipping GCC download"
	fi
	
	if [ ! -d $GCC_ARM_DIR_NAME ]; then
		extension="${FILE##*.}"
		echo "Extracting GCC... ($extension)"
		
		if [[ $extension == "zip" ]]; then
			unzip -qn $FILE -d $GCC_ARM_DIR_NAME
		else
			tar -xjf $FILE
		fi
	else
		echo "GCC is already extracted"
	fi
	
	if [ ! -d $GCC_ARM_DIR_NAME ]; then
		echo "Error: extracted directory is not named $GCC_ARM_DIR_NAME"
		exit 1
	fi
	echo "GCC downloaded and extracted"
}

downloadGcc $GCC_ARM_WIN_URL