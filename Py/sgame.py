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

class Positionable:
	typeNum = 0
	def __init__(self, uuid):
		self.uuid = uuid
		self.pos = genRandPos()
	def getPos(self):
		return self.pos
	def setPos(self, pos):
		self.pos = pos
class Player(Positionable):
	def __init__(self, uuid):
		super().__init__(uuid)
		self.typeNum = 1
		self.score = 0
	def incrementScore(self):
		self.score += 1
class Fruit(Positionable):
	def __init__(self, uuid):
		super().__init__(uuid)
		self.typeNum = 2

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

def getNewPosObj(fromObj: Positionable):
	newPosObj = {"newPos": {"name": fromObj.uuid, "pos": fromObj.getPos(), "type": fromObj.typeNum}} #'{{"newPos": {{"name": "{}", "pos": [{}, {}]}}}}'.format(id, pos[0], pos[1])
	if isinstance(fromObj, Player):
		newPosObj["newPos"]["score"] = fromObj.score
	return newPosObj

def getAll():
	outObj = list()
	for player in players.values():
		outObj.append(getNewPosObj(player))
	for fruit in fruits.values():
		outObj.append(getNewPosObj(fruit))
	return outObj


players: Dict[str, Player] = dict()
fruits: Dict[str, Fruit] = dict()