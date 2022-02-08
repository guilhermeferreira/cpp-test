testcase_setup "ini_delete" "/tmp/test/integration/database.ini"

# SET
set test_cmd "SET City Amsterdam\r"
set test_result "OK"
append test_report [ testcase "ini_delete" "SET" $test_cmd $test_result ]

# DELETE
set test_cmd "DELETE City\r"
set test_result "OK"
append test_report [ testcase "ini_delete" "DELETE" $test_cmd $test_result ]

# GET
set test_cmd "GET City\r"
set test_result ">"
append test_report [ testcase "ini_delete" "GET" $test_cmd $test_result ]

testcase_teardown "ini_delete"
