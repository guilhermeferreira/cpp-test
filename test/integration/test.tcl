#!/usr/bin/env expect
#

set force_conservative 0  ;# set to 1 to force conservative mode even if
                          ;# script wasn't run conservatively originally
if {$force_conservative} {
	# Prevent the script from out-type the input of the program that was
	# designed for human interaction
	set send_slow {1 .1}
	proc send { ignore arg } {
		sleep .1
		exp_send -s -- $arg
	}
}
set argv1 [lindex $argv 0]
set basedir [file dirname $argv0]
set prog_cmd "$argv1"
set shell_prompt "$ "
set prog_prompt "> "
set timeout 5
set test_fail 0

spawn $env(SHELL)
match_max 100000
expect -re $shell_prompt
send -- "cd ../..\r"
expect -re $shell_prompt


############################################################################
# TEST FUNCTIONS
############################################################################

proc test_setup {} {
	# variable works like global. It allows access to global scope
	variable shell_prompt
	variable test_count

	send -- "PS1=\"$shell_prompt\"\r"
	# Disable echo for the send above
	log_user 0

	puts "\n"
	puts "\u001b\[32m\[==========]\u001b\[0m Running test cases."

	set test_count 0
}

proc test_teardown {} {
	# variable works like global. It allows access to global scope
	variable shell_prompt
	variable test_count

	expect -re $shell_prompt
	send -- "exit\r"
	# Disable echo for the send above
	log_user 0
	expect eof

	puts ""
	puts "\u001b\[32m\[==========]\u001b\[0m $test_count test(s) ran."
}


############################################################################
# TEST CASE (INTERACTIVE) FUNCTIONS
############################################################################

#
# Start test fixture
#
proc testcase_setup { classname database } {
	# variable works like global. It allows access to global scope
	variable shell_prompt
	variable prog_cmd
	variable prog_prompt
	variable testcase_count
	variable test_start

	send -- "$prog_cmd --database $database\r"
	# Disable echo for the send above
	log_user 0
	expect -re $prog_prompt

	puts ""
	puts "\u001b\[32m\[----------]\u001b\[0m $classname"

	set test_start [clock clicks -millisec]

	set testcase_count 0
}

#
# End test fixture
#
proc testcase_teardown { classname } {
	# variable works like global. It allows access to global scope
	variable testcase_count
	variable test_start
	variable shell_prompt
	variable prog_cmd
	variable prog_prompt

	set test_end [clock clicks -millisec]
	set elapsed_time [expr $test_end - $test_start ]

	expect -re $prog_prompt
	send -- "exit\r"
	expect {
		-re $shell_prompt {
		}
		timeout {
			expect *
		}
	}

	puts "\u001b\[32m\[----------]\u001b\[0m $testcase_count tests from $classname ($elapsed_time ms total)"
}

#
# Test case
#
proc testcase { classname name command result } {
	# variable works like global. It allows access to global scope
	variable test_count
	variable test_fail
	variable testcase_count
	variable shell_prompt
	variable prog_prompt
	variable prog_cmd
	# variable expect_out

	puts "\u001b\[32m\[ RUN      ]\u001b\[0m $classname.$name"

	incr test_count
	incr testcase_count

	set report ""

	set testcase_start [clock clicks -millisec]

	send -- $command
	# Enable echo for the send above
	log_user 1

	expect {
		-re $result {
			# puts "expect_out(buffer)='$expect_out(buffer)'"
			# puts "expect_out(0,string)='$expect_out(0,string)'"
			# puts "result='$result'"

			set testcase_end [clock clicks -millisec]
			set elapsed_time [expr $testcase_end - $testcase_start ]

			append report "    <testcase name=\"$name\" status=\"run\" time=\"0\" classname=\"$classname\"/>\n"

			puts ""
			puts "\u001b\[32m\[       OK ]\u001b\[0m $classname.$name ($elapsed_time ms)"
		}
		timeout {
			set testcase_end [clock clicks -millisec]
			set elapsed_time [expr $testcase_end - $testcase_start ]

			incr test_fail

			append report "    <testcase name=\"$name\" status=\"run\" time=\"0\" classname=\"$classname\">\n"
			append report "      <failure message=\"Failed $name testcase\"/>\n"
			append report "    </testcase>\n"

			puts ""
			puts "\u001b\[31m\[     FAIL ]\u001b\[0m $classname.$name ($elapsed_time ms)"
		}
	}

	expect {
		-re $prog_prompt {
		}
		timeout {
		}
	}

	# Disable echo for the send above
	log_user 0

	return $report
}


############################################################################
# TEST REPORTING FUNCTIONS
############################################################################

#
# GTest-style test report.
# Useful for command line output.
#
proc testreport_stdout { test_count test_fail test_report } {
	set test_pass [expr $test_count - $test_fail]

	puts "\u001b\[32m\[  PASSED  ]\u001b\[0m $test_pass test(s)."
	if { $test_fail > 0 } {
		puts "\u001b\[31m\[  FAILED  ]\u001b\[0m $test_fail tests."
	}
}

#
# JUnit-style test report.
# Useful for Jenkins
#  Try these libraries
#    https://github.com/Xilinx/XilinxTclStore/tree/master/tclapp/xilinx/junit
#    https://github.com/makr/tclunit
proc testreport_xml { test_count test_fail test_report } {
	#
	# Timestamp
	#
	set systemTime [clock seconds]
	set timestamp [clock format $systemTime -format {%Y-%m-%dT%H:%M:%S}]

	set test_detail [open "test_detail_wht.xml" w]
	puts $test_detail "<\?xml version=\"1.0\" encoding=\"UTF-8\"\?>"
	puts $test_detail "<testsuites tests=\"$test_count\" failures=\"$test_fail\" disabled=\"0\" errors=\"0\" timestamp=\"$timestamp\" time=\"0\" name=\"AllTests\">"
	puts $test_detail "  <testsuite name=\"integration\" tests=\"$test_count\" failures=\"$test_fail\" disabled=\"0\" errors=\"0\" time=\"0\">"
	puts -nonewline $test_detail $test_report
	puts $test_detail "  </testsuite>"
	puts $test_detail "</testsuites>"
	close $test_detail
}


############################################################################
# TEST CASES
############################################################################

set test_report ""

test_setup

# Execute each test case in the current directory
set test_cases [ glob $basedir/*_test.tcl ]
foreach test $test_cases {
	source $test
}

test_teardown


############################################################################
# TEST CASE REPORT
############################################################################

testreport_stdout $test_count $test_fail $test_report

testreport_xml $test_count $test_fail $test_report
