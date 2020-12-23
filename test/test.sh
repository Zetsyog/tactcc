. ./functions.sh
TARGET="../bin/scalpa"
LOG="test.log"

rm test.log

title "Lexical analysis test "
. ./lexical/test.sh

show_result
