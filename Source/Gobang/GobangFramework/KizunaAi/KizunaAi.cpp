#include "KizunaAi.h"
#include "../Board.h"
#include <iostream>
#include <vector>
#include <sstream>
//#include "atlbase.h"
//#include "atlstr.h"
//// Open the debug output about detection info
const bool SHOW_DETECTION_INFO = false;
void OutputDebugPrintf(const char* strOutputString, ...)
{
	//char strBuffer[4096] = { 0 };
	//va_list vlArgs;
	//va_start(vlArgs, strOutputString);
	////_vsnprintf_s(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
	//vsprintf_s(strBuffer,strOutputString,vlArgs);
	//va_end(vlArgs);
	//OutputDebugString(CA2W(strBuffer));
}

std::string getChessConsStr(ChessConstruction co)
{
	std::string reason = "ERR";
	switch ((int)co)
	{
		case 0:		// Construction of live five type, only saved for special.
			reason = "CONS_APPEND_5";
			break;
		case 1:
			reason = "CONS_LIVE_4";
			break;
		case 2:
			reason = "CONS_FORSTER_4";
			break;
		case 3:
			reason = "CONS_SLEEP_4";
			break;
		case 4:
			reason = "CONS_LIVE_3";
			break;
		case 5:
			reason = "CONS_SLEEP_3";
			break;
		case 6:
			reason = "CONS_LIVE_2";
			break;
		case 7:
			reason = "CONS_SLEEP_2";
			break;
	}
	return reason;
}

void addVal(KizunaAi* ai, KizunaAiCache< std::pair<bool, ChessConstruction>, DecisionOffer>* cache, const Board& board, std::pair<bool, ChessConstruction> key, DecisionOffer val)
{
	if (Board::onBoard(val.getLocation().first, val.getLocation().second) && board.isAvailable(val.getLocation().first, val.getLocation().second, ai->getPlayer()) == 0)
	{
		cache->addVal(key, val);
		if (SHOW_DETECTION_INFO)
		{
			using namespace std;
			string reason = getChessConsStr(key.second);
			ostringstream str;
			str << "	Suggestion enlarged: for reason " << reason << " AT (" << val.getLocation().first << ", " << val.getLocation().second << ")" << endl;
			cout << str.str();
			OutputDebugPrintf(str.str().c_str());
		}
	}
}

void __DEBUG_DETECT__(int x, int y, std::string constructionName)
{
	if (!SHOW_DETECTION_INFO) return;
	using namespace std;
	ostringstream str;
	str << "Detected construction: " << constructionName << " AT (" << x << ", " << y << ")" << endl;
	cout << str.str();
	OutputDebugPrintf(str.str().c_str());
}

bool judgeOfferSame(DecisionOffer a, DecisionOffer b)
{
	return a.getLocation() == b.getLocation();
}
KizunaAi::KizunaAi()
{
	mem.setJudgeFunc(judgeOfferSame);
	mem.addKey(std::make_pair(true, ChessConstruction::CONS_APPEND_5));
	mem.addKey(std::make_pair(true, ChessConstruction::CONS_LIVE_4));
	mem.addKey(std::make_pair(true, ChessConstruction::CONS_FORSTER_4));
	mem.addKey(std::make_pair(true, ChessConstruction::CONS_SLEEP_4));
	mem.addKey(std::make_pair(true, ChessConstruction::CONS_LIVE_3));
	mem.addKey(std::make_pair(true, ChessConstruction::CONS_SLEEP_3));
	mem.addKey(std::make_pair(true, ChessConstruction::CONS_LIVE_2));
	mem.addKey(std::make_pair(true, ChessConstruction::CONS_SLEEP_2));
	mem.addKey(std::make_pair(false, ChessConstruction::CONS_APPEND_5));
	mem.addKey(std::make_pair(false, ChessConstruction::CONS_LIVE_4));
	mem.addKey(std::make_pair(false, ChessConstruction::CONS_FORSTER_4));
	mem.addKey(std::make_pair(false, ChessConstruction::CONS_SLEEP_4));
	mem.addKey(std::make_pair(false, ChessConstruction::CONS_LIVE_3));
	mem.addKey(std::make_pair(false, ChessConstruction::CONS_SLEEP_3));
	mem.addKey(std::make_pair(false, ChessConstruction::CONS_LIVE_2));
	mem.addKey(std::make_pair(false, ChessConstruction::CONS_SLEEP_2));

	ratingData[std::make_pair(true, ChessConstruction::CONS_APPEND_5)] = 99999999;
	ratingData[std::make_pair(false, ChessConstruction::CONS_APPEND_5)] = 500000;

	ratingData[std::make_pair(true, ChessConstruction::CONS_LIVE_4)] = 9000;
	ratingData[std::make_pair(false, ChessConstruction::CONS_LIVE_4)] = 950;

	ratingData[std::make_pair(true, ChessConstruction::CONS_FORSTER_4)] = 100;
	ratingData[std::make_pair(false, ChessConstruction::CONS_FORSTER_4)] = 65;

	ratingData[std::make_pair(true, ChessConstruction::CONS_SLEEP_4)] = 45;
	ratingData[std::make_pair(false, ChessConstruction::CONS_SLEEP_4)] = 35;

	ratingData[std::make_pair(true, ChessConstruction::CONS_LIVE_3)] = 40;
	ratingData[std::make_pair(false, ChessConstruction::CONS_LIVE_3)] = 35;

	ratingData[std::make_pair(true, ChessConstruction::CONS_SLEEP_3)] = 5;
	ratingData[std::make_pair(false, ChessConstruction::CONS_SLEEP_3)] = 5;

	ratingData[std::make_pair(true, ChessConstruction::CONS_LIVE_2)] = 2;
	ratingData[std::make_pair(false, ChessConstruction::CONS_LIVE_2)] = 2;

	ratingData[std::make_pair(true, ChessConstruction::CONS_SLEEP_2)] = 1;
	ratingData[std::make_pair(false, ChessConstruction::CONS_SLEEP_2)] = 1;
}

void KizunaAi::setPlayer(Board::ChessPlayer player)
{
	this->selfPlayer = player;
}

Board::ChessPlayer KizunaAi::getPlayer()
{
	return selfPlayer;
}

void KizunaAi::detectSituation(const Board& board)
{
	OutputDebugPrintf("===== New round =====\n");
	
	for (int i = 0; i < board.getBoardSize(); i++)
	{
		for (int j = 0; j < board.getBoardSize(); j++)
		{		
			detect_CONS_APPEND_5(board, i, j);
			detect_CONS_LIVE_4(board, i, j);
			detect_CONS_FORSTER_4(board, i, j);
			detect_CONS_SLEEP_4(board, i, j);
			detect_CONS_LIVE_3(board, i, j);
			detect_CONS_SLEEP_3(board, i, j);
			detect_CONS_LIVE_2(board, i, j);
		}
	}
}

DecisionOffer KizunaAi::getBestChoice(std::vector<DecisionOffer>& options)
{
	OutputDebugPrintf("Get best choice:\n");
	std::ostringstream str;
	int bestRate = -1;
	DecisionOffer result;
	for (int i = 0; i < options.size(); i++)
	{
		DecisionOffer offer = options[i];
		str << "\tOption " << i << ": (" << offer.getLocation().first << ", " << offer.getLocation().second << "), TYPE = " << (offer.isAtk() ? "ATK " : "DEF ") << getChessConsStr(offer.getType()) << std::endl;
		std::vector<std::pair<bool, ChessConstruction> > temp = mem.getRelativeKeySet(offer);
		int rate = 0;
		for (int j = 0; j < temp.size(); j++)
		{
			if (getDifficulty() < 2 && ((int) temp[j].second) > 2) continue;
			str << "\t\tPoint: " << (temp[j].first ? "ATK" : "DEF") << " " << getChessConsStr(temp[j].second) << " Rate:" << ratingData[temp[j]] << std::endl;
			rate += ratingData[temp[j]];
		}
		if (rate > bestRate)
		{
			bestRate = rate;
			result = offer;
		}
	}
	str << "\tFinal: (" << result.getLocation().first << ", " << result.getLocation().second << "), TYPE = " << (result.isAtk() ? "ATK " : "DEF ") << getChessConsStr(result.getType()) << " At rating: " << bestRate << std::endl;
	OutputDebugPrintf(str.str().c_str());
	return result;
}

std::pair<int, int> KizunaAi::makeAction(const Board& board)
{
	mem.clearVals();
	detectSituation(board);
	std::vector<DecisionOffer> temp;

	// 1: See if we can win.
	temp = mem.getValSet(std::make_pair(true, ChessConstruction::CONS_APPEND_5));
	if (temp.size() > 0) return temp[0].getLocation();

	// 2: See if we need to prevent the competitor from winning.
	temp = mem.getValSet(std::make_pair(false, ChessConstruction::CONS_APPEND_5));
	if (temp.size() > 0) return getBestChoice(temp).getLocation();

	//// 3: Try to make a LIVE_4 attack.
	//temp = mem.getValSet(std::make_pair(true, ChessConstruction::CONS_LIVE_4));
	//if (temp.size() > 0) return getBestChoice(temp).getLocation();

	//// 4: Stop the competitor from making a LIVE_4.
	//temp = mem.getValSet(std::make_pair(false, ChessConstruction::CONS_LIVE_4));
	//if (temp.size() > 0) return getBestChoice(temp).getLocation();

	// 5£ºMaking FORSTER_4 attack.(ABANDONED)
	//temp = mem.getValSet(std::make_pair(true, ChessConstruction::CONS_FORSTER_4));
	//if (temp.size() > 0) return getBestChoice(temp).getLocation();

	// 6: Select a best choice from the rest.
	temp = mem.getAllElements();
	if (temp.size() > 0) return getBestChoice(temp).getLocation();
	else // No suggestion
	{
		// check if this is the first chess
		if (board.isEmpty())
		{
			std::pair<int, int> selected = std::make_pair(board.getBoardSize() / 2, board.getBoardSize() / 2);
			if (board.isAvailable(selected.first, selected.second, getPlayer()) == 0) return selected;
			else return std::make_pair(-1, -1);
		}

		// Select the possible best:
		int best = 0;
		std::pair<int, int> res = std::make_pair(-1, -1);
		for (int i = 0; i < board.getBoardSize(); i++)
		{
			for (int j = 0; j < board.getBoardSize(); j++)
			{
				if (board.isAvailable(i, j, getPlayer()) == 0)
				{
					int score = 0;
					if (board.getBoard(i - 1, j - 1) == Board::OUTED) score -= 1;
					else if (board.getBoard(i - 1, j - 1) != Board::VOIDED) score += 3;
					if (board.getBoard(i - 1, j) == Board::OUTED) score -= 1;
					else if (board.getBoard(i - 1, j) != Board::VOIDED) score += 3;
					if (board.getBoard(i - 1, j + 1) == Board::OUTED) score -= 1;
					else if (board.getBoard(i - 1, j + 1) != Board::VOIDED) score += 3;

					if (board.getBoard(i, j - 1) == Board::OUTED) score -= 1;
					else if (board.getBoard(i - 1, j - 1) != Board::VOIDED) score += 3;
					if (board.getBoard(i, j + 1) == Board::OUTED) score -= 1;
					else if (board.getBoard(i, j + 1) != Board::VOIDED) score += 3;

					if (board.getBoard(i + 1, j - 1) == Board::OUTED) score -= 1;
					else if (board.getBoard(i + 1, j - 1) != Board::VOIDED) score += 3;
					if (board.getBoard(i + 1, j) == Board::OUTED) score -= 1;
					else if (board.getBoard(i + 1, j) != Board::VOIDED) score += 3;
					if (board.getBoard(i + 1, j + 1) == Board::OUTED) score -= 1;
					else if (board.getBoard(i + 1, j + 1) != Board::VOIDED) score += 3;

					bool n = true, s = true, e = true, w = true, nw = true, ne = true, se = true, sw = true;
					for (int k = 1; k < 5; k++)
					{
						if (n)
						{
							if (board.getBoard(i - k, j) == Board::OUTED) n = false;
							else if (board.getBoard(i - k, j) == getPlayer()) score += (5 - k);
							else n = false;
						}
						if (s)
						{
							if (board.getBoard(i + k, j) == Board::OUTED) s = false;
							else if (board.getBoard(i + k, j) == getPlayer()) score += (5 - k);
							else s = false;
						}
						if (e)
						{
							if (board.getBoard(i, j + k) == Board::OUTED) e = false;
							else if (board.getBoard(i, j + k) == getPlayer()) score += (5 - k);
							else e = false;
						}
						if (w)
						{
							if (board.getBoard(i, j - k) == Board::OUTED) w = false;
							else if (board.getBoard(i, j - k) == getPlayer()) score += (5 - k);
							else w = false;
						}
						if (ne)
						{
							if (board.getBoard(i - k, j + k) == Board::OUTED) ne = false;
							else if (board.getBoard(i - k, j + k) == getPlayer()) score += (5 - k);
							else ne = false;
						}
						if (nw)
						{
							if (board.getBoard(i - k, j - k) == Board::OUTED) nw = false;
							else if (board.getBoard(i - k, j - k) == getPlayer()) score += (5 - k);
							else nw = false;
						}
						if (sw)
						{
							if (board.getBoard(i + k, j - k) == Board::OUTED) sw = false;
							else if (board.getBoard(i + k, j - k) == getPlayer()) score += (5 - k);
							else sw = false;
						}
						if (se)
						{
							if (board.getBoard(i + k, j + k) == Board::OUTED) se = false;
							else if (board.getBoard(i + k, j + k) == getPlayer()) score += (5 - k);
							else se = false;
						}
					}

					if (score > best)
					{
						best = score;
						res = std::make_pair(i, j);
					}
				}
			}
		}
		return res;	// if -1, -1 then error
	}

	return std::pair<int, int>(-1, -1);	// error
}

void KizunaAi::detect_CONS_APPEND_5(const Board& board, int x, int y)
{
	Board::ChessPlayer cur = board.getBoard(x, y);
	if (cur == Board::VOIDED) return;		// No chess here
	bool right = true, down = true, rd = true, ld = true;
	for (int k = 1; k < 5; k++)
	{
		if (!right || board.getBoard(x, y + k) != cur) right = false;
		if (!down || board.getBoard(x + k, y) != cur) down = false;
		if (!rd || board.getBoard(x + k, y + k) != cur) rd = false;
		if (!ld || board.getBoard(x + k, y - k) != cur) ld = false;
	}
	if (right || down || rd || ld)
	{
		__DEBUG_DETECT__(x, y, "APPEND_5");
		// Confused because the game must be over.
		// Preserved for tactical calculations in the future.
	}
}

void KizunaAi::detect_CONS_LIVE_4(const Board& board, int x, int y)
{
	Board::ChessPlayer cur = board.getBoard(x, y);
	if (cur == Board::VOIDED) return;		// No chess here
	bool right = board.getBoard(x, y - 1) == Board::VOIDED && board.getBoard(x, y + 4) == Board::VOIDED;
	bool down = board.getBoard(x - 1, y) == Board::VOIDED && board.getBoard(x + 4, y) == Board::VOIDED;
	bool rd = board.getBoard(x - 1, y - 1) == Board::VOIDED && board.getBoard(x + 4, y + 4) == Board::VOIDED;
	bool ld = board.getBoard(x - 1, y + 1) == Board::VOIDED && board.getBoard(x + 4, y - 4) == Board::VOIDED;
	for (int k = 1; k < 4; k++)
	{
		if (!right || board.getBoard(x, y + k) != cur) right = false;
		if (!down || board.getBoard(x + k, y) != cur) down = false;
		if (!rd || board.getBoard(x + k, y + k) != cur) rd = false;
		if (!ld || board.getBoard(x + k, y - k) != cur) ld = false;
	}
	if (right)
	{
		__DEBUG_DETECT__(x, y, "LIVE_4_R");
		if (getPlayer() == cur)	// win!
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x, y - 1, ChessConstruction::CONS_APPEND_5, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x, y + 4, ChessConstruction::CONS_APPEND_5, true));
		}
		else // lost
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x, y - 1, ChessConstruction::CONS_APPEND_5, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x, y + 4, ChessConstruction::CONS_APPEND_5, false));
		}
	}
	if (down)
	{
		__DEBUG_DETECT__(x, y, "LIVE_4_D");
		if (getPlayer() == cur)	// win!
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x - 1, y, ChessConstruction::CONS_APPEND_5, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + 4, y, ChessConstruction::CONS_APPEND_5, true));
		}
		else // lost
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x - 1, y, ChessConstruction::CONS_APPEND_5, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + 4, y, ChessConstruction::CONS_APPEND_5, false));
		}
	}
	if (rd)
	{
		__DEBUG_DETECT__(x, y, "LIVE_4_RD");
		if (getPlayer() == cur)	// win!
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x - 1, y - 1, ChessConstruction::CONS_APPEND_5, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + 4, y + 4, ChessConstruction::CONS_APPEND_5, true));
		}
		else // lost
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x - 1, y - 1, ChessConstruction::CONS_APPEND_5, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + 4, y + 4, ChessConstruction::CONS_APPEND_5, false));
		}
	}
	if (ld)
	{
		__DEBUG_DETECT__(x, y, "LIVE_4_LD");
		if (getPlayer() == cur)	// win!
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x - 1, y + 1, ChessConstruction::CONS_APPEND_5, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + 4, y - 4, ChessConstruction::CONS_APPEND_5, true));
		}
		else // lost
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x - 1, y + 1, ChessConstruction::CONS_APPEND_5, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + 4, y - 4, ChessConstruction::CONS_APPEND_5, false));
		}
	}
}

void KizunaAi::detect_CONS_FORSTER_4(const Board& board, int x, int y)
{
	Board::ChessPlayer cur = board.getBoard(x, y);
	if (cur == Board::VOIDED) return;		// No chess here
	int right = ((board.getBoard(x, y - 1) == Board::VOIDED || board.getBoard(x, y + 5) == Board::VOIDED) && !(board.getBoard(x, y - 1) == Board::VOIDED && board.getBoard(x, y + 5) == Board::VOIDED)) ? 2 : 0;
	int down = ((board.getBoard(x - 1, y) == Board::VOIDED || board.getBoard(x + 5, y) == Board::VOIDED) && !(board.getBoard(x - 1, y) == Board::VOIDED && board.getBoard(x + 5, y) == Board::VOIDED)) ? 2 : 0;
	int rd = ((board.getBoard(x - 1, y - 1) == Board::VOIDED || board.getBoard(x + 5, y + 5) == Board::VOIDED) && !(board.getBoard(x - 1, y - 1) == Board::VOIDED && board.getBoard(x + 5, y + 5) == Board::VOIDED)) ? 2 : 0;
	int ld = ((board.getBoard(x - 1, y + 1) == Board::VOIDED || board.getBoard(x + 5, y - 5) == Board::VOIDED) && !(board.getBoard(x - 1, y + 1) == Board::VOIDED && board.getBoard(x + 5, y - 5) == Board::VOIDED)) ? 2 : 0;
	for (int k = 1; k < 5; k++)
	{
		Board::ChessPlayer aspect = board.getBoard(x, y + k);
		if (right > 0)
		{
			if (aspect == Board::VOIDED) right--;
			else if (aspect != cur) right = 0;
		}

		aspect = board.getBoard(x + k, y);
		if (down > 0)
		{
			if (aspect == Board::VOIDED) down--;
			else if (aspect != cur) down = 0;
		}

		aspect = board.getBoard(x + k, y + k);
		if (rd > 0)
		{
			if (aspect == Board::VOIDED) rd--;
			else if (aspect != cur) rd = 0;
		}

		aspect = board.getBoard(x + k, y - k);
		if (ld > 0)
		{
			if (aspect == Board::VOIDED) ld--;
			else if (aspect != cur) ld = 0;
		}
	}
	int right2 = (board.getBoard(x, y - 1) == Board::VOIDED && board.getBoard(x, y + 5) == Board::VOIDED) ? 2 : 0;
	int down2 = (board.getBoard(x - 1, y) == Board::VOIDED && board.getBoard(x + 5, y) == Board::VOIDED) ? 2 : 0;
	int rd2 = (board.getBoard(x - 1, y - 1) == Board::VOIDED && board.getBoard(x + 5, y + 5) == Board::VOIDED) ? 2 : 0;
	int ld2 = (board.getBoard(x - 1, y + 1) == Board::VOIDED && board.getBoard(x + 5, y - 5) == Board::VOIDED) ? 2 : 0;
	for (int k = 1; k < 4; k++)
	{
		Board::ChessPlayer aspect = board.getBoard(x, y + k);
		if (right2 > 0)
		{
			if (aspect == Board::VOIDED) right2--;
			else if (aspect != cur) right2 = 0;
		}

		aspect = board.getBoard(x + k, y);
		if (down2 > 0)
		{
			if (aspect == Board::VOIDED) down2--;
			else if (aspect != cur) down2 = 0;
		}

		aspect = board.getBoard(x + k, y + k);
		if (rd2 > 0)
		{
			if (aspect == Board::VOIDED) rd2--;
			else if (aspect != cur) rd2 = 0;
		}

		aspect = board.getBoard(x + k, y - k);
		if (ld2 > 0)
		{
			if (aspect == Board::VOIDED) ld2--;
			else if (aspect != cur) ld2 = 0;
		}
	}
	if (board.getBoard(x, y + 4) != cur) right2 = 0;
	if (board.getBoard(x + 4, y) != cur) down2 = 0;
	if (board.getBoard(x + 4, y + 4) != cur) rd2 = 0;
	if (board.getBoard(x + 4, y - 4) != cur) ld2 = 0;
	if (right == 1 || right2 == 1)
	{
		__DEBUG_DETECT__(x, y, "FORSTER_4_R");
		for (int k = 1; k < 5; k++)
		{
			if (board.getBoard(x, y + k) == Board::VOIDED)
			{
				if (getPlayer() == cur) // win
				{
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x, y + k, ChessConstruction::CONS_APPEND_5, true));
				}
				else // must destroy immediately
				{
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x, y + k, ChessConstruction::CONS_APPEND_5, false));
				}
				break;
			}
		}
	}
	if (down == 1 || down2 == 1)
	{
		__DEBUG_DETECT__(x, y, "FORSTER_4_D");
		for (int k = 1; k < 5; k++)
		{
			if (board.getBoard(x + k, y) == Board::VOIDED)
			{
				if (getPlayer() == cur) // win
				{
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + k, y, ChessConstruction::CONS_APPEND_5, true));
				}
				else // must destroy immediately
				{
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + k, y, ChessConstruction::CONS_APPEND_5, false));
				}
				break;
			}
		}
	}
	if (rd == 1 || rd2 == 1)
	{
		__DEBUG_DETECT__(x, y, "FORSTER_4_RD");
		for (int k = 1; k < 5; k++)
		{
			if (board.getBoard(x + k, y + k) == Board::VOIDED)
			{
				if (getPlayer() == cur) // win
				{
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + k, y + k, ChessConstruction::CONS_APPEND_5, true));
				}
				else // must destroy immediately
				{
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + k, y + k, ChessConstruction::CONS_APPEND_5, false));
				}
				break;
			}
		}
	}
	if (ld == 1 || ld2 == 1)
	{
		__DEBUG_DETECT__(x, y, "FORSTER_4_LD");
		for (int k = 1; k < 5; k++)
		{
			if (board.getBoard(x + k, y - k) == Board::VOIDED)
			{
				if (getPlayer() == cur) // win
				{
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + k, y - k, ChessConstruction::CONS_APPEND_5, true));
				}
				else // must destroy immediately
				{
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + k, y - k, ChessConstruction::CONS_APPEND_5, false));
				}
				break;
			}
		}
	}
	if (right == 1 || down == 1 || rd == 1 || ld == 1 || right2 == 1 || down2 == 1 || rd2 == 1 || ld2 == 1) return;
	// Special type of Forster 4
	bool right3 = (board.getBoard(x, y - 1) == Board::VOIDED || board.getBoard(x, y + 4) == Board::VOIDED) && !(board.getBoard(x, y - 1) == Board::VOIDED && board.getBoard(x, y + 4) == Board::VOIDED);
	bool down3 = (board.getBoard(x - 1, y) == Board::VOIDED || board.getBoard(x + 4, y) == Board::VOIDED) && !(board.getBoard(x - 1, y) == Board::VOIDED && board.getBoard(x + 4, y) == Board::VOIDED);
	bool rd3 = (board.getBoard(x - 1, y - 1) == Board::VOIDED || board.getBoard(x + 4, y + 4) == Board::VOIDED) && !(board.getBoard(x - 1, y - 1) == Board::VOIDED && board.getBoard(x + 4, y + 4) == Board::VOIDED);
	bool ld3 = (board.getBoard(x - 1, y + 1) == Board::VOIDED || board.getBoard(x + 4, y - 4) == Board::VOIDED) && !(board.getBoard(x - 1, y + 1) == Board::VOIDED && board.getBoard(x + 4, y - 4) == Board::VOIDED);
	for (int k = 1; k < 4; k++)
	{
		if (!right3 || board.getBoard(x, y + k) != cur) right3 = false;
		if (!down3 || board.getBoard(x + k, y) != cur) down3 = false;
		if (!rd3 || board.getBoard(x + k, y + k) != cur) rd3 = false;
		if (!ld3 || board.getBoard(x + k, y - k) != cur) ld3 = false;
	}
	if (right3)
	{
		__DEBUG_DETECT__(x, y, "LIVE_4_R_SPC");
		if (getPlayer() == cur)	// win!
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x, y - 1, ChessConstruction::CONS_APPEND_5, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x, y + 4, ChessConstruction::CONS_APPEND_5, true));
		}
		else
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x, y - 1, ChessConstruction::CONS_APPEND_5, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x, y + 4, ChessConstruction::CONS_APPEND_5, false));
		}
	}
	if (down3)
	{
		__DEBUG_DETECT__(x, y, "LIVE_4_D_SPC");
		if (getPlayer() == cur)	// win!
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x - 1, y, ChessConstruction::CONS_APPEND_5, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + 4, y, ChessConstruction::CONS_APPEND_5, true));
		}
		else
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x - 1, y, ChessConstruction::CONS_APPEND_5, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + 4, y, ChessConstruction::CONS_APPEND_5, false));
		}
	}
	if (rd3)
	{
		__DEBUG_DETECT__(x, y, "LIVE_4_RD_SPC");
		if (getPlayer() == cur)	// win!
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x - 1, y - 1, ChessConstruction::CONS_APPEND_5, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + 4, y + 4, ChessConstruction::CONS_APPEND_5, true));
		}
		else
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x - 1, y - 1, ChessConstruction::CONS_APPEND_5, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + 4, y + 4, ChessConstruction::CONS_APPEND_5, false));
		}
	}
	if (ld3)
	{
		__DEBUG_DETECT__(x, y, "LIVE_4_LD_SPC");
		if (getPlayer() == cur)	// win!
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x - 1, y + 1, ChessConstruction::CONS_APPEND_5, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + 4, y - 4, ChessConstruction::CONS_APPEND_5, true));
		}
		else
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x - 1, y + 1, ChessConstruction::CONS_APPEND_5, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_APPEND_5), DecisionOffer(x + 4, y - 4, ChessConstruction::CONS_APPEND_5, false));
		}
	}
}

void KizunaAi::detect_CONS_SLEEP_4(const Board& board, int x, int y)
{
	Board::ChessPlayer cur = board.getBoard(x, y);
	if (cur == Board::VOIDED) return;		// No chess here
	int right = (board.getBoard(x, y - 1) == Board::VOIDED && board.getBoard(x, y + 4) == Board::VOIDED) ? 2 : 0;
	int down = (board.getBoard(x - 1, y) == Board::VOIDED && board.getBoard(x + 4, y) == Board::VOIDED) ? 2 : 0;
	int rd = (board.getBoard(x - 1, y - 1) == Board::VOIDED && board.getBoard(x + 4, y + 4) == Board::VOIDED) ? 2 : 0;
	int ld = (board.getBoard(x - 1, y + 1) == Board::VOIDED && board.getBoard(x + 4, y - 4) == Board::VOIDED) ? 2 : 0;
	for (int k = 1; k < 3; k++)
	{
		Board::ChessPlayer aspect = board.getBoard(x, y + k);
		if (right > 0)
		{
			if (aspect == Board::VOIDED) right--;
			else if (aspect != cur) right = 0;
		}

		aspect = board.getBoard(x + k, y);
		if (down > 0)
		{
			if (aspect == Board::VOIDED) down--;
			else if (aspect != cur) down = 0;
		}

		aspect = board.getBoard(x + k, y + k);
		if (rd > 0)
		{
			if (aspect == Board::VOIDED) rd--;
			else if (aspect != cur) rd = 0;
		}

		aspect = board.getBoard(x + k, y - k);
		if (ld > 0)
		{
			if (aspect == Board::VOIDED) ld--;
			else if (aspect != cur) ld = 0;
		}
	}
	if (right > 0 && board.getBoard(x, y + 3) == cur)
	{
		__DEBUG_DETECT__(x, y, "SLEEP_4_R");
		for (int k = 1; k < 4; k++)
		{
			if (board.getBoard(x, y + k) == Board::VOIDED)
			{
				if (getPlayer() == cur) // win next place
				{
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_4), DecisionOffer(x, y + k, ChessConstruction::CONS_LIVE_4, true));
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x, y - 1, ChessConstruction::CONS_FORSTER_4, true));
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x, y + 4, ChessConstruction::CONS_FORSTER_4, true));
					// Only added other "FORSTER_4" to add other ways to win.
				}
				else // must destroy unless have priority
				{
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x, y + k, ChessConstruction::CONS_LIVE_4, false));
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x, y - 1, ChessConstruction::CONS_LIVE_4, false));
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x, y + 4, ChessConstruction::CONS_LIVE_4, false));
					// Because to destroy a SLEEP_4 has thess three ways, this is not an error.
				}
				break;
			}
		}
	}
	if (down > 0 && board.getBoard(x + 3, y) == cur)
	{
		__DEBUG_DETECT__(x, y, "SLEEP_4_D");
		for (int k = 1; k < 4; k++)
		{
			if (board.getBoard(x + k, y) == Board::VOIDED)
			{
				if (getPlayer() == cur) // win next place
				{
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_4), DecisionOffer(x + k, y, ChessConstruction::CONS_LIVE_4, true));
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x - 1, y, ChessConstruction::CONS_FORSTER_4, true));
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x + 4, y, ChessConstruction::CONS_FORSTER_4, true));
					// Only added other "FORSTER_4" to add other ways to win.
				}
				else // must destroy unless have priority
				{
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x + k, y, ChessConstruction::CONS_LIVE_4, false));
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x - 1, y, ChessConstruction::CONS_LIVE_4, false));
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x + 4, y, ChessConstruction::CONS_LIVE_4, false));
					// Because to destroy a SLEEP_4 has thess three ways, this is not an error.
				}
				break;
			}
		}
	}
	if (rd > 0 && board.getBoard(x + 3, y + 3) == cur)
	{
		__DEBUG_DETECT__(x, y, "SLEEP_4_RD");
		for (int k = 1; k < 4; k++)
		{
			if (board.getBoard(x + k, y + k) == Board::VOIDED)
			{
				if (getPlayer() == cur) // win next place
				{
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_4), DecisionOffer(x + k, y + k, ChessConstruction::CONS_LIVE_4, true));
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x - 1, y - 1, ChessConstruction::CONS_FORSTER_4, true));
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x + 4, y + 4, ChessConstruction::CONS_FORSTER_4, true));
					// Only added other "FORSTER_4" to add other ways to win.
				}
				else // must destroy unless have priority
				{
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x + k, y + k, ChessConstruction::CONS_LIVE_4, false));
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x - 1, y - 1, ChessConstruction::CONS_LIVE_4, false));
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x + 4, y + 4, ChessConstruction::CONS_LIVE_4, false));
					// Because to destroy a SLEEP_4 has thess three ways, this is not an error.
				}
				break;
			}
		}
	}
	if (ld > 0 && board.getBoard(x + 3, y - 3) == cur)
	{
		__DEBUG_DETECT__(x, y, "SLEEP_4_LD");
		for (int k = 1; k < 4; k++)
		{
			if (board.getBoard(x + k, y - k) == Board::VOIDED)
			{
				if (getPlayer() == cur) // win next place
				{
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_4), DecisionOffer(x + k, y - k, ChessConstruction::CONS_LIVE_4, true));
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x - 1, y + 1, ChessConstruction::CONS_FORSTER_4, true));
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x + 4, y - 4, ChessConstruction::CONS_FORSTER_4, true));
					// Only added other "FORSTER_4" to add other ways to win.
				}
				else // must destroy unless have priority
				{
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x + k, y - k, ChessConstruction::CONS_LIVE_4, false));
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x - 1, y + 1, ChessConstruction::CONS_LIVE_4, false));
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x + 4, y - 4, ChessConstruction::CONS_LIVE_4, false));
					// Because to destroy a SLEEP_4 has thess three ways, this is not an error.
				}
				break;
			}
		}
	}
}

void KizunaAi::detect_CONS_LIVE_3(const Board& board, int x, int y)
{
	Board::ChessPlayer cur = board.getBoard(x, y);
	if (cur == Board::VOIDED) return;		// No chess here
	bool right = board.getBoard(x, y - 1) == Board::VOIDED && board.getBoard(x, y + 3) == Board::VOIDED;
	bool down = board.getBoard(x - 1, y) == Board::VOIDED && board.getBoard(x + 3, y) == Board::VOIDED;
	bool rd = board.getBoard(x - 1, y - 1) == Board::VOIDED && board.getBoard(x + 3, y + 3) == Board::VOIDED;
	bool ld = board.getBoard(x - 1, y + 1) == Board::VOIDED && board.getBoard(x + 3, y - 3) == Board::VOIDED;
	for (int k = 1; k < 3; k++)
	{
		if (!right || board.getBoard(x, y + k) != cur) right = false;
		if (!down || board.getBoard(x + k, y) != cur) down = false;
		if (!rd || board.getBoard(x + k, y + k) != cur) rd = false;
		if (!ld || board.getBoard(x + k, y - k) != cur) ld = false;
	}
	if (right)
	{
		__DEBUG_DETECT__(x, y, "LIVE_3_R");
		if (getPlayer() == cur)	// win on next round
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_4), DecisionOffer(x, y - 1, ChessConstruction::CONS_LIVE_4, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_4), DecisionOffer(x, y + 3, ChessConstruction::CONS_LIVE_4, true));
		}
		else // must destroy unless have priority
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x, y - 1, ChessConstruction::CONS_LIVE_4, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x, y + 3, ChessConstruction::CONS_LIVE_4, false));
		}
	}
	if (down)
	{
		__DEBUG_DETECT__(x, y, "LIVE_3_D");
		if (getPlayer() == cur)	// win on next round
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_4), DecisionOffer(x - 1, y, ChessConstruction::CONS_LIVE_4, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_4), DecisionOffer(x + 3, y, ChessConstruction::CONS_LIVE_4, true));
		}
		else // must destroy unless have priority
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x - 1, y, ChessConstruction::CONS_LIVE_4, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x + 3, y, ChessConstruction::CONS_LIVE_4, false));
		}
	}
	if (rd)
	{
		__DEBUG_DETECT__(x, y, "LIVE_3_RD");
		if (getPlayer() == cur)	// win on next round
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_4), DecisionOffer(x - 1, y - 1, ChessConstruction::CONS_LIVE_4, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_4), DecisionOffer(x + 3, y + 3, ChessConstruction::CONS_LIVE_4, true));
		}
		else // must destroy unless have priority
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x - 1, y - 1, ChessConstruction::CONS_LIVE_4, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x + 3, y + 3, ChessConstruction::CONS_LIVE_4, false));
		}
	}
	if (ld)
	{
		__DEBUG_DETECT__(x, y, "LIVE_3_LD");
		if (getPlayer() == cur)	// win on next round
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_4), DecisionOffer(x - 1, y + 1, ChessConstruction::CONS_LIVE_4, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_4), DecisionOffer(x + 3, y - 3, ChessConstruction::CONS_LIVE_4, true));
		}
		else // must destroy unless have priority
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x - 1, y + 1, ChessConstruction::CONS_LIVE_4, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_4), DecisionOffer(x + 3, y - 3, ChessConstruction::CONS_LIVE_4, false));
		}
	}
}

void KizunaAi::detect_CONS_SLEEP_3(const Board& board, int x, int y)
{
	Board::ChessPlayer cur = board.getBoard(x, y);
	if (cur == Board::VOIDED) return;		// No chess here
	// form 1:
	Board::ChessPlayer a, b;
	a = board.getBoard(x, y - 1);
	b = board.getBoard(x, y + 3);
	if (a == Board::VOIDED && b == Board::VOIDED && cur == board.getBoard(x, y + 2) && board.getBoard(x, y + 1) == Board::VOIDED)
	{
		__DEBUG_DETECT__(x, y, "SLEEP_3_F1_R");
		if (getPlayer() == cur)	// win on next round
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_3), DecisionOffer(x, y + 1, ChessConstruction::CONS_LIVE_3, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_SLEEP_4), DecisionOffer(x, y + 3, ChessConstruction::CONS_SLEEP_4, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_SLEEP_4), DecisionOffer(x, y - 1, ChessConstruction::CONS_SLEEP_4, true));
		}
		else // must destroy unless have priority
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x, y + 1, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x, y + 3, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x, y - 1, ChessConstruction::CONS_LIVE_3, false));
		}
	}
	a = board.getBoard(x - 1, y);
	b = board.getBoard(x + 3, y);
	if (a == Board::VOIDED && b == Board::VOIDED && cur == board.getBoard(x + 2, y) && board.getBoard(x + 1, y) == Board::VOIDED)
	{
		__DEBUG_DETECT__(x, y, "SLEEP_3_F1_D");
		if (getPlayer() == cur)	// win on next round
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 1, y, ChessConstruction::CONS_LIVE_3, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_SLEEP_4), DecisionOffer(x + 3, y, ChessConstruction::CONS_SLEEP_4, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_SLEEP_4), DecisionOffer(x - 1, y, ChessConstruction::CONS_SLEEP_4, true));
		}
		else // must destroy unless have priority
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 1, y, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 3, y, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x - 1, y, ChessConstruction::CONS_LIVE_3, false));
		}
	}
	a = board.getBoard(x - 1, y - 1);
	b = board.getBoard(x + 3, y + 3);
	if (a == Board::VOIDED && b == Board::VOIDED && cur == board.getBoard(x + 2, y + 2) && board.getBoard(x + 1, y + 1) == Board::VOIDED)
	{
		__DEBUG_DETECT__(x, y, "SLEEP_3_F1_RD");
		if (getPlayer() == cur)	// win on next round
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 1, y + 1, ChessConstruction::CONS_LIVE_3, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_SLEEP_4), DecisionOffer(x + 3, y + 3, ChessConstruction::CONS_SLEEP_4, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_SLEEP_4), DecisionOffer(x - 1, y - 1, ChessConstruction::CONS_SLEEP_4, true));
		}
		else // must destroy unless have priority
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 1, y + 1, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 3, y + 3, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x - 1, y - 1, ChessConstruction::CONS_LIVE_3, false));
		}
	}
	a = board.getBoard(x - 1, y + 1);
	b = board.getBoard(x + 3, y - 3);
	if (a == Board::VOIDED && b == Board::VOIDED && cur == board.getBoard(x + 2, y - 2) && board.getBoard(x + 1, y - 1) == Board::VOIDED)
	{
		__DEBUG_DETECT__(x, y, "SLEEP_3_F1_LD");
		if (getPlayer() == cur)	// win on next round
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 1, y - 1, ChessConstruction::CONS_LIVE_3, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_SLEEP_4), DecisionOffer(x + 3, y - 3, ChessConstruction::CONS_SLEEP_4, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_SLEEP_4), DecisionOffer(x - 1, y + 1, ChessConstruction::CONS_SLEEP_4, true));
		}
		else // must destroy unless have priority
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 1, y - 1, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 3, y - 3, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x - 1, y + 1, ChessConstruction::CONS_LIVE_3, false));
		}
	}

	// form 2:
	a = board.getBoard(x, y - 1);
	b = board.getBoard(x, y + 3);
	if (a == Board::VOIDED && b != cur && b != Board::VOIDED || b == Board::VOIDED && a != cur && a != Board::VOIDED)
	{
		if (cur == board.getBoard(x, y + 1) && cur == board.getBoard(x, y + 2))
		{
			__DEBUG_DETECT__(x, y, "SLEEP_3_F2_R");
			if (getPlayer() == cur)
			{
				if (a == Board::VOIDED)
				{
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x, y - 1, ChessConstruction::CONS_FORSTER_4, true));
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x, y - 2, ChessConstruction::CONS_FORSTER_4, true));
				}
				else
				{
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x, y + 3, ChessConstruction::CONS_FORSTER_4, true));
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x, y + 4, ChessConstruction::CONS_FORSTER_4, true));
				}
			}
			else
			{
				if (a == Board::VOIDED)
				{
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x, y - 1, ChessConstruction::CONS_FORSTER_4, false));
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x, y - 2, ChessConstruction::CONS_FORSTER_4, false));
				}
				else
				{
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x, y + 3, ChessConstruction::CONS_FORSTER_4, false));
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x, y + 4, ChessConstruction::CONS_FORSTER_4, false));
				}
			}
		}
	}
	a = board.getBoard(x - 1, y);
	b = board.getBoard(x + 3, y);
	if (a == Board::VOIDED && b != cur && b != Board::VOIDED || b == Board::VOIDED && a != cur && a != Board::VOIDED)
	{
		if (cur == board.getBoard(x + 1, y) && cur == board.getBoard(x + 2, y))
		{
			__DEBUG_DETECT__(x, y, "SLEEP_3_F2_D");
			if (getPlayer() == cur)
			{
				if (a == Board::VOIDED)
				{
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x - 1, y, ChessConstruction::CONS_FORSTER_4, true));
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x - 2, y, ChessConstruction::CONS_FORSTER_4, true));
				}
				else
				{
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x + 3, y, ChessConstruction::CONS_FORSTER_4, true));
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x + 4, y, ChessConstruction::CONS_FORSTER_4, true));
				}
			}
			else
			{
				if (a == Board::VOIDED)
				{
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x - 1, y, ChessConstruction::CONS_FORSTER_4, false));
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x - 2, y, ChessConstruction::CONS_FORSTER_4, false));
				}
				else
				{
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x + 3, y, ChessConstruction::CONS_FORSTER_4, false));
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x + 4, y, ChessConstruction::CONS_FORSTER_4, false));
				}
			}
		}
	}
	a = board.getBoard(x - 1, y - 1);
	b = board.getBoard(x + 3, y + 3);
	if (a == Board::VOIDED && b != cur && b != Board::VOIDED || b == Board::VOIDED && a != cur && a != Board::VOIDED)
	{
		if (cur == board.getBoard(x + 1, y + 1) && cur == board.getBoard(x + 2, y + 2))
		{
			__DEBUG_DETECT__(x, y, "SLEEP_3_F2_RD");
			if (getPlayer() == cur)
			{
				if (a == Board::VOIDED)
				{
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x - 1, y - 1, ChessConstruction::CONS_FORSTER_4, true));
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x - 2, y - 2, ChessConstruction::CONS_FORSTER_4, true));
				}
				else
				{
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x + 3, y + 3, ChessConstruction::CONS_FORSTER_4, true));
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x + 4, y + 4, ChessConstruction::CONS_FORSTER_4, true));
				}
			}
			else
			{
				if (a == Board::VOIDED)
				{
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x - 1, y - 1, ChessConstruction::CONS_FORSTER_4, false));
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x - 2, y - 2, ChessConstruction::CONS_FORSTER_4, false));
				}
				else
				{
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x + 3, y + 3, ChessConstruction::CONS_FORSTER_4, false));
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x + 4, y + 4, ChessConstruction::CONS_FORSTER_4, false));
				}
			}
		}
	}
	a = board.getBoard(x - 1, y + 1);
	b = board.getBoard(x + 3, y - 3);
	if (a == Board::VOIDED && b != cur && b != Board::VOIDED || b == Board::VOIDED && a != cur && a != Board::VOIDED)
	{
		if (cur == board.getBoard(x + 1, y - 1) && cur == board.getBoard(x + 2, y - 2))
		{
			__DEBUG_DETECT__(x, y, "SLEEP_3_F2_LD");
			if (getPlayer() == cur)
			{
				if (a == Board::VOIDED)
				{
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x - 1, y + 1, ChessConstruction::CONS_FORSTER_4, true));
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x - 2, y + 2, ChessConstruction::CONS_FORSTER_4, true));
				}
				else
				{
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x + 3, y - 3, ChessConstruction::CONS_FORSTER_4, true));
					addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x + 4, y - 4, ChessConstruction::CONS_FORSTER_4, true));
				}
			}
			else
			{
				if (a == Board::VOIDED)
				{
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x - 1, y + 1, ChessConstruction::CONS_FORSTER_4, false));
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x - 2, y + 2, ChessConstruction::CONS_FORSTER_4, false));
				}
				else
				{
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x + 3, y - 3, ChessConstruction::CONS_FORSTER_4, false));
					addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_FORSTER_4), DecisionOffer(x + 4, y - 4, ChessConstruction::CONS_FORSTER_4, false));
				}
			}
		}
	}
}

void KizunaAi::detect_CONS_LIVE_2(const Board& board, int x, int y)
{
	Board::ChessPlayer cur = board.getBoard(x, y);
	if (cur == Board::VOIDED) return;		// No chess here
	Board::ChessPlayer a, b;
	a = board.getBoard(x, y - 1);
	b = board.getBoard(x, y + 2);
	if (a == Board::VOIDED && b == Board::VOIDED && cur == board.getBoard(x, y + 1))
	{
		__DEBUG_DETECT__(x, y, "LIVE_2_R");
		if (getPlayer() == cur)
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_3), DecisionOffer(x, y + 1, ChessConstruction::CONS_LIVE_3, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_SLEEP_4), DecisionOffer(x, y + 2, ChessConstruction::CONS_SLEEP_4, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_SLEEP_4), DecisionOffer(x, y - 2, ChessConstruction::CONS_SLEEP_4, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_3), DecisionOffer(x, y - 1, ChessConstruction::CONS_LIVE_3, true));
		}
		else
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x, y + 1, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x, y + 2, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x, y - 2, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x, y - 1, ChessConstruction::CONS_LIVE_3, false));
		}
	}
	a = board.getBoard(x - 1, y);
	b = board.getBoard(x + 2, y);
	if (a == Board::VOIDED && b == Board::VOIDED && cur == board.getBoard(x + 1, y))
	{
		__DEBUG_DETECT__(x, y, "LIVE_2_D");
		if (getPlayer() == cur)
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 2, y, ChessConstruction::CONS_LIVE_3, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_SLEEP_4), DecisionOffer(x + 3, y, ChessConstruction::CONS_SLEEP_4, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_SLEEP_4), DecisionOffer(x - 2, y, ChessConstruction::CONS_SLEEP_4, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_3), DecisionOffer(x - 1, y, ChessConstruction::CONS_LIVE_3, true));
		}
		else
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 2, y, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 3, y, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x - 2, y, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x - 1, y, ChessConstruction::CONS_LIVE_3, false));
		}
	}
	a = board.getBoard(x - 1, y - 1);
	b = board.getBoard(x + 2, y + 2);
	if (a == Board::VOIDED && b == Board::VOIDED && cur == board.getBoard(x + 1, y + 1))
	{
		__DEBUG_DETECT__(x, y, "LIVE_2_RD");
		if (getPlayer() == cur)
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 2, y + 2, ChessConstruction::CONS_LIVE_3, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_SLEEP_4), DecisionOffer(x + 3, y + 3, ChessConstruction::CONS_SLEEP_4, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_SLEEP_4), DecisionOffer(x - 2, y - 2, ChessConstruction::CONS_SLEEP_4, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_3), DecisionOffer(x - 1, y - 1, ChessConstruction::CONS_LIVE_3, true));
		}
		else
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 2, y + 2, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 3, y + 3, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x - 2, y - 2, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x - 1, y - 1, ChessConstruction::CONS_LIVE_3, false));
		}
	}
	a = board.getBoard(x - 1, y + 1);
	b = board.getBoard(x + 2, y - 2);
	if (a == Board::VOIDED && b == Board::VOIDED && cur == board.getBoard(x + 1, y - 1))
	{
		__DEBUG_DETECT__(x, y, "LIVE_2_LD");
		if (getPlayer() == cur)
		{
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 2, y - 2, ChessConstruction::CONS_LIVE_3, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_SLEEP_4), DecisionOffer(x + 3, y - 3, ChessConstruction::CONS_SLEEP_4, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_SLEEP_4), DecisionOffer(x - 2, y + 2, ChessConstruction::CONS_SLEEP_4, true));
			addVal(this, &mem, board, std::make_pair(true, ChessConstruction::CONS_LIVE_3), DecisionOffer(x - 1, y + 1, ChessConstruction::CONS_LIVE_3, true));
		}
		else
		{
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 2, y - 2, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x + 3, y - 3, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x - 2, y + 2, ChessConstruction::CONS_LIVE_3, false));
			addVal(this, &mem, board, std::make_pair(false, ChessConstruction::CONS_LIVE_3), DecisionOffer(x - 1, y + 1, ChessConstruction::CONS_LIVE_3, false));
		}
	}
}

void KizunaAi::detect_CONS_SLEEP_2(const Board& board, int x, int y)
{
	Board::ChessPlayer cur = board.getBoard(x, y);
	if (cur == Board::VOIDED) return;		// No chess here
	// type 1: XOO / OOX

}
