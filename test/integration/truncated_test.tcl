#
# Test the deletion of the storage file
#

testcase_setup "truncated" "/tmp/truncated.ini"

# SET 1st key,value
set test_cmd "SET key1 value1\r"
set test_result "OK\r\n> $"
append test_report [ testcase "truncated" "SET_key1" $test_cmd $test_result ]

# SET 2nd key,value
set test_cmd "SET key2 value2\r"
set test_result "OK\r\n> $"
append test_report [ testcase "truncated" "SET_key2" $test_cmd $test_result ]

# SET 3rd key,value
set test_cmd "SET key3 value3\r"
set test_result "OK\r\n> $"
append test_report [ testcase "truncated" "SET_key3" $test_cmd $test_result ]

# save the file
set test_cmd "store\r"
set test_result "OK\r\n> $"
append test_report [ testcase "truncated" "store" $test_cmd $test_result ]

# DELETE 2nd key
set test_cmd "DELETE key2\r"
set test_result "OK\r\n> $"
append test_report [ testcase "truncated" "DELETE_key2" $test_cmd $test_result ]

# DELETE 3rd key
set test_cmd "DELETE key3\r"
set test_result "OK\r\n> $"
append test_report [ testcase "truncated" "DELETE_key3" $test_cmd $test_result ]

testcase_teardown "truncated"

#
# Reopen the file to see if the deleted data has gone
#

testcase_setup "truncated" "/tmp/truncated.ini"

# GET an existing key
set test_cmd "GET key1\r"
set test_result "value1\r\n> $"
append test_report [ testcase "truncated" "GET_key1" $test_cmd $test_result ]

# GET a non-existing key
set test_cmd "GET key2\r"
set test_result "> \r\n> $"
append test_report [ testcase "truncated" "GET_key2" $test_cmd $test_result ]

# GET a non-existing key
# BUG: because of the delete sequence, the 
set test_cmd "GET key3\r"
set test_result "> \r\n> $"
append test_report [ testcase "truncated" "GET_key3" $test_cmd $test_result ]

testcase_teardown "truncated"
