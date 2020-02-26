#include "Board.h"

Board::Board()
{
	useBan = false;
	Board::clearBoard();
}

Board::Board(bool useBanMode)
{
	useBan = useBanMode;
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

int MX[] = { -1, -1, -1, 0, 1, 1, 1, 0 };
int MY[] = { -1, 0, 1, 1, 1, 0, -1, -1 };
int GF(int ori, int diff, int scale)
{
	return ori + diff * scale;
}
int Board::isAvailable(int x, int y, Board::ChessPlayer player) const
{
	if (getBoard(x, y) != Board::VOIDED) return 1;
	if (useBan && player == Board::PLAYER_1)
	{
		
		// Judge Long-link.
		int lens1[] = { 0, 0, 0, 0 };
		for (int i = 0; i < 4; i++)
		{
			// positive
			for (int j = 1; j < 6; j++)
			{
				if (getBoard(GF(x, MX[i], j), GF(y, MY[i], j)) == player) lens1[i]++;
				else break;
			}
			// negitive
			for (int j = 1; j < 6; j++)
			{
				if (getBoard(GF(x, MX[i], -j), GF(y, MY[i], -j)) == player) lens1[i]++;
				else break;
			}
		}
		for (int i = 0; i < 4; i++) if (lens1[i] + 1 >= 6) return 2;

		// Judge if can win.
		for (int i = 0; i < 4; i++)
		{
			lens1[i] = 0;
			// positive
			for (int j = 1; j < 5; j++)
			{
				if (getBoard(GF(x, MX[i], j), GF(y, MY[i], j)) == player) lens1[i]++;
				else break;
			}
			// negitive
			for (int j = 1; j < 5; j++)
			{
				if (getBoard(GF(x, MX[i], -j), GF(y, MY[i], -j)) == player) lens1[i]++;
				else break;
			}
		}
		for (int i = 0; i < 4; i++) if (lens1[i] + 1 == 5) return 0;
		
		int jud_type[] = { 0, 0, 0, 0 }; // 0 = none, 1 = four_round, 2 = live_three;
		// Judge four_round
		int bef_gap[] = { 0, 0, 0, 0 };
		int aft_gap[] = { 0, 0, 0, 0 };
		int bef_gap_for3[] = { 0, 0, 0, 0 };
		int aft_gap_for3[] = { 0, 0, 0, 0 };
		int bef_gap_rev[] = { 0, 0, 0, 0 };
		int aft_gap_rev[] = { 0, 0, 0, 0 };
		int bef_gap_rev_for3[] = { 0, 0, 0, 0 };
		int aft_gap_rev_for3[] = { 0, 0, 0, 0 };
		for (int i = 0; i < 4; i++)
		{
			// positive
			bool before_gap = true;
			for (int j = 1; j < 5; j++)
			{
				if (before_gap)
				{
					if (getBoard(GF(x, MX[i], j), GF(y, MY[i], j)) == Board::VOIDED)
					{
						before_gap = false;
						bef_gap_for3[i] = bef_gap[i];
						continue;
					}
					else if (getBoard(GF(x, MX[i], j), GF(y, MY[i], j)) == player)
					{
						bef_gap[i]++;
					}
					else
					{
						break;
					}
				}
				else
				{
					if (getBoard(GF(x, MX[i], j), GF(y, MY[i], j)) == player)
					{
						aft_gap[i]++;
					}
					else
					{
						if (getBoard(GF(x, MX[i], j), GF(y, MY[i], j)) == Board::VOIDED) aft_gap_for3[i] = aft_gap[i];
						break;
					}
				}
			}
			// negitive
			before_gap = true;
			for (int j = 1; j < 5; j++)
			{
				if (before_gap)
				{
					if (getBoard(GF(x, MX[i], -j), GF(y, MY[i], -j)) == Board::VOIDED)
					{
						before_gap = false;
						bef_gap_rev_for3[i] = bef_gap_rev[i];
						continue;
					}
					else if (getBoard(GF(x, MX[i], -j), GF(y, MY[i], -j)) == player)
					{
						bef_gap_rev[i]++;
					}
					else
					{
						break;
					}
				}
				else
				{
					if (getBoard(GF(x, MX[i], -j), GF(y, MY[i], -j)) == player)
					{
						aft_gap_rev[i]++;
					}
					else
					{
						if (getBoard(GF(x, MX[i], j), GF(y, MY[i], j)) == Board::VOIDED) aft_gap_rev_for3[i] = aft_gap_rev[i];
						break;
					}
				}
			}
		}
		int live_three_num = 0;
		int forster_four_num = 0;
		for (int i = 0; i < 4; i++)
		{
			if (bef_gap[i] + aft_gap[i] + bef_gap_rev[i] + 1 == 4 || bef_gap_rev[i] + aft_gap_rev[i] + bef_gap[i] + 1 == 4)
			{
				forster_four_num++;
			}
			else if (bef_gap_for3[i] + bef_gap_rev_for3[i] + 1 == 3)
			{ // Judge live_three:appended
				live_three_num++;
			}
			else if (bef_gap_for3[i] + aft_gap_for3[i] + bef_gap_rev_for3[i] + 1 == 4 || bef_gap_rev_for3[i] + aft_gap_rev_for3[i] + bef_gap_for3[i] + 1 == 4)
			{ // Judge live_three:heaped
				live_three_num++;
			}
		}
		if (live_three_num >= 2) return 2;
		if (forster_four_num >= 2) return 2;
		return 0;
	}
	return 0;
}
