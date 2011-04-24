#!/usr/bin/python3

import traceback
from sys import exit
from tests import *

def test_list():
	return [
		codetests,
		proteintests,
		souptests,
		runtests,
		fitnesstests,
		multiprocessingtests
	]

def main():
	tests = test_list()
	for test in tests:
		try:
			if not test():
				print("Test failed: %s" % test)
				exit(1)
		except:
			traceback.print_exc()
			exit(1)

	print("All passed")
	exit(0)

if __name__ == '__main__':
	main()

