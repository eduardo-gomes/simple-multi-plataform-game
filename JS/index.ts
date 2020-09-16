document.body.innerHTML += "JS";

interface position{
	x: number, y:number;
};

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

class player{
	private _pos: position;
	private playerColor = "blue";
	constructor(x = 0, y = 0){
		this._pos = {x, y};
	}
	drawn(ctx: Screen2D){
		ctx.drawnPixel(this._pos, this.playerColor);
	}
	get pos(): position{
		return this._pos;
	}
	set pos(newPos: position){
		this._pos = newPos;
	}
}

class gameBoard{
	size: position;
	players: Array<player>;
	constructor(size: position){
		this.size = size;
		this.players = new Array();
		this.players.push(new player());
		console.log("Create gameBoard with x: %d, y: %d", size.x, size.y);
	}
	display(){
		render.clear();
		for(var player in this.players){
			this.players[player].drawn(render);
		}
	}
}

class gameLogic{
	static getdeltaPosFromKey(key: string){
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
	static posAdd(p1: position, p2: position){
		return { x: p1.x + p2.x, y: p1.y + p2.y};
	}
	static isInsideBoard(pos: position){
		if((pos.x < game.size.x && pos.x >= 0) && (pos.y < game.size.y && pos.y >= 0))
			return true;
		else
			return false;
	}
	static handleUserInput(input: string){
		let player = game.players[0];
		const oldPos = player.pos;
		let newPos = gameLogic.posAdd(oldPos, gameLogic.getdeltaPosFromKey(input));
		if(gameLogic.isInsideBoard(newPos))
			player.pos = newPos;
		else
			console.log("Cant move player to x: %d, y: %d", newPos.x, newPos.y);
	}
}

let game = new gameBoard({x: 20, y: 10});

var canvas = document.getElementById("myCanvas") as HTMLCanvasElement;
var render = new Screen2D(canvas);

game.players.push(new player(2, 2));
game.players.push(new player(5, 7));

game.display();

document.addEventListener("keypress", function(e){
	gameLogic.handleUserInput(e.key);
});

{
function walk(){//ES5 dont allow function declaration inside block in strict mode
	game.display();
	setTimeout(walk, 10);//100FPS
}
walk();
}