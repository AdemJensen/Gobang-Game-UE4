#pragma once

#include "../Board.h"
#include <utility>
#include "ChessConstruction.h"
#include "DecisionOffer.h"
#include "KizunaAiCache.h"

class KizunaAi
{
private:
	Board::ChessPlayer selfPlayer;

	KizunaAiCache< std::pair<bool, ChessConstruction>, DecisionOffer> mem;	// pair<isAttack, type that about to form>
	
	std::map<std::pair<bool, ChessConstruction>, int> ratingData;

	int difficulty = 2;

protected:
	// These declarations are for the board detections.

	void detect_CONS_APPEND_5(const Board& board, int x, int y);
	void detect_CONS_LIVE_4(const Board& board, int x, int y);
	void detect_CONS_FORSTER_4(const Board& board, int x, int y);
	void detect_CONS_SLEEP_4(const Board& board, int x, int y);
	void detect_CONS_LIVE_3(const Board& board, int x, int y);
	void detect_CONS_SLEEP_3(const Board& board, int x, int y);
	void detect_CONS_LIVE_2(const Board& board, int x, int y);
	void detect_CONS_SLEEP_2(const Board& board, int x, int y);

public:
	KizunaAi();

	void setPlayer(Board::ChessPlayer player);
	Board::ChessPlayer getPlayer();

	void setDifficulty(int dif) { difficulty = dif; }
	int getDifficulty() { return difficulty; }
	
	void detectSituation(const Board& board);

	DecisionOffer getBestChoice(std::vector<DecisionOffer>& options);

	std::pair<int, int> makeAction(const Board& board);
	
};

