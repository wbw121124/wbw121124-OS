const colors = ['red', 'blue', 'green', 'yellow', 'purple', 'orange'];
let gameBoard = [];
const size = 14;
let selectedColorIndex = 0;
var startCell = null;

function initGame() {
    const game = document.getElementById('game');
    game.innerHTML = '';
    gameBoard = [];
    for (let i = 0; i < size; i++) {
        gameBoard[i] = [];
        for (let j = 0; j < size; j++) {
            const cell = document.createElement('div');
            cell.classList.add('cell');
            cell.style.backgroundColor = colors[Math.floor(Math.random() * colors.length)];
			cell.cellIndex = i * size + j;
            game.appendChild(cell);
            gameBoard[i][j] = cell;
        }
    }
    // floodFill();
}

// function floodFill() {
//     startCell = gameBoard;
//     fill(startCell);
// }

function fill(cell) {
    // if (cell.filled) return;
    cell.style.backgroundColor = colors[selectedColorIndex];
    cell.filled = true;

    // 递归填充相邻的同色块
    const directions = [
        [-1, 0], // 上
        [1, 0],  // 下
        [0, -1], // 左
        [0, 1]   // 右
    ];
    directions.forEach(([dx, dy]) => {
        const nx = cell.cellIndex % size + dx;
        const ny = Math.floor(cell.cellIndex / size) + dy;
        if (nx >= 0 && nx < size && ny >= 0 && ny < size) {
            const neighbor = gameBoard[ny][nx];
            if (neighbor.style.backgroundColor === startCell) {
                fill(neighbor);
            }
        }
    });
}

// 添加颜色选择器
const colorPicker = document.getElementById('color-picker');

function fillfi() {
	//从右上角开始填充colorPicker.value
    startCell = gameBoard[0][size - 1].style.backgroundColor;
    selectedColorIndex = colorPicker.value;
	//去除已填充的标记
    gameBoard.forEach(row => row.forEach(cell => delete cell.filled));
	//填充
    fill(gameBoard[0][size - 1]);
}

initGame();