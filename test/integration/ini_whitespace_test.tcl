testcase_setup "ini_whitespace" "/tmp/test/integration/database.ini"

# SET
set test_cmd "SET key1 'value 1'\r"
set test_result "OK"
append test_report [ testcase "ini_whitespace" "SET" $test_cmd $test_result ]

# GET
set test_cmd "GET key1\r"
set test_result "'value 1'"
append test_report [ testcase "ini_whitespace" "GET" $test_cmd $test_result ]

testcase_teardown "ini_whitespace"
