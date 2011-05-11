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
		
	def test_protein_serialization(self):
		import tempfile
		src = vpush.Protein.random(100)
		tmpfile = tempfile.NamedTemporaryFile(delete=False)
		tmpfile.close()
		src.save(tmpfile.name)
		dst = vpush.Protein()
		dst.load(tmpfile.name)
		self.assertEqual(src.energy, dst.energy)
		self.assertEqual(vpush.run_protein(src), vpush.run_protein(dst))
		self.assertEqual(src.energy, dst.energy)

def test_triangle(initial, consumed, final):
	if abs(final + consumed - initial) > 1e-9:
		print(initial, "-", consumed, "!=", final, "(difference:", initial-consumed-final, ")")
		return False
	overshoot_threshold = 0.01
	if final < 0 and abs(final) > (overshoot_threshold * initial):
		print("Overshoot threshold violated: ", final, " > ", overshoot_threshold, " * ", initial)
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
	
	def test_soup_serialization(self):
		import tempfile
		vpush.get_soup().set_size(100, 100, 100)
		src = vpush.Soup(vpush.get_soup())
		tmpfile = tempfile.NamedTemporaryFile(delete=False)
		tmpfile.close()
		src.save(tmpfile.name)
		dst = vpush.Soup()
		dst.load(tmpfile.name)

		vpush.get_soup().clear()
		vpush.set_soup(src)
		vpush.clear_incubator()
		src_initial_energy = vpush.get_soup().energy()
		src_initial_count = len(vpush.get_soup())
		src_initial_deepcount = vpush.get_soup().deep_count()
		src_consumed_energy = vpush.get_soup().run(trace=False)
		vpush.flush_incubator()
		src_final_energy = vpush.get_soup().energy()
		self.assertTrue(test_triangle(src_initial_energy, src_consumed_energy, src_final_energy))
		
		vpush.get_soup().clear()
		vpush.set_soup(dst)
		vpush.clear_incubator()
		dst_initial_energy = vpush.get_soup().energy()
		dst_initial_count = len(vpush.get_soup())
		dst_initial_deepcount = vpush.get_soup().deep_count()
		dst_consumed_energy = vpush.get_soup().run(trace=False)
		vpush.flush_incubator()
		dst_final_energy = vpush.get_soup().energy()
		self.assertTrue(test_triangle(dst_initial_energy, dst_consumed_energy, dst_final_energy))
		
		self.assertEqual(src_initial_energy, dst_initial_energy)
		self.assertEqual(src_initial_count, dst_initial_count)
		self.assertEqual(src_initial_deepcount, dst_initial_deepcount)

		# Can't do these as there are non-deterministic op-codes
		# self.assertEqual(src_consumed_energy, dst_consumed_energy)
		# self.assertEqual(src_final_energy, dst_final_energy)

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
		vpush.clear_incubator()
		consumed_energy = vpush.run_protein(p)
		vpush.detach_gestator()
		incubator_energy = vpush.incubator_energy()
		self.assertTrue(test_triangle(initial_energy, consumed_energy+incubator_energy, p.energy))
	
	def test_protein_random_run(self):
		p = vpush.Protein.random(500)
		initial_energy = 100.0
		p.energy = initial_energy
		vpush.clear_gestator()
		consumed_energy = vpush.run_protein(p)
		vpush.detach_gestator()
		incubator_energy = vpush.incubator_energy()
		self.assertTrue(test_triangle(initial_energy, consumed_energy + incubator_energy, p.energy))
	
	def test_soup_run(self):
		proteins = 1000
		protein_size = 500
		initial_protein_energy = 100
		vpush.clear_incubator()
		vpush.get_soup().set_size(proteins, protein_size, initial_protein_energy)
		initial_energy = vpush.get_soup().energy()
		consumed_energy = vpush.get_soup().run(trace=False)
		vpush.flush_incubator()
		remaining_energy = vpush.get_soup().energy()
		self.assertTrue(test_triangle(initial_energy, consumed_energy, remaining_energy))

	@staticmethod
	def stdev(sequence):
		if len(sequence) < 1: 
			return None
		else:
			avg = float(sum(sequence) / len(sequence))
			sdsq = sum([(i - avg) ** 2 for i in sequence])
			stdev = (sdsq / (len(sequence) - 1)) ** .5
			return stdev

	def test_gestation(self):
		vpush.get_soup().clear()
		vpush.get_soup().set_size(1000, 500, 100)
		sizes = [len(vpush.get_soup())]
		for _ in range(5):
			vpush.clear_incubator()
			vpush.get_soup().run(trace=False)
			vpush.flush_incubator()
			sizes.append(len(vpush.get_soup()))
		# print("Sizes: ", sizes)
		self.assertTrue(self.stdev(sizes) != 0)

