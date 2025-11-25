
strace -e trace=all -o strout -f ./minishell && less strout

strace -e trace=execve,access,openat,fstat,open,close,read,write,/dup.*, -o strout -f ./minishell && less strout

rm strout*; strace -e trace=/exec,access,/open,fstat,close,read,write,/dup,/pipe,/wait,/sigaction,/clone -o strout -ff ./minishell  && less strout*