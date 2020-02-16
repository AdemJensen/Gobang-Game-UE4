
#include "DecisionOffer.h"

DecisionOffer::DecisionOffer()
{
	this->coordinate = std::make_pair(-1, -1);
	this->type = ChessConstruction::CONS_VOID;
	this->attack = false;
}

DecisionOffer::DecisionOffer(int x, int y, ChessConstruction type, bool isAttack)
{
	this->coordinate.first = x;
	this->coordinate.second = y;
	this->type = type;
	this->attack = isAttack;
}

std::pair<int, int> DecisionOffer::getLocation()
{
	return std::make_pair(coordinate.first, coordinate.second);
}

ChessConstruction DecisionOffer::getType()
{
	return type;
}

bool DecisionOffer::isAtk()
{
	return attack;
}
