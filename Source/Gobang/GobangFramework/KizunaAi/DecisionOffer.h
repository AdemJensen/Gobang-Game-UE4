#pragma once

#include <utility>
#include "ChessConstruction.h"
class DecisionOffer
{
private:
	std::pair<int, int> coordinate;
	ChessConstruction type;
	bool attack;

public:
	DecisionOffer();
	DecisionOffer(int x, int y, ChessConstruction type, bool isAttack);

	std::pair<int, int> getLocation();

	ChessConstruction getType();

	bool isAtk();
};
