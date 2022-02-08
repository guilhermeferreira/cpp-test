testcase_setup "invalid" "/tmp/test/integration/database.ini"

# BET
set test_cmd "BET Key Arg\$\$\$\$\r"
set test_result ">"
append test_report [ testcase "invalid" "BET" $test_cmd $test_result ]

# FET
set test_cmd "FET Key\r"
set test_result ">"
append test_report [ testcase "invalid" "FET" $test_cmd $test_result ]

# SELECT
set test_cmd "SELECT Key Arg\r"
set test_result ">"
append test_report [ testcase "invalid" "SELECT" $test_cmd $test_result ]

# GOLET
set test_cmd "GOLET Key\r"
set test_result ">"
append test_report [ testcase "invalid" "GOLET" $test_cmd $test_result ]

# MARKET
set test_cmd "MARKET Key Arg*\r"
set test_result ">"
append test_report [ testcase "invalid" "MARKET" $test_cmd $test_result ]

# PISTOLET
set test_cmd "PISTOLET Key\r"
set test_result ">"
append test_report [ testcase "invalid" "PISTOLET" $test_cmd $test_result ]

testcase_teardown "invalid"
