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
		# self._date = {"2002":1, "2003":2, "2004":4}
		# self._from = {"A":1, "B":2, "C":4}
		# self._to = {"D":1, "E":2, "F":4}

	def readFile(self, file_path):
		self._file = file_path
		with open(file_path) as file: 
			for pc in file:  
				self._postcards.append(pc.rstrip())

	def getNumberOfPostcards(self):
		return len(self._postcards)

	def _parsePostcards(self):
		for record, pc in enumerate(self._postcards[len(self._date):self.getNumberOfPostcards()]):
			# print(pc.split(';')[0].split(':')[1])
			record += len(self._date)
			# key_date = pc.split(';')[0].split(':')[1]
			# key_from = pc.split(';')[1].split(':')[1]
			# key_to = pc.split(';')[2].split(':')[1]
			# if(self_date.get(key_date, 0)):
			# 	self._date[ pc.split(';')[0].split(':')[1] ] = record
			# self._date[ pc.split(';')[0].split(':')[1] ].append(record)		
			# self._from[ pc.split(';')[1].split(':')[1] ].append(record)
			# self._to[ pc.split(';')[2].split(':')[1] ].append(record)
			self._date[ pc.split(';')[0].split(':')[1] ] = (record)		
			self._from[ pc.split(';')[1].split(':')[1] ] = (record)
			self._to[ pc.split(';')[2].split(':')[1] ] = (record)
		
	def writeFile(self, file_path):
		'''
		def writeFile(self):
			with open(self._file, 'w') as file:
		'''
		with open(file_path, 'w') as file:
			iterable = zip(self._date.keys(), self._from.keys(), self._to.keys())
			for date, fromm, to in iterable:
				line = "date:{d}; from:{f}; to:{t};\n".format(\
							d=date, f=fromm, t=to)
				file.write(line)

	def getPostcardsByDateRange(self): # returns the postcards within a date_range
		print(sorted(self._date))

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
	postL.writeFile("./test.txt")
	postL.getPostcardsByDateRange()
	print(postL._file)
	print(postL._postcards)
	print(numberOfPostcards)