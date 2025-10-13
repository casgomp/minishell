# Bash behavior *see notion page for clearer style: https://www.notion.so/42wolfsburgberlin/Bash-behavior-288937251cae8043b76bfa7390c5ce29?source=copy_link

## In execution

### *Codeflow (for error handling see examples section):*

- First, parent always checks if there are (b) redirections and/or (c) pipes, or neither (a).
- That sets the basis to decide the codeflow:

(a) Simple command

- Parent:
    1. Check validity of cmd: i) call `stat(const char *pathname, struct stat *statbuf)`, ii) call `access(const char *path, int amode)`.
    2. Execution: `execve(const char *pathname, char *const argv[],
    char *const envp[])`.
    3. Call `exit(int status)`.

- (b) Simple command with redirections
- Parent:
    1. Check validity of cmd: i) call `stat`, ii) call `access`.
    2. Call `fork()`, and `waitpid(pid_t pid, int *wstatus, int options)`.
    3. Call `exit`.
- Child:
    1. For N redirections (sequential):
        1.  `open(const char *path, int oflag, …)`
        2. `dup2(int oldfd, int newfd)`
        3. `close(int fd)`
    2. Execution: `execve`
    3. Call `exit`. If execve fails or doesn’t happen due to invalid file, then clean memory before exiting. No need to close stdin/out as that is taken care of by the kernel.

- (c) Pipeline
- Parent:
    1. For N pipes:
        1. `pipe(int[2])`
        2. `fork(int pid)`
        3. `close`
    2. `wait`
- For N children:
    1. `close` unused pipe ends and redirect stdin/out to pipe ends with `dup2`.
    2. Check validity of cmd: i) call `stat`, ii) call `access`.
    3. For N redirections (sequential):
        1.  `open(const char *path, int oflag, …)`
        2. `dup2(int oldfd, int newfd)`
        3. `close(int fd)`
    4. Execution: `execve`
    5. Call `exit`. If execve fails or doesn’t happen due to invalid file, then clean memory before exiting. No need to close stdin/out as that is taken care of by the kernel.

### *Examples:*

Simple command

cat filex

- No children (no forking, cloning, etc). Everything happens in parent.
- Parent calls `newfstatat` and then `access` to validate command (apparently using both is important, since stat tells if the file, i.e. the command, exists, what kind of file it is, permission bits, etc., while access tells if current user has permission to execute the file). Then calls `execve`.

cat invalidcmd

- Same as above, but calling `newfstat` gets -1 ENOENT, and calls `exit(127)` without continuing with validation or execution.

cat wrongfilename

- Same as in cat filex, but at execve, when cat tries to open file with wrong file name, it gets -1 ENOENT (No such file or directory).

cat filewithoutpermissions

- Same as above but return is -1 EACCESS (permission denied).

Simple command with redirections

Parent: Verify command with newfstatat and access → Create child process. Wait.

Child: Process redirections one (open, dup2, close) by one (open, dup2, close). Call execve.

cat < filex

- Parent calls `newfstatat` and then `access` to validate command. Then calls `fork` (or clone) to create one child process and `wait`.
- Child `open("filex", O_RDONLY)= 3`. Calls `dup2(3, 0)`, and `close(3)`. Then calls `execve` for cat.

cat < filex >filep

- Parent calls `newfstatat` and then `access` to validate command. Then calls `fork` (or clone) to create one child process and `wait`.
- Child `open("filex", O_RDONLY)= 3`. Calls `dup2(3, 0)`, and `close(3)`. Then does next redirection: `open("filep", O_WRONLY|O_CREAT|O_TRUNC, 0666) = 3`, calls `dup2(3, 1)`, and `close(3)`. Then calls `execve` for cat.

cat < filex >> filep

- Same as above but filep gets opened in append mode: `open("filep", O_WRONLY|O_CREAT|O_APPEND, 0666)`.

cat < filex <filey <filez >filep   or  cat < filex <filey >filep <filez

- Parent: Same as above.
- Child: Same as above. Both cases redirections happen sequentially regardless of whether it is to stdin or stdout. Even if stdout redirection happens between other stdin redirections (as in the second case), all redirections are done sequentially: `open("filep", O_WRONLY|O_CREAT|O_TRUNC, 0666) = 3`, then `dup(3,1)`, `close(3)`. And then continues stdin redirections: `open("filez", O_RDONLY) = 3`, `dup2(3,0)`, `close(3)`.

cat >outfile filewithoutpermissions

- Same as simple case but first parent forks a child, and in child after execve, cat return is -1 EACCESS (permission denied).

cat >outfile <filewithoutpermissions

- Same as case that works (cat < filex >filep), but when trying to open file without permissions gets -1 EACCESS, so it never calls execve. outfile gets creatd anyways because it appears before. If >outfile was after <filewithoutpermissions, then outfile wouldn’t be created.

*heredoc: (Always parent first reads heredoc contents and stores in memory (heap). Then access them in child if necessary).*

cat << EOF

- Parent: calls `newfstatat` and then `access` to validate command. Then calls `fork` (or clone) to create one child process and `wait`.
- Child: Create pipe for heredoc: `pipe` [3, 4], `write(4, “heredoc text”, 12)`, `close(4)`, `dup(3,0)`, `close(3)`. Then call `execve` for cat.

cat << EOF > outfile

- Parent: calls `newfstatat` and then `access` to validate command. Then calls `fork` (or clone) to create one child process and `wait`.
- Child: `pipe` [3, 4], `write(4, “heredoc text”, 12)`, `close(4)`, `dup(3,0)`, `close(3)`. Then `open("filep", O_WRONLY|O_CREAT|O_TRUNC, 0666)`=3, `dup(3,1)`, `close(3)`. Then call `execve` for cat.

cat <filex <<EOF <filey >filep <filez >filepp >fileppp

- Parent: calls `newfstatat` and then `access` to validate command. Then calls `fork` (or clone) to create one child process and `wait`.
- Child: `open("filex", O_RDONLY)`= 3, `dup2(3,0)`, `close(3)`. Then child opens pipe for heredoc and writes even though it wont be used by cat because there’’s another stdin redirection after: `pipe` [3, 4], `write(4, “heredoc text”, 12)`, `close(4)`, `dup(3,0)`, `close(3)`. Then `open("filey", O_RDONLY)= 3`, `dup2(3, 0)`, and `close(3)`. Then `open("filep", O_WRONLY|O_CREAT|O_TRUNC, 0666)`=3, `dup(3,1)`, `close(3)`. Then `open("filez", O_RDONLY)= 3`, `dup2(3, 0)`, and `close(3)`. Then `open("filepp", O_WRONLY|O_CREAT|O_TRUNC, 0666)`=3, `dup(3,1)`, `close(3)`. Then `open("fileppp", O_WRONLY|O_CREAT|O_TRUNC, 0666)`=3, `dup(3,1)`, `close(3)`. Then call `execve` for cat.

cat <<A  > filep <<B (here each heredoc starts independently. B starts after A)

- Parent: calls `newfstatat` and then `access` to validate command. Then calls `fork` (or clone) to create one child process and `wait`.
- Child: First heredoc1: `pipe` [3, 4], `write(4, “heredoc1 text”, 13)`, `close(4)`, `dup(3,0)`, `close(3)`. Then `open("filep", O_WRONLY|O_CREAT|O_TRUNC, 0666)`=3, `dup(3,1)`, `close(3)`. Second heredoc: `pipe` [3, 4], `write(4, “heredoc2 text”, 13)`, `close(4)`, `dup(3,0)`, `close(3)`. Then call `execve` for cat.

<<EOF cat wc >filep

- Parent: calls `newfstatat` and then `access` to validate command. Then calls `fork` (or clone) to create one child process and `wait`.
- Child: Same as usual for heredoc. Call execve for cat, and cat fails because it thinks wc is a filename, so returns -1 ENOENT (No such file or directory, and filep gets nothing written. All that is handled by cat or whatever command is being used).

cat <<A <filex <nonexistingfile <<B

- Parent: same as above.
- Child: Same as usual, but getting -1 ENOENT when trying to open nonexisiting file, then `write` “bash: nonexisitingfile: No such file or directory”, and `exit(1)`.

Pipes (parent will exit with exit status of last cmd in pipeline)

cat filex filey filez | grep c

- Parent calls `pipe`, which sets fd[] to, let’s say, [3,4]. Then `fork` to create first child. Then `close` write end of pipe. Then `fork` second child, Then `close` read end of pipe. Then `wait`.
- Child 1: Close read end of pipe: `close(3)`, redirect stdout to write end of pipe with `dup2(4,1)`, and `close(4)`. Call `newfstat` and `access` for cat. Then `execve` with cat. Cleaning (i.e. closing fd’s) is done by cat. If it were to fail, then the child process would clean.
- Child 2: Redirect stdin to read end of pipe with `dup2(3,0)`, and `close(3)`. No need to close write end of pipe at the beginning because it has inherited a closed write end already as parent closes it before forking child 2. Call `newfstat` and `access` for grep. Then `execve` with grep. Cleaning (i.e. closing fd’s) is done by grep. If it were to fail, then the child process would clean.

cat < filex <filey <filez | grep a | wc -c >filep >filepp

- Parent: `pipe` [3, 4], `fork` child1, `close(4)`, `pipe` [4,5], `fork` child 2, `close(3)`, `close(5)`, `fork` child3, `close(4)`, `wait`.
- Child1: `close(3)`, redirect stdout to write end of pipe with `dup2(4,1)`, and `close(4)`. Call `newfstat` and `access` for cat. Then `open("filex", O_RDONLY)`=3, `dup2(3,0)`, `close(3)`. Then `open("filey", O_RDONLY)`=3, `dup2(3,0)`, `close(3)`. Then `open("filez", O_RDONLY)`=3, `dup2(3,0)`, `close(3)`. Then `execve` for cat.
- Child2: `close(4)`, redirect stdin to read end of pipe with `dup2(3,0)`, and `close(3)`. Then redirect stdout to write end of pipe with `dup2(5,1)`, and `close(5)`. Call `newfstat` and `access` for grep. Then `execve` for grep.
- Child3: Redirect stdin to read end of pipe with `dup2(4,0)`, and `close(4)`. Call `newfstat` and `access` for wc. Then `open("filep", O_WRONLY|O_CREAT|O_TRUNC, 0666)`=3, `dup2(3,1)`. Then `open("filepp", O_WRONLY|O_CREAT|O_TRUNC, 0666)`=3, `dup2(3,1)`. Then `execve` for wc.`exit(0)`

cat < filex <nonexistingfile <filez | grep a | wc -c >filep >filepp

- Parent: same as above ”well-working” example in green.
- Child1: starts same as above, but then when opening “nonexisitingfile”. gets -1 ENOENT, and exits (bash uses `exit_group` but I should use `exit`). So it doesn’t open other files, or call newfstatat, access, or execve. `write` “nonexistingfilename: No such file or directory”, and calls `exit(1)`. The same is the case for files to stdout. These are not created if they are after the non-exisiting file.
- Child2: same as above ”well-working” example in green.
- Child3. same as above ”well-working” example in green. So 2 and 3 same but because 1 never writes anything, final ouput by wc -c  is 0.

< filex >filep <filey <nonexisitingfile cat <filez >filepp | grep a | wc -c >fileppp >filepppp

- Same as usual….validity of cmd happens before redirections. Then redirections stop when nonexisitingfile is opened and get -1 ENOENT and child should clean and call `exit(1)`.

invalidcmd < filex <filey <filez | >filep >filepp invalidcmd hello | echo hello >fileppp

- Parent: same as above ”well-working” example in green.
- Child1: `close(3)`, redirect stdout to write end of pipe with `dup2(4,1)`, and `close(4)`. Then calls `newfstatat` for invalidcmd and gets -1 ENOENT. Yet, it still goes on to open every file and duplicate them onto stdin as in the above ”well-working” example in green. `write` “invalidcmdname: command not found” and calls `exit(127)`. Btw…order where invalidcmd is doesn’t matter. Even if it after or in-between file redirections, always first newfstat fails, and then files are opened.
- Child2: same as above ”well-working” example in green. But when it arrives at `newfstatat` gets -1 ENOENT, `write` invalid cmd name message and calls `exit(127)`.
- Child3: same as above ”well-working” example in green (but for for only one file and echo, of course!).

cat filex > filep | "" | echo hi ….i.e. empty command.

- Parent: same as above ”well-working” example in green.
- Children: same. Only child2 will call newfstatat and get -1 ENOENT and call exit(127). But rest of pipeline carries on normally. In a similar pipeline where there would be file redirections to stdout, they would be opened.

cat <filex >filep <filewopermissions >filep

- Same behavior as nonexisitingfile including exit with 1.

case where command executable has no permissions.

- Probably newfstatat will work, but then access won’t work and exit value should be 126.

cases with #:

- echo hi > filep | #echo ho | echo hi > filepp …This will write hi to filep but not do the rest of pipeline. The pipes are not opened at all.
- echo hi # > filep | echo ho | echo hi > filepp …This will write hi to stdout without creating filep.

*heredoc: (Always parent first reads heredoc contents and stores in memory (heap). Then access them in child if necessary).*

Same thing!
