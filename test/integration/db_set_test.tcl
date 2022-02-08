testcase_setup "db_set" "/tmp/test/integration/database.db"

# SET
set test_cmd "SET key1 value1\r"
set test_result "OK"
append test_report [ testcase "db_set" "SET" $test_cmd $test_result ]

# GET
set test_cmd "GET key1\r"
set test_result "KeyAsService"
append test_report [ testcase "db_set" "GET" $test_cmd $test_result ]

testcase_teardown "db_set"
