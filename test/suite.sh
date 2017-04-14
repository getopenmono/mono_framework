#!/bin/bash

EMUNO_DIR=../../emuno
EMUNO_URL="https://$BITB_USERNAME@bitbucket.org/openmono/emuno.git"
EMUNO_SSH="git@bitbucket.org:openmono/emuno.git"

if [[ ! -d $EMUNO_DIR ]]; then
	echo "Emuno does not exist, downloading..."
	CUR_DIR=`pwd`
	cd `dirname $EMUNO_DIR`
	
	if [[ $BITB_SSH_KEY ]]; then
		echo "using SSH key"
		CUR=`pwd`
		echo "$BITB_SSH_KEY" > bitbucket.key
		chmod 600 bitbucket.key
		DEST=`basename $EMUNO_DIR`
		ssh-agent bash -c "ssh-add bitbucket.key; git clone $EMUNO_SSH $DEST"
		rm bitbucket.key
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
./build/tests/date_time_test.o
