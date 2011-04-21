#!/usr/bin/python3

from sys import float_info

from . import vpush

def codetests():
	name = "ADD.INT"
	c = vpush.functions().get_code(name)
	assert(vpush.functions().get_name(c) == name)
	
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
	# incremental soup size
	size = len(vpush.get_soup())
	p = vpush.Protein.random(500)
	vpush.get_soup().push_back(p)
	assert(len(vpush.get_soup()) == size + 1)
	vpush.get_soup().clear()
	assert(len(vpush.get_soup()) == 0)

	# manual insertion
	protein_count = 100
	protein_size = 1000
	initial_energy = 12
	for i in range(protein_count):
		p = vpush.Protein.random(protein_size)
		p.energy = initial_energy
		vpush.get_soup().push_back(p)
	assert(len(vpush.get_soup()) == protein_count)
	assert(vpush.get_soup().deep_count() == protein_count * protein_size)
	assert(vpush.get_soup().energy() == protein_count * initial_energy)
	
	# bulk insertion
	vpush.get_soup().clear()
	vpush.get_soup().set_size(protein_count, protein_size, initial_energy)
	assert(len(vpush.get_soup()) == protein_count)
	assert(vpush.get_soup().deep_count() == protein_count * protein_size)
	assert(vpush.get_soup().energy() == protein_count * initial_energy)

	return True

def runtests():
	# protein runs
	p = vpush.Protein.random(500)
	initial_energy = 100.0
	p.energy = initial_energy
	consumed_energy = vpush.run_protein(p)
	assert(consumed_energy == initial_energy - p.energy)
	
	# soup runs
	proteins = 1000
	protein_size = 500
	initial_energy = 100
	vpush.get_soup().clear()
	vpush.get_soup().set_size(proteins, protein_size, initial_energy)
	consumed_energy = vpush.get_soup().run(trace=False)
	remaining_energy = vpush.get_soup().energy()
	if False:
		print("Initial energy: ", initial_energy*proteins)
		print("Consumed: ", consumed_energy)
		print("Remaining: ", remaining_energy)
		print("Residue: ", abs(remaining_energy + consumed_energy - (initial_energy*proteins)))
	assert(abs(remaining_energy + consumed_energy - (initial_energy*proteins)) <= 1e-9)
	if remaining_energy < 0:
		assert(abs(remaining_energy / proteins) < 0.01 * initial_energy)

	return True

def fitnesstests():
	# TODO: add reward mechanism (during soup.run() or after?)
	return True
