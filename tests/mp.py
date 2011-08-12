#!/usr/bin/python

import sys
import multiprocessing
from Queue import Empty as QueueEmpty
import random
import unittest
import time

from . import vpush

#
# Remote functions
#
def task():
	ret = dict()
	ret['init_size'] = len(vpush.get_soup())
	soup_size = random.randint(0,1000)
	vpush.get_soup().set_size(soup_size, 100, 100)
	ret['final_size'] = len(vpush.get_soup())
	return ret

def soup_size():
	return {'soup_size': len(vpush.get_soup())}

functions = {'task':task,
			 'soup_size': soup_size}

#
# Remote run loop
#
def run_loop(tasks=None, results=None):
	if tasks is None:
		return
	while(True):
		try:
			func_name, args, kwargs = tasks.get_nowait()
			if func_name is None: # stop sentinel
				break
			if args is None:
				args = []
			if kwargs is None:
				kwargs = dict()
			result = functions[func_name](*args, **kwargs)
			results.put(result)
		except QueueEmpty:
			pass

		# advance your computation
		time.sleep(0.5)

#
# Local Symmetric Pool manager
#		
class SymmetricPool:
	def __init__(self, count):
		self._results = multiprocessing.Queue()
		self._processes = list()
		for _ in range(count):
			q = multiprocessing.Queue()
			p = multiprocessing.Process(target=run_loop, kwargs={'tasks':q, 'results':self._results})
			self._processes.append((p, q))
			p.start()

	def send_all(self, func_name, args=None, kwargs=None):
		for _, queue in self._processes:
			queue.put((func_name, args, kwargs))

	def get_all_results(self):
		results = list()
		for _ in range(len(self._processes)):
			results.append(self._results.get())
		return results

	def do_all(self, func_name, args=None, kwargs=None):
		self.send_all(func_name, args, kwargs)
		return self.get_all_results()
	
	def end(self):
		self.send_all(None) # stop sentinel
		for _, queue in self._processes:
			queue.close()
		for process, _ in self._processes:
			process.join()

#
# Tests
#
class TestMultiprocessing(unittest.TestCase):
	def setUp(self):
		vpush.get_soup().clear()
		self._processes = SymmetricPool(4)
	
	def test_multiprocessing(self):
		# print(self._processes.do_all('task'))
		# print(self._processes.do_all('soup_size'))
		pass

	def tearDown(self):
		self._processes.end()

