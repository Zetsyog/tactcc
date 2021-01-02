. ./functions.sh
TARGET="../bin/scalpa"
LOG="test.log"
export LOG_LEVEL=100

rm test.log

title "Lexical analysis tests "
. ./lexical/run_tests.sh

title "MIPS execution tests"
. ./mips/run_tests.sh

title "Func tests"
. ./func/run_tests.sh

title "Operations tests"
. ./op/run_tests.sh

title "Valgrind tests"
. ./memory/run_tests.sh

show_result
