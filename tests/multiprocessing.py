#!/usr/bin/python3

import sys
import multiprocessing
import random
import unittest

from . import vpush

def task():
	ret = dict()
	ret['init_size'] = len(vpush.get_soup())
	soup_size = random.randint(0,1000)
	vpush.get_soup().set_size(soup_size, 100, 100)
	ret['final_size'] = len(vpush.get_soup())
	print(ret)

def end():
	sys.exit(0)

def run_loop(queue=None):
	if queue is None:
		return
	while(True):
		func, args, kwargs = queue.get()
		if args is None:
			args = []
		if kwargs is None:
			kwargs = dict()
		func(*args, **kwargs)

class SymmetricPool:
	def __init__(self, count=1):
		self._processes = []
		for _ in range(count):
			q = multiprocessing.Queue()
			p = multiprocessing.Process(target=run_loop, kwargs={'queue':q})
			p.start()
			self._processes.append((p, q))


	def do_all(self, func, args=None, kwargs=None):
		for process, queue in self._processes:
			queue.put((func, args, kwargs))
	
	def end(self):
		self.do_all(end)
		for process, queue in self._processes:
			queue.close()
			process.join()

class TestMultiprocessing(unittest.TestCase):
	def setUp(self):
		vpush.get_soup().clear()
	
	def test_multiprocessing(self):
		processes = SymmetricPool(4)
		processes.do_all(task)
		processes.end()
		
		# see if we successfully join
		self.assertTrue(True)

