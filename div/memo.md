## valgrind
	alias cmv='clear && make && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes'

### suppressing specific output
	--gen-suppressions=all
will show example suppression syntax for all errors found

	--suppressions=div/valgrind.supp
specifies a file, that contains suppressions formatted as output from gen-suppressions.

	-s

shows a summary of suppressed errors

### tracking file descriptors
	--track-fds=yes

### data races, pthread related stuff
	--tool=helgrind oder --tool=drd

The default tool used by Valgrind is `memcheck`

## misc
[awk delete lines following a pattern](https://stackoverflow.com/questions/4396974/sed-or-awk-delete-n-lines-following-a-pattern)
