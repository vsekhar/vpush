#!/usr/bin/python3

from . import vpush

def codetests():
	name = "ADD.INT"
	c = vpush.functions.get_code(name)
	assert(vpush.functions.get_name(c) == name)
	
	open_code = vpush.Functions.open()
	close_code = vpush.Functions.close()
	assert(open_code.type == vpush.OPEN)
	assert(close_code.type == vpush.CLOSE)
	return True

def proteintests():
	p = vpush.Protein()
	p.push_int(47)
	assert(p.pop_int() == 47)

	return True

def souptests():
	size = len(vpush.soup)
	p = vpush.Protein.random(500)
	vpush.soup.push_back(p)
	assert(len(vpush.soup) == size + 1)

	vpush.soup.set_size(1000)
	assert(len(vpush.soup) == 1000)
	
	vpush.soup.clear()
	assert(len(vpush.soup) == 0)

	protein_count = 100
	for i in range(protein_count):
		p = vpush.Protein.random(1000)
		p.energy = 1000
		vpush.soup.push_back(p)
	assert(len(vpush.soup) == protein_count)

	return True

def runtests():
	p = vpush.Protein.random(500)
	initial_energy = 100
	p.energy = initial_energy
	assert(vpush.run_protein(p) == 0)
	assert(p.energy < initial_energy)
	
	# populate and run a soup
	return True

def fitnesstests():
	return True
