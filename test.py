#!/usr/bin/python3

import os
import sys

sys.path.append(os.path.join(os.path.dirname(__file__), 'Release'))

import vpush

def main():
	c = vpush.functions.get_code("ADD.INT")
	print(c)
	print("Done")

if __name__ == '__main__':
	main()

