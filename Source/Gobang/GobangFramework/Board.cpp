#include "Board.h"

Board::Board()
{
	Board::clearBoard();
}

bool Board::onBoard(int x, int y)
{
	return x >= 0 && y >=0 && x < Board::SIZE && y < Board::SIZE;
}

void Board::clearBoard()
{
	for (int i = 0; i < Board::SIZE; i++) 
	{
		for (int j = 0; j < Board::SIZE; j++)
		{
			board[i][j] = 0;
		}
	}
	while (!moves.empty()) moves.pop();
}

bool Board::isEmpty() const
{
	return moves.empty();
}

int Board::getBoardSize()
{
	return Board::SIZE;
}

/*
 * The winner can only be:
 * 0: None.
 * 1: Player 1.
 * 2: Player 2.
 * -1: Board full.
 */
Board::ChessPlayer Board::getWinner() const
{
	bool isFull = true;
	for (int i = 0; i < SIZE; i++) 
	{
		for (int j = 0; j < SIZE; j++)
		{
			int cur = board[i][j];
			if (cur == 0)
			{
				isFull = false;
				continue;
			} 
			bool right = true, down = true, rd = true, ld = true; 
			for (int k = 1; k < 5; k++)
			{
				if (!right || getBoard(i, j + k) != cur) right = false;
				if (!down || getBoard(i + k, j) != cur) down = false;
				if (!rd || getBoard(i + k, j + k) != cur) rd = false;
				if (!ld || getBoard(i + k, j - k) != cur) ld = false;
			}
			if (right || down || rd || ld) return cur;
		}
	}
	if (isFull) return -1;
	return 0;
}

std::pair<int, std::pair<int, int>> Board::getWinLocation() const
{
	if (getWinner() <= 0) return std::make_pair(-1, std::make_pair(-1, -1));
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			int cur = board[i][j];
			if (cur == 0) continue;
			bool right = true, down = true, rd = true, ld = true;
			for (int k = 1; k < 5; k++)
			{
				if (!right || getBoard(i, j + k) != cur) right = false;
				if (!down || getBoard(i + k, j) != cur) down = false;
				if (!rd || getBoard(i + k, j + k) != cur) rd = false;
				if (!ld || getBoard(i + k, j - k) != cur) ld = false;
			}
			if (right) return std::make_pair(1, std::make_pair(i, j));
			if (down) return std::make_pair(2, std::make_pair(i, j));
			if (rd) return std::make_pair(3, std::make_pair(i, j));
			if (ld) return std::make_pair(4, std::make_pair(i, j));
		}
	}
	return std::make_pair(-1, std::make_pair(-1, -1));
}

/*
 * To judge if there is a winner on the board.
 * return true if there is a winnner.
 */
bool Board::isOver() const
{
	return Board::getWinner() != 0;
}

/*
 * Make a move (place a chess) on the board.
 * return false if the board is already in use.
 */
bool Board::placeChess(ChessPlayer player, int x, int y)
{
	if (board[x][y] != 0) return false;
	board[x][y] = player;
	moves.push(std::make_pair(x, y));
	return true;
}

std::pair<int, int> Board::getLastChess() const
{
	if (moves.empty()) return std::make_pair(-1, -1);
	return moves.top();
}

/*
 * Retract the last chess on the plate.
 * return false if the board is empty.
 */
bool Board::retract()
{
	if (moves.empty()) return false;
	std::pair<int, int> location = moves.top();
	moves.pop();
	board[location.first][location.second] = 0;
	return true;
}

Board::ChessPlayer Board::getBoard(int x, int y) const
{
	if (onBoard(x, y)) return Board::board[x][y];
	return Board::OUTED;
}

/*
 * To judge if a location is able to place a chess or not.
 * return true if the board is empty.
 */
bool Board::isAvailable(int x, int y) const
{
	return Board::board[x][y] == 0;
}
