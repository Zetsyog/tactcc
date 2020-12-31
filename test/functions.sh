TEST_NB=0
SUCCESS_NB=0
FAILED_NB=0

title() {
    title="==== $1 ===="

    coloredEcho "$title" blue
    echo "$title" >>$LOG
}

if [ -z ${COLORIZE+x} ]; then
    COLORIZE=1
fi

coloredEcho() {
    local exp="$1"
    local color=$2

    case $(echo $color | tr '[:upper:]' '[:lower:]') in
    black) color=0 ;;
    red) color=1 ;;
    magenta) color=5 ;;
    green) color=2 ;;
    yellow) color=3 ;;
    blue) color=4 ;;
    cyan) color=6 ;;
    gray) color=8 ;;
    white | *) color=7 ;; # white or invalid color
    esac
    if [ $COLORIZE -eq 1 ]; then
        tput setaf $color
        echo "$exp"
        tput sgr0
    else
        echo "$exp"
    fi
}

success() {
    SUCCESS_NB=$((SUCCESS_NB + 1))
    coloredEcho "OK" green
    echo " = Success = " >>$LOG
}

fail() {
    FAILED_NB=$((FAILED_NB + 1))
    coloredEcho "Test failed" red
}

test_diff() {
    diff "$1" "$2"
}

test_grep() {
    cat "$1" | grep "$2"
}

test_in_file() {
    STDIN=$1
    EXPECTED=$2
    OUT=/tmp/compil-test.out.$$
    TEST_NB=$((TEST_NB + 1))

    # execute le programme, conserve la sortie (stdout et stderr confondu)
    echo "cat $STDIN | ./$TARGET" >>$LOG
    echo "#### Output" >>$LOG
    cat $STDIN | ./$TARGET 2>&1 >$OUT
    EXITCODE=$?

    # conserve la sorte dans le log
    cat $OUT >>$LOG
    echo "==> exitcode: $EXITCODE ; expected: => $EXPECTED" >>$LOG

    if [ $EXITCODE -ne $EXPECTED ]; then
        fail
    else
        success
    fi
    printf "\n" >>$LOG
    printf "\n"
}

run_test_simple() {
    echo "# Test (exitcode):" >>$LOG
    printf "${PREFIX}/$1 "
    STDIN="${PREFIX}/$1"
    test_in_file "$STDIN" "$2"
}

test_compil_exec() {
    test_compil_exec_in $1 "echo" $2
}

test_compil_exec_in_file() {
    test_compil_exec_in $1 "cat ${PREFIX}/$1/in" $2
}

# test_compil_exec_in <test_dir> <spim_input> <comp_func>
test_compil_exec_in() {
    DIR="${PREFIX}/$1"
    ASM_FILE="${DIR}/mips.asm.tmp"
    OUT=/tmp/mips-test.out.$$
    IN_CODE="${DIR}/src.scpa"
    IN_SPIM="$2"
    EXPECTED="${DIR}/out"
    TEST_FUNC=$3
    TEST_NB=$((TEST_NB + 1))

    printf "Compiling test/$IN_CODE "
    # On compile le code scalpa
    echo "# Test (compilation & execution):" >>$LOG
    echo "cat $IN_CODE | $TARGET -o $ASM_FILE" >>$LOG
    echo "#### Output" >>$LOG
    cat "$IN_CODE" | ./$TARGET -o "$ASM_FILE" 2>&1 >>$LOG
    EXITCODE=$?

    if [ $EXITCODE -ne 0 ]; then
        # conserve la sorte dans le log
        echo "==> compilation failed" >>$LOG
        fail
        printf "\n" >>$LOG
        return
    else
        coloredEcho "OK" green
    fi

    echo "spim -file $ASM_FILE > $OUT" >>$LOG
    printf "Exec test/$ASM_FILE "
    $2 | spim -file $ASM_FILE | tail -n +6 >$OUT

    echo "" >>$LOG
    echo "#### Comp" >>$LOG
    echo "$TEST_FUNC $OUT $EXPECTED" >>$LOG
    $TEST_FUNC "$OUT" "$EXPECTED" >>$LOG

    if [ $? -ne 0 ]; then
        fail
    else
        success
    fi
    printf "\n" >>$LOG
    echo ""
}

test_valgrind() {
    DIR="${PREFIX}/$1"
    IN_CODE="${DIR}/src.scpa"
    TEST_NB=$((TEST_NB + 1))

    printf "Valgrind test/$IN_CODE "

    echo "# Test Valgrind:" >>$LOG
    echo "valgrind --error-exitcode=42 --leak-check=full -q $TARGET 2>&1 < $IN_CODE" >>$LOG
    OUT=$(valgrind --error-exitcode=42 --leak-check=full -q $TARGET 2>&1 <$IN_CODE)
    EXITCODE=$?

    echo "#### Output" >>$LOG
    echo "$OUT" >>$LOG

    if [ $EXITCODE -eq 42 ]; then
        # conserve la sorte dans le log
        echo "==> Valgrind failed, memory leak detected" >>$LOG
        fail
        printf "\n" >>$LOG
        return
    else
        success
    fi
    printf "\n" >>$LOG
}

show_result() {
    title "Result"
    coloredEcho "$TEST_NB tests" magenta
    coloredEcho "$SUCCESS_NB / $TEST_NB passed" green
    coloredEcho "${Red}$FAILED_NB / $TEST_NB failed" red
}
