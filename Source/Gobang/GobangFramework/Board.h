#pragma once

#include <stack>
#include <utility>

class Board	// Gobang chess board
{
private:
	const static int SIZE = 15;	// Enlarged gobang board size
	
	std::stack< std::pair<int, int> > moves;
	
	int board[SIZE][SIZE];

	bool useBan;
	
public:
	typedef int ChessPlayer;
	const static int OUTED = -1;
	const static int VOIDED = 0;
	const static int PLAYER_1 = 1;
	const static int PLAYER_2 = 2;
	
	Board();
	Board(bool useBanMode);

	std::stack< std::pair<int, int> > getReviewData() { return moves; }

	void setBanMode(bool useBanMode) { useBan = useBanMode; }
	bool getBanMode() { return useBan; }

	/*
	 * To check if a coordinate is on the board
	 * x = line num, y = col num
	 */
	static bool onBoard(int x, int y);

	void clearBoard();

	bool isEmpty() const;
	
	static int getBoardSize();
	
	/*
	 * The winner can only be:
	 * 0: None.
	 * 1: Player 1.
	 * 2: Player 2.
	 * -1: Board full.
	 */
	ChessPlayer getWinner() const;

	/*
	 * Find the location and direction of winning:
	 * 1: right.
	 * 2: down.
	 * 3: right-down.
	 * 4: left-down.
	 * -1: No winner.
	 */
	std::pair<int, std::pair<int, int> > getWinLocation() const;
	
	/*
	 * To judge if there is a winner on the board.
	 * return true if there is a winnner.
	 */
	bool isOver() const;
	
	/*
	 * Make a move (place a chess) on the board.
	 * return false if the board is already in use.
	 */
	bool placeChess(ChessPlayer player, int x, int y);
	
	std::pair<int, int> getLastChess() const;
	
	/*
	 * Retract the last chess on the plate.
	 * return false if the board is empty.
	 */
	bool retract();
	
	/*
	 * The board can only be:
	 * 0: None (open space).
	 * 1: Player 1's chess.
	 * 2: Player 2's chess.
	 */
	ChessPlayer getBoard(int x, int y) const;
	
	/*
	 * To judge if a location is able to place a chess or not.
	 * 0: Success.
	 * 1: Position taken.
	 * 2: Banned.
	 */
	int isAvailable(int x, int y, Board::ChessPlayer player) const;
	
};
