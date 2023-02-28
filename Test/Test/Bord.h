#pragma once
#include "Vector2.h"
#include "vector"
class Bord {
	public:
	//白い石が置かれているか黒い石が置かれているか判定する列挙型
	enum STATE
	{
		NONE = 0,
		WHITE = 1,
		BLACK = 2
	};
	const int margin = 50;
	void setState(vector2 index,Bord::STATE ChangeState)
	{
		stone_State[index.x][index.y] = ChangeState;
	}
	std::vector<vector2> ReturnCanSetindex(Bord::STATE Now_Turn)
	{
		std::vector<vector2> canSetIndex;
		vector2 index;
		index.setpos(1,1);
		canSetIndex.push_back(index);
		return canSetIndex;
	}
	STATE stone_State[8][8] ={STATE::NONE};
	STATE GetState(vector2 index)
	{
		return stone_State[index.x][index.y];
	}
	Bord();
	private:
	
};
Bord::Bord() {
	//最初から置かれている石をコンストラクタで設定
	stone_State[4][3] = { STATE::BLACK };
	stone_State[3][4] = { STATE::BLACK };
	stone_State[3][3] = { STATE::WHITE };
	stone_State[4][4] = { STATE::WHITE };
}
