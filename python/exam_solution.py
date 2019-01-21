#!/usr/bin/python


import os
import datetime

## Documentation for PostcardList class.
#
#	PostcardList() class handles the data stored in .txt files.
#	An object of this class is linked directly to an unique exam_postcard_list?.txt file.
#	It can read its content, update it if needed and write its information into another file.
class PostcardList():
 	
 	## The constructor
	def __init__(self):
		self._file = None
		self._postcards = [] 
		self._date = {}
		self._from = {}
		self._to = {}

	## readFile to initialize object
	# This function initialize the PostcardList with _file = <from_file_path>
	# It is meant to use if you want to initialize the object from a file.
	#  @param self The object pointer.
	#  @param from_file_path Path to the file that is going to be associated with the instance of the class.		
	def readFile(self, from_file_path): 
		self._file = from_file_path
		self._postcards = []
		with open(from_file_path) as file: 
			for pc in file:  
				self._postcards.append(pc)
		self.parsePostcards()

	## Updates PostcardList
	# This function updates the list of postcards (_postcards) from a given file: <from_file_path>
	# The <from_file_path> should not be the same file as the one associated to the object (_file).
	# It also automatically updates the _file associated with the instance of PostcardList.
	#  @param self The object pointer.
	#  @param to_file_path Path to the file from were to read the postcard list.		
	def updateLists(self, from_file_path):
		pc_list = []
		with open(from_file_path) as file: 
			for pc in file:  
				self._postcards.append(pc)
				pc_list.append(pc)
		self.parsePostcards()
		# print(pc_list)
		self.updateFile(pc_list)

	## Updates the file associated with PostcardLists
	# Meant to be a private function called by updateLists(). Do not run by it self!!
	def updateFile(self, pc_list):
		with open(self._file, 'a') as file:
			for pc in pc_list:
				file.write(pc)

	## Writes postcardList into a new file
	#  @param self The object pointer.
	#  @param to_file_path Path to the file you want to write the postcard list to.		
	def writeFile(self, to_file_path):
		with open(to_file_path, 'w') as file:
			for pc in self._postcards:
				file.write(pc)

	## Parse the elements stored in _postcards
	# Based on that information it creates three dictionaries to store the data: _date, _from, _to.
	def parsePostcards(self):
		prev_len = len(self._date) 
		for record, pc in enumerate(self._postcards[prev_len:self.getNumberOfPostcards()],0):
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

	## Returns Postcards sent by a specific name given as an argument of the function (e.g. 'Batman')
	#  @param self The object pointer.
	#  @param sender Sender name as a string
	def getPostcardsBySender(self, sender):
		pcs = []
		if sender in self._from.keys():
			pcs = [pc for i, pc in enumerate(self._postcards, 0) if i in self._from.get(sender, [-1])]
		return pcs	

	## Returns Postcards recieved by a specific name given as an argument of the function (e.g. 'Batman')
	#  @param self The object pointer.
	#  @param sender Receiver name as a string
	def getPostcardsByReceiver(self, receiver):
		pcs = []
		if receiver in self._to.keys():
			pcs = [pc for i, pc in enumerate(self._postcards, 0) if i in self._to.get(receiver, [-1])]
		return pcs	

	## Returns list of Postcards sent in a date range.
	#  @param self The object pointer.
	#  @param date_range Date range as a tuple of datetime (start,end)
	def getPostcardsByDateRange(self, date_range): # returns the postcards within a date_range
		datesInRange = []
		date_ini = date_range[0]
		date_end = date_range[1]
		for key in self._date.keys():
			datetime_key = datetime.datetime.strptime(key, "%Y-%m-%d") 
			if ( datetime_key >= date_ini  and  datetime_key <= date_end ):
				datesInRange += self._date[key]
		pcInRange = [ self._postcards[i] for i in datesInRange ]
		return pcInRange

	## Returns the number of postcards.
	#  @param self The object pointer.
	def getNumberOfPostcards(self):
		return len(self._postcards)

	## @var _file
	#  Name of file asociated with the PostcardList instance.
	## @var _postcards
	#  Full list of postcards.
	## @var _date
	#  Dictionary with key: dates values: list of postcards associated to that date.
	## @var _from
	#  Dictionary with key: <sender name> values: list of postcards associated to that sender.
	## @var _to
	#  Dictionary with key: <receiver name> values: list of postcards associated to that receiver.



if __name__ == '__main__':

	curret_directory = os.getcwd()

	# file_name = 'exam_postcard_list0.txt'

	file_name = 'test0.txt'
	other_file_name = 'test1.txt'
	new_file_name = 'test2.txt'

	file_path = curret_directory + "/" + file_name
	other_file_path = curret_directory + "/" + other_file_name
	new_file_path = curret_directory + "/" + new_file_name

	postL = PostcardList()

	postL.readFile(file_path)
	print("antes \n", postL._from)
	postL.updateLists(other_file_path)
	print("despues \n", postL._from)
	postL.writeFile(new_file_path)
