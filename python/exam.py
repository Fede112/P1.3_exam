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
		self._postcards = []
		with open(file_path) as file: 
			for pc in file:  
				self._postcards.append(pc.rstrip())
		# self._parsePostcards()

	def getNumberOfPostcards(self):
		return len(self._postcards)

	def _parsePostcards(self):
		prev_len = len(self._date) 

		for record, pc in enumerate(self._postcards[prev_len:self.getNumberOfPostcards()],1):
			record += prev_len
			date_key =  pc.split(';')[0].split(':')[1]
			from_key = pc.split(';')[1].split(':')[1]
			to_key = pc.split(';')[2].split(':')[1]

			if date_key not in self._date:
				self._date[ date_key ] = []

			if from_key not in self._from:
				self._from[ from_key ] = []

			if to_key  not in self._to:
				self._to[ to_key ] = []

			self._date[ date_key ].append(record)  
			self._from[ from_key ].append(record)  
			self._to[ to_key  ].append(record)   
			
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

	def getPostcardsBySender(self, sender):
		pcs = []
		if sender in self._from.keys():
			pcs = [pc for i, pc in enumerate(self._postcards, 1) if i in self._from[sender]]
		return pcs	

	def getPostcardsByReceiver(self, receiver):
		pcs = []
		if receiver in self._from.keys():
			pcs = [pc for i, pc in enumerate(self._postcards, 1) if i in self._to[receiver]]
		return pcs	

	def getPostcardsByDateRange(self, date_ini, date_end): # returns the postcards within a date_range
		datesInRange = []
		for key in self._date.keys():
			datetime_key = datetime.datetime.strptime(key, "%Y-%m-%d") 
			if ( datetime_key >= date_ini  and  datetime_key <= date_end ):
				datesInRange += self._date[key]
		pcInRange = [ self._postcards[i-1] for i in datesInRange ]
		print(pcInRange)

if __name__ == '__main__':

	curret_directory = os.getcwd()

	file_name = 'exam_postcard_list4.txt'
	file_path = curret_directory + "/" + file_name

	postL = PostcardList()
	postL.readFile(file_path)

	numberOfPostcards = postL.getNumberOfPostcards()
	postL._parsePostcards()
	# print(postL._postcards)
	postL.writeFile("./test.txt")
	# print(postL._file)
	# print(postL._from)
	# print(postL.getPostcardsBySender("Dewey"))
	# print(postL.getPostcardsByReceiver("Dewey"))
	postL.getPostcardsByDateRange(datetime.datetime.strptime("2010-01-01", "%Y-%m-%d"),datetime.datetime.strptime("2010-10-08", "%Y-%m-%d") )
	# print(postL._file)
	# print(postL._postcards)
	print(numberOfPostcards)
