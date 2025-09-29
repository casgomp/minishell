#!/bin/bash

MSHBIN="../minishell"
SHELL_TESTCASE_DIR="./shell_tests"
MSHOUT="$SHELL_TESTCASE_DIR/mshout"
BSHOUT="$SHELL_TESTCASE_DIR/bshout"
DFFOUT="$SHELL_TESTCASE_DIR/diffout"

RED='\033[31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUEB='\033[1;34m'
RESET='\033[0m'

clear;
find "$SHELL_TESTCASE_DIR" -name 'tc_*' |
find "$SHELL_TESTCASE_DIR" -type f -name 'tc_*' -not -name "tc_*.result" |
	while IFS= read -r testcasescript; do
	{
		printf "${BLUEB}Test:${RESET} %s ..." "$testcasescript"
		TCOUT="${testcasescript}.result"

		<"$testcasescript" "$MSHBIN" >"$MSHOUT"
		MSRET=$?
		<"$testcasescript" bash >"$BSHOUT"
		BRET=$?
		diff --text "$MSHOUT" "$BSHOUT" > "$DFFOUT"
		DRET=$?
		if [[ $DRET == 0 && $MSRET == $BRET ]]; then
			printf "$GREEN OK\n$RESET"
		else {
			printf "$RED KO\n$RESET"
			if [[ $DRET != 0 ]]; then
			{
				printf "$RED diff fail ( < msh, > bash )\n$RESET" | tee -a > "$TCOUT"
				cat "$DFFOUT" | tee -a > "$TCOUT"
			} fi
			if [[ $MSRET != $BRET ]]; then
			{
				printf "$RED Return values differ:\n$RESET" | tee -a > "$TCOUT"
				printf "  msh=%d, bsh=%d, diff=%d\n" $MSRET $BRET $DRET | tee -a > "$TCOUT"
			} fi
		}
		fi
	} done

rm -f "$MSHOUT" "$BSHOUT" "$DFFOUT"