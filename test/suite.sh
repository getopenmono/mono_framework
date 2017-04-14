#!/bin/bash

echo "Building DateTime test case..."
make -f date_time.mk && \
echo "Running DateTime test..." && \
./build/tests/date_time_test.o
