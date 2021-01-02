PREFIX="func"
run_test_simple "test-0/src.scpa" 0
test_compil_exec "test-1" test_diff
test_compil_exec "test-2" test_diff
test_compil_exec "test-3" test_diff
test_compil_exec_in_file "test-4" test_diff
test_compil_exec_in_file "test-5" test_diff
