testcase_setup "db_delete" "/tmp/test/integration/database.db"

# SET
set test_cmd "SET City Amsterdam\r"
set test_result "OK"
append test_report [ testcase "db_delete" "SET" $test_cmd $test_result ]

# DELETE
set test_cmd "DELETE City\r"
set test_result "OK"
append test_report [ testcase "db_delete" "DELETE" $test_cmd $test_result ]

# GET
set test_cmd "GET City\r"
set test_result ">"
append test_report [ testcase "db_delete" "GET" $test_cmd $test_result ]

testcase_teardown "db_delete"
