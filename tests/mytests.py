#!/usr/bin/python3

import unittest
from sys import float_info, getsizeof

from . import vpush

class TestCode(unittest.TestCase):
	def test_name_lookup(self):
		name = "ADD.INT"
		c1 = vpush.functions().get_code(name)
		c2 = vpush.Code.byName(name)
		self.assertEqual(vpush.functions().get_name(c1), name)
		self.assertEqual(vpush.functions().get_name(c2), name)
	
	def test_open_close_codes(self):
		open_code = vpush.Functions.open()
		close_code = vpush.Functions.close()
		self.assertEqual(open_code.type, vpush.OPEN)
		self.assertEqual(close_code.type, vpush.CLOSE)
		
class TestProteins(unittest.TestCase):
	def test_proteins(self):
		p = vpush.Protein()
		self.assertEqual(len(p), 0)
		self.assertEqual(p.count(), 0)

		overhead = getsizeof(p)
		p.push_int(47)
		self.assertEqual(len(p), getsizeof(int(0)))
		self.assertEqual(p.count(), 1)

		self.assertEqual(p.pop_int(), 47)
		self.assertEqual(len(p), 0)
		self.assertEqual(p.count(), 0)
		
		p = vpush.Protein()
		p.push_code(vpush.functions().open())
		code_csize = len(p)
		codecount = 500
		p = vpush.Protein.random(codecount)
		self.assertEqual(p.count(), codecount)
		self.assertEqual(len(p), codecount * code_csize)
		
	def test_protein_pickling(self):
		import tempfile
		import pickle
		src = vpush.Protein.random(100)
		with tempfile.SpooledTemporaryFile(max_size=1024*1024) as file:
			pickle.dump(src, file)
			dst = pickle.load(file)
		self.assertEqual(src.energy, dst.energy)
		self.assertEqual(vpush.run_protein(src), vpush.run_protein(dst))
		self.assertEqual(src.energy, dst.energy)

def test_triangle(initial, consumed, final):
	if abs(final + consumed - initial) > 1e-9:
		return False
	if final < 0 and abs(final) > (0.01 * initial):
		return False
	return True

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
	
	def test_soup_pickling(self):
		import tempfile
		import pickle
		import copy
		vpush.get_soup().set_size(100, 100, 100)
		src = copy.deepcopy(vpush.get_soup())
		with tempfile.SpooledTemporaryFile(max_size=1024*1024) as file:
			pickle.dump(src, file)
			dst = pickle.load(file)
		vpush.get_soup().clear()
		vpush.set_soup(src)
		src_initial_energy = vpush.get_soup().energy()
		src_consumed_energy = vpush.get_soup().run(trace=False)
		src_final_energy = vpush.get_soup().energy()
		self.assertTrue(test_triangle(self, src_initial_energy, src_consumed_energy, src_final_energy))
		
		vpush.get_soup().clear()
		vpush.set_soup(dst)
		dst_initial_energy = vpush.get_soup().energy()
		dst_consumed_energy = vpush.get_soup().run(trace=False)
		dst_final_energy = vpush.get_soup().energy()
		self.assertTrue(test_triangle(dst_initial_energy, dst_consumed_energy, dst_final_energy))
		
		self.assertEqual(src_initial_energy, dst_initial_energy)
		self.assertEqual(src_consumed_energy, dst_consumed_energy)
		self.assertEqual(src_final_energy, dst_final_energy)

class RunTests(unittest.TestCase):
	def setUp(self):
		vpush.get_soup().clear()
	
	def test_protein_run(self):
		p = vpush.Protein.random(1000)
		p.energy = 2000
		p.push_int(7)
		p.push_int(31)
		p.push_int(4)
		p.push_int(8)
		p.push_code(vpush.functions().close())
		p.push_code(vpush.functions().get_code("RANDOM.CODE"))
		p.push_code(vpush.functions().close())
		p.push_code(vpush.functions().get_code("MAKELIST.CODE"))
		p.push_code(vpush.functions().open())
		p.push_code(vpush.functions().open())
		p.push_exec(vpush.functions().get_code("QUOTE.EXEC"))
		p.push_exec(vpush.functions().get_code("DUP.CODE"))

		initial_energy = p.energy
		consumed_energy = vpush.run_protein(p)
		self.assertEqual(consumed_energy, initial_energy - p.energy)
	
	def test_protein_random_run(self):
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
		self.assertTrue(test_triangle(initial_energy*proteins, consumed_energy, remaining_energy))
