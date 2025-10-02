#!/bin/bash

SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
MSHBIN="$SCRIPT_DIR/../../minishell"
SHELL_TESTCASE_DIR="$SCRIPT_DIR/../shell_tests"
MSHOUT="$SHELL_TESTCASE_DIR/mshout"
BSHOUT="$SHELL_TESTCASE_DIR/bshout"
DFFOUT="$SHELL_TESTCASE_DIR/diffout"

RED='\033[31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUEB='\033[1;34m'
RESET='\033[0m'

clear
set +o noclobber
find "$SHELL_TESTCASE_DIR" -name 'tc_*.result' -execdir rm -f '{}' ';'
find "$SHELL_TESTCASE_DIR" -type f -name 'tc_*' -not -name "tc_*.result" |
	while IFS= read -r TCSCRIPT; do
	{
		printf "${BLUEB}Test:${RESET} %s ..." "$TCSCRIPT"
		FAIL_OK=$([[ "$TCSCRIPT" =~ failOK$ ]] && echo "true" || echo "false")
		TCOUT="${TCSCRIPT}.result"
		truncate -s 0 "$TCOUT"

		<"$TCSCRIPT" "$MSHBIN" >"$MSHOUT" 2>&1
		MSRET=$?
		<"$TCSCRIPT" bash >"$BSHOUT" 2>&1
		BRET=$?
		diff --text "$MSHOUT" "$BSHOUT" > "$DFFOUT" 2>&1
		DRET=$?
		if [[ $DRET == 0 && $MSRET == $BRET ]]; then {
			printf "$GREEN OK\n$RESET"
		}
		else {
			if [[ $DRET != 0 ]]; then
			{
				printf "$RED diff fail ( < msh, > bash )\n$RESET" >> "$TCOUT" 2>&1
				cat "$DFFOUT" >> "$TCOUT" 2>&1
			} fi
			if [[ ($MSRET != $BRET) ]]; then
			{
				printf "$RED Return values differ:\n$RESET" >> "$TCOUT" 2>&1
				printf "  msh=%d, bsh=%d, diff=%d\n" $MSRET $BRET $DRET >> "$TCOUT" 2>&1
			} fi
			if $FAIL_OK; then
				printf "$YELLOW Fail OK\n$RESET"
			else
				printf "$RED KO\n$RESET"
				cat "$TCOUT"
			fi
		}
		fi
	} done

rm -f "$MSHOUT" "$BSHOUT" "$DFFOUT"