#!/bin/bash

EMUNO_DIR=../../emuno
BITBUCKET_HOST=bitbucket.org
EMUNO_URL="https://$BITB_USERNAME@bitbucket.org/openmono/emuno.git"
EMUNO_SSH="git@bitbucket.org:openmono/emuno.git"

TEST_APPS="https://github.com/getopenmono/simpletemp.git
https://github.com/getopenmono/templog.git
https://github.com/getopenmono/accelBeep.git
https://github.com/getopenmono/temperature.git
https://github.com/getopenmono/ttt.git
https://github.com/getopenmono/pong.git
https://github.com/getopenmono/chores.git"

if [ $TEST_CASES ]; then

	if [[ ! -d $EMUNO_DIR ]]; then
		echo "Emuno does not exist, downloading..."
		CUR_DIR=`pwd`
		cd `dirname $EMUNO_DIR`

		if [[ $BITB_SSH_KEY ]]; then
			echo "using SSH key"
			echo "$BITB_SSH_KEY" > bitbucket_key
			chmod 400 bitbucket_key
			DEST=`basename $EMUNO_DIR`
			mkdir -p ~/.ssh
			ssh-keyscan $BITBUCKET_HOST >> ~/.ssh/known_hosts
			ssh-agent bash -c "ssh-add bitbucket_key; git clone $EMUNO_SSH $DEST"
			rm bitbucket_key
		elif [[ $BITB_USERNAME ]]; then
			echo "using interactive login, with: $BITB_USERNAME"
			git clone $EMUNO_URL `basename $EMUNO_DIR`
		else
			echo "Env. var. BITB_USERNAME is not set. Run docker with this var set!"
			exit 1
		fi

		cd $CUR_DIR
	fi

	echo "Building DateTime test case..."
	make -f date_time.mk && \
	echo "Running DateTime test..." && \
	./build/tests/date_time_test.o && \
	echo "Building mono String test case..." && \
	make -f mn_string.mk && \
	echo "Running String test..." && \
	make -f mn_string.mk run &&
	echo "Building mono Queue test case..." && \
	make -f queue.mk && \
	echo "Running Queue test..." && \
	make -f queue.mk run &&
	echo "Building mono File I/O test case..." && \
	make -f file.mk && \
	echo "Running File I/O test..." && \
	make -f file.mk run || exit 1

fi

# Run kiosk apps

function bail {
	echo "$1 failed"
	exit 1
}

function buildApp {
	FILENAME="${1##*/}"
	FOLDER="${FILENAME%.*}"
	echo "Testing: $FOLDER"
	if [ -d testApps/$FOLDER ]; then rm -rf testApps/$FOLDER; fi
	git clone $1 testApps/$FOLDER || exit 1
	cp app_makefile/main.cpp testApps/$FOLDER/.
	APP_NAME="$FOLDER" APP_DIR_PATH="." make -C testApps/$FOLDER -f ../../app_makefile/Makefile -j || bail $FOLDER
}

if [ $TEST_EXISTING_APPS ]; then
	echo "Testing existing apps from GitHub..."
	IFS=$'\n'
	for url in $TEST_APPS; do
		buildApp $url
	done
fi