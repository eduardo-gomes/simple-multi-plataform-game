interface position {
	x: number, y: number;
}
interface newPos {
	name: string, pos: [number, number];
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
	addPoint() {
		this._points++;
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
			console.log("Cant move player to x: %d, y: %d", newPos.x, newPos.y);
			return false;
		}
	}
}

class Fruit extends Drawnable {
	constructor(x: number, y: number, id:string) {
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
		const sendObj = {move: input};
		socket.send(JSON.stringify(sendObj));
	}
}
//}//namespace gameLogic

var myID: string;
var myPlayer: Player;
function newPlayer(id: string, position: position): Player{
	const newPlayerDesc = Object.create(null) as PropertyDescriptor;
	newPlayerDesc.enumerable = true;
	newPlayerDesc.writable = true;
	newPlayerDesc.value = new Player(id) as Player;
	newPlayerDesc.value.pos = position;
	return Object.defineProperty(game.players, id, newPlayerDesc)[id];
}
function SetPlayerPos(id: string, position: position){
	const players: any = game.players;//Unsafe ?
	if (!players.hasOwnProperty(id)){
		newPlayer(id, position);
	}
	const player = players[id] as Player;
	player.pos = position;
	//console.log(player);
}
function handleServerMsg(event: MessageEvent<any>){
	//console.log('Message from server ', event.data);
	const msg = JSON.parse(event.data);
	if ("YourID" in msg){
		myID = msg.YourID;
		myPlayer = newPlayer(myID, {x: 0, y: 0});
		console.log("My id: %s", myID)
	}else if ("newPos" in msg){
		const newPosDesc = Object.getOwnPropertyDescriptor(msg, "newPos");
		if(newPosDesc === undefined) throw new Error("newPosDesc Received is undefined");
		const newPos = newPosDesc.value as newPos;
		const pos = {x: newPos.pos[0], y: newPos.pos[1]};
		console.log("SetPlayerPos(%s, %s)", newPos.name, JSON.stringify(pos));
		SetPlayerPos(newPos.name, pos);
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
