testcase_setup "ini_special" "/tmp/test/integration/database.ini"

# SET Dollar
set test_cmd "SET Special Dollar\$\$\$\$\r"
set test_result "OK"
append test_report [ testcase "ini_special" "SET Dollar" $test_cmd $test_result ]

# GET Dollar
set test_cmd "GET Special\r"
set test_result "Dollar\$\$\$\$"
append test_report [ testcase "ini_special" "GET Dollar" $test_cmd $test_result ]

# SET Percent
set test_cmd "SET Special Percent%\r"
set test_result "OK"
append test_report [ testcase "ini_special" "SET Percent" $test_cmd $test_result ]

# GET Percent
set test_cmd "GET Special\r"
set test_result "Percent%"
append test_report [ testcase "ini_special" "GET Percent" $test_cmd $test_result ]

# SET Asterisk
set test_cmd "SET Special Asterisk*\r"
set test_result "OK"
append test_report [ testcase "ini_special" "SET Asterisk" $test_cmd $test_result ]

# GET Asterisk
set test_cmd "GET Special\r"
set test_result "Asterisk*"
append test_report [ testcase "ini_special" "GET Asterisk" $test_cmd $test_result ]

testcase_teardown "ini_special"
