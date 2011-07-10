#!/usr/bin/python

import os
import sys
import time

write = sys.stdout.write

base_path = os.getcwd()
original_path = base_path
ret_code = 0
tests_passed = True
tests_run = False

fail_list = {}

print 'Using path [' + base_path + '] for running base.'

if (os.path.isdir(base_path) == False):
	print base_path + ' is not a valid directory!'
	exit()

os.putenv("RUN_QUIET", "1")

test_count = 0
world_start = time.time()
for dir in os.listdir(base_path):
	if os.path.isdir(dir) == True:
		os.putenv("CXX_WORLD", dir)
		if dir != 'template' and os.path.isfile(dir + '/cxxtest_make') == True:
			test_count = test_count+1
			suite_start = time.time()
			write('[' + dir + ']\n')
			ret_code = os.system('cd ' + base_path + '/' + dir + ' && ' + './cxxtest_make')
			os.system('cd ' + original_path)
			tests_run = True
			if ret_code != 0:
				fail_list[dir] = str(ret_code);
				sys.stderr.write('TEST ' + dir + ' FAILED...RETURNED [' + str(ret_code) + ']\n')
				tests_passed = False
			suite_end = time.time()
			print "[%d:%s] Took %0.1f seconds\n"%(test_count,dir,suite_end-suite_start)

world_stop = time.time()

if tests_run == True:
	if tests_passed == True:
		write('All tests passed!!!\n')
		ret_code = 0
	else:
		write('Tests failed!!!\n')
		ret_code = 1
else:
	write('No tests to run?!\n')

print "[%d] tests took [%0.1f] seconds"%(test_count,world_stop-world_start)

if len(fail_list) > 0:
	print "Failed Tests:"
	for test in fail_list:
		print 'Test [' + test + '] with code [' + fail_list[test] + ']'

sys.exit(ret_code)

