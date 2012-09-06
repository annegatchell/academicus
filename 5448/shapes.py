import random


class shape:
	x,y,z = 0,0,0
	def display(self):
		print 'I am a SHAPE!'
	def getLocation(self):
		return (self.x, self.y, self.z)
	def __init__(self,position):
		x,y,z = position


class square(shape):
	def display(self):
		print 'I and a SQUARE! haha'

class triangle(shape):
	def display(self):
		print 'I am a TRIANGLE! And I am quite acute'

class circle(shape):
	def display(self):
		print 'I am a CIRCLE when I get a-round to it)'

"""
class collection:
	shapes = []
	def display(self):
"""

class database:
	shapes = []

	def get_database(self):
		return self.shapes


	def __init__(self, num):
		random.seed()
		self.shapes = [random.randint(0,2) for a in range(num)]
		for a in range(num):
			x,y,z = random.randint(1,10), random.randint(1,10), random.randint(1,10)
			if self.shapes[a] == 0:
				self.shapes[a] = square((x,y,z))
			elif self.shapes[a] == 1:
				self.shapes[a] = circle((x,y,z))
			else:
				self.shapes[a] = triangle((x,y,z))




def main():
	data = database(20)
	shapes = data.get_database()
	print shapes
	for shape in shapes:
		shape.display()


if __name__ == '__main__':
	main();