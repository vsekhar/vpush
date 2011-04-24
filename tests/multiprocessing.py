#!/usr/bin/python3

import sys
import multiprocessing
import random

from . import vpush

def task(index):
	ret = dict()
	ret['index'] = index
	ret['init_size'] = len(vpush.get_soup())
	soup_size = random.randint(0,1000)
	vpush.get_soup().set_size(soup_size, 100, 100)
	ret['final_size'] = len(vpush.get_soup())
	return ret
	

def multiprocessingtests():
	vpush.get_soup().clear()
	pool = multiprocessing.Pool(4)
	results = pool.map_async(func=task, iterable=range(4), chunksize=1)
	pool.close()
	pool.join()
	print(results.get())
	return True

