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
			for pc in file:  
				self._postcards.append(pc)

	def getNumberOfPostcards(self):
		return len(self._postcards)

	def _parsePostcards(self):
		for record, pc in enumerate(self._postcards):
			# print(pc.split(';')[0].split(':')[1])
			self._date[ pc.split(';')[0].split(':')[1] ] = record
			self._from[ pc.split(';')[1].split(':')[1] ] = record
			self._to[ pc.split(';')[2].split(':')[1] ] = record




if __name__ == '__main__':

	curret_directory = os.getcwd()

	file_name = 'exam_postcard_list0.txt'
	file_path = curret_directory + "/" + file_name

	postL = PostcardList()
	postL.readFile(file_path)

	numberOfPostcards = postL.getNumberOfPostcards()

	# print(postL._postcards)
	postL._parsePostcards()
	print(postL._date)
	print(numberOfPostcards)