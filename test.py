#!/usr/bin/python3

from tests import *

def test_list():
	return [
		codetests,
		proteintests,
		souptests,
		runtests,
		fitnesstests
	]

def main():
	tests = test_list()
	for test in tests:
		if not test():
			print("Test failed: %s" % test)
			exit(1)
	print("Done")

if __name__ == '__main__':
	main()

