#!/bin/bash

find . -name infile |
	while IFS= read -r file; do
	{
		<$file ../../minishell >msout
		MSRET=$?
		<$file bash >bashout
		BRET=$?
		diff --text msout bashout >/dev/null
		DRET=$?
		printf "retvals: msh=%d, bsh=%d, diff=%d\n" $MSRET $BRET $DRET
		[[ $DRET == 0 ]] && echo diff ok || echo diff fail
		[[ $MSRET == $BRET ]] && echo return ok || echo return fail
	} done