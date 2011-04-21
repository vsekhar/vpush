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
	consumed_energy = vpush.get_soup().run()
	remaining_energy = vpush.get_soup().energy()
	print("Initial energy: %f" % (initial_energy * proteins))
	print("Consumed: %f" % consumed_energy)
	print("Remaining: %f" % remaining_energy)
	assert(abs((initial_energy*proteins) - consumed_energy - remaining_energy) <= float_info.epsilon)

	return True

def fitnesstests():
	return True
