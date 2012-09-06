#!/usr/bin/python

# Anne Gatchell
# Homework 1
# CSCI 5448
# Problem 4

"""
The main method in this code generates a database with a collection of
20 shapes. It then displays the shapes and their current positions
before sorting the shapes by their z position and re-displaying.

Run with ./shapes.py 
or
python shapes.py
"""

import random


class shape:
	x,y,z = 0,0,0
	def display(self):
		print 'I am a SHAPE!'
	def getLocation(self):
		return (self.x, self.y, self.z)
	def __init__(self,xnew,ynew,znew):
		self.x,self.y,self.z = xnew,ynew,znew

class square(shape):
	def display(self):
		print 'I am a SQUARE! haha                    ',self.x,self.y,self.z


class triangle(shape):
	def display(self):
		print 'I am a TRIANGLE! And I am quite acute  ',self.x,self.y,self.z

class circle(shape):
	def display(self):
		print 'I am a CIRCLE when I get a-round to it)',self.x,self.y,self.z


class collection:
	shapes = []
	def display(self):
		for shape in self.shapes:
			shape.display()

	def sort(self):
		self.shapes.sort(key = lambda a:a.z)
		#sorted(L, key=operator.itemgetter('resultType'))
		#somelist.sort(key = lambda x: x.resultType)
	def getCollection(self):
		return self.shapes

	def __init__(self, num):
		random.seed()
		self.shapes = [random.randint(0,2) for a in range(num)]
		for a in range(num):
			x,y = random.randint(1,10), random.randint(1,10)
			z = random.randint(1,10)
			if self.shapes[a] == 0:
				self.shapes[a] = square(x,y,z)
			elif self.shapes[a] == 1:
				self.shapes[a] = circle(x,y,z)
			else:
				self.shapes[a] = triangle(x,y,z)


class database:
	coll = []

	def getCollection(self):
		return self.coll

	def __init__(self,num):
		self.coll = collection(num)
		
def main():
	db = database(20)
	data = db.getCollection()
	print "\nThe shapes before sorting (shape, x, y, z):\n"
	data.display()
	data.sort()
	print "\nThe shapes after sorting by z (shape, x, y, z):\n"
	data.display()
	

if __name__ == '__main__':
	main();