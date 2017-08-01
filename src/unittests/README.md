# Unit tests for Mono framework

Place all unit tests in this directory.

If you are implementing, say, a queue class in `src/containers/queue.cpp`, then put the unit test `test_queue.cpp` in this directory and add `containers/queue.cpp` to the list of files to be linked into the unit tests in [`unittests.mk`](`unittests.mk`).

If you need some static data or misc functionality for the unit test, put it under the subdirectories `fixtures` or `lib`.
