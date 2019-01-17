#!/usr/bin/python

import os
import datetime

class PostcardList():
  
	def __init__(self):
		self._file = None
		self._postcards = [] 
		self._date = {"2002":1, "2003":2, "2004":4}
		self._from = {"A":1, "B":2, "C":4}
		self._to = {"D":1, "E":2, "F":4}

	def readFile(self, file_path):
		self._file = file_path
		with open(file_path) as file: 
			for postcard in file:  
				self._postcards.append(postcard)

	def getNumberOfPostcards(self):
		return len(self._postcards)

		
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



if __name__ == '__main__':

	curret_directory = os.getcwd()

	file_name = 'exam_postcard_list0.txt'
	file_path = curret_directory + "/" + file_name

	postL = PostcardList()
	postL.readFile(file_path)

	numberOfPostcards = postL.getNumberOfPostcards()

	postL.writeFile("./test.txt")
	print(postL._file)
	print(postL._postcards)
	print(numberOfPostcards)