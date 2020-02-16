#pragma once

enum class ChessConstruction
{
	CONS_VOID = -1,
	CONS_APPEND_5 = 0,		// Construction of live five type, only saved for special.
	CONS_LIVE_4 = 1,		// Construction of live four type.
	CONS_FORSTER_4 = 2,
	CONS_SLEEP_4 = 3,
	CONS_LIVE_3 = 4,
	CONS_SLEEP_3 = 5,
	CONS_LIVE_2 = 6,
	CONS_SLEEP_2 = 7
};