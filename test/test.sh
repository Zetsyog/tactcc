. ./functions.sh
TARGET="../bin/scalpa"
LOG="test.log"
export LOG_LEVEL=100

rm test.log

title "Lexical analysis tests "
. ./lexical/test.sh

title "MIPS execution tests"
. ./mips/test.sh

title "Func tests"
. ./func/test.sh

title "Operations tests"
. ./op/test.sh
show_result
