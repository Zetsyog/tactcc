. ./functions.sh
TARGET="../bin/scalpa"
LOG="test.log"

rm test.log

title "Lexical analysis tests "
. ./lexical/test.sh

title "MIPS execution tests"
. ./mips/test.sh

show_result
