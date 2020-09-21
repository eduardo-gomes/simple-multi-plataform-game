#!/usr/bin/env python
#https://www.websocket.org/echo.html
import asyncio
import websockets
import uuid
import json
from typing import Dict
import sgame

wsConnection = websockets.server.WebSocketServerProtocol
USERS: Dict[wsConnection, str] = dict()

async def register(websocket: wsConnection):
	userID = uuid.uuid4().hex
	USERS[websocket] = userID
	print("registered", userID)
	sgame.players[userID] = sgame.Player(userID)
	await websocket.send('{{"YourID": "{}"}}'.format(userID))
	await notify_move(userID)
async def unregister(websocket: wsConnection):
	removedID = USERS.pop(websocket)
	sgame.players.pop(removedID)
	print("unregistered", removedID)

async def notify_move(id: str):
	pos = sgame.players[id].getPos()
	msg = '{{"newPos": {{"name": "{}", "pos": [{}, {}]}}}}'.format(id, pos[0], pos[1])
	if USERS:  # asyncio.wait doesn't accept an empty list
		await asyncio.wait([ws.send(msg) for ws in USERS])
async def notify_fruit(id: str, create: bool):
	fruit = sgame.Fruit(id)
	fruit.setPos(sgame.genRandPos())
	sgame.fruits[id] = fruit
	pos = fruit.getPos()
	msg = '{{"fruit": {{"name": "{}", "pos": [{}, {}], "create": "{}"}}}}'.format(id, pos[0], pos[1], create)
	if USERS:  # asyncio.wait doesn't accept an empty list
		await asyncio.wait([ws.send(msg) for ws in USERS])
	print("Fruit {}: {}".format(id, create))

async def handler(ws: wsConnection, message: str):
	jmsg = json.loads(message)
	if "move" in jmsg:
		print('{} move in direction: {}'.format(USERS[ws], jmsg["move"]))
		player = sgame.players[USERS[ws]]
		player.setPos(sgame.addPos(player.getPos(), sgame.deltaFromDir(jmsg["move"])))
		await notify_move(USERS[ws])
		removedFruits = sgame.collide(player)
		for id in removedFruits:
			await notify_fruit(id, 0)
			sgame.removeFruit(id)
	if "create" in jmsg:
		newFruitID = uuid.uuid4().hex
		sgame.newFruit(newFruitID)
		await notify_fruit(newFruitID, 1)

async def wsmain(websocket: wsConnection, path):
	await register(websocket)
	try:
		async for message in websocket:
			await handler(websocket, message)
	finally:
		await unregister(websocket)
	#here closes the connection

asyncio.get_event_loop().run_until_complete(
    websockets.serve(wsmain, 'localhost', 8765))
asyncio.get_event_loop().run_forever()