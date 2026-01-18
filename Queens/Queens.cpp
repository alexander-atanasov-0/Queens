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
const char MAIN_COLOR[] = "\033[38;2;255;165;0m";
const char SECONDARY_COLOR[] = "\033[0;96m";
const char DEFAULT_COLOR[] = "\033[0m";
const char BOARD_COLOR[] = "\033[0;90m";
const char BLINKING_YELLOW[] = "\033[1;5;33m";

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

void resetColor() {
	std::cout << DEFAULT_COLOR;
}

void boardColor() {
	std::cout << BOARD_COLOR;
}

void mainColor() {
	std::cout << MAIN_COLOR;
}

void setColor1() {
	std::cout << MAIN_COLOR;
}

void setColor2() {
	std::cout << SECONDARY_COLOR;
}

void setBlinking() {
	std::cout << BLINKING_YELLOW;
}

void showWelcomeBoard() {
	char matrix[2][9] = { {'*' , '1', '*', '+', '*', '+', '*', '*', '*'},
						  {'*', '+', '*', '+' , '2', '+', '+', '+', '+'} };
	for (size_t i = 0; i < 2; i++)
	{
		boardColor();
		std::cout << "  ";
		for (size_t i = 0; i < 9; i++)
		{
			std::cout << "+---";
		}
		std::cout << "+\n";
		setColor2();
		std::cout << i;
		boardColor();
		std::cout << (i < 10 ? " " : "");
		for (size_t j = 0; j < 9; j++)
		{
			std::cout << "| ";
			if (matrix[i][j] == PLAYER_1_MARKER || matrix[i][j] == '1')
			{
				setColor2();
			}
			else
			{
				setColor1();
			}
			std::cout << matrix[i][j];
			boardColor();
			std::cout << " ";
		}
		std::cout << "|\n";
	}
	std::cout << "  ";
	for (size_t i = 0; i < 9; i++)
	{
		std::cout << "+---";
	}
	std::cout << "+\n";
}

void showWelcomeText() {
	std::cout
		<< BOARD_COLOR
		<< "  =========="
		<< MAIN_COLOR
		<< "WELCOME TO QUEENS!"
		<< BOARD_COLOR
		<< "=========\n"
		<< SECONDARY_COLOR
		<< "Start by creating a new board\n"
		<< "Type "
		<< MAIN_COLOR
		<< "new N M "
		<< SECONDARY_COLOR
		<< "to create a new N x M board\n"
		<< "Type "
		<< MAIN_COLOR
		<< "Play X Y "
		<< SECONDARY_COLOR
		<< "to put a queen at (X,Y) position\n"
		<< "Use "
		<< MAIN_COLOR
		<< "help "
		<< SECONDARY_COLOR
		<< "to show all commands\n"	
		<< MAIN_COLOR;
}

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

void saveGame(Game& game, char* str) {
	std::ofstream write("games.txt", std::ios::app);
	write << "---\n";
	write << str << "\n";
	write << game.rows << " " << game.cols << " " << game.turns << "\n";
	for (size_t i = 0; i < game.turns; i++)
	{
		Move move = game.history[i];
		write << move.player << " " << move.x << " " << move.y << " ";
	}
	write << "\n";
	write.close();
	std::cout << "Game saved!\n";
}

void writeHistory(Move* history, int currentPlayer, int x, int y) {
	int index = 0;
	for (size_t i = 0; i < BUFFER_SIZE; i++)
	{
		if (history[i].player == 0)
		{
			break;
		}
		index++;
	}
	Move move = { currentPlayer, x, y };
	history[index] = move;
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
			if (abs(i - row) == abs(j - col))
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
	writeHistory(game.history, player, x, y);
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
	mainColor();
	for (size_t i = 0; i < game.cols; i++)
	{
		std::cout << i;
		std::cout << (i < 10 ? "   " : "  ");
	}
	boardColor();
	std::cout << "\n";
	for (size_t i = 0; i < game.rows; i++)
	{
		std::cout << "  ";
		for (size_t i = 0; i < game.cols; i++)
		{
			std::cout << "+---";
		}
		std::cout << "+\n";
		mainColor();
		std::cout << i;
		std::cout << (i < 10 ? " " : "");
		boardColor();
		for (size_t j = 0; j < game.cols; j++)
		{
			std::cout << "| ";
			if (game.board[i][j] == PLAYER_1_MARKER || game.board[i][j] == '1')
			{
				setColor2();
			}
			else
			{
				setColor1();
			}
			std::cout << game.board[i][j];
			boardColor();
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
	mainColor();
	std::cout << "Player " << (game.turns % 2 ? "2" : "1") << "'s turn\n";
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

void deleteLastMove(Game& game) {
	game.history[game.turns - 1] = { 0,0,0 };
}

void printHistory(Game& game) {
	for (size_t i = 0; i < game.turns; i++)
	{
		Move move = game.history[i];
		std::cout << "Player " << move.player << "-> (" << move.x << "," << move.y << ") ";
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
	copyBoard(game.backupBoard, game.board, game.rows, game.cols);
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
				setBlinking();
				std::cout << "Player " << winner << " won!\n";
				mainColor();
				printHistory(game);
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
			saveGame(game, gameName);
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
			printHistory(game);
		}
		else if (strequal("back", input))
		{
			if (boardsEqual(game.backupBoard, game.board, game.rows, game.cols))
			{
				std::cout << "You cannot recover further!\n";
				continue;
			}
			copyBoard(game.board, game.backupBoard, game.rows, game.cols);
			deleteLastMove(game);
			game.turns--;
			displayBoard(game);
		}
		else if (strequal("help", input)) {
			std::cout << "+\n";
		}
	}
}

void loadGame() {
	std::ifstream read("games.txt");
	char str[BUFFER_SIZE];
	int gameCount = 0;
	while (read.getline(str, BUFFER_SIZE))
	{
		if (strequal("---", str))
		{
			gameCount++;
			read.getline(str, BUFFER_SIZE);
			std::cout << gameCount << ". " << str << "\n";
		}
	}
	read.clear();
	read.seekg(0, std::ios::beg);
	std::cout << "There are " << gameCount << " saved games found.\n";
	if (gameCount == 0)
	{
		return;
	}
	int chosenGame;
	while (true)
	{
		//std::cout << "Enter a number from 1 to " << gameCount << "\n";
		std::cout << "Enter a number from the list\n";
		std::cin >> chosenGame;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(BUFFER_SIZE, '\n');
			continue;
		}
		if (chosenGame >= 1 && chosenGame <= gameCount)
		{
			break;
		}
	}
	gameCount = 0;
	while (read.getline(str, BUFFER_SIZE))
	{
		if (strequal("---", str))
		{
			gameCount++;
		}
		if (gameCount == chosenGame)
		{
			break;
		}
	}
	read.getline(str, BUFFER_SIZE);
	int rows, cols, turns;
	read >> rows >> cols >> turns;
	char** board = initialiseBoard(rows, cols);
	Game game = { {}, 0, rows, cols, board };
	for (size_t i = 0; i < turns; i++)
	{
		int player, x, y;
		read >> player >> x >> y;
		playAt(game, x, y);
	}
	read.ignore();
	startGame(game);
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
	showWelcomeBoard();
	showWelcomeText();
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
			loadGame();
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
