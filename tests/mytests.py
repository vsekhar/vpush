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
	protein_size = 1000
	for i in range(protein_count):
		p = vpush.Protein.random(protein_size)
		p.energy = 1000
		vpush.soup.push_back(p)
	assert(len(vpush.soup) == protein_count)
	assert(vpush.soup.deep_size() == protein_count * protein_size)

	return True

def runtests():
	p = vpush.Protein.random(500)
	initial_energy = 100.0
	p.energy = initial_energy
	consumed_energy = vpush.run_protein(p)
	assert(consumed_energy == initial_energy - p.energy)
	
	# populate and run a soup
	return True

def fitnesstests():
	return True
