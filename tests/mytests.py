#!/usr/bin/python3

import unittest
from sys import float_info

from . import vpush

class TestCode(unittest.TestCase):
	def test_name_lookup(self):
		name = "ADD.INT"
		c = vpush.functions().get_code(name)
		self.assertEqual(vpush.functions().get_name(c), name)
	
	def test_open_close_codes(self):
		open_code = vpush.Functions.open()
		close_code = vpush.Functions.close()
		self.assertEqual(open_code.type, vpush.OPEN)
		self.assertEqual(close_code.type, vpush.CLOSE)
		
class TestProteins(unittest.TestCase):
	def test_proteins(self):
		p = vpush.Protein()
		p.push_int(47)
		self.assertEqual(p.pop_int(), 47)

class TestSoup(unittest.TestCase):
	def setUp(self):
		self.protein_count = 100
		self.protein_size = 1000
		self.initial_energy = 12
		vpush.get_soup().clear()

	def test_incremental(self):
		size = len(vpush.get_soup())
		p = vpush.Protein.random(500)
		vpush.get_soup().push_back(p)
		self.assertEqual(len(vpush.get_soup()), size + 1)
		vpush.get_soup().clear()
		self.assertEqual(len(vpush.get_soup()), 0)

	def test_manual_insertion(self):
		for i in range(self.protein_count):
			p = vpush.Protein.random(self.protein_size)
			p.energy = self.initial_energy
			vpush.get_soup().push_back(p)
		self.assertEqual(len(vpush.get_soup()), self.protein_count)
		self.assertEqual(vpush.get_soup().deep_count(), self.protein_count * self.protein_size)
		self.assertEqual(vpush.get_soup().energy(), self.protein_count * self.initial_energy)

	def test_bulk_insertion(self):	
		vpush.get_soup().set_size(self.protein_count, self.protein_size, self.initial_energy)
		self.assertEqual(len(vpush.get_soup()), self.protein_count)
		self.assertEqual(vpush.get_soup().deep_count(), self.protein_count * self.protein_size)
		self.assertEqual(vpush.get_soup().energy(), self.protein_count * self.initial_energy)

class RunTests(unittest.TestCase):
	def setUp(self):
		vpush.get_soup().clear()
	
	def test_protein_run(self):
		p = vpush.Protein.random(500)
		initial_energy = 100.0
		p.energy = initial_energy
		consumed_energy = vpush.run_protein(p)
		self.assertEqual(consumed_energy, initial_energy - p.energy)
	
	def test_soup_run(self):
		proteins = 1000
		protein_size = 500
		initial_energy = 100
		vpush.get_soup().set_size(proteins, protein_size, initial_energy)
		consumed_energy = vpush.get_soup().run(trace=False)
		remaining_energy = vpush.get_soup().energy()
		if False:
			print("Initial energy: ", initial_energy*proteins)
			print("Consumed: ", consumed_energy)
			print("Remaining: ", remaining_energy)
			print("Residue: ", abs(remaining_energy + consumed_energy - (initial_energy*proteins)))
		self.assertTrue(abs(remaining_energy + consumed_energy - (initial_energy*proteins)) <= 1e-9)
		if remaining_energy < 0:
			self.assertTrue(abs(remaining_energy / proteins) < 0.01 * initial_energy)

