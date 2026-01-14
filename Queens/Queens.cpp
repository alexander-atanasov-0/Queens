/**
*
* Solution to course project # 6
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Aleksandar Atanasov
* @idnumber 5MI0600592
* @compiler VCC
*
* <file with helper functions>
*
*/

#include <iostream>
#include <fstream>
const int INPUT_BUFFER = 64;
const int BUFFER_SIZE = 128;
const char PLAYER_1_MARKER = '*';
const char PLAYER_2_MARKER = '+';
const char EMPTY_SPACE = ' ';

struct Move {
	int player;
	int x;
	int y;
};

struct Game {
	Move history[BUFFER_SIZE];
	int turns;
	int rows;
	int cols;
	char** board;
	char** backupBoard;
};

bool strequal(const char* c1, const char* c2) {
	if (!c1 || !c2) return 0;
	for (size_t i = 0; c1[i] | c2[i]; i++)
	{
		if (c1[i] != c2[i])
		{
			return 0;
		}
	}
	return 1;
}

void writeMove(Game& game, int x, int y) {
	int row = y;
	int col = x;
	char currentMarker;
	int player;
	if (game.turns % 2 == 0)
	{
		player = 1;
		currentMarker = PLAYER_1_MARKER;
	}
	else
	{
		player = 2;
		currentMarker = PLAYER_2_MARKER;
	}
	for (int i = 0; i < game.rows; i++)
	{
		game.board[i][col] = currentMarker;
		for (int j = 0; j < game.cols; j++)
		{
			if (abs(i - col) == abs(j - row))
			{
				game.board[i][j] = currentMarker;
			}
		}
	}
	for (size_t i = 0; i < game.cols; i++)
	{
		game.board[row][i] = currentMarker;
	}
	game.board[row][col] = game.turns % 2 == 0 ? '1' : '2';
}

bool playAt(Game& game, int x, int y) {
	int row = y;
	int col = x;
	if (row < 0 || row >= game.rows || col < 0 || col >= game.cols)
	{
		std::cout << "Parameter out of range!\n";
		return 0;
	}
	if (game.board[row][col] != EMPTY_SPACE)
	{
		std::cout << "Cell is occupied\n";
		return 0;
	}
	writeMove(game, x, y);
	game.turns++;
	return 1;
}

void displayBoard(Game& game) {
	system("cls");
	std::cout << "    ";
	for (size_t i = 0; i < game.cols; i++)
	{
		std::cout << i;
		std::cout << (i < 10 ? "   " : "  ");
	}
	std::cout << "\n";
	for (size_t i = 0; i < game.rows; i++)
	{
		std::cout << "  ";
		for (size_t i = 0; i < game.cols; i++)
		{
			std::cout << "+---";
		}
		std::cout << "+\n" << i;
		std::cout << (i < 10 ? " " : "");
		for (size_t j = 0; j < game.cols; j++)
		{
			std::cout << "| ";
			std::cout << game.board[i][j];
			std::cout << " ";
		}
		std::cout << "|\n";
	}
	std::cout << "  ";
	for (size_t i = 0; i < game.cols; i++)
	{
		std::cout << "+---";
	}
	std::cout << "+\n";
	std::cout << "Player " << (game.turns % 2 ? "2" : "1") << "'s turn\n";
}

void showFreeCells(Game& game) {
	std::cout << "Free cells are:\n";
	for (size_t i = 0; i < game.cols; i++)
	{
		for (size_t j = 0; j < game.rows; j++)
		{
			if (game.board[j][i] == EMPTY_SPACE)
			{
				std::cout << "(" << i << "," << j << ") ";
			}
		}
	}
	std::cout << "\n";
}

bool boardsEqual(char** board1, char** board2, int rows, int cols) {
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			if (board1[i][j] != board2[i][j])
			{
				return 0;
			}
		}
	}
	return 1;
}

void copyBoard(char** destination, char** origin, int rows, int cols) {
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			destination[i][j] = origin[i][j];
		}
	}
}

char** initialiseBoard(int rows, int cols) {
	char** board = new char* [rows];
	for (size_t i = 0; i < rows; i++)
	{
		board[i] = new char[cols];
		for (size_t j = 0; j < cols; j++)
		{
			board[i][j] = EMPTY_SPACE;
		}
	}
	return board;
}

void deallocateGameMemory(Game& game) {
	for (size_t i = 0; i < game.rows; i++)
	{
		delete[] game.board[i];
		delete[] game.backupBoard[i];
	}
	delete[] game.board;
	delete[] game.backupBoard;
}

bool hasNextMove(Game& game) {
	for (size_t i = 0; i < game.rows; i++)
	{
		for (size_t j = 0; j < game.cols; j++)
		{
			if (game.board[i][j] == EMPTY_SPACE) {
				return 1;
			}
		}
	}
	return 0;
}

void startGame(Game& game) {
	displayBoard(game);
	char** backupBoard = initialiseBoard(game.rows, game.cols);
	game.backupBoard = backupBoard;
	while (true) {
		char input[INPUT_BUFFER];
		std::cin >> input;
		if (strequal("play", input))
		{
			int x, y;
			std::cin >> x >> y;
			if (std::cin.fail())
			{
				std::cin.clear();
				continue;
			}
			copyBoard(game.backupBoard, game.board, game.rows, game.cols);
			if (playAt(game, x, y))
			{
				displayBoard(game);
			}
			if (!hasNextMove(game)) {
				char winner = game.turns % 2 == 0 ? '2' : '1';
				std::cout << "Player " << winner << " won!\n";
				deallocateGameMemory(game);
				return;
			}
		}
		else if (strequal("save", input))
		{
			std::cout << "Game file name:\n";
			std::cin.ignore();
			char gameName[BUFFER_SIZE];
			std::cin.getline(gameName, BUFFER_SIZE);
			deallocateGameMemory(game);
			return;
		}
		else if (strequal("exit", input)) {
			deallocateGameMemory(game);
			std::cout << "Current game closed. Exit again to close application\n";
			return;
		}
		else if (strequal("free", input))
		{
			showFreeCells(game);
		}
		else if (strequal("history", input))
		{

		}
		else if (strequal("back", input))
		{
			if (boardsEqual(game.backupBoard, game.board, game.rows, game.cols))
			{
				std::cout << "You cannot recover further!\n";
				continue;
			}
			copyBoard(game.board, game.backupBoard, game.rows, game.cols);
			game.turns--;
			displayBoard(game);
		}
		else if (strequal("help", input)) {
			
		}
	}
}

void newGame() {
	int n, m;
	std::cin >> n >> m;
	if (std::cin.fail()) {
		std::cin.clear();
		return;
	}
	if (m < 2 || n < 2)
	{
		std::cout << "Board too small!\n";
		return;
	}
	int rows = m;
	int cols = n;
	char** board = initialiseBoard(rows, cols);
	Game game = { {}, 0, rows, cols, board };
	startGame(game);
}

void mainMenu() {
	while (true)
	{
		char input[INPUT_BUFFER];
		std::cin >> input;
		if (strequal("new", input))
		{
			newGame();
		}
		else if (strequal("load", input))
		{

		}
		else if (strequal("help", input))
		{

		}
		else if (strequal("exit", input))
		{
			return;
		}
		else
		{
			std::cout << "Invalid command\n";
		}
	}
}

int main()
{
	mainMenu();
}
