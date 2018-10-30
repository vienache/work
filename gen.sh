#!/bin/bash

echo 'Install astyle...'
cd .git/hooks &>/dev/null
if ! { [ -L "pre-commit" ] && [ -e "pre-commit" ]; }
then
  ln -s ../../config/githook-astyle.sh pre-commit
fi
cd - &>/dev/null

echo 'Create build directory...'
mkdir -p build && cd build

echo 'Build project...'
cmake .. -G"Unix Makefiles"
