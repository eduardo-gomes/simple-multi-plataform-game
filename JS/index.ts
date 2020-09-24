interface position {
	x: number, y: number;
}
interface newPos {
	name: string, pos: [number, number], type: number;
	score?: number;
}

class Screen2D {
	context: CanvasRenderingContext2D;
	canvas: HTMLCanvasElement;
	width: number;
	height: number;
	constructor(canvas: HTMLCanvasElement) {
		this.canvas = canvas;
		this.width = canvas.width;
		this.height = canvas.height;
		this.context = canvas.getContext("2d") as CanvasRenderingContext2D;
	}
	clear() {
		this.context.fillStyle = "white";
		this.context.fillRect(0, 0, this.width, this.height);
	}
	drawnPixel(pos: position, color: string) {
		this.context.fillStyle = color;
		this.context.fillRect(pos.x, pos.y, 1, 1);
	}
}

class ScoreBoard {
	scoreboard: HTMLDivElement;
	constructor(scoreboard: HTMLDivElement) {
		this.scoreboard = scoreboard;
	}
	createNewScoreElement(id: string) {
		const scoreContainer = document.createElement("div");
		const scorePlayerID = document.createElement("span");
		const scorePlayerScore = document.createElement("span");
		scoreContainer.id = id;
		scorePlayerID.className = "id";
		scorePlayerID.textContent = id;
		scorePlayerScore.className = "score";
		scoreContainer.appendChild(scorePlayerID);
		scoreContainer.appendChild(scorePlayerScore);
		this.scoreboard.appendChild(scoreContainer);
		return scoreContainer;
	}
	getScoreElementByID(id: string) {
		let scoreDiv = document.getElementById(id) as HTMLDivElement | null;
		if (scoreDiv === null) {
			scoreDiv = this.createNewScoreElement(id);
		}
		const scoreSpan = scoreDiv.getElementsByClassName("score").item(0) as HTMLSpanElement | null;
		if (scoreSpan === null) {
			throw new Error(`Cant find score span for ID: ${id}`);
		}
		return scoreSpan;
	}
	setScore(id: string, score: number) {
		const scoreSpan = this.getScoreElementByID(id);
		scoreSpan.textContent = String(score);
	}
}
const scoreBoard = new ScoreBoard(document.getElementById("scoresContainer") as HTMLDivElement);

abstract class Drawnable {
	private _id: string;
	private _pos: position;
	private color: string;
	protected constructor(x = 0, y = 0, id: string, color = "black") {
		this._pos = { x, y };
		this._id = id;
		this.color = color;
	}
	drawn(ctx: Screen2D) {
		ctx.drawnPixel(this._pos, this.color);
	}
	get pos(): position {
		return this._pos;
	}
	set pos(newPos: position) {
		this._pos = newPos;
	}
	public getid(): string {
		return this._id;
	}
}

class Player extends Drawnable {
	_points: number;
	constructor(id: string) {
		const playerColor = "blue";
		super(-1, -1, id, playerColor);
		this._points = 0;
	}
	set points(to: number) {
		this._points = to;
	}
	get points() {
		return this._points;
	}
	moveDelta(delta: position) {
		const oldPos = this.pos;
		const newPos = posAdd(oldPos, delta);
		if (isInsideBoard(newPos)) {
			this.pos = newPos;
			return true;
		} else {
			//console.log("Cant move player to x: %d, y: %d", newPos.x, newPos.y);
			return false;
		}
	}
}

class Fruit extends Drawnable {
	constructor(x: number, y: number, id: string) {
		const color = "red";
		super(x, y, id, color);
	}
}

class GameBoard {
	size: position;
	players: Object;
	fruits: Object;
	constructor(size: position) {
		this.size = size;
		this.players = {};
		this.fruits = {};
		console.log("Create gameBoard with x: %d, y: %d", size.x, size.y);
	}
	display() {
		render.clear();
		Object.values(this.fruits).forEach((fruit: Fruit) => {
			fruit.drawn(render);
		});
		Object.values(this.players).forEach((player: Player) => {
			player.drawn(render);
		});
	}
}

//namespace gameLogic{
function getdeltaPosFromKey(key: string) {
	switch (key) {
		case "w":
			return { x: 0, y: -1 };
		case "a":
			return { x: -1, y: 0 };
		case "s":
			return { x: 0, y: 1 };
		case "d":
			return { x: 1, y: 0 };
	}
	throw new Error(`Invalid Key: ${key}`);
}
function posAdd(p1: position, p2: position) {
	return { x: p1.x + p2.x, y: p1.y + p2.y };
}
function isInsideBoard(pos: position) {
	const isInsideBoard = (pos.x < game.size.x && pos.x >= 0) && (pos.y < game.size.y && pos.y >= 0);
	return isInsideBoard;
}
function handleUserInput(input: string) {
	const player = myPlayer;
	const moveDetla = getdeltaPosFromKey(input);
	const hasMoved = player.moveDelta(moveDetla);
	if (hasMoved) {//This is done on server side too
		const sendObj = { move: input };
		socket.send(JSON.stringify(sendObj));
	}
}
//}//namespace gameLogic

var myID: string;
var myPlayer: Player;
function newFruitOrPlayer(id: string, obj: Player | Fruit): Player | Fruit {
	const newDesc = Object.create(null) as PropertyDescriptor;
	newDesc.configurable = true;
	newDesc.enumerable = true;
	newDesc.writable = true;
	newDesc.value = obj;
	let DescTarget;
	if (obj instanceof Player) {
		DescTarget = game.players;
	} else if (obj instanceof Fruit) {
		DescTarget = game.fruits;
	} else throw new Error("Object isan't instance of Player or Fruit");
	return Object.defineProperty(DescTarget, id, newDesc)[id];
}
function getByID(ID: string) {
	let TargetObj;
	if (ID in game.players) {
		const players: any = game.players;//Unsafe ?
		TargetObj = players[ID] as Player;
	} else if (ID in game.fruits) {
		const fruits: any = game.fruits;//Unsafe ?
		TargetObj = fruits[ID] as Fruit;
	} else return;
	return TargetObj;
}
function SetPos(newPos: newPos) {
	let TargetObj: Player | Fruit;
	const got = getByID(newPos.name);
	if (got != undefined) {
		TargetObj = got;
		if (got instanceof Fruit) {
			console.warn("Fruits Position shouldn't be changed ID: %s", newPos.name)
		}
	} else {
		let newObj: Fruit | Player;
		if (newPos.type === 1) {
			newObj = new Player(newPos.name);
		} else if (newPos.type === 2) {
			newObj = new Fruit(newPos.pos[0], newPos.pos[1], newPos.name);
		} else throw new Error("Unknow newPosObj Type " + JSON.stringify(newPos));
		TargetObj = newFruitOrPlayer(newPos.name, newObj);
	}
	TargetObj.pos = { x: newPos.pos[0], y: newPos.pos[1] };
	if (newPos.score != undefined) {
		const player = TargetObj;
		if (player instanceof Player && player.points != newPos.score) {
			player.points = newPos.score;
			scoreBoard.setScore(newPos.name, player.points);
		}
	}
	return TargetObj;
}
function removeByID(removeID: string) {
	console.log("remove: %s", removeID)
	if (game.players.hasOwnProperty(removeID)) {
		const players: any = game.players; //TS7053
		delete players[removeID];
	} else if (game.fruits.hasOwnProperty(removeID)) {
		const fruits: any = game.fruits; //TS7053
		delete fruits[removeID];
	} else {
		console.warn("not find %s", removeID);
	}
}
function handleServerMsgJson(msg: any) {
	if ("YourID" in msg) {
		myID = msg.YourID;
		myPlayer = newFruitOrPlayer(myID, new Player(myID)) as Player;
		console.log("My id: %s", myID)
	} else if ("newPos" in msg) {
		const newPosDesc = Object.getOwnPropertyDescriptor(msg, "newPos");
		if (newPosDesc === undefined) throw new Error("newPosDesc Received is undefined");
		const newPos = newPosDesc.value as newPos;
		console.log("SetrPos(%s, %s)", newPos.name, JSON.stringify(newPos.pos));
		SetPos(newPos);
	} else if ("remove" in msg) {
		const removeID = msg.remove as string;
		removeByID(removeID);
	} else {
		console.log(msg)
	}
}
function handleServerMsg(event: MessageEvent<any>) {
	//console.log('Message from server ', event.data);
	const msg = JSON.parse(event.data);
	if (Array.isArray(msg)) {
		msg.forEach(handleServerMsgJson)
	} else {
		handleServerMsgJson(msg)
	}
}

const socket = new WebSocket('ws://localhost:8765');
socket.addEventListener('message', handleServerMsg);

const game = new GameBoard({ x: 20, y: 10 });

const canvas = document.getElementById("gameCanvas") as HTMLCanvasElement;
const render = new Screen2D(canvas);

game.display();

document.addEventListener("keypress", function (e) {
	handleUserInput(e.key);
});


function walk() {
	game.display();
	setTimeout(walk, 10);//100FPS
}
walk();

function requestFruit() {
	socket.send(JSON.stringify({ "create": "" }))
}