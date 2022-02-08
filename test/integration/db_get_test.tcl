testcase_setup "db_get" "/tmp/test/integration/database.db"

# SET
set test_cmd "SET DeviceName TestDevice_123\r"
set test_result "OK"
append test_report [ testcase "db_get" "SET" $test_cmd $test_result ]

# GET
set test_cmd "GET DeviceName\r"
set test_result "TestDevice_123"
append test_report [ testcase "db_get" "GET" $test_cmd $test_result ]

# DELETE
set test_cmd "DELETE DeviceName\r"
set test_result "OK"
append test_report [ testcase "db_get" "DELETE" $test_cmd $test_result ]

# GET
set test_cmd "GET DeviceName\r"
set test_result ">"
append test_report [ testcase "db_get" "GET empty" $test_cmd $test_result ]

# GET Many enters
set test_cmd "GET DeviceName\r\r\r"
set test_result ">"
append test_report [ testcase "db_get" "GET Many enters" $test_cmd $test_result ]

testcase_teardown "db_get"
