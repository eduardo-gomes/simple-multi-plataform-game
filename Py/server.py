#!/usr/bin/env python3
#https://www.websocket.org/echo.html
import asyncio
import websockets
import uuid
import json
from typing import Dict
import sgame

wsConnection = websockets.server.WebSocketServerProtocol
USERS: Dict[wsConnection, str] = dict()

async def sendToAll(toSend):
	if USERS:  # asyncio.wait doesn't accept an empty list
		await asyncio.wait([ws.send(toSend) for ws in USERS])

async def sendAllTo(websocket: wsConnection):
	Jobject = json.dumps(sgame.getAll())
	await websocket.send(Jobject)

async def removeByID(id: str):
	await sendToAll('{{"remove": "{}"}}'.format(id))

async def register(websocket: wsConnection):
	await sendAllTo(websocket)
	userID = uuid.uuid4().hex
	USERS[websocket] = userID
	print("registered  ", userID)
	sgame.players[userID] = sgame.Player(userID)
	await websocket.send('{{"YourID": "{}"}}'.format(userID))
	await notify_move(userID)
async def unregister(websocket: wsConnection):
	removedID = USERS.pop(websocket)
	sgame.players.pop(removedID)
	print("unregistered", removedID)
	await removeByID(removedID)

async def notify_move(id: str):
	if id in sgame.players:
		Obj = sgame.players[id]
	elif id in sgame.fruits:
		Obj = sgame.fruits[id]
	else: return
	newPosObj = sgame.getNewPosObj(Obj)
	await sendToAll(json.dumps(newPosObj))

async def createNewFruit():
	newFruitID = uuid.uuid4().hex
	sgame.newFruit(newFruitID)
	await notify_move(newFruitID)

async def handler(ws: wsConnection, message: str):
	jmsg = json.loads(message)
	if "move" in jmsg:
		print('{} move in direction: {}'.format(USERS[ws], jmsg["move"]))
		player = sgame.players[USERS[ws]]
		player.setPos(sgame.addPos(player.getPos(), sgame.deltaFromDir(jmsg["move"])))
		removedFruits = sgame.collide(player)
		player.score += len(removedFruits)
		await notify_move(USERS[ws])
		for id in removedFruits:
			await removeByID(id)
			sgame.removeFruit(id)
	if "create" in jmsg:
		await createNewFruit()

async def wsmain(websocket: wsConnection, path):
	await register(websocket)
	try:
		async for message in websocket:
			await handler(websocket, message)
	finally:
		await unregister(websocket)
	#here closes the connection

async def foodGenLoop():
	while(1):
		timer = asyncio.sleep(2)
		if sgame.players and (len(sgame.fruits) < 10):
			await createNewFruit()
		await timer

asyncio.get_event_loop().run_until_complete(
    websockets.serve(wsmain, 'localhost', 8765))
asyncio.get_event_loop().run_until_complete(foodGenLoop())
asyncio.get_event_loop().run_forever()