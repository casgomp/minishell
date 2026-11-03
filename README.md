*This project has been created as part of the 42 curriculum by rhaas, pecastro.*

# minishell

> As beautiful as a shell — a small, bash‑like command interpreter written in C.

---

## Description

**minishell** is our own little Bash. It reads a command line, splits it into
tokens while respecting quotes, expands environment variables, builds a list of
commands with their redirections, and then runs them, connecting them with pipes
and forking child processes where needed.

The goal of the project is to understand, from the inside, what a shell actually
does: **processes** (`fork`, `execve`, `waitpid`), **file descriptors** (`pipe`,
`dup2`, `open`), **signals** (`sigaction`) and **terminal handling**
(`readline`, `termios`). Whenever the subject leaves a behaviour open, we use
`bash` as the reference.

### Features at a glance

| Area | What is supported |
|---|---|
| **Prompt & history** | Interactive prompt `$USER@minishell$`, history via GNU readline (empty lines are not stored) |
| **Command lookup** | Search in `$PATH`, relative paths (`./a.out`), absolute paths (`/bin/ls`) |
| **Quotes** | `'single'` (nothing interpreted), `"double"` (only `$` interpreted) |
| **Expansion** | `$VAR`, `$?` (exit status of the last foreground pipeline), word splitting of unquoted expansions |
| **Redirections** | `<` input, `>` truncate, `>>` append, `<<` here‑doc (not added to history) |
| **FD‑numbered redirections** | `2>err.log`, `3<file`, … (fd up to 1023) |
| **Pipes** | Any number of commands: `cmd1 \| cmd2 \| … \| cmdN` |
| **Unfinished pipe** | A line ending in `\|` shows a `> ` continuation prompt |
| **Signals** | `ctrl‑C` new prompt, `ctrl‑D` exits, `ctrl‑\` does nothing (interactive mode) |
| **Built‑ins** | `echo [-n]`, `cd`, `pwd`, `export`, `unset`, `env`, `exit` |
| **Non‑interactive mode** | `./minishell script.sh` or `cmd \| ./minishell` |
| **Extras** | `#` comments, `;` command separator, bash‑style error messages and exit codes |

---

## Instructions

### Requirements

- Linux (developed and tested on Ubuntu / WSL2)
- `cc` (gcc or clang), `make`
- GNU readline development headers

```bash
# Debian / Ubuntu
sudo apt install build-essential libreadline-dev
```

### Build

```bash
make        # builds libft first, then ./minishell
make clean  # removes object files
make fclean # removes object files, libft.a and the binary
make re     # fclean + all
```

Compiled with `-Wall -Wextra -Werror`. Our `libft` (including our own quote‑aware
tokenizer, see *Architecture*) is compiled through its own Makefile.

### Run

```bash
./minishell                 # interactive mode
./minishell script.sh       # run a script file
echo 'echo hi | wc -c' | ./minishell   # read commands from stdin
```

### Quick demo

```bash
user@minishell$ export NAME=world
user@minishell$ echo "hello $NAME" '$NAME'
hello world $NAME
user@minishell$ ls nope
ls: cannot access 'nope': No such file or directory
user@minishell$ echo $?
2
user@minishell$ cat << EOF | wc -l
> one
> two
> EOF
2
user@minishell$ < infile grep a | sort > outfile
user@minishell$ notacommand
msh: notacommand: command not found
user@minishell$ echo $?
127
user@minishell$ exit 42
exit
```

---

## Architecture

### The big picture

```
                    ┌──────────────────────────────────────────────┐
  readline / gnl    │                 msh_compile_cmds             │
 ─────────────────► │  1. split line into sections (newline / ;)   │
   raw command line │  2. tokenize (quote‑aware, metachars kept)   │
                    │  3. per token: redirection? pipe? word?      │
                    │  4. expand $VAR / $?, word‑split, rm quotes  │
                    │  5. syntax check (empty cmd, dangling pipe)  │
                    └──────────────────────┬───────────────────────┘
                                           │ linked list of t_cmd
                                           ▼
                    ┌──────────────────────────────────────────────┐
                    │                   executor                   │
                    │  single built‑in? ──► run in parent process  │
                    │  otherwise for each cmd:                     │
                    │    heredocs → pipe() → fork()                │
                    │      child : wire pipes, apply redirections, │
                    │              find in PATH, execve            │
                    │      parent: close used fds, next cmd        │
                    │  waitpid all children → $?                   │
                    └──────────────────────────────────────────────┘
```

`main()` is intentionally tiny: initialise → loop `process_cmdline()` until
`exit` / EOF → clean up.

### Source map

| File | Responsibility |
|---|---|
| [src/minishell.c](src/minishell.c) | `main`, init (env copy, signals, tokenizer config), cleanup |
| [src/input.c](src/input.c) | Reading input (readline vs. get_next_line), history, script mode |
| [src/msh_compile_cmds.c](src/msh_compile_cmds.c) | **Parser** entry: line sections → tokens → `t_cmd` list, continuation prompt |
| [src/msh_compile_cmds2.c](src/msh_compile_cmds2.c) | Expansion + word splitting per token, metacharacters, syntax checks |
| [src/msh_compile_cmds_utils*.c](src/) | List helpers and cleanup for commands / arguments / redirections |
| [src/msh_ioredirect.c](src/msh_ioredirect.c) | Parsing `<`, `>`, `>>`, `<<`, `N>`; ambiguous redirect / syntax errors |
| [src/msh_sqed_functions.c](src/msh_sqed_functions.c) | Callbacks for expansion: variable lookup (`$?`, env) and valid name chars |
| [src/process.c](src/process.c) | **Executor** entry: context setup, built‑in vs. fork decision |
| [src/setup_parent.c](src/setup_parent.c) | Parent side: here‑docs, `pipe`, `fork`, closing used fds |
| [src/setup_children.c](src/setup_children.c), [src/setup_children2.c](src/setup_children2.c) | Child side: pipe wiring, opening redirection files, `execve` |
| [src/paths.c](src/paths.c), [src/permissions.c](src/permissions.c) | `$PATH` splitting, executable lookup, 126 / 127 decision |
| [src/wait.c](src/wait.c) | `waitpid` for all children, `$?`, error messages for failed lookups |
| [src/heredocs_setup.c](src/heredocs_setup.c), [src/heredocs_prompt.c](src/heredocs_prompt.c) | Here‑doc input through a pipe, ctrl‑C aware, `termios` tweak |
| [src/signals.c](src/signals.c), [src/signals_heredoc.c](src/signals_heredoc.c) | Signal handlers for prompt, here‑doc and while waiting for children |
| [src/builtins1.c](src/builtins1.c) | `echo`, `cd`, `pwd`, `exit` and the built‑in dispatcher |
| [src/builtins2.c](src/builtins2.c) | `env`, `export`, `unset` |
| [src/utils.c](src/utils.c), [src/utils2.c](src/utils2.c) | Environment copy, built‑in detection, error printing |
| [libft/src/rh_split_quotes*.c](libft/src/) | Generic quote‑aware tokenizer |
| [libft/src/rh_rmquotes_esc_expnd*.c](libft/src/) | Generic quote removal + `$` expansion engine |

### 1. Parsing

The lexer is a **generic, configurable** tokenizer living in our libft
(`rh_split_quotes`, `rh_rmqed`). The shell configures it once in `ms_init()`
through a `t_sqed` struct:

```c
pcd->default_sqed = (t_sqed){
    .q       = "\"'",        // quote characters
    .blank   = " \t",        // token separators that are dropped
    .metachr = "|<>\n;",     // separators that become their own tokens
    .lncmt   = "# \t|<>\n;", // where a '#' starts a comment
    .ifs     = " \t\n",      // word splitting after expansion
    .expnd   = '$',          // expansion character
    .dict    = &msh_dict,    // callback: name -> value ($? or getenv)
    ...
};
```

Each token then goes through three steps in `msh_expand_wordsplit_token()`:

1. **Expand** `$VAR` / `$?` (not inside single quotes) — quotes are still kept.
2. **Word‑split** the result on `IFS`, but *only* if an unquoted expansion
   actually happened (so `"$VAR"` stays one word, `$VAR` may become several).
3. **Remove quotes**.

Redirection operators consume the next token as their target. If that target
expands to zero or several words we report `ambiguous redirect`, like bash.

The result is a linked list of commands:

```c
typedef struct s_cmd {
    char          **args;     // argv for execve, NULL‑terminated
    t_io           *iolst;    // redirections, in command‑line order
    char            ctrlchr;  // what ended the command: '|', ';' or '\n'
    char           *path;     // resolved executable path
    int             stdio[3]; // saved fds (for built‑ins in the parent)
    struct s_cmd   *next;
    ...
} t_cmd;

typedef struct s_io {
    t_rd   tredirect;   // >, >>, <, <<
    char  *fname_delim; // file name or here‑doc delimiter
    int    new_fd;      // fd to replace (default 0 or 1, or N in "N>")
    int    old_fd;      // fd we opened
    struct s_io *next;
} t_io;
```

Syntax checks happen on this list: a `|` or `;` with no command before it gives
`syntax error near unexpected token`, and a trailing `|` triggers the `> `
continuation prompt instead of an error.

### 2. Execution

- **Single built‑in without a pipe** (e.g. `cd`, `export`, `exit`) runs **in the
  parent**, otherwise it could not change the shell's own state. Its
  redirections are applied to the real stdin/stdout, which are saved with `dup`
  beforehand and restored afterwards.
- **Everything else is forked.** For each command, `parent_setup()`:
  1. reads all its here‑docs into pipes (in the parent, so ctrl‑C can abort
     cleanly before anything runs),
  2. creates a `pipe()` if the command is followed by `|`,
  3. `fork()`s; the child connects the previous pipe's read end to stdin and
     the new pipe's write end to stdout, then applies its own redirections
     **in order** (so the last `>` wins, exactly like bash),
  4. the parent closes every fd it no longer needs, so readers see EOF and no
     descriptors leak.
- The child resolves the command: names containing `/` are used as they are,
  others are searched in each `$PATH` entry. Not found → `127`, found but not
  executable → `126`. The child exits **silently** with that code.
- After the loop the parent `waitpid()`s every child **in order**; the status of
  the **last** command becomes `$?`. For a 126 / 127 exit the **parent** prints
  the error (`command not found`, `No such file or directory`,
  `Permission denied`). Because only one process writes these messages, the
  errors from a pipeline like `nope1 | nope2` never come out mixed together. A child killed by a signal gives
  `128 + signal` (ctrl‑C → `130`, ctrl‑\ → `131` with `Quit (core dumped)`).

### 3. Signals

The subject allows **one** global variable, and it only holds the signal
number:

```c
volatile sig_atomic_t g_signal_received;
```

The handlers never touch the shell's data structures. The behaviour changes
with the context:

| Context | ctrl‑C (SIGINT) | ctrl‑\ (SIGQUIT) |
|---|---|---|
| Prompt | newline + fresh prompt (`rl_replace_line`, `rl_redisplay`); readline's own handlers are disabled with `rl_catch_signals = 0` | ignored |
| Here‑doc | aborts the here‑doc, `$? = 130`, command not run | ignored |
| Waiting for children | shell ignores it; the children get default handlers and die | child dumps core → `131` |
| Child process | reset to `SIG_DFL` right before `execve` | `SIG_DFL` |

`ctrl‑D` on an empty prompt makes `readline` return `NULL`, and the shell exits
with the last exit status.

### 4. Environment

At start‑up `environ` is copied into a heap‑allocated array with a few spare
slots (`ENV_XTRA_ELEMS`), so `export` can usually add variables without
re‑allocating. `export` checks names like bash (`not a valid identifier`),
`export` with no arguments prints `declare -x NAME="value"`, and `unset`
removes entries in place. Children receive this array through `execve`.

---

## Built‑ins

| Command | Behaviour |
|---|---|
| `echo [-n] [args]` | Accepts repeated `-n` (`echo -n -n hi`) |
| `cd [path]` | Relative or absolute path; no argument → `$HOME`; options are rejected |
| `pwd` | Uses `getcwd`; options are rejected |
| `export [NAME=value ...]` | Sets / updates variables; without arguments lists them |
| `unset [NAME ...]` | Removes variables |
| `env` | Prints the environment |
| `exit [n]` | Exits with `n % 256`; non‑numeric → `numeric argument required`, `2` |

---

## Exit status cheat sheet

| Situation | `$?` |
|---|---|
| Success | `0` |
| Generic failure (e.g. cannot open redirect file) | `1` |
| Syntax error / bad `exit` argument | `2` |
| Found but not executable | `126` |
| Command not found / no such file | `127` |
| Killed by signal N | `128 + N` (ctrl‑C → `130`, ctrl‑\ → `131`) |

---

## Testing

We test **against bash itself**. Every file in [tests/shell_tests/](tests/shell_tests/)
is a small shell script; the runner feeds it to both `./minishell` and
`bash --norc` and compares **stdout, stderr and the exit code**:

```bash
make
tests/shell_scripts/run_shell_tests.sh              # all ~85 test cases
tests/shell_scripts/run_shell_tests.sh 'tc_echo*'   # only matching cases
```

Shell names in error messages (`msh:` vs `bash:`) are normalised before the
diff. Cases whose name ends in `_failOK` document known, accepted differences.
Results are stored in `/tmp/mshtest_<date>/`.

Covered areas include echo / quoting edge cases, expansion and word splitting,
`export`/`unset`/`env`, redirections (incl. multiple and fd‑numbered ones),
here‑docs, pipelines with failing commands, unfinished pipes and syntax errors.

Memory checks are done with valgrind, suppressing readline's own leaks:

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes \
         --suppressions=div/valgrind.supp ./minishell
```

---

## Evaluation walkthrough

A suggested order for showing the project:

1. **Build**: `make` (no relink on a second `make`), norminette.
2. **Simple commands**: `/bin/ls`, `ls -la`, `./minishell` inside minishell, empty line, only spaces.
3. **Quotes & expansion**: `echo "$USER '$HOME'"`, `echo '$USER'`, `echo $?`, `echo "$NOPE"`.
4. **Redirections**: `> out`, `>> out`, `< in`, `cat << EOF`, several redirections in one command.
5. **Pipes**: `ls | grep src | wc -l`, `cat | cat | ls`, a pipe with a failing command.
6. **Built‑ins**: `cd` + `pwd`, `export`/`unset`/`env`, `exit 42` then `echo $?` in the parent shell.
7. **Signals**: ctrl‑C / ctrl‑D / ctrl‑\ at an empty prompt, with text typed, during `cat`, during a here‑doc.
8. **Errors**: `notacommand` (127), a non‑executable file (126), `| ls`, `ls >`.
9. **Leaks**: run the valgrind command above.

---

## Resources

### References

- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) — especially *Shell Operation*, *Quoting*, *Shell Expansions*, *Redirections*
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/readline.html)
- Linux man pages: `fork(2)`, `execve(2)`, `pipe(2)`, `dup2(2)`, `waitpid(2)`, `sigaction(2)`, `termios(3)`, `open(2)`
- [42 minishell wiki by jmartini89](https://github.com/jmartini89/42_minishell/wiki/)
- [minishell evaluation sheet](https://wormav.github.io/42_eval/Cursus/Minishell/index.html)

### How AI was used

An AI assistant was used for the following tasks. Everything it produced was
reviewed, tested against bash and discussed by the team before it went into the
repository.

- **Understanding bash:** explaining edge cases where the subject says "take bash
  as a reference": word splitting after `$VAR`, quoting rules, `$?` after a
  signal, and how here-docs and ctrl-C interact.
- **System calls:** clarifying the behaviour of `fork`, `pipe`, `dup2`,
  `waitpid`, `sigaction` and `termios`, and why unused pipe ends must be closed.
- **Debugging:** analysing file-descriptor leaks and memory leaks reported by
  valgrind, and failures found by our bash-comparison test suite.
- **Documentation:** first draft of this README, written from the existing code
  and checked against the final build.
- **Review:** explaining specific functions and suggesting small fixes, which
  were applied and tested by the team.

## Known limitations

- Only the mandatory part is implemented: no `&&`, `||`, parentheses or `*`
  wildcards (bonus).
- Redirections to another file descriptor (`2>&1`) are not supported; `&1` is
  treated as a file name.
- `export` without arguments does not escape `"` inside values, unlike bash.
- `;` and `#` comments are interpreted, although the subject does not require
  them.
