#!/usr/bin/python3

import tests

def main():
	tests.codetests()
	tests.proteintests()
	tests.souptests()
	tests.runtests()
	tests.fitnesstests()
	print("Done")

if __name__ == '__main__':
	main()

