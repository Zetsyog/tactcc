# Reset
Reset="\e[0m"       # Text Reset

# Regular Colors
Black="\e[0;30m"        # Black
Red="\e[0;31m"          # Red
Green="\e[0;32m"        # Green
Yellow="\e[0;33m"       # Yellow
Blue="\e[0;34m"         # Blue
Purple="\e[0;35m"       # Purple
Cyan="\e[0;36m"         # Cyan
White="\e[0;37m"        # White

# Bold
BBlack="\e[1;30m"       # Black
BRed="\e[1;31m"         # Red
BGreen="\e[1;32m"       # Green
BYellow="\e[1;33m"      # Yellow
BBlue="\e[1;34m"        # Blue
BPurple="\e[1;35m"      # Purple
BCyan="\e[1;36m"        # Cyan
BWhite="\e[1;37m"       # White

# Underline
UBlack="\e[4;30m"       # Black
URed="\e[4;31m"         # Red
UGreen="\e[4;32m"       # Green
UYellow="\e[4;33m"      # Yellow
UBlue="\e[4;34m"        # Blue
UPurple="\e[4;35m"      # Purple
UCyan="\e[4;36m"        # Cyan
UWhite="\e[4;37m"       # White

# Background
On_Black="\e[40m"       # Black
On_Red="\e[41m"         # Red
On_Green="\e[42m"       # Green
On_Yellow="\e[43m"      # Yellow
On_Blue="\e[44m"        # Blue
On_Purple="\e[45m"      # Purple
On_Cyan="\e[46m"        # Cyan
On_White="\e[47m"       # White

# High Intensity
IBlack="\e[0;90m"       # Black
IRed="\e[0;91m"         # Red
IGreen="\e[0;92m"       # Green
IYellow="\e[0;93m"      # Yellow
IBlue="\e[0;94m"        # Blue
IPurple="\e[0;95m"      # Purple
ICyan="\e[0;96m"        # Cyan
IWhite="\e[0;97m"       # White

# Bold High Intensity
BIBlack="\e[1;90m"      # Black
BIRed="\e[1;91m"        # Red
BIGreen="\e[1;92m"      # Green
BIYellow="\e[1;93m"     # Yellow
BIBlue="\e[1;94m"       # Blue
BIPurple="\e[1;95m"     # Purple
BICyan="\e[1;96m"       # Cyan
BIWhite="\e[1;97m"      # White

# High Intensity backgrounds
On_IBlack="\e[0;100m"   # Black
On_IRed="\e[0;101m"     # Red
On_IGreen="\e[0;102m"   # Green
On_IYellow="\e[0;103m"  # Yellow
On_IBlue="\e[0;104m"    # Blue
On_IPurple="\e[0;105m"  # Purple
On_ICyan="\e[0;106m"    # Cyan
On_IWhite="\e[0;107m"   # White

TEST_NB=0
SUCCESS=0
FAILED=0

title ()
{
    COURANT="$1"
    echo "${Blue}==== $1 ====${Reset}"
}

ok ()
{
    echo "${Green}OK${Reset}"
}

fail ()
{
    echo "${BRed}Test failed${Reset}"
    echo "==> Exit"
    exit 1
}

test_diff()
{
    echo "$2" | diff "$1" -
}
 
test_in_out ()
{
    STDIN=$1
    EXEPECTED=$2
    TEST_FUNC=$3
    OUT=/tmp/compil-test.out.$$
    TEST_NB=$((TEST_NB+1))

    # execute le programme, conserve la sortie (stdout et stderr confondu)
    echo "" >> $LOG
    echo "### Exec" >> $LOG
    echo "$STDIN" | ./$TARGET 2>&1 > $OUT
    EXITCODE=$?

    # conserve la sorte dans le log
    cat $OUT >> $LOG
    echo "==> exitcode: $EXITCODE" >> $LOG

    if [ $EXITCODE -ne 0 ]
    then
    FAILED=$((FAILED+1))
    fail
    fi


    # Compare avec l'attendu
    echo ""
    echo "#### Comp" >> $LOG
    $TEST_FUNC "$OUT" "$EXPECTED" >> $LOG
    if [ $? -ne 0 ]
    then
        FAILED=$((FAILED+1))
        fail
    else
        echo "${Green}OK"
        SUCCESS=$((SUCCESS+1))
    fi
}

test_file_in_out ()
{
    STDIN=$(cat $1)
    EXPECTED=$(cat $2)
    test_in_out "$STDIN" "$EXPECTED" "$3"
}

test_in_file ()
{
    STDIN=$1
    EXPECTED=$2
    OUT=/tmp/compil-test.out.$$
    TEST_NB=$((TEST_NB+1))

    # execute le programme, conserve la sortie (stdout et stderr confondu)
    echo "" >> $LOG
    echo "### Exec" >> $LOG
    echo "$STDIN" > ./$TARGET 2>&1 > $OUT
    EXITCODE=$?

    # conserve la sorte dans le log
    cat $OUT >> $LOG
    echo "==> exitcode: $EXITCODE" >> $LOG

    if [ $EXITCODE -ne $EXPECTED ]
    then
        FAILED=$((FAILED+1))
        fail
    else
        echo "${Green}OK"
        SUCCESS=$((SUCCESS+1))
    fi
}


run_test_file () 
{
    echo "Exec $TARGET with input ${PREFIX}/$1"
    test_file_in_out "${PREFIX}/$1" "${PREFIX}/$2" "$3"
}

run_test_simple ()
{
    printf "Exec $TARGET with input ${PREFIX}/$1..."
    STDIN=$(cat ${PREFIX}/$1)
    test_in_file "$STDIN" "$2"
}

show_result () 
{
    title "Result"
    echo "$TEST_NB tests"
    echo "${Green}$SUCCESS / $TEST_NB passed"
    echo "${Red}$FAILED / $TEST_NB failed"
}