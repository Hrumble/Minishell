*bla bla bla*

# Features
- Line editing: OK
- Lexing: OK
- Parsing: OK
- Expansion (env vars): OK
- Execution: OK
- Builtins : OK
- Pipes and redirections: OK
- Exit status : OK
- HEREDOCS : OK
- History : OK

- <C-x>(x being any key): NONE

## Builtins
- `echo`: OK
- `exit`: OK
- `which`: OK
- `cd`: OK
- `export`: OK
- `env`: OK
- `unset`: OK

- `pwd`: NONE (external)

# Issues
- Half the freeing doesn't work.

# TODO
- [x] Lexer, handle fix: quote handling
- [x] Expansions, fix: quote handling
- [x] Redirections, fix: redirections must not happen on the fork, they must happen before executing and after parsing
- [x] History, save command history
- [x] History, cycle through command history
- [x] History, free history
- [x] Heredocs, handle normal delimiter
- [x] Heredocs, unquoted delim -> expand $VAR in body (e.g. << EOF)
- [x] Heredocs, quoted delim -> do **not** $VAR in body (e.g. << "EOF")
- [x] Heredocs, handle multiple (e.g. cat << A << B)
- [x] Heredocs, handle pipeline (e.g. grep hello << A | grep lo)
- [x] Heredocs, handle redirection (e.g. cat << A > file.txt | cat << B > file1.txt)
- [x] Heredocs, handle pipeline chained heredocs (e.g. cat << EOF | grep << EOF) in that case, grep disregards the output of cat, and takes << as stdin only

- [ ] which command, fix: prints directory if directory is passed as param
- [ ] Redirections, fix: pretty sure none of the file descriptors are closed after redir
- [ ] Exit builtin command doesn't free anything
- [ ] Lexer, fix: does not error on invalid characters, just returns (0) which stands for eof
- [ ] exit code on no exec perm is 126 not 127
