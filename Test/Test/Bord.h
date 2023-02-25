#pragma once
#include "Vector2.h"
class Bord {
	public:
	//”’‚¢Î‚ª’u‚©‚ê‚Ä‚¢‚é‚©•‚¢Î‚ª’u‚©‚ê‚Ä‚¢‚é‚©”»’è‚·‚é—ñ‹“Œ^
	enum STATE
	{
		NONE = 0,
		WHITE = 1,
		BLACK = 2
	};
	const int margin = 50;
	STATE stone_State[8][8] ={STATE::NONE};
	void setState(vector2 index,STATE ChangeState)
	{
		stone_State[index.x][index.y] = ChangeState;
	}
	Bord();
	private:
	
};
Bord::Bord() {
	stone_State[4][3] = { STATE::BLACK };
	stone_State[3][4] = { STATE::BLACK };
	stone_State[3][3] = { STATE::WHITE };
	stone_State[4][4] = { STATE::WHITE };
}
