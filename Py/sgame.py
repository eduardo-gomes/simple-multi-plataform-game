from typing import Dict
import random
gameSize = (20, 10)

def deltaFromDir(dir: str):
	if(dir == "w"):
		return (0, -1)
	elif(dir == "a"):
		return (-1, 0)
	elif(dir == "s"):
		return (0, 1)
	elif(dir == "d"):
		return (1, 0)
	else:
		return (0, 0)

def addPos(p1, p2):
	return (p1[0] + p2[0], p1[1] + p2[1])

class Player:
	def __init__(self, uuid):
		self.uuid = uuid
		self.pos = genRandPos()
	def getPos(self):
		return self.pos
	def setPos(self, pos):
		self.pos = pos
Fruit = Player

def genRandPos():
	return (random.randrange(0, gameSize[0]), random.randrange(0, gameSize[1]))

def collide(player: Player):
	removed = set()
	for fruit in fruits.values():
		if fruit.getPos() == player.getPos():
			removed.add(fruit.uuid)
	return removed



def newFruit(id: str):
	fruit = Fruit(id)
	fruit.setPos(genRandPos())
	fruits[id] = fruit
	return fruit

def removeFruit(id: str):
	removedFruit = fruits.pop(id)
	return removedFruit




players: Dict[str, Player] = dict()
fruits: Dict[str, Fruit] = dict()