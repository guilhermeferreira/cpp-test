testcase_setup "config_file" "/tmp/test/integration/database.ini"

# SET the ConfigFile
set test_cmd "SET ConfigFile /tmp/database.cfg\r"
set test_result "OK"
append test_report [ testcase "config_file" "SET" $test_cmd $test_result ]

set test_cmd "GET ConfigFile\r"
set test_result "/tmp/database.cfg"
append test_report [ testcase "config_file" "GET" $test_cmd $test_result ]

testcase_teardown "config_file"
