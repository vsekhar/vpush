#!/usr/bin/python3

from . import vpush

def codetests():
	c = vpush.functions.get_code("ADD.INT")
	print(c)
	return True

def proteintests():
	p = vpush.Protein.random(500)
	assert(vpush.run_protein(p, False) == 0)
	return True

def souptests():
	size = len(vpush.soup)
	p = vpush.Protein.random(500)
	vpush.soup.push_back(p)
	assert(len(vpush.soup) == size + 1)
	return True

def runtests():
	return True

def fitnesstests():
	return True
