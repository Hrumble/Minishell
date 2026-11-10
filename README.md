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

- `pwd`: NONE (external)
- `export`: NONE
- `unset`: NONE
- `env`: NONE

# Issues
- Half the freeing doesn't work.

# TODO
- [ ] Lexer, handle fix quote handling
- [ ] Heredocs, handle normal delimiter
- [ ] Heredocs, unquoted delim -> expand $VAR in body (e.g. << EOF)
- [ ] Heredocs, quoted delim -> do **not** $VAR in body (e.g. << "EOF")
- [ ] Heredocs, handle multiple (e.g. cat << A << B)
- [ ] Heredocs, handle pipeline (e.g. grep hello << A | grep lo)
