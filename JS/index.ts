document.body.innerHTML += "JS";

interface Context2D extends CanvasRenderingContext2D{};

class player{
	x: number;
	y: number;
	constructor(x = 0, y = 0){
		this.x = x;
		this.y = y;
	}
	drawn(ctx: Context2D){
		ctx.fillRect(this.x, this.y, 1, 1);
	}
}

class gameBoard{
	x: number;
	y: number;
	players: Array<player>;
	//board: Array<Array<number>>;
	constructor(x: number, y: number){
		this.x = x;
		this.y = y;
		this.players = new Array();
		//this.board = new Array(x);
		this.players.push(new player());
		//for(var i = 0; i < x; ++i)
		//	this.board[i] = new Array(y);
		console.log("Create gameBoard with x: %d, y: %d", x, y);
		//console.log(this.board);
	}
	display(ctx: Context2D){
		for(var player in this.players){
			this.players[player].drawn(ctx);
		}
	}
}

let game = new gameBoard(10, 10);

var canvas = document.getElementById("myCanvas") as HTMLCanvasElement;
var context = canvas.getContext("2d") as CanvasRenderingContext2D;

context.fillStyle = "blue";

game.players.push(new player(2, 2));
game.players.push(new player(5, 7));

game.display(context);