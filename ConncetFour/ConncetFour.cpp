#include "ConncetFour.h"
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))



vector<vector<int>> board(NUM_ROW, vector<int>(NUM_COL));

void playGame() {
	printBoard(board);
	while (!gameOver) {
		if (currentPlayer == COMPUTER) {
			auto start = std::chrono::high_resolution_clock::now();
			makeMove(board, aiMove(), COMPUTER);
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> duration = (end - start);
			std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
		}
		else if (currentPlayer == PLAYER) {
			makeMove(board, userMove(), PLAYER);
		}
		else if (turns == NUM_ROW * NUM_COL) {
			gameOver = true;
		}
		gameOver = winningMove(board, currentPlayer);
		currentPlayer = (currentPlayer == 1) ? 2 : 1;
		turns++;
		cout << endl;
		printBoard(board);
	}
	if (turns == NUM_ROW * NUM_COL) {
		cout << "Draw!" << endl;
	}
	else {
		cout << ((currentPlayer == PLAYER) ? "AI Wins!" : "Player Wins!") << endl;
	}
}


void makeMove(vector<vector<int> >& b, int c, unsigned int p) {
	for (unsigned int r = 0; r < NUM_ROW; r++) {
		if (b[r][c] == 0) {
			b[r][c] = p;
			break;
		}
	}
}


int userMove() {
	int move = -1;
	while (true) {
		cout << "Enter a column: ";
		cin >> move;
		if (!cin) {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			errorMessage(1);
		}
		else if (!((unsigned int)move < NUM_COL && move >= 0)) {
			errorMessage(2);
		}
		else if (board[NUM_ROW - 1][move] != 0) {
			errorMessage(3);
		}
		else {
			break;
		}
		cout << endl << endl;
	}
	return move;
}


int aiMove() {
	cout << "AI is thinking about a move..." << endl;
	return miniMax(board, MAX_DEPTH, COMPUTER)[1];
}


array<int, 2> miniMax(vector<vector<int> >& b, unsigned int d, unsigned int p) {

	if (d == 0 || d >= (NUM_COL * NUM_ROW) - turns) {
		return array<int, 2>{tabScore(b, COMPUTER), -1};
	}

	if (p == COMPUTER) {
		array<int, 2> moveSoFar = { INT_MIN, -1 };
		if (winningMove(b, PLAYER))
			return moveSoFar;

		for (unsigned int c = 0; c < NUM_COL; c++) {
			if (b[NUM_ROW - 1][c] == 0) {
				vector<vector<int> > newBoard = copyBoard(b);
				makeMove(newBoard, c, p);
				int score = miniMax(newBoard, d - 1, PLAYER)[0];

				if (score > moveSoFar[0]) {
					moveSoFar = { score, (int)c };
				}

			}
		}
		return moveSoFar;
	}
	else {
		array<int, 2> moveSoFar = { INT_MAX, -1 };
		if (winningMove(b, COMPUTER)) {
			return moveSoFar;
		}
		for (unsigned int c = 0; c < NUM_COL; c++) {
			if (b[NUM_ROW - 1][c] == 0) {
				vector<vector<int> > newBoard = copyBoard(b);
				makeMove(newBoard, c, p);
				int score = miniMax(newBoard, d - 1, COMPUTER)[0];
				if (score < moveSoFar[0]) {
					moveSoFar = { score, (int)c };
				}

			}
		}
		return moveSoFar;
	}
}

int tabScore(vector<vector<int> > b, unsigned int p) {
	int score = 0;
	vector<unsigned int> rs(NUM_COL);
	vector<unsigned int> cs(NUM_ROW);
	vector<unsigned int> set(4);
	// horizontal
	 for (unsigned int r = 0; r < NUM_ROW; r++) {
	 	for (unsigned int c = 0; c < NUM_COL; c++) {
	 		rs[c] = b[r][c]; 
	 	}
	 	for (unsigned int c = 0; c < NUM_COL - 3; c++) {
	 		for (int i = 0; i < 4; i++) {
	 			set[i] = rs[c + i]; 
	 		}
	 		score += scoreSet(set, p); 
	 	}
	 }

	// vertical
	for (unsigned int c = 0; c < NUM_COL; c++) {
		for (unsigned int r = 0; r < NUM_ROW; r++) {
			cs[r] = b[r][c];
		}
		for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
			for (int i = 0; i < 4; i++) {
				set[i] = cs[r + i];
			}
			score += scoreSet(set, p);
		}
	}
	// diagonals
	for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
		for (unsigned int c = 0; c < NUM_COL; c++) {
			rs[c] = b[r][c];
		}
		for (unsigned int c = 0; c < NUM_COL - 3; c++) {
			for (int i = 0; i < 4; i++) {
				set[i] = b[r + i][c + i];
			}
			score += scoreSet(set, p);
		}
	}
	for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
		for (unsigned int c = 0; c < NUM_COL; c++) {
			rs[c] = b[r][c];
		}
		for (unsigned int c = 0; c < NUM_COL - 3; c++) {
			for (int i = 0; i < 4; i++) {
				set[i] = b[r + 3 - i][c + i];
			}
			score += scoreSet(set, p);
		}
	}
	return score;
}


int scoreSet(vector<unsigned int> v, unsigned int p) {
	unsigned int good = 0;
	unsigned int bad = 0;
	unsigned int empty = 0;
	for (unsigned int i = 0; i < v.size(); i++) {
		good += (v[i] == p) ? 1 : 0;
		bad += (v[i] == PLAYER || v[i] == COMPUTER) ? 1 : 0;
		empty += (v[i] == 0) ? 1 : 0;
	}
	bad -= good;
	return heurFunction(good, bad, empty);
}


int heurFunction(unsigned int g, unsigned int b, unsigned int z) {
	int score = 0;
	if (g == 4) { score += 500001; }
	else if (g == 3 && z == 1) { score += 5000; }
	else if (g == 2 && z == 2) { score += 500; }
	else if (b == 2 && z == 2) { score -= 501; }
	else if (b == 3 && z == 1) { score -= 5001; }
	else if (b == 4) { score -= 500000; }
	return score;
}


bool winningMove(vector<vector<int> >& b, unsigned int p) {
	unsigned int winSequence = 0;
	for (unsigned int c = 0; c < NUM_COL - 3; c++) {
		for (unsigned int r = 0; r < NUM_ROW; r++) {
			for (int i = 0; i < 4; i++) {
				if ((unsigned int)b[r][c + i] == p) {
					winSequence++;
				}
				if (winSequence == 4) { return true; }
			}
			winSequence = 0;
		}
	}

	for (unsigned int c = 0; c < NUM_COL; c++) {
		for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
			for (int i = 0; i < 4; i++) {
				if ((unsigned int)b[r + i][c] == p) {
					winSequence++;
				}
				if (winSequence == 4) { return true; }
			}
			winSequence = 0;
		}
	}
	for (unsigned int c = 0; c < NUM_COL - 3; c++) {
		for (unsigned int r = 3; r < NUM_ROW; r++) {
			for (int i = 0; i < 4; i++) {
				if ((unsigned int)b[r - i][c + i] == p) {
					winSequence++;
				}
				if (winSequence == 4) { return true; }
			}
			winSequence = 0;
		}
	}
	for (unsigned int c = 0; c < NUM_COL - 3; c++) {
		for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
			for (int i = 0; i < 4; i++) {
				if ((unsigned int)b[r + i][c + i] == p) {
					winSequence++;
				}
				if (winSequence == 4) { return true; }
			}
			winSequence = 0;
		}
	}
	return false;
}


void initBoard() {
	for (unsigned int r = 0; r < NUM_ROW; r++) {
		for (unsigned int c = 0; c < NUM_COL; c++) {
			board[r][c] = 0;
		}
	}
}


vector<vector<int> > copyBoard(vector<vector<int> > b) {
	vector<vector<int>> newBoard(NUM_ROW, vector<int>(NUM_COL));
	for (unsigned int r = 0; r < NUM_ROW; r++) {
		for (unsigned int c = 0; c < NUM_COL; c++) {
			newBoard[r][c] = b[r][c];
		}
	}
	return newBoard;
}


void printBoard(vector<vector<int> >& b) {
	for (unsigned int i = 0; i < NUM_COL; i++) {
		cout << " " << i;
	}
	cout << endl << "---------------" << endl;
	for (unsigned int r = 0; r < NUM_ROW; r++) {
		for (unsigned int c = 0; c < NUM_COL; c++) {
			cout << "|";
			switch (b[NUM_ROW - r - 1][c]) {
			case 0: cout << " "; break;
			case 1: cout << "O"; break;
			case 2: cout << "X"; break;
			}
			if (c + 1 == NUM_COL) { cout << "|"; }
		}
		cout << endl;
	}
	cout << "---------------" << endl;
	cout << endl;
}


void errorMessage(int t) {
	if (t == 1) {
		cout << "Use a value 0.." << NUM_COL - 1 << endl;
	}
	else if (t == 2) {
		cout << "That is not a valid column." << endl;
	}
	else if (t == 3) {
		cout << "That column is full." << endl;
	}
	cout << endl;
}

void printProjectInfo() {
	cout << "Colum number: " << NUM_COL << endl;
	cout << "Row number: " << NUM_ROW << endl;
	cout << "DEPTH: " << MAX_DEPTH << endl<<endl;
}

int main(int argc, char** argv) {
	cout << "Connect Four Serial" << endl;
	printProjectInfo();
	int i = -1; bool flag = false;
	if (argc == 2) {
		istringstream in(argv[1]);
		if (!(in >> i)) { flag = true; }
		if (i > (int)(NUM_ROW * NUM_COL) || i <= -1) { flag = true; }
		if (flag) { cout << "Invalid command line argument, using default depth = 5." << endl; }
		else { MAX_DEPTH = i; }
	}
	initBoard();
	playGame();
	return 0;
}







