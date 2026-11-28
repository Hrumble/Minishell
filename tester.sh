#!/usr/bin/env bash
# ============================================================================
#  minishell_tester — a full tester for the 42 minishell project
#
#  Every test is fed to your minishell AND to bash (the reference) through
#  stdin, in an identical sandbox directory with an identical environment.
#  For each test it compares:
#     OUT    stdout (exact, after removing your prompt / readline echo)
#     ERR    stderr (both empty / both non-empty; exact text with --strict-err)
#     EXIT   the exit status of the shell
#     FILES  every file created / modified in the sandbox and in $HOME
#  Optional:  --leaks   valgrind (leaks, invalid accesses, unclosed fds)
#             --signals Ctrl-C / Ctrl-\ / Ctrl-D in a real pseudo-terminal
# ============================================================================

VERSION="1.0"

# ---------------------------------------------------------------- defaults --
MINISHELL=""
REF_SHELL="${REF_SHELL:-bash}"
TIMEOUT=5
CATS=""
RUN_MANDATORY=1
CORE=1
RUN_EXTRA=0
RUN_BONUS=0
RUN_LEAKS=0
RUN_SIGNALS=0
STRICT_ERR=0
CHECK_ERR=0
VERBOSE=0
STOP_ON_FAIL=0
ONLY_IDS=""
PROMPT_RE=""
EXTRA_FILE=""
LIST_ONLY=0
LOG_FILE="$PWD/minishell_tester.log"
COLOR=1
[ -t 1 ] || COLOR=0

usage() {
cat <<EOF
minishell_tester v$VERSION

Usage: $0 [options] [path/to/minishell]

Test selection
  By default only the ~200 "core" tests run: the things evaluators actually
  check at a defence (no obscure bash trivia).
  -F, --full           run the full mandatory suite (~600 tests, many edge cases)
  -a, --all            also extras + bonus (combine with -F for everything)
  -b, --bonus          also run bonus tests (&&, ||, (), wildcards)
  -e, --extra          also run "extra" tests (behaviour bash has but the
                       subject does not strictly require: ~, cd -, \${VAR}, ...)
  -B, --bonus-only     only bonus tests
  -c, --cat LIST       only these categories, all of their tests (e.g. echo,pipes)
  -t, --test LIST      only these test numbers (e.g. 4,17,30-42), implies -v
  -f, --file FILE      add your own tests from FILE (same format, see below)
  -l, --list           list categories and number of tests, then exit

Checks
  -L, --leaks          also run every test under valgrind (Linux, slow)
  -s, --signals        also run the interactive signal tests (needs python3)
  -S, --signals-only   only the signal tests
      --check-err      fail a test when bash printed an error and your minishell
                       didn't (or the opposite). By default error messages never
                       make a test fail: differences are only shown as a note.
      --strict-err     also fail when the error text differs (prefixes like
                       "bash: line 1:" or "minishell:" are stripped first)

Misc
  -p, --prompt REGEX   perl regex matching your prompt, if auto-detection
                       fails (e.g. a prompt that contains the cwd)
  -T, --timeout SEC    per-test timeout (default $TIMEOUT)
  -x, --stop           stop at the first failing test
  -v, --verbose        print diffs of failing tests in the terminal
      --no-color
  -h, --help

Without a path, ./minishell then ../minishell are tried.
Failures are written with full details to: minishell_tester.log

Test file format: one test per line, "@@" is a newline inside a test,
"### name" starts a category, other lines starting with "#" are comments.
  ### my_tests
  echo hello | cat -e
  export A=42@@echo \$A
EOF
}

# ------------------------------------------------------------- arguments ---
expand_ids() { # "1,3,5-8" -> " 1 3 5 6 7 8 "
	local out=" " part a b i
	local IFS=','
	for part in $1; do
		case "$part" in
			*-*) a=${part%-*}; b=${part#*-}; i=$a
			     while [ "$i" -le "$b" ]; do out="$out$i "; i=$((i+1)); done ;;
			*)   out="$out$part " ;;
		esac
	done
	echo "$out"
}

while [ $# -gt 0 ]; do
	case "$1" in
		-a|--all) RUN_EXTRA=1; RUN_BONUS=1 ;;
		-F|--full) CORE=0 ;;
		-b|--bonus) RUN_BONUS=1 ;;
		-B|--bonus-only) RUN_BONUS=1; RUN_MANDATORY=0 ;;
		-e|--extra) RUN_EXTRA=1 ;;
		-c|--cat) CATS=",$2,"; CORE=0; shift ;;
		-t|--test) ONLY_IDS=$(expand_ids "$2"); VERBOSE=1; CORE=0; RUN_EXTRA=1; RUN_BONUS=1; shift ;;
		-f|--file) EXTRA_FILE="$2"; shift ;;
		-l|--list) LIST_ONLY=1 ;;
		-L|--leaks) RUN_LEAKS=1 ;;
		-s|--signals) RUN_SIGNALS=1 ;;
		-S|--signals-only) RUN_SIGNALS=2 ;;
		--strict-err) STRICT_ERR=1; CHECK_ERR=1 ;;
		--check-err) CHECK_ERR=1 ;;
		-p|--prompt) PROMPT_RE="$2"; shift ;;
		-T|--timeout) TIMEOUT="$2"; shift ;;
		-x|--stop) STOP_ON_FAIL=1 ;;
		-v|--verbose) VERBOSE=1 ;;
		--no-color) COLOR=0 ;;
		-h|--help) usage; exit 0 ;;
		-*) echo "unknown option: $1" >&2; usage >&2; exit 2 ;;
		*) MINISHELL="$1" ;;
	esac
	shift
done

if [ "$COLOR" = 1 ]; then
	R=$'\e[31m'; G=$'\e[32m'; Y=$'\e[33m'; BL=$'\e[34m'; M=$'\e[35m'
	C=$'\e[36m'; D=$'\e[2m'; BD=$'\e[1m'; N=$'\e[0m'
else
	R=; G=; Y=; BL=; M=; C=; D=; BD=; N=
fi

die() { echo "${R}error:${N} $*" >&2; exit 2; }

command -v perl >/dev/null 2>&1 || die "perl is required"
command -v "$REF_SHELL" >/dev/null 2>&1 || die "reference shell '$REF_SHELL' not found"
REF_SHELL=$(command -v "$REF_SHELL")

# ======================================================================
#  TEST DATABASE
#  Categories starting with "extra_" are extras, "bonus_" are bonus.
# ======================================================================
tests_db() {
cat <<'TESTS_EOF'
### echo
echo
echo hello
echo hello world
echo    hello      world
echo -n hello
echo -n
echo -n -n hello
echo -nnnnnnn hello
echo -n -nnn -n hello
echo -nx hello
echo -n-n hello
echo -nnnnx hello
echo hello -n
echo hello -n world
echo - hello
echo -- hello
echo "-n" hello
echo "-n hello"
echo '-n' hello
echo "-nnn" "-n" hello
echo -n "" hello
echo ""
echo "" ""
echo '' a ''
echo a"b"c
echo "a" "b"
echo "    spaces    inside    "
echo -n -n -n
echo $
echo $ $
echo hello$
echo "$"
echo '$'
echo "hello"world'again'
/bin/echo hello world
/bin/echo -n hello
echo hello | cat -e
echo -n hello | cat -e
echo "" | cat -e
echo a b c d e f g h i j k l m n o p q r s t u v w x y z
echo '   '
echo "tab	inside"

### quotes
echo "single 'inside' double"
echo 'double "inside" single'
echo "'$USER'"
echo '"$USER"'
echo "$USER"
echo '$USER'
echo "'"'"'
echo '"'"'"
"echo" hello
'echo' hello
"ec"'ho' hello
e"ch"o hello
ec''ho hello
"echo hello"
"echo" "hello world"
echo "|"
echo '>' out
echo "<" ">" ">>" "<<"
echo "a | b"
echo 'a > b'@@ls
echo "a < b"
echo "ls | cat"
"ls" "-a"
"ls"" -a"
echo "$USER$USER"
echo "$USER"'$USER'"$USER"
echo "  $USER  "
echo $USER'$USER'$USER
echo "'"$USER"'"
echo '"'$USER'"'
echo "''"
echo ''""''
echo """"""
echo ''''''
echo "hello"'world'"!"
echo 'it''s'
echo "a""b"'c''d'
echo " ' " ' " '
echo "$ USER"
echo "$"USER
echo '$'USER
echo "    "
echo "x" | "cat" -e
"/bin/echo" "quoted path"

### expansion
echo $USER
echo $HOME
echo $PATH
echo $NONEXISTENT
echo $NONEXISTENT hello
echo hello $NONEXISTENT world
echo "$NONEXISTENT"
echo "$NONEXISTENT" | cat -e
echo $NONEXISTENT | cat -e
echo $USER$HOME
echo $USER-$HOME
echo $USER.txt
echo "$USER.txt"
echo $USER_
echo $USERx
echo $USER$NONEXISTENT$USER
echo $USER/sub
echo $HOME/folder
echo "$HOME"'$HOME'
echo hello$USER
export A=hello@@echo $A
export A="  spaced   out  "@@echo $A
export A="  spaced   out  "@@echo "$A"
export A="  spaced   out  "@@echo [$A]
export A="  spaced   out  "@@echo "[$A]"
export A="a   b"@@echo x$A"y"
export A="ls -a"@@$A
export A="echo hi"@@$A
export A="echo hi"@@"$A"
export A='"quoted"'@@echo $A
export A="'single'"@@echo $A
export A='$USER'@@echo $A
export A='$USER'@@echo "$A"
export A=">"@@echo hi $A out
export A="|"@@echo hi $A cat
export A="<"@@echo hi $A infile
export A="-n"@@echo $A hello
export A="-n hello"@@echo $A
export A=" "@@echo a${A}b
export A=e B=cho@@$A$B hello
export A=ec@@"$A"ho hello
export A="  "@@echo [$A]
export A=@@echo [$A]
export A=""@@$A
$NONEXISTENT
$NONEXISTENT echo hi
"$NONEXISTENT"
echo $USER"$USER"'$USER'
export A=1@@export B=$A@@echo $B
export A=hello@@export A="$A world"@@echo $A
export A=x@@echo $A$A$A
export A=x@@echo "$A"$A'$A'

### exit_status
echo $?
echo $?$?
echo "$?"
echo '$?'
echo $?abc
echo abc$?
echo $? $? $?
true@@echo $?
false@@echo $?
false@@echo $?@@echo $?
ls nonexistent_file@@echo $?
/bin/ls nonexistent_file
nonexistentcmd@@echo $?
nonexistentcmd
./nonexistent@@echo $?
/nonexistent/path/cmd@@echo $?
./script.sh@@echo $?
./script.sh hello
./noexec.sh@@echo $?
./noexec.sh
./dir@@echo $?
./dir
dir@@echo $?
./noperm@@echo $?
./
/
..
false | true@@echo $?
true | false@@echo $?
nonexistentcmd | true@@echo $?
true | nonexistentcmd@@echo $?
$?
echo $?@@echo $?
expr $? + 1
false@@expr $? + $?
ls nonexistent_file@@echo "status: $?"
cat nonexistent_file@@echo $?
grep zzz infile@@echo $?
grep line infile@@echo $?

### pwd
pwd
pwd extra args
pwd | cat -e
pwd@@pwd
cd dir@@pwd
cd ..@@pwd
cd /@@pwd
cd /tmp@@pwd
cd dir/sub@@pwd
cd dir/sub@@cd ../..@@pwd

### cd
cd@@pwd
cd dir@@echo $?
cd nonexistent@@echo $?
cd nonexistent
cd infile@@echo $?
cd infile
cd dir extra
cd dir extra@@pwd
cd .@@pwd
cd ./dir/../dir@@pwd
cd /tmp/..@@pwd
cd dir@@cd ..@@pwd
cd dir@@echo $PWD
cd dir@@echo $OLDPWD
cd dir@@cd sub@@echo $PWD $OLDPWD
cd ../../../../../../../../..@@pwd
cd /@@cd ..@@pwd
cd ""@@pwd
cd "dir"@@pwd
cd 'dir/sub'@@pwd
unset HOME@@cd
unset HOME@@cd@@echo $?
export HOME=/@@cd@@pwd
export HOME=/tmp@@cd@@pwd@@echo $PWD
cd dir@@ls
cd dir | pwd
cd dir@@echo hi > from_cd@@cd ..@@ls dir
cd $HOME@@pwd
cd "$NONEXISTENT"@@pwd
cd noperm_dir@@echo $?
mkdir -p a/b/c@@cd a/b/c@@pwd@@cd ../../..@@pwd
cd /tmp@@cd /usr@@echo $OLDPWD@@pwd

### env
env | grep ^USER=
env | grep ^HOME=
env | grep ^PATH=
export NEWVAR=42@@env | grep NEWVAR
export NEWVAR@@env | grep NEWVAR
export NEWVAR@@env | grep NEWVAR@@echo $?
export NEWVAR=@@env | grep NEWVAR
export NEWVAR=""@@env | grep NEWVAR | cat -e
unset USER@@env | grep ^USER=
export A=1 B=2@@env | grep "^[AB]="
export A=1@@export A=2@@env | grep "^A="
env | grep -c "^HOME="
export ZZ="a b  c"@@env | grep ZZ
export ZZ=1@@unset ZZ@@env | grep ZZ

### export
export A=1@@echo $A
export A=1 B=2 C=3@@echo $A$B$C
export A=1@@export A=2@@echo $A
export A=@@echo "[$A]"
export A@@echo "[$A]"
export A=a=b=c@@echo $A
export A==@@echo $A
export A="hello world"@@echo $A
export "A=hello world"@@echo $A
export A='$USER'@@echo $A
export A="$USER"@@echo $A
export A=1@@export A@@echo $A
export 1A=x@@echo $?
export 1A=x
export =x@@echo $?
export =
export A-B=x@@echo $?
export A.B=x
export "A B"=x
export A B=2 9C=3@@echo $? $B
export _A=1@@echo $_A
export A1=1@@echo $A1
export _=x@@echo ok
export A_B_C=abc@@echo $A_B_C
export abc=lower@@echo $abc
export A=x@@/bin/sh -c 'echo $A'
export A=1@@unset A@@/bin/sh -c 'echo "[$A]"'
export PATH=/nonexistent:$PATH@@ls dir
export A="echo hi"@@$A
export A=hello@@export B=$A@@echo $B
export A=$NONEXISTENT@@echo "[$A]"
export A="a"@@export A=$A$A$A@@echo $A
export "A"=quoted@@echo $A
export A"B"=x@@echo $AB
export A=1 A=2@@echo $A
export ''@@echo $?
export ""
export A=1 1B=2 C=3@@echo $A $C $?

### unset
export A=1@@unset A@@echo "[$A]"
unset NONEXISTENT@@echo $?
unset
unset PATH@@ls
unset PATH@@ls@@echo $?
unset PATH@@/bin/ls
export A=1 B=2@@unset A B@@echo "[$A$B]"
unset HOME@@echo "[$HOME]"
unset USER@@echo "[$USER]"
export A=1@@unset A@@unset A@@echo $?
export A=1@@unset A@@export A=2@@echo $A
unset PWD@@echo "[$PWD]"
export A=1@@unset A=1@@echo $A
unset PATH@@export PATH=/bin:/usr/bin@@ls dir

### exit
exit
exit 0
exit 1
exit 42
exit 255
exit 256
exit 257
exit 1000
exit -1
exit -255
exit +5
exit 00042
exit abc
exit 42abc
exit abc42
exit 1 2
exit 1 2@@echo still alive $?
exit abc 2
exit 1 abc
exit 9223372036854775807
exit 9223372036854775808
exit -9223372036854775808
exit -9223372036854775809
exit 99999999999999999999999
exit ""
exit "42"
exit '42'
exit " 42"
false@@exit
ls nonexistent_file@@exit
nonexistentcmd@@exit
exit 42@@echo should not print
exit 5 | echo hi@@echo $?
echo hi | exit 3@@echo $?
exit | exit | exit 7@@echo $?
export CODE=12@@exit $CODE
false@@exit $?
exit --
exit -
exit 0 0
exit "1 2"

### pipes
echo hello | cat
echo hello | cat | cat | cat | cat
ls | wc -l
ls | sort -r
cat infile | grep two
cat infile | grep nothing@@echo $?
echo hi | grep hi | wc -c
cat infile | head -2 | tail -1
echo a | echo b
ls | cat -e
nonexistentcmd | echo hi
echo hi | nonexistentcmd
echo hi | nonexistentcmd | cat
nonexistentcmd | nonexistentcmd2
ls nonexistent_file | cat@@echo $?
cat /dev/urandom | head -c 10 | wc -c
yes | head -3
yes no | head -n 5 | tail -n 1
cat | cat | ls
export A=1 | echo "[$A]"
cd dir | pwd
echo hi | cat < infile
echo hi > out | cat@@cat out
cat infile | cat > out@@cat out
echo a|cat
echo a |cat| cat
echo a| cat |cat
seq 1 1000 | sort -n | tail -1
cat infile | wc -l | cat -e
ls | grep in | sort
echo "a b c" | tr ' ' '\n' | sort -r
cat infile | grep line | grep two | cat -e
echo hi | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat
exit 3 | echo after@@echo $?
echo hi | ./noexec.sh@@echo $?
echo hi | ./dir@@echo $?
ls | exit
cat infile | ./script.sh
env | grep -c ^HOME= | cat
echo $USER | cat -e
echo "$USER | cat" | cat -e
echo '|' | cat
sleep 0.2 | echo fast
echo slow | sleep 0.2
seq 1 100000 | head -1
cat empty | cat | wc -c

### redirections
echo hello > out@@cat out
echo hello > out@@echo world >> out@@cat out
echo hello >> out@@cat out
echo hello > out@@echo bye > out@@cat out
cat < infile
< infile cat
< infile cat > out@@cat out
> out echo hi@@cat out
echo hi > out1 > out2@@cat out1@@cat out2
echo hi > out1 >> out2 > out3
echo hi>out
echo hi>>out@@echo hi>>out
cat<infile
cat <infile >out
cat < infile < infile2
cat < nonexistent
cat < nonexistent@@echo $?
cat < nonexistent > out
cat > out < nonexistent
cat > out < nonexistent@@echo $?
echo hi > dir
echo hi > dir@@echo $?
cat < dir
echo hi > "file with spaces"@@cat "file with spaces"
cat < "file with spaces"
echo hi > "out"put
echo hi > 'out'"put"
echo hi > $NONEXISTENT
echo hi > $NONEXISTENT@@echo $?
export F=out@@echo hi > $F@@cat out
export F="a b"@@echo hi > $F
export F="a b"@@echo hi > "$F"@@cat "a b"
echo hi > "$NONEXISTENT"
> out
>> out
< infile
< nonexistent
< nonexistent@@echo $?
echo hi | > out
ls > out | cat
cat infile > out@@cat out > out2@@cat out2
echo a > out@@cat < out > out
nonexistentcmd > out
nonexistentcmd > out@@echo $?
echo hi 1 > out@@cat out
wc -l < infile
grep two < infile > out@@cat out
ls > out > out2@@cat out
echo hello > out world@@cat out
> out echo a b c@@cat out
echo a > out1 > out2 > out3 > out4@@ls
echo a > out1@@echo b >> out1@@echo c >> out1@@cat out1
cat < infile > out1 < infile2 > out2
echo hi > nonexist_dir/out
echo hi > nonexist_dir/out@@echo $?
cat < noperm@@echo $?
echo hi > noperm@@echo $?
echo hi > dir/inner@@cat dir/inner
cat < infile | cat > out@@cat out
cat < infile | grep two > out | echo pipe@@cat out
echo $USER > out@@cat out
echo "$USER" > out@@cat out
echo '$USER' > out@@cat out
echo hi > out@@< out cat@@< out cat
cat infile > /dev/null
cat infile >> /dev/null@@echo $?
ls >out@@cat out
echo hi >out1>out2@@ls
cat<infile>out@@cat out
echo a>out|cat
cat < /dev/null
echo -n hi > out@@cat out | cat -e

### heredoc
cat << EOF@@hello@@EOF
cat << EOF@@hello@@world@@EOF
cat << EOF@@$USER@@EOF
cat << 'EOF'@@$USER@@EOF
cat << "EOF"@@$USER@@EOF
cat << E'O'F@@$USER@@EOF
cat << EOF@@"$USER"@@EOF
cat << EOF@@'$USER'@@EOF
cat << EOF@@$USER$HOME@@EOF
cat << EOF@@  $USER  @@EOF
cat << EOF@@$NONEXISTENT@@EOF
cat << EOF@@$@@EOF
cat << EOF@@$?@@EOF
false@@cat << EOF@@$?@@EOF
cat << EOF | wc -l@@a@@b@@c@@EOF
cat << EOF > out@@hi@@EOF@@cat out
cat << EOF >> out@@a@@EOF@@cat << EOF >> out@@b@@EOF@@cat out
cat << A << B@@a@@A@@b@@B
cat << A << B << C@@a@@A@@b@@B@@c@@C
<< EOF cat@@x@@EOF
cat << EOF@@@@EOF
cat << EOF@@  EOF@@EOF
cat << EOF@@EOF @@EOF
cat << EOF@@EOFX@@EOF
cat << EOF@@XEOF@@EOF
<< EOF@@x@@EOF
<< EOF@@x@@EOF@@echo $?
cat << EOF | cat << EOF2@@a@@EOF@@b@@EOF2
cat << EOF@@hello
cat < infile << EOF@@x@@EOF
cat << EOF < infile@@x@@EOF
grep a << EOF@@abc@@def@@xyz a@@EOF
cat << EOF | grep b | cat -e@@abc@@bcd@@cde@@EOF
cat << eof@@EOF@@eof
cat << "E O F"@@x@@E O F
cat << EOF@@'single'"double"@@EOF
cat << EOF@@|@@>@@<@@EOF
cat << EOF@@$USER'$USER'"$USER"@@EOF
cat <<EOF@@nospace@@EOF
cat<<EOF@@nospace@@EOF
cat << EOF > out | cat@@x@@EOF@@cat out
nonexistentcmd << EOF@@x@@EOF
nonexistentcmd << EOF@@x@@EOF@@echo $?
cat << EOF@@echo not executed@@EOF
export A=val@@cat << EOF@@$A@@EOF
export A=val@@cat << 'EOF'@@$A@@EOF
cat << EOF@@$USER$@@EOF
cat << EOF@@a@@b@@c@@d@@e@@f@@g@@EOF
cat << EOF | cat << EOF | cat@@first@@EOF@@second@@EOF
cat << EOF > out1 > out2@@x@@EOF@@cat out2
ls << EOF@@x@@EOF
echo hi << EOF@@x@@EOF

### syntax_errors
|
| ls
ls |
ls | | cat
ls ||| cat
| | |
>
<
>>
<<
> >
< <
>> >>
<< <<
>>>
<<<
> |
< |
echo hi >
echo hi <
echo hi >>
echo hi > > out
echo hi >> >> out
echo hi < < infile
echo hi | > out | |
echo hi |
echo hi | |
echo > | cat
cat < | cat
cat << |
cat << >
cat << <
echo hi |   |
>> | <<
echo a > out | > | cat

### commands
/bin/ls
/bin/ls -a
/bin/ls dir
/bin/ls -a dir
ls -a
ls dir
ls dir nonexistent
ls -a dir infile
"/bin/ls"
/bin/ls/
/bin/ls/nonexist
script.sh
/bin/cat infile
cat infile infile2
head -n 1 infile
tail -n 1 infile
touch newfile@@ls
mkdir newdir@@cd newdir@@pwd
mkdir newdir@@ls
rm infile@@ls
touch "a b"@@ls
touch a@@touch b@@rm a@@ls
expr 1 + 1
expr 6 \* 7
whoami
uname
printf "%s-%s\n" a b
printf hello
env -i /bin/echo hi
/usr/bin/env echo hi
""
''
" "
"   "
""""
nonexistentcmd arg1 arg2
./script.sh a b c
./script.sh "a b"
/bin/sh -c "exit 42"@@echo $?
/bin/sh -c "exit 0"@@echo $?
sleep 0.1
/bin/false@@echo $?
/bin/true@@echo $?
ls -z
ls -z@@echo $?
grep
grep@@echo $?
cat infile | ./script.sh arg

### whitespace
@@
@@@@


echo hi@@@@echo there
   echo    spaces
	echo	tabs
  	 echo mixed 	 whitespace
echo hi   |   cat   -e
   ls   >   out   @@cat out
echo a@@   @@echo b
@@echo after empty

### multi_commands
echo one@@echo two@@echo three
export A=1@@export B=2@@echo $A $B@@unset A@@echo $A $B
cd dir@@pwd@@cd ..@@pwd@@cd dir/sub@@pwd
echo a > out@@cat out@@echo b >> out@@cat out@@rm out@@ls
false@@echo $?@@true@@echo $?@@nonexistentcmd@@echo $?
ls nonexistent_file@@echo $?@@echo $?
export A=hi@@echo $A | cat -e@@unset A@@echo $A | cat -e
mkdir t@@cd t@@touch f@@ls@@cd ..@@ls t
cat << EOF@@a@@EOF@@cat << EOF@@b@@EOF
export X=1@@export X=$X$X@@export X=$X$X@@echo $X

TESTS_EOF

# ------------------------------------------------------- generated tests ---
echo "### stress"
printf 'echo stress'; i=0; while [ $i -lt 60 ]; do printf ' | cat'; i=$((i+1)); done; echo
printf 'echo'; i=0; while [ $i -lt 600 ]; do printf ' arg%d' $i; i=$((i+1)); done; echo
printf 'export LONG='; i=0; while [ $i -lt 300 ]; do printf 'abcdefghij'; i=$((i+1)); done; printf '@@echo $LONG | wc -c\n'
i=0; while [ $i -lt 100 ]; do printf 'echo line%d@@' $i; i=$((i+1)); done; echo 'echo end'
i=0; while [ $i -lt 50 ]; do printf 'export V%d=%d@@' $i $i; i=$((i+1)); done; echo 'echo $V0 $V25 $V49'
printf 'cat << EOF'; i=0; while [ $i -lt 200 ]; do printf '@@heredoc line %d $USER' $i; i=$((i+1)); done; echo '@@EOF'
printf 'echo hi'; i=0; while [ $i -lt 30 ]; do printf ' > out%d' $i; i=$((i+1)); done; echo '@@ls'
i=0; while [ $i -lt 30 ]; do printf 'cat << EOF@@x@@EOF@@' ; i=$((i+1)); done; echo 'echo done'
i=0; while [ $i -lt 200 ]; do printf 'nonexistentcmd@@' ; i=$((i+1)); done; echo 'echo $?'
i=0; while [ $i -lt 100 ]; do printf 'ls | cat | cat > /dev/null@@' ; i=$((i+1)); done; echo 'echo $?'

cat <<'TESTS_EOF'

### extra_tilde
echo ~
echo ~/folder
echo "~"
echo '~'
echo ~~
echo a~
cd ~@@pwd
cd ~/@@pwd
export HOME=/tmp@@echo ~

### extra_cd_minus
cd dir@@cd -
cd dir@@cd -@@pwd
cd -
cd dir@@cd sub@@cd -@@cd -@@pwd

### extra_special_vars
echo ${USER}
echo "${USER}x"
echo ${NONEXISTENT}hi
echo $1
echo $1abc
echo "$1abc"
echo $9
echo $-USER
echo $"USER"
echo $/ $= $+ $, $:
echo "$/ $= $+"
echo $SHLVL
echo $"HOME"

### extra_export_print
export ZZTEST=val@@export | grep ZZTEST
export ZZTEST@@export | grep ZZTEST
export ZZTEST=""@@export | grep ZZTEST
export ZZTEST='a"b'@@export | grep ZZTEST
export ZZTEST=1@@export ZZTEST+=2@@echo $ZZTEST
export ZZTEST+=x@@echo $ZZTEST

### extra_misc
echo "unclosed
echo 'unclosed
exit "  42  "
unset ""
unset "1A"@@echo $?
cat << $USER@@x@@$USER
ls nonexistent_file 2> err@@cat err
ls nonexistent_file 2>/dev/null@@echo $?
echo hi 1> out@@cat out
cat <> infile
echo $((1+1))
env | grep -c ^_=
cd dir@@unset PWD@@cd ..@@pwd

### bonus_and_or
true && echo yes
false && echo yes
false || echo yes
true || echo yes
true && false || echo x
false || true && echo x
false && echo a || echo b && echo c
echo a && echo b || echo c
true && true && true && echo all
false || false || false || echo none
nonexistentcmd && echo yes@@echo $?
nonexistentcmd || echo fallback
ls nonexistent_file || echo $?
echo a > out && cat out
false && echo a > out@@ls
echo a | grep b || echo not found
echo a | grep a && echo found
true || false && echo x
false && true || echo y
cat << EOF && echo after@@doc@@EOF
true&&echo nospace
false||echo nospace

### bonus_subshell
(echo a)
(echo a && echo b) | cat
(echo a; ) 2>/dev/null || true
(cd dir)@@pwd
(cd dir && pwd)@@pwd
(exit 3)@@echo $?
(exit 3) || echo $?
(export A=1)@@echo "[$A]"
(echo a) && (echo b)
(false || true) && echo ok
false && (echo a)
echo a | (cat && echo b)
((echo nested))
(echo a) > out@@cat out
(echo a > out)@@cat out
(ls nonexistent_file)@@echo $?
( echo spaced )
(echo a || echo b) && (echo c && echo d)
(true && false) || (echo recovered)

### bonus_wildcards
echo *
echo *.sh
echo "*"
echo '*'
echo '*'.sh
ls *
ls *.sh
echo in*
echo *file*
echo *2
echo *nomatch*
echo nomatch*
cat *2
echo */
echo i*e
echo *e
echo *i*e*
export A="*"@@echo $A
export A="*"@@echo "$A"
mkdir w@@cd w@@echo *
echo * | wc -w
echo .*
echo "in"*

### bonus_syntax_errors
&&
||
&& echo hi
|| echo hi
echo hi &&
echo hi ||
echo hi && && echo b
echo hi || || echo b
echo hi &&& echo b
(
)
()
( echo hi
echo hi )
(echo hi))
((echo hi)
echo hi (echo b)
(echo a) echo b
| && |
TESTS_EOF
}

# ----------------------------------------------- core tests (default) ---
# Exact copies of test lines from above. The "stress" category is always
# included in core mode.
core_db() {
cat <<'CORE_EOF'
echo
echo hello
echo hello world
echo -n hello
echo -n
echo -n -n hello
echo -nnnnnnn hello
echo hello -n
echo ""
echo "" ""
echo a"b"c
echo "    spaces    inside    "
/bin/echo hello world
echo hello | cat -e
echo -n hello | cat -e
echo "single 'inside' double"
echo 'double "inside" single'
echo "'$USER'"
echo '"$USER"'
echo "$USER"
echo '$USER'
"echo" hello
'echo' hello
e"ch"o hello
"echo hello"
echo "|"
echo '>' out
echo "a | b"
echo "$USER"'$USER'"$USER"
echo "hello"'world'"!"
echo $USER
echo $HOME
echo $NONEXISTENT
echo $NONEXISTENT hello
echo "$NONEXISTENT"
echo $USER$HOME
echo $USER.txt
echo "$USER.txt"
echo $USER_
echo hello$USER
export A=hello@@echo $A
export A='$USER'@@echo $A
export A=">"@@echo hi $A out
export A="|"@@echo hi $A cat
$NONEXISTENT
"$NONEXISTENT"
export A=hello@@export A="$A world"@@echo $A
echo $?
echo "$?"
echo '$?'
true@@echo $?
false@@echo $?
ls nonexistent_file@@echo $?
nonexistentcmd@@echo $?
nonexistentcmd
./nonexistent@@echo $?
./script.sh@@echo $?
./noexec.sh@@echo $?
./dir@@echo $?
false | true@@echo $?
true | false@@echo $?
expr $? + 1
false@@expr $? + $?
pwd
cd dir@@pwd
cd ..@@pwd
cd /@@pwd
cd@@pwd
cd nonexistent@@echo $?
cd infile@@echo $?
cd .@@pwd
cd dir@@cd ..@@pwd
cd dir@@echo $PWD
cd dir@@echo $OLDPWD
unset HOME@@cd@@echo $?
cd dir | pwd
env | grep ^USER=
export NEWVAR=42@@env | grep NEWVAR
export NEWVAR@@env | grep NEWVAR
unset USER@@env | grep ^USER=
export A=1@@echo $A
export A=1 B=2 C=3@@echo $A$B$C
export A=1@@export A=2@@echo $A
export A=@@echo "[$A]"
export A=a=b=c@@echo $A
export A="hello world"@@echo $A
export 1A=x@@echo $?
export =x@@echo $?
export A-B=x@@echo $?
export _A=1@@echo $_A
export A=x@@/bin/sh -c 'echo $A'
export A=1@@unset A@@echo "[$A]"
unset NONEXISTENT@@echo $?
unset PATH@@ls@@echo $?
unset PATH@@/bin/ls
export A=1 B=2@@unset A B@@echo "[$A$B]"
exit
exit 0
exit 42
exit 255
exit 256
exit -1
exit abc
exit 42abc
exit 1 2@@echo still alive $?
false@@exit
exit 42@@echo should not print
echo hi | exit 3@@echo $?
echo hello | cat
echo hello | cat | cat | cat | cat
ls | wc -l
cat infile | grep two
cat infile | head -2 | tail -1
echo a | echo b
nonexistentcmd | echo hi
echo hi | nonexistentcmd
cat | cat | ls
yes | head -3
cat /dev/urandom | head -c 10 | wc -c
export A=1 | echo "[$A]"
echo hi > out | cat@@cat out
cat infile | cat > out@@cat out
echo a|cat
ls | grep in | sort
echo hello > out@@cat out
echo hello > out@@echo world >> out@@cat out
echo hello >> out@@cat out
echo hello > out@@echo bye > out@@cat out
cat < infile
< infile cat
< infile cat > out@@cat out
echo hi > out1 > out2@@cat out1@@cat out2
cat < infile < infile2
cat < nonexistent@@echo $?
cat < nonexistent > out
cat > out < nonexistent
echo hi > dir@@echo $?
echo hi>out
cat<infile
echo hi > "file with spaces"@@cat "file with spaces"
export F=out@@echo hi > $F@@cat out
> out
wc -l < infile
grep two < infile > out@@cat out
echo hello > out world@@cat out
nonexistentcmd > out@@echo $?
cat < infile | grep two > out | echo pipe@@cat out
echo hi > nonexist_dir/out@@echo $?
cat << EOF@@hello@@EOF
cat << EOF@@hello@@world@@EOF
cat << EOF@@$USER@@EOF
cat << 'EOF'@@$USER@@EOF
cat << "EOF"@@$USER@@EOF
cat << EOF | wc -l@@a@@b@@c@@EOF
cat << EOF > out@@hi@@EOF@@cat out
cat << A << B@@a@@A@@b@@B
<< EOF cat@@x@@EOF
cat << EOF@@  EOF@@EOF
cat << EOF@@$NONEXISTENT@@EOF
cat << EOF | cat << EOF2@@a@@EOF@@b@@EOF2
grep a << EOF@@abc@@def@@xyz a@@EOF
cat << EOF@@'single'"double"@@EOF
|
ls |
ls | | cat
>
<
>>
<<
echo hi >
echo hi <
echo hi > > out
cat < | cat
/bin/ls
ls -a
ls dir
/bin/cat infile
script.sh
./script.sh hello
touch newfile@@ls
mkdir newdir@@cd newdir@@pwd
""
/bin/sh -c "exit 42"@@echo $?
ls -z@@echo $?
@@
echo hi@@@@echo there
echo hi   |   cat   -e
echo one@@echo two@@echo three
export A=1@@export B=2@@echo $A $B@@unset A@@echo $A $B
cd dir@@pwd@@cd ..@@pwd@@cd dir/sub@@pwd
false@@echo $?@@true@@echo $?@@nonexistentcmd@@echo $?
CORE_EOF
}

# ======================================================================
#  helpers
# ======================================================================
cat_tier() {
	case "$1" in
		bonus_*) echo bonus ;;
		extra_*) echo extra ;;
		*) echo mandatory ;;
	esac
}

cat_enabled() {
	if [ -n "$CATS" ]; then
		case "$CATS" in *",$1,"*) return 0 ;; esac
		return 1
	fi
	case "$(cat_tier "$1")" in
		mandatory) [ "$RUN_MANDATORY" = 1 ] ;;
		extra) [ "$RUN_EXTRA" = 1 ] ;;
		bonus) [ "$RUN_BONUS" = 1 ] ;;
	esac
}

all_tests() {
	tests_db
	if [ -n "$EXTRA_FILE" ]; then
		echo "### custom"
		cat "$EXTRA_FILE"
	fi
}

if [ "$LIST_ONLY" = 1 ]; then
	echo "core mode (default): $(core_db | wc -l | tr -d ' ') core tests + the stress category"
	echo "full suite (-F):"
	all_tests | awk '
		/^### / { cat=$2; order[++n]=cat; next }
		/^#/ || /^$/ { next }
		{ cnt[cat]++ }
		END { for (i=1;i<=n;i++) printf "  %-26s %4d tests\n", order[i], cnt[order[i]] }'
	exit 0
fi

# ------------------------------------------------------- find minishell ---
if [ -z "$MINISHELL" ]; then
	for p in ./minishell ../minishell; do
		[ -x "$p" ] && MINISHELL="$p" && break
	done
fi
[ -n "$MINISHELL" ] || die "minishell not found: pass its path (e.g. $0 ../minishell)"
[ -x "$MINISHELL" ] || die "'$MINISHELL' is not executable"
MINISHELL="$(cd "$(dirname "$MINISHELL")" && pwd)/$(basename "$MINISHELL")"

# ------------------------------------------------------------- workspace ---
ROOT=$(mktemp -d "${TMPDIR:-/tmp}/mstester.XXXXXX") || die "mktemp failed"
ROOT=$(cd "$ROOT" && pwd -P)
SANDBOX="$ROOT/sandbox"
HOMEDIR="$ROOT/home"
RES="$ROOT/res"
mkdir -p "$RES"
core_db > "$ROOT/core.txt"

cleanup() {
	chmod -R u+rwx "$ROOT" 2>/dev/null
	rm -rf "$ROOT"
}
trap cleanup EXIT
trap 'echo; echo "${Y}interrupted${N}"; exit 130' INT

TEST_USER="${USER:-$(id -un 2>/dev/null || echo student)}"
TEST_ENV=(
	"PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"
	"HOME=$HOMEDIR"
	"USER=$TEST_USER"
	"LOGNAME=$TEST_USER"
	"PWD=$SANDBOX"
	"SHLVL=1"
	"TERM=${TERM:-xterm-256color}"
	"LANG=C"
	"LC_ALL=C"
)

if command -v timeout >/dev/null 2>&1; then
	TO=(timeout -k 2 "$TIMEOUT")
elif command -v gtimeout >/dev/null 2>&1; then
	TO=(gtimeout -k 2 "$TIMEOUT")
else
	TO=(perl -e 'alarm shift; exec @ARGV or exit 127' "$TIMEOUT")
fi

reset_sandbox() {
	chmod -R u+rwx "$SANDBOX" "$HOMEDIR" 2>/dev/null
	rm -rf "$SANDBOX" "$HOMEDIR"
	mkdir -p "$SANDBOX/dir/sub" "$HOMEDIR/folder"
	(
		cd "$SANDBOX" || exit 1
		printf 'line one\nline two\nline three\n' > infile
		printf 'hello\nworld\n' > infile2
		: > empty
		printf 'inner\n' > dir/inner
		printf '#!/bin/sh\necho "script ran: $*"\nexit 7\n' > script.sh
		chmod 755 script.sh
		cp script.sh noexec.sh; chmod 644 noexec.sh
		printf 'secret\n' > noperm; chmod 000 noperm
		mkdir noperm_dir; chmod 000 noperm_dir
		printf 'with spaces\n' > "file with spaces"
		printf 'home file\n' > "$HOMEDIR/homefile"
	)
}

snapshot() {
	local base
	for base in "$SANDBOX" "$HOMEDIR"; do
		[ -d "$base" ] || { echo "[missing] $base"; continue; }
		(
			cd "$base" || exit
			find . -mindepth 1 2>/dev/null | LC_ALL=C sort | while IFS= read -r f; do
				if [ -d "$f" ]; then
					echo "[dir]  $f"
				elif [ -r "$f" ]; then
					echo "[file] $f"; cat "$f"; echo "[eof]"
				else
					echo "[file] $f (unreadable)"
				fi
			done
		)
	done | sed "s#$ROOT#<root>#g"
}

# $1 = prefix for result files, rest = command ; stdin from $INPUT
run_shell() {
	local pfx="$1"; shift
	reset_sandbox
	{ ( cd "$SANDBOX" && exec "${TO[@]}" env -i "${TEST_ENV[@]}" "$@" ) \
		< "$INPUT" > "$pfx.out" 2> "$pfx.err" ; } 2>/dev/null
	echo $? > "$pfx.code"
	snapshot > "$pfx.files"
}

# strip terminal escape codes, readline markers and the prompt + echoed line
clean_output() { # file heredoc(0/1)
	P="$PROMPT_LIT" PR="$PROMPT_RE" PM="$PROMPT_MODE" BAN="$BANNER" HD="$2" HDP="$HD_PROMPT" INF="$INPUT" perl -0777 -pe '
		s/\e\[[0-9;?]*[ -\/]*[@-~]//g;
		s/\e\][^\a\e]*(?:\a|\e\\)//g;
		s/\e[()][A-Za-z0-9]//g; s/\e[=>]//g;
		s/[\x01\x02\r]//g;
		if (length $ENV{BAN}) { s/\A\Q$ENV{BAN}\E//; }
		if (length $ENV{PR}) {
			s/(?:$ENV{PR})[^\n]*(?:\n|\z)//g;
		} elsif (length $ENV{P}) {
			if ($ENV{PM} eq "str") { s/\Q$ENV{P}\E//g; }
			else { s/\Q$ENV{P}\E[^\n]*(?:\n|\z)//g; }
		}
		if ($ENV{HD} && length $ENV{HDP} && open(my $in, "<", $ENV{INF})) {
			my %typed; local $/ = "\n"; while (my $l = <$in>) { chomp $l; $typed{$l} = 1; }
			my $h = quotemeta $ENV{HDP};
			s/^$h([^\n]*)(?:\n|\z)/exists $typed{$1} ? "" : $&/mge;
		}
	' "$1"
}

# remove lines that are exactly "exit" if the reference has none
strip_exit_lines() { # ms_file ref_file
	# also handles "exit" glued after output that had no final newline
	REFX="$2" perl -0777 -i -pe '
		BEGIN { local $/; open my $fh, "<", $ENV{REFX}; $r = <$fh> // ""; }
		if ($r !~ /^exit$/m) { s/^exit(?:\n|\z)//mg; }
		if ($r !~ /exit\n?\z/) { s/exit\n?\z//; }
	' "$1"
}

normalize_err() {
	perl -pe 's/^[^:\n]*: (line \d+: )?//; s/^\s+//; s/\s+$/\n/' "$1"
}

# ---------------------------------------------------------- prompt probe ---
PROMPT_LIT=""
PROMPT_MODE="line"
BANNER=""
HD_PROMPT=""
detect_prompt() {
	INPUT="$ROOT/probe.in"
	printf 'echo __PROBE__\n' > "$INPUT"
	run_shell "$ROOT/probe" "$MINISHELL"
	local out
	for s in out err; do
		out=$(PROMPT_LIT= PROMPT_RE= BANNER= clean_output "$ROOT/probe.$s" 0 | perl -0777 -ne '
			if (/^(.*?)echo __PROBE__/m) {
				my $p = $1; my $ban = substr($_, 0, $-[0]);
				print "line\x1f$p\x1f$ban";
			} elsif (/^(.*?)__PROBE__/m && length $1) {
				my $p = $1; my $ban = substr($_, 0, $-[0]);
				print "str\x1f$p\x1f$ban";
			}')
		if [ -n "$out" ]; then
			PROMPT_MODE=${out%%$'\x1f'*}; out=${out#*$'\x1f'}
			PROMPT_LIT=${out%%$'\x1f'*}; BANNER=${out#*$'\x1f'}
			break
		fi
	done
	[ -n "$PROMPT_RE" ] && { PROMPT_LIT=""; BANNER=""; }
	# heredoc prompt: whatever is printed before a typed heredoc line
	INPUT="$ROOT/probe_hd.in"
	printf 'cat << __END__
__HDLINE__
__END__
' > "$INPUT"
	run_shell "$ROOT/probe_hd" "$MINISHELL"
	for s in out err; do
		out=$(HD_PROMPT= clean_output "$ROOT/probe_hd.$s" 0 | perl -ne 'if (/^(.+?)__HDLINE__$/) { print $1; exit }')
		if [ -n "$out" ]; then HD_PROMPT="$out"; return; fi
	done
}

# ---------------------------------------------------------- valgrind -------
make_supp() {
cat > "$ROOT/readline.supp" <<'EOF'
{
   readline_leaks
   Memcheck:Leak
   ...
   fun:readline
}
{
   add_history_leaks
   Memcheck:Leak
   ...
   fun:add_history
}
{
   rl_leaks
   Memcheck:Leak
   ...
   obj:*libreadline*
}
{
   tinfo_leaks
   Memcheck:Leak
   ...
   obj:*libtinfo*
}
EOF
}

# prints a short reason if problems are found
check_leaks() {
	local vgdir="$ROOT/vg" f problems="" n
	rm -rf "$vgdir"; mkdir -p "$vgdir"
	reset_sandbox
	{ ( cd "$SANDBOX" && exec "${TO[@]}" env -i "${TEST_ENV[@]}" valgrind \
		--leak-check=full --show-leak-kinds=definite,indirect \
		--errors-for-leak-kinds=definite,indirect --track-fds=yes \
		--trace-children=no --suppressions="$ROOT/readline.supp" \
		--log-file="$vgdir/vg.%p" "$MINISHELL" ) < "$INPUT" > /dev/null 2>&1 ; } 2>/dev/null
	for f in "$vgdir"/vg.*; do
		[ -f "$f" ] || continue
		if grep -q "definitely lost: [1-9]" "$f" || grep -q "indirectly lost: [1-9]" "$f"; then
			case "$problems" in *leak*) ;; *) problems="$problems leak" ;; esac
		fi
		if grep -qE "Invalid (read|write|free)|uninitialised|Mismatched free|Conditional jump|Source and destination overlap" "$f"; then
			case "$problems" in *memerr*) ;; *) problems="$problems memerr" ;; esac
		fi
		# fds still open at exit, not inherited, beyond 0 1 2
		n=$(awk '/Open (file|AF_UNIX socket|pipe)? ?descriptor [0-9]+:/ || /Open file descriptor [0-9]+:/ {
				match($0, /descriptor [0-9]+/); fd=substr($0, RSTART+11, RLENGTH-11)+0;
				getline nxt; if (fd > 2 && nxt !~ /inherited from parent/) c++ }
			END { print c+0 }' "$f")
		if [ "$n" -gt 0 ]; then
			case "$problems" in *fd*) ;; *) problems="$problems fd" ;; esac
		fi
	done
	if [ -n "$problems" ]; then
		mkdir -p "$ROOT/vglogs"; cat "$vgdir"/vg.* > "$ROOT/vglogs/last" 2>/dev/null
	fi
	echo "$problems"
}

# ======================================================================
#  running a test
# ======================================================================
PASS=0; FAIL=0; LEAKFAIL=0; TOTAL=0
FAILED_IDS=""
CUR_CAT=""
CAT_STATS=""

oneline() { # visual one-line version of a test
	local s="$1"
	s=${s//@@/ ⏎ }
	s=${s//	/→}
	if [ ${#s} -gt 72 ]; then s="${s:0:69}..."; fi
	printf '%s' "$s"
}

show_file() { # label file
	echo "  ---- $1 ----"
	if [ -s "$2" ]; then sed 's/^/  | /' "$2" | head -40; else echo "  | (empty)"; fi
}

log_failure() { # id cat test reasons
	{
		echo "=================================================================="
		echo "TEST #$1  [$2]  failed: $4"
		echo "------------------------------------------------------------------"
		echo "input:"
		printf '%s\n' "$3" | sed 's/@@/\n/g' | sed 's/^/    /'
		echo
		case "$4" in *OUT*|*CRASH*|*TIMEOUT*)
			echo "stdout diff (< bash   > minishell):"
			diff "$RES/ref.out" "$RES/ms.out" | head -60 | sed 's/^/    /'
			echo ;;
		esac
		case "$4" in *ERR*)
			echo "bash stderr:";      sed 's/^/    /' "$RES/ref.err" | head -20
			echo "minishell stderr:"; sed 's/^/    /' "$RES/ms.err" | head -20
			echo ;;
		esac
		case "$4" in *EXIT*|*CRASH*|*TIMEOUT*)
			echo "exit status: bash=$(cat "$RES/ref.code")  minishell=$(cat "$RES/ms.code")"
			echo ;;
		esac
		case "$4" in *FILES*)
			echo "files diff (< bash   > minishell):"
			diff "$RES/ref.files" "$RES/ms.files" | head -60 | sed 's/^/    /'
			echo ;;
		esac
		case "$4" in *LEAK*|*MEM*|*FD*)
			echo "valgrind report (all processes):"
			grep -E "lost:|Invalid|uninitialised|Conditional|Open .*descriptor|inherited|at 0x|by 0x" \
				"$ROOT/vglogs/last" 2>/dev/null | head -60 | sed 's/^/    /'
			echo ;;
		esac
	} >> "$LOG_FILE"
}

print_cat_header() {
	echo
	echo "${BD}${C}── $1 $(printf '─%.0s' $(seq 1 $((50 - ${#1}))))${N}"
}

run_test() { # id cat test
	local id="$1" cat="$2" t="$3" reasons="" hd=0 refcode mscode errnote=""
	INPUT="$RES/input"
	printf '%s\n' "${t//@@/$'\n'}" > "$INPUT"
	case "$t" in *"<<"*) hd=1 ;; esac

	run_shell "$RES/ref" "$REF_SHELL"
	run_shell "$RES/ms.raw" "$MINISHELL"
	clean_output "$RES/ms.raw.out" $hd > "$RES/ms.out"
	clean_output "$RES/ms.raw.err" $hd > "$RES/ms.err"
	mv "$RES/ms.raw.code" "$RES/ms.code"; mv "$RES/ms.raw.files" "$RES/ms.files"
	strip_exit_lines "$RES/ms.out" "$RES/ref.out"
	strip_exit_lines "$RES/ms.err" "$RES/ref.err"

	refcode=$(cat "$RES/ref.code"); mscode=$(cat "$RES/ms.code")

	if [ "$mscode" = 124 ] || [ "$mscode" = 137 ] || [ "$mscode" = 142 ]; then
		[ "$refcode" != "$mscode" ] && reasons="$reasons TIMEOUT"
	elif [ "$mscode" -gt 128 ] 2>/dev/null && [ "$refcode" != "$mscode" ]; then
		case "$mscode" in
			134) reasons="$reasons CRASH(abort)" ;;
			136) reasons="$reasons CRASH(fpe)" ;;
			138) reasons="$reasons CRASH(bus)" ;;
			139) reasons="$reasons CRASH(segv)" ;;
			*)   reasons="$reasons CRASH(sig$((mscode-128)))" ;;
		esac
	fi
	case "$reasons" in
		*TIMEOUT*|*CRASH*) ;;
		*)
		cmp -s "$RES/ref.out" "$RES/ms.out" || reasons="$reasons OUT"
		if [ -s "$RES/ref.err" ] && [ ! -s "$RES/ms.err" ]; then errnote="ERR(missing)"; fi
		if [ ! -s "$RES/ref.err" ] && [ -s "$RES/ms.err" ]; then errnote="ERR(unexpected)"; fi
		if [ -z "$errnote" ] && [ "$STRICT_ERR" = 1 ] && \
			! cmp -s <(normalize_err "$RES/ref.err") <(normalize_err "$RES/ms.err"); then errnote="ERR"; fi
		if [ -n "$errnote" ] && [ "$CHECK_ERR" = 1 ]; then reasons="$reasons $errnote"; errnote=""; fi
		[ "$refcode" = "$mscode" ] || reasons="$reasons EXIT"
		cmp -s "$RES/ref.files" "$RES/ms.files" || reasons="$reasons FILES"
		;;
	esac

	local leak=""
	if [ "$RUN_LEAKS" = 1 ]; then
		leak=$(check_leaks)
		case "$leak" in *leak*) reasons="$reasons LEAK" ;; esac
		case "$leak" in *memerr*) reasons="$reasons MEM" ;; esac
		case "$leak" in *fd*) reasons="$reasons FD" ;; esac
	fi

	TOTAL=$((TOTAL+1))
	if [ -z "$reasons" ]; then
		PASS=$((PASS+1))
		if [ -n "$errnote" ]; then
			printf "  ${D}%4d${N}  ${G}✔ PASS${N}  ${D}%s${N}  ${D}(note: %s, not counted)${N}\n" "$id" "$(oneline "$t")" "$errnote"
		else
			printf "  ${D}%4d${N}  ${G}✔ PASS${N}  ${D}%s${N}\n" "$id" "$(oneline "$t")"
		fi
		CAT_STATS="$CAT_STATS $cat:1"
	else
		reasons=${reasons# }
		FAIL=$((FAIL+1)); FAILED_IDS="$FAILED_IDS $id"
		case "$reasons" in *LEAK*|*MEM*|*FD*) LEAKFAIL=$((LEAKFAIL+1)) ;; esac
		printf "  ${D}%4d${N}  ${R}✘ FAIL${N}  %s  ${Y}[%s]${N}%s\n" "$id" "$(oneline "$t")" "$reasons" \
			"${errnote:+  ${D}(note: $errnote, not counted)${N}}"
		CAT_STATS="$CAT_STATS $cat:0"
		log_failure "$id" "$cat" "$t" "$reasons"
		if [ "$VERBOSE" = 1 ]; then
			case "$reasons" in *OUT*|*CRASH*|*TIMEOUT*)
				show_file "bash stdout" "$RES/ref.out"; show_file "minishell stdout" "$RES/ms.out" ;;
			esac
			case "$reasons" in *ERR*)
				show_file "bash stderr" "$RES/ref.err"; show_file "minishell stderr" "$RES/ms.err" ;;
			esac
			case "$reasons" in *EXIT*)
				echo "  ---- exit: bash=$refcode minishell=$mscode ----" ;;
			esac
			case "$reasons" in *FILES*)
				echo "  ---- files diff (< bash  > minishell) ----"
				diff "$RES/ref.files" "$RES/ms.files" | head -30 | sed 's/^/  | /' ;;
			esac
			case "$reasons" in *LEAK*|*MEM*|*FD*)
				echo "  ---- valgrind ----"
				grep -E "lost:|Invalid|uninitialised|Conditional|Open .*descriptor" "$ROOT/vglogs/last" | head -15 | sed 's/^/  | /' ;;
			esac
		fi
		if [ "$STOP_ON_FAIL" = 1 ]; then summary; exit 1; fi
	fi
}

# ======================================================================
#  signal tests (python3 + pty)
# ======================================================================
write_signal_script() {
cat > "$ROOT/signals.py" <<'PYEOF'
import os, sys, pty, time, select, re, signal

CMD = sys.argv[1:]
COLOR = os.environ.get("MST_COLOR") == "1"
G, R, D, N = ("\033[32m", "\033[31m", "\033[2m", "\033[0m") if COLOR else ("", "", "", "")
TEST_ENV = dict(e.split("=", 1) for e in os.environ.get("MST_ENV", "").split("\x1f") if "=" in e)

class Shell:
    def __init__(self):
        self.pid, self.fd = pty.fork()
        if self.pid == 0:
            os.chdir(TEST_ENV.get("PWD", "/"))
            os.execve(CMD[0], CMD, TEST_ENV)
        self.buf = b""
        self.status = None
        self.read(0.6)

    def read(self, t=0.3):
        end = time.time() + t
        while True:
            left = end - time.time()
            if left <= 0:
                break
            try:
                r, _, _ = select.select([self.fd], [], [], left)
            except InterruptedError:
                continue
            if not r:
                break
            try:
                data = os.read(self.fd, 4096)
            except OSError:
                break
            if not data:
                break
            self.buf += data
        return self.buf

    def send(self, s, wait=0.4):
        try:
            os.write(self.fd, s.encode() if isinstance(s, str) else s)
        except OSError:
            pass
        return self.read(wait)

    def wait_for(self, pattern, t=3.0):
        end = time.time() + t
        while time.time() < end:
            m = re.search(pattern, self.text())
            if m:
                return m
            self.read(0.1)
        return re.search(pattern, self.text())

    def text(self):
        t = self.buf.decode(errors="replace")
        return re.sub(r"\x1b\[[0-9;?]*[ -/]*[@-~]|\r", "", t)

    def alive(self):
        if self.status is not None:
            return False
        pid, st = os.waitpid(self.pid, os.WNOHANG)
        if pid == 0:
            return True
        self.status = st
        return False

    def wait_exit(self, t=2.0):
        end = time.time() + t
        while time.time() < end:
            self.read(0.05)
            if not self.alive():
                return True
        return False

    def exit_code(self):
        if self.status is None:
            return None
        if os.WIFEXITED(self.status):
            return os.WEXITSTATUS(self.status)
        return 128 + os.WTERMSIG(self.status)

    def close(self):
        if self.alive():
            try:
                os.kill(self.pid, signal.SIGKILL)
                os.waitpid(self.pid, 0)
            except Exception:
                pass
        try:
            os.close(self.fd)
        except OSError:
            pass

STATUS = r'S:(\d+):'
def status_of(sh):
    mark = len(sh.text())
    sh.send('echo "S:$?:"\n')
    end = time.time() + 3
    while time.time() < end:
        m = re.search(STATUS, sh.text()[mark:])
        if m:
            return int(m.group(1))
        sh.read(0.1)
    return None

def alive_check(sh):
    mark = len(sh.text())
    sh.send('echo AL""IVE\n')
    end = time.time() + 3
    while time.time() < end:
        if "ALIVE" in sh.text()[mark:]:
            return True
        sh.read(0.1)
    return False

results = []
def test(name):
    def deco(fn):
        sh = None
        try:
            sh = Shell()
            ok, detail = fn(sh)
        except Exception as e:
            ok, detail = False, "exception: %r" % (e,)
        finally:
            out = sh.text() if sh else ""
            if sh:
                sh.close()
        results.append((name, ok, detail, out))
        mark = (G + "✔ PASS" + N) if ok else (R + "✘ FAIL" + N)
        print("  %s  %s%s" % (mark, name, "" if ok else "  " + D + "(" + detail + ")" + N), flush=True)
        return fn
    return deco

@test("Ctrl-C on empty prompt gives a new prompt and $? = 130")
def _(sh):
    sh.send(b"\x03", 0.5)
    if not sh.alive(): return False, "shell exited"
    st = status_of(sh)
    return st == 130, "$? = %s" % st

@test("Ctrl-C discards the line being typed")
def _(sh):
    sh.send('echo ZZ"Q"Q', 0.3)
    sh.send(b"\x03", 0.5)
    ok = alive_check(sh)
    return ok and "ZZQQ" not in sh.text(), "line was executed" if ok else "shell not responding"

@test("Ctrl-C three times, shell still alive")
def _(sh):
    for _ in range(3): sh.send(b"\x03", 0.3)
    return alive_check(sh), "shell not responding"

@test("Ctrl-\\ on empty prompt does nothing")
def _(sh):
    sh.send(b"\x1c", 0.5)
    if not sh.alive(): return False, "shell exited (status %s)" % sh.exit_code()
    ok = alive_check(sh)
    st = status_of(sh)
    return ok and st == 0, "alive=%s $?=%s" % (ok, st)

@test("Ctrl-\\ while typing does nothing")
def _(sh):
    sh.send('echo par', 0.2)
    sh.send(b"\x1c", 0.3)
    sh.send('tial\n', 0.5)
    m = re.search(r"(?m)^partial$", sh.text())
    return bool(m), "expected 'partial' to be printed"

@test("Ctrl-D on empty prompt exits the shell with status 0")
def _(sh):
    sh.send(b"\x04", 0.3)
    if not sh.wait_exit(): return False, "shell did not exit"
    return sh.exit_code() == 0, "exit status %s" % sh.exit_code()

@test("Ctrl-D exits with the last exit status")
def _(sh):
    sh.send("ls /nonexistent_zz_dir\n", 0.6)
    sh.send(b"\x04", 0.3)
    if not sh.wait_exit(): return False, "shell did not exit"
    return sh.exit_code() == 2, "exit status %s (expected 2)" % sh.exit_code()

@test("Ctrl-D on a non-empty line does not exit")
def _(sh):
    sh.send("echo abc", 0.2)
    sh.send(b"\x04", 0.4)
    if not sh.alive(): return False, "shell exited"
    sh.send("\n", 0.4)
    return alive_check(sh), "shell not responding"

@test("Ctrl-C during 'cat' -> $? = 130, shell alive")
def _(sh):
    sh.send("cat\n", 0.5)
    sh.send(b"\x03", 0.5)
    if not sh.alive(): return False, "shell exited"
    st = status_of(sh)
    return st == 130, "$? = %s" % st

@test("Ctrl-\\ during 'cat' -> $? = 131, shell alive")
def _(sh):
    sh.send("cat\n", 0.5)
    sh.send(b"\x1c", 0.5)
    if not sh.alive(): return False, "shell exited"
    st = status_of(sh)
    return st == 131, "$? = %s" % st

@test("Ctrl-D during 'cat' -> cat ends, $? = 0")
def _(sh):
    sh.send("cat\n", 0.5)
    sh.send(b"\x04", 0.5)
    if not sh.alive(): return False, "shell exited"
    st = status_of(sh)
    return st == 0, "$? = %s" % st

@test("Ctrl-C during 'sleep 5' interrupts it")
def _(sh):
    sh.send("sleep 5\n", 0.4)
    t = time.time()
    sh.send(b"\x03", 0.2)
    st = status_of(sh)
    took = time.time() - t
    return st == 130 and took < 3, "$? = %s after %.1fs" % (st, took)

@test("Ctrl-C during 'cat | cat | cat' -> $? = 130")
def _(sh):
    sh.send("cat | cat | cat\n", 0.5)
    sh.send(b"\x03", 0.5)
    if not sh.alive(): return False, "shell exited"
    st = status_of(sh)
    return st == 130, "$? = %s" % st

@test("Ctrl-C in heredoc cancels the command, $? = 130")
def _(sh):
    sh.send("cat << EOF\n", 0.4)
    sh.send("X$?X\n", 0.3)
    sh.send(b"\x03", 0.5)
    if not sh.alive(): return False, "shell exited"
    st = status_of(sh)
    return st == 130 and "X0X" not in sh.text(), "$? = %s%s" % (st, ", heredoc was executed" if "X0X" in sh.text() else "")

@test("Ctrl-D in heredoc ends it and runs the command")
def _(sh):
    sh.send("cat << EOF\n", 0.4)
    sh.send("Y$?Y\n", 0.3)
    sh.send(b"\x04", 0.6)
    if not sh.alive(): return False, "shell exited"
    m = sh.wait_for(r"Y0Y", 2)
    return bool(m) and alive_check(sh), "heredoc content not printed"

@test("Ctrl-C after an interrupted command: prompt not printed twice")
def _(sh):
    sh.send("sleep 5\n", 0.4)
    mark = len(sh.text())
    sh.send(b"\x03", 0.8)
    after = sh.text()[mark:]
    lines = [l for l in after.split("\n") if l.strip() and l.strip() != "^C"]
    return len(lines) <= 1, "output after ^C: %r" % after

@test("Nested minishell: Ctrl-C handled by the inner shell, then exit 5")
def _(sh):
    sh.send(CMD[0] + "\n", 0.8)
    sh.send(b"\x03", 0.4)
    sh.send("exit 5\n", 0.6)
    st = status_of(sh)
    return st == 5 and alive_check(sh), "outer $? = %s" % st

@test("Nested minishell: Ctrl-\\ in inner shell does nothing")
def _(sh):
    sh.send(CMD[0] + "\n", 0.8)
    sh.send(b"\x1c", 0.4)
    sh.send("exit 6\n", 0.6)
    st = status_of(sh)
    return st == 6, "outer $? = %s" % st

@test("After Ctrl-C, next command works normally")
def _(sh):
    sh.send(b"\x03", 0.3)
    sh.send("echo ab""cd | cat -e\n", 0.6)
    return "abcd$" in sh.text(), "pipeline output missing"

passed = sum(1 for r in results if r[1])
logf = os.environ.get("MST_LOG")
if logf:
    with open(logf, "a") as f:
        for name, ok, detail, out in results:
            if not ok:
                f.write("=" * 66 + "\nSIGNAL TEST failed: %s\n  %s\n  terminal output:\n" % (name, detail))
                for l in out.split("\n")[-25:]:
                    f.write("    | " + l + "\n")
print("SIGRESULT %d %d" % (passed, len(results)))
PYEOF
}

SIG_PASS=0; SIG_TOTAL=0
run_signals() {
	local py
	py=$(command -v python3) || { echo "${Y}python3 not found: signal tests skipped${N}"; return; }
	write_signal_script
	print_cat_header "signals (interactive, pty)"
	reset_sandbox
	local envs="" e
	for e in "${TEST_ENV[@]}"; do envs="$envs$e"$'\x1f'; done
	local out l
	MST_ENV="$envs" MST_COLOR="$COLOR" MST_LOG="$LOG_FILE" "$py" "$ROOT/signals.py" "$MINISHELL" |
	while IFS= read -r l; do
		case "$l" in SIGRESULT*) echo "$l" > "$ROOT/sigresult" ;; *) echo "$l" ;; esac
	done
	out=$(cat "$ROOT/sigresult" 2>/dev/null)
	SIG_PASS=$(echo "$out" | awk '{print $2}'); SIG_TOTAL=$(echo "$out" | awk '{print $3}')
	[ -n "$SIG_PASS" ] || { SIG_PASS=0; SIG_TOTAL=0; }
}

# ======================================================================
#  summary
# ======================================================================
summary() {
	echo
	echo "${BD}═══════════════════════════ SUMMARY ═══════════════════════════${N}"
	if [ "$TOTAL" -gt 0 ]; then
		echo "$CAT_STATS" | tr ' ' '\n' | awk -F: -v g="$G" -v r="$R" -v n="$N" '
			NF==2 { if (!($1 in tot)) order[++k]=$1; tot[$1]++; ok[$1]+=$2 }
			END { for (i=1;i<=k;i++) { c=order[i]; col=(ok[c]==tot[c])?g:r;
				printf "  %-26s %s%3d / %-3d%s\n", c, col, ok[c], tot[c], n } }'
		echo
		local pct=$((PASS * 100 / TOTAL))
		local col=$G; [ "$FAIL" -gt 0 ] && col=$Y; [ "$pct" -lt 70 ] && col=$R
		echo "  ${BD}Tests:${N}   ${col}$PASS / $TOTAL passed ($pct%)${N}"
		[ "$RUN_LEAKS" = 1 ] && echo "  ${BD}Valgrind:${N} $LEAKFAIL test(s) with leaks / memory errors / open fds"
	fi
	if [ "$CORE" = 1 ] && [ "$TOTAL" -gt 0 ]; then
		if [ "$FAIL" -eq 0 ]; then
			echo "  ${G}All core tests pass.${N} ${D}Next: -s (signals), -L (leaks), or -F for the full edge-case suite.${N}"
		else
			echo "  ${D}Core mode: these are worth fixing. -F runs the full edge-case suite.${N}"
		fi
	fi
	if [ "$SIG_TOTAL" -gt 0 ] 2>/dev/null; then
		local scol=$G; [ "$SIG_PASS" != "$SIG_TOTAL" ] && scol=$R
		echo "  ${BD}Signals:${N} ${scol}$SIG_PASS / $SIG_TOTAL passed${N}"
	fi
	if [ "$FAIL" -gt 0 ]; then
		local ids short
		ids=$(echo $FAILED_IDS | tr ' ' ',')
		short=$(echo $FAILED_IDS | tr ' ' '\n' | head -40 | tr '\n' ',' | sed 's/,$//')
		[ "$FAIL" -gt 40 ] && short="$short ... (+$((FAIL-40)) more, see log)"
		echo
		echo "  Failed tests: ${Y}$short${N}"
		echo "  Details:      $LOG_FILE"
		echo "  Re-run one:   $0 -t $(echo $FAILED_IDS | cut -d' ' -f1) $MINISHELL_ARG"
		echo "Failed tests: $ids" >> "$LOG_FILE"
	fi
	echo
}

# ======================================================================
#  main
# ======================================================================
MINISHELL_ARG="$MINISHELL"
: > "$LOG_FILE"
{
	echo "minishell_tester v$VERSION — $(date)"
	echo "minishell: $MINISHELL"
	echo "reference: $REF_SHELL ($("$REF_SHELL" --version 2>/dev/null | head -1))"
	echo
} >> "$LOG_FILE"

echo "${BD}${M}"
echo "  ┌──────────────────────────────────────────┐"
echo "  │          minishell tester  v$VERSION          │"
echo "  └──────────────────────────────────────────┘${N}"
echo "  minishell : $MINISHELL"
echo "  reference : $REF_SHELL"

if [ "$RUN_LEAKS" = 1 ]; then
	command -v valgrind >/dev/null 2>&1 || die "--leaks needs valgrind"
	make_supp
fi

if [ "$RUN_SIGNALS" != 2 ]; then
	detect_prompt
	if [ -n "$PROMPT_RE" ]; then
		echo "  prompt    : regex /$PROMPT_RE/"
	elif [ -n "$PROMPT_LIT" ]; then
		echo "  prompt    : \"$PROMPT_LIT\" (auto-detected, mode: $PROMPT_MODE)"
	else
		echo "  prompt    : none detected (fine if you don't print one when stdin isn't a tty)"
	fi
	[ -n "$HD_PROMPT" ] && echo "  heredoc   : prompt \"$HD_PROMPT\" (auto-detected)"
	[ -n "$BANNER" ] && echo "  banner    : detected and ignored"
	PROBE_CODE=$(cat "$ROOT/probe.code" 2>/dev/null)
	if [ "${PROBE_CODE:-0}" -gt 128 ] 2>/dev/null && [ "$PROBE_CODE" != 255 ]; then
		echo "  ${R}warning: your minishell died with signal $((PROBE_CODE-128)) on 'echo __PROBE__' piped to stdin${N}"
		echo "  ${R}         (non-tty stdin must work: readline returns lines normally, then NULL at EOF)${N}"
	elif [ "${PROBE_CODE:-0}" != 0 ] && [ "${PROBE_CODE:-0}" -le 128 ] 2>/dev/null; then
		echo "  ${R}warning: after 'echo __PROBE__' then end of input (Ctrl-D), your minishell exits"
		echo "           with status $PROBE_CODE. bash exits with the status of the last command (0 here),"
		echo "           so the EXIT check will fail on almost every test until this is fixed.${N}"
	fi
	if grep -q "__PROBE__" "$ROOT/probe.out" 2>/dev/null || [ -n "$PROMPT_RE" ]; then :; else
		echo "  ${Y}warning: 'echo __PROBE__' printed nothing — is your minishell reading stdin?${N}"
	fi
	if [ "$CORE" = 1 ]; then
		echo "  mode      : ${BD}core${N} (what evaluators check)  ${D}— run with -F for every edge case${N}"
	else
		echo "  mode      : full suite"
	fi
	echo "  ${D}OUT=stdout  ERR=stderr  EXIT=exit status  FILES=sandbox contents${N}"

	while IFS= read -r line; do
		case "$line" in
			'### '*) CUR_CAT=${line#'### '}; HEADER_DONE=0; continue ;;
			'#'*|'') continue ;;
		esac
		ID=$((${ID:-0}+1))
		cat_enabled "$CUR_CAT" || continue
		if [ "$CORE" = 1 ] && [ "$(cat_tier "$CUR_CAT")" = mandatory ] && [ "$CUR_CAT" != stress ] \
			&& [ "$CUR_CAT" != custom ] && ! grep -qxF -- "$line" "$ROOT/core.txt"; then
			continue
		fi
		if [ -n "$ONLY_IDS" ]; then
			case "$ONLY_IDS" in *" $ID "*) ;; *) continue ;; esac
		fi
		if [ "$HEADER_DONE" != 1 ]; then print_cat_header "$CUR_CAT"; HEADER_DONE=1; fi
		run_test "$ID" "$CUR_CAT" "$line"
	done < <(all_tests)
fi

if [ "$RUN_SIGNALS" -ge 1 ] && [ -z "$ONLY_IDS" ]; then
	run_signals
fi

summary
[ "$FAIL" -eq 0 ] && [ "${SIG_PASS:-0}" = "${SIG_TOTAL:-0}" ]
