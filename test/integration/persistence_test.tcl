#
# Test the creation of a new file
#

testcase_setup "persistence" "/tmp/newfile.ini"

# GET a non-existing key
set test_cmd "GET NewKey\r"
set test_result "> "
append test_report [ testcase "persistence" "GET" $test_cmd $test_result ]

# SET a new key,value
set test_cmd "SET NewKey AnewValue\r"
set test_result "OK"
append test_report [ testcase "persistence" "SET" $test_cmd $test_result ]

# GET the now-existing key
set test_cmd "GET NewKey\r"
set test_result "AnewValue"
append test_report [ testcase "persistence" "GET" $test_cmd $test_result ]

testcase_teardown "persistence"

#
# Reopen the file to see if data has persisted
#

testcase_setup "persistence" "/tmp/newfile.ini"

# GET the now-existing-and-persistent key
set test_cmd "GET NewKey\r"
set test_result "AnewValue"
append test_report [ testcase "persistence" "GET" $test_cmd $test_result ]

testcase_teardown "persistence"
