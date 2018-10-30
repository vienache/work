#!/bin/bash
# Installation:
#   cd .git/hooks
#   ln -s ../../githook-astyle.sh pre-commit

OPTIONS_FILE="config/astyle-options"
OPTIONS="--options=$OPTIONS_FILE"

ASTYLE=$(which astyle)
if [ $? -ne 0 ]; then
	echo "[ERROR] astyle not installed. Unable to check source file format policy." >&2
	exit 1
fi

if [ ! -f $OPTIONS_FILE ];
then
	echo "[ERROR] There is no $OPTIONS_FILE with formatting rules." >&2
	exit 1
fi

FILES=`git diff --cached --name-only --diff-filter=ACMR | grep -E "\.(c|cpp|h|hpp)$"`
for FILE in $FILES; do
	echo "[ASTYLE INFO]" >&2
	$ASTYLE $OPTIONS $FILE
    git add $FILE
done

exit 0
