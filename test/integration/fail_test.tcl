testcase_setup "fail" "/tmp/test/integration/database.ini"

set test_cmd "SET keyF valueF\r"
set test_result "OK"
append test_report [ testcase "fail" "SET" $test_cmd $test_result ]

set test_cmd "GET keyF\r"
set test_result "valueF"
append test_report [ testcase "fail" "GET" $test_cmd $test_result ]

set test_cmd "DELETE keyF\r"
set test_result "OK"
append test_report [ testcase "fail" "DELETE" $test_cmd $test_result ]

# NOTE: This test fails ON PURPOSE
set test_cmd "GET keyF\r"
set test_result "valueF"
append test_report [ testcase "fail" "GET" $test_cmd $test_result ]

testcase_teardown "fail"
