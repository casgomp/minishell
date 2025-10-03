strace (see man strace): trace system calls and signals. In the simplest case strace runs the specified command until it exits.  
It  intercepts  and  records the system calls which are called by a process and the signals which are received by a process.  
The name of each system call, its arguments  and its  return  value are printed on standard error or to the file specified with the
-o option. See example below:

pecastro@c1b2c3:~$ strace -f -e trace=access,execve,fork,pipe,pipe2,dup2 bash -c 'cat filex | "" | cat filey > filep'
execve("/usr/bin/bash", ["bash", "-c", "cat filex | \"\" | cat filey 
> fil"...], 0x7ffd6f09a058 /* 53 vars */) = 0
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or 
directory)
access("/usr/bin/bash", X_OK)           = 0
access("/usr/bin/bash", R_OK)           = 0
access("/usr/bin/bash", X_OK)           = 0
access("/usr/bin/bash", R_OK)           = 0
pipe2([3, 4], 0)                        = 0
strace: Process 15560 attached
[pid 15559] pipe2([4, 5], 0)            = 0
strace: Process 15561 attached
strace: Process 15562 attached
[pid 15560] access("/usr/bin/cat", X_OK) = 0
[pid 15560] access("/usr/bin/cat", R_OK) = 0
[pid 15560] access("/usr/bin/cat", X_OK) = 0
[pid 15562] access("/usr/bin/cat", X_OK) = 0
[pid 15560] access("/usr/bin/cat", R_OK) = 0
[pid 15562] access("/usr/bin/cat", R_OK) = 0
[pid 15560] execve("/usr/bin/cat", ["cat", "filex"], 0x55911a8c2340 /* 
53 vars */ <unfinished ...>
[pid 15562] access("/usr/bin/cat", X_OK) = 0
[pid 15562] access("/usr/bin/cat", R_OK) = 0
[pid 15560] <... execve resumed>)       = 0
bash: line 1: : command not found
[pid 15560] access("/etc/ld.so.preload", R_OK) = -1 ENOENT (No such file
 or directory)
[pid 15561] +++ exited with 127 +++
[pid 15562] execve("/usr/bin/cat", ["cat", "filey"], 0x55911a8c2340 /* 
53 vars */) = 0
[pid 15562] access("/etc/ld.so.preload", R_OK) = -1 ENOENT (No such file
 or directory)
[pid 15560] --- SIGPIPE {si_signo=SIGPIPE, si_code=SI_USER, 
si_pid=15560, si_uid=103223} ---
[pid 15560] +++ killed by SIGPIPE +++
[pid 15562] +++ exited with 0 +++
access("/etc/terminfo/x/xterm-256color", R_OK) = -1 ENOENT (No such file
 or directory)
access("/lib/terminfo/x/xterm-256color", R_OK) = 0
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=15561, 
si_uid=103223, si_status=127, si_utime=0, si_stime=0} ---
+++ exited with 0 +++

Can also combine with -ff -o, which builds on -f: not only follows children, 
but splits the trace output into separate files per process:

strace -ff -o traceout -e trace=access,execve,fork,pipe,pipe2,dup2 bash -c 'cat filex | grep h | cat filey > filep'

where traceout is a series of files with detailed description per process. Each traceout file (doesn’t have to 
be called traceout, can be any filename) is named traceout.pid, where pid is the actual number of the pid of each process.
