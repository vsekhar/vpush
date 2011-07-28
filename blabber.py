#!/usr/bin/python3

import tests.vpush as vpush
import random
import copy

def main():
	l = []
	for _ in range(5):
		l.append(vpush.Blabber(random.randint(1,100)))
	for p in l:
		print(p.value)
	print(l)
	print(vpush.top_blabber(l))
	input()

if __name__ == '__main__':
	main()

