#!/bin/bash --norc

OLDWD="$(pwd)"
WORKDIR="/tmp/mshtest_$(date +%F_%T)"
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
MSHBIN="$SCRIPT_DIR/../../minishell"
SHELL_TESTCASE_DIR="$SCRIPT_DIR/../shell_tests"
# MSHOUT="$WORKDIR/mshout"
# BSHOUT="$WORKDIR/bshout"
# DFFOUT="$WORKDIR/diffout"
MSHOUT="mshout"
BSHOUT="bashout"
DFFOUT="diffout"


RED='\033[31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUEB='\033[1;34m'
RESET='\033[0m'

export SHELLSCRIPTDIR="$SCRIPT_DIR/../shell_scripts/"
export LC_ALL=C

set +o noclobber
set +x
clear

printf "################################################################\n"
printf "### msh tests\n"
printf "################################################################\n"

WORKDIRS="${WORKDIR%5-*}*"
WORKDIRS="$(ls -dt1 $WORKDIRS | tail -n+3)"
if [[ ! -z "$WORKDIRS" ]]; then
	printf "\n\nDo you want to cleanup all old output data in:\n"
	printf "(The 2 latest ones will be kept in any case)\n"
	echo "$WORKDIRS"
	PS3="Please select an option (number): "
	select ANS in YES no; do
		if [[ "$ANS" == "YES" ]]; then
			rm -r $WORKDIRS
			printf "\nStill available output dirs:\n"
			ls -1 -d /tmp/ms* # | tail -n +3
		fi
		break	
	done
fi

mkdir -p "$WORKDIR"
cd "$WORKDIR"
printf "\nWorking and results directory: %s\n\n" $(pwd)

if [[ "$WORKDIR" == "$(pwd)" ]]; then
	printf "Cleaning up $(pwd):\n"
	rm -v *
	printf " ... done.\n\n"
fi

TC_SCRPTS="tc_*"
if [[ "$1" ]]; then
	TC_SCRPTS=$1
fi

# find "$SHELL_TESTCASE_DIR" -name 'tc_*.result' -execdir rm -f '{}' ';'
TC_SCRPTS="$(find "$SHELL_TESTCASE_DIR" -type f -name $TC_SCRPTS -not -name "tc_*.result" | sort)"
if [[ -z "$TC_SCRPTS" ]]; then
	printf "No testcases found for pattern '$TC_SCRPTS'.\n"
	printf "Please check you input parameter.\n\n"
else
echo "$TC_SCRPTS" |
	while IFS= read -r TCSCRIPT; do
	{
		printf "\n################################################################################################\n"
		printf "${BLUEB}Test:${RESET} %s\n" "$TCSCRIPT"
		FAIL_OK=$([[ "$TCSCRIPT" =~ failOK$ ]] && echo "true" || echo "false")

		cd "$WORKDIR"
		TESTDIR="$(basename $TCSCRIPT)"
		mkdir -p "$TESTDIR"
		cd "$TESTDIR"		
		TCOUT="${TESTDIR}.result"
		truncate -s 0 "$TCOUT"

		<"$TCSCRIPT" "$MSHBIN" >"$MSHOUT"_1 2>"$MSHOUT"_2
		MSRET=$?
		<"$TCSCRIPT" bash --norc >"$BSHOUT"_1 2>"$BSHOUT"_2
		BRET=$?
		diff --text "$MSHOUT"_1 "$BSHOUT"_1 > "$DFFOUT"_1 2>&1
		DRET_1=$?
		# replace tje different shell names with "xxsh"
		# and rm the occasional "line x:" filed from the error output
		sed -Ei 's/(m|ba)sh:( line [0-9]+:)?/xxsh:/' "$MSHOUT"_2 "$BSHOUT"_2
		diff --text "$MSHOUT"_2 "$BSHOUT"_2 > "$DFFOUT"_2 2>&1
		DRET_2=$?
		if [[ $DRET_1 == 0 && $DRET_2 == 0 && $MSRET == $BRET ]]; then {
			printf "$GREEN  ... OK\n$RESET"
		}
		else {
			printf "${BLUEB}StdOut Diff: $RESET" >> "$TCOUT" 2>&1
			if [[ $DRET_1 != 0 ]]; then
			{
				printf "$RED fail ( < msh, > bash )\n$RESET" >> "$TCOUT" 2>&1
				cat "$DFFOUT"_1 >> "$TCOUT" 2>&1
			} 
			else
				printf "$GREEN OK\n$RESET" >> "$TCOUT" 2>&1
			fi
			printf "${BLUEB}StdErr Diff: $RESET" >> "$TCOUT" 2>&1
			if [[ $DRET_2 != 0 ]]; then
			{
				printf "$RED fail ( < msh, > bash )\n$RESET" >> "$TCOUT" 2>&1
				cat "$DFFOUT"_2 >> "$TCOUT" 2>&1
			}
			else
				printf "$GREEN OK\n$RESET" >> "$TCOUT" 2>&1
			fi
			printf "${BLUEB}Return values: $RESET" >> "$TCOUT" 2>&1
			if [[ ($MSRET != $BRET) ]]; then
			{
				printf "$RED fail\n$RESET" >> "$TCOUT" 2>&1
				printf "  msh=%d, bash=%d, diffstdout=%d, diffstderr=%d\n" $MSRET $BRET $DRET_1 $DRET_2 >> "$TCOUT" 2>&1
			}
			else
				printf "$GREEN OK\n$RESET" >> "$TCOUT" 2>&1
			fi
			if $FAIL_OK; then
				printf "$YELLOW Fail OK\n$RESET"
			else
				printf "$RED  ... KO\n$RESET"
				cat "$TCOUT"
			fi
		}
		fi
		printf "================================================================================================\n"		
		=rm {"$MSHOUT","$BSHOUT","$DFFOUT"}_{1,2} 2>/dev/null
	} done
fi
cd "$OLDWD"