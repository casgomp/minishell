#!/bin/bash

MSHBIN="../minishell"
SHELL_TESTCASE_DIR="./shell_tests"
MSHOUT="$SHELL_TESTCASE_DIR/mshout"
BSHOUT="$SHELL_TESTCASE_DIR/bshout"
DFFOUT="$SHELL_TESTCASE_DIR/diffout"

GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUEB='\033[1;34m'
RESET='\033[0m'

clear;
find "$SHELL_TESTCASE_DIR" -name 'tc_*' |
	while IFS= read -r testcasescript; do
	{
		printf "\n${BLUEB}Test: %s${RESET}\n" "$testcasescript"
		<"$testcasescript" "$MSHBIN" >"$MSHOUT"
		MSRET=$?
		<"$testcasescript" bash >"$BSHOUT"
		BRET=$?
		diff --text "$MSHOUT" "$BSHOUT" > "$DFFOUT"
		DRET=$?
		printf "retvals: msh=%d, bsh=%d, diff=%d\n" $MSRET $BRET $DRET
		[[ $DRET == 0 ]] && echo diff ok || {
			printf "diff fail ( < msh, > bash )\n"
			cat "$DFFOUT"
		}
		[[ $MSRET == $BRET ]] && echo return ok || echo return fail
	} done