document.body.innerHTML += "JS";

interface position{
	x: number, y:number;
}

class Screen2D{
	context: CanvasRenderingContext2D;
	canvas: HTMLCanvasElement;
	width: number;
	height: number;
	constructor(canvas: HTMLCanvasElement){
		this.canvas = canvas;
		this.width = canvas.width;
		this.height = canvas.height;
		this.context = canvas.getContext("2d") as CanvasRenderingContext2D;
	}
	clear(){
		this.context.fillStyle = "white";
		this.context.fillRect(0, 0, this.width, this.height);
	}
	drawnPixel(pos:position, color: string){
		this.context.fillStyle = color;
		this.context.fillRect(pos.x, pos.y, 1, 1);
	}
}

abstract class Drawnable{
	private _pos: position;
	private color: string;
	protected constructor(x = 0, y = 0, color = "black") {
		this._pos = { x, y };
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
}

class Player extends Drawnable{
	points: number;
	constructor(x = 0, y = 0){
		const playerColor = "blue";
		super(x, y, playerColor);
		this.points = 0;
	}
	addPoint(){
		this.points++;
	}
}

class Fruit extends Drawnable{
	constructor(x: number, y: number) {
		const color = "red";
		super(x, y, color);
	}
}

class GameBoard{
	size: position;
	players: Array<Player>;
	fruits: Array<Fruit>;
	constructor(size: position){
		this.size = size;
		this.players = [];
		this.fruits = [];
		this.players.push(new Player());
		console.log("Create gameBoard with x: %d, y: %d", size.x, size.y);
	}
	genFood(){
		const randX = Math.floor((Math.random() * this.size.x));
		const randy = Math.floor((Math.random() * this.size.y));
		const newFood = new Fruit(randX, randy);
		this.fruits.push(newFood);
	}
	display(){
		render.clear();
		this.fruits.forEach((food) => {food.drawn(render);});
		this.players.forEach((player) => {player.drawn(render);});
	}
}

//namespace gameLogic{
function getdeltaPosFromKey(key: string){
	switch(key){
		case "w":
			return {x: 0, y: -1};
		case "a":
			return {x: -1, y: 0};
		case "s":
			return {x: 0, y: 1};
		case "d":
			return {x: 1, y: 0};
	}
	throw new Error(`Invalid Key: ${key}`);
}
function posAdd(p1: position, p2: position){
	return { x: p1.x + p2.x, y: p1.y + p2.y};
}
function isInsideBoard(pos: position){
	if((pos.x < game.size.x && pos.x >= 0) && (pos.y < game.size.y && pos.y >= 0))
		return true;
	else
		return false;
}
function handleUserInput(input: string){
	const player = game.players[0];
	const oldPos = player.pos;
	const newPos = posAdd(oldPos, getdeltaPosFromKey(input));
	if(isInsideBoard(newPos)){
		player.pos = newPos;
		collisionDetectionAndHandle(player, game.fruits);
	}else
		console.log("Cant move player to x: %d, y: %d", newPos.x, newPos.y);
}
function collisionDetectionAndHandle(player: Player, fruits: Array<Fruit>){
	const playerPos = player.pos;
	const detectFunction = (function(food: Fruit){
		const foodPos = food.pos;
		const colide = (playerPos.x === foodPos.x && playerPos.y === foodPos.y);
		if(colide){
			player.addPoint();
			console.log("player collided with food at x: %d, y: %d", playerPos.x, playerPos.y);
		}
		return !colide;
	});
	game.fruits = fruits.filter(detectFunction);
}
//}//namespace gameLogic

const game = new GameBoard({x: 20, y: 10});

const canvas = document.getElementById("myCanvas") as HTMLCanvasElement;
const render = new Screen2D(canvas);

game.display();

document.addEventListener("keypress", function(e){
	if(e.key === " ") game.genFood();
	else
		handleUserInput(e.key);
});


function walk(){
	game.display();
	setTimeout(walk, 10);//100FPS
}
walk();

let spawnDelay = 2500;
function spawn(){
	game.genFood();
	setTimeout(spawn, spawnDelay);
}
spawn();