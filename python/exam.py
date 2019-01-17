#!/usr/bin/python

import os
import datetime

class PostcardList():
  
	def __init__(self):
		self._file = None
		self._postcards = [] 
		self._date = {}
		self._from = {}
		self._to = {}

	def readFile(self, file_path):
		self._file = file_path
		with open(file_path) as file: 
			for postcard in file:  
				self._postcards.append(postcard)




if __name__ == '__main__':

	curret_directory = os.getcwd()

	file_name = 'exam_postcard_list0.txt'
	file_path = curret_directory + "/" + file_name

	postL = PostcardList()
	postL.readFile(file_path)

	print(postL._file, postL._postcards)