*bla bla bla*

# Features
- Line editing: **DOGSHIT**
- Lexing: OK
- Parsing: OK
- Expansion (env vars): OK
- Execution: OK
- Builtins : OK
- Pipes and redirections: OK

- HEREDOCS : NONE
- Exit status : NONE
- History : NONE
- History : NONE
- <C-x>(x being any key): NONE

## Builtins
- `echo`: OK
- `exit`: OK
- `which`: OK
- `cd`: OK
- `export`: OK
- `env`: OK

- `pwd`: NONE (external)
- `unset`: NONE

# Issues
- Half the freeing doesn't work.

# TODO
- [x] Lexer, handle fix: quote handling
- [x] Expansions, fix: quote handling
- [ ] which command, fix: prints directory if directory is passed as param
- [ ] Heredocs, handle normal delimiter
- [ ] Heredocs, unquoted delim -> expand $VAR in body (e.g. << EOF)
- [ ] Heredocs, quoted delim -> do **not** $VAR in body (e.g. << "EOF")
- [ ] Heredocs, handle multiple (e.g. cat << A << B)
- [ ] Heredocs, handle pipeline (e.g. grep hello << A | grep lo)
