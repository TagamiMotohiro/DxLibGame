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
		BLACK = 2,
		CAN_SET=3
	};
	const int margin = 50;
	STATE stone_State[8][8] ={STATE::NONE};
	void setState(vector2 index,Bord::STATE ChangeState)
	{
		stone_State[index.x][index.y] = ChangeState;
	}
	std::vector<vector2> ReturnCanSetindex(STATE nowTurn)
	{
		std::vector<vector2> canSetIndex;
		vector2 index;
		for (int y = 0; y<8; y++) {
			for (int x = 0; x < 8; x++) {
				if (stone_State[x][y] == STATE::CAN_SET) { stone_State[x][y] = STATE::NONE; }
				if (stone_State[x][y] != STATE::NONE) { continue; }
				index.SetValue(x, y);
				if (CheckCanSet(index, nowTurn)) {
					canSetIndex.push_back(index);
				}
			}
		}
		return canSetIndex;
	}
	bool CheckCanSet(vector2 index,STATE nowTurn)
	{
		bool Canset=false;
		vector2 Line_Vec;
		Line_Vec.SetValue(-1,-1);
		//各方向に対して石を返せる配置になっているか探索(8方向)
		for (int i = 0; i < 2; i++) { 
			if (Line_Search(index, Line_Vec, nowTurn)) { Canset = true; }
			Line_Vec.PlusValue(1,0);
		}
		for (int i = 0; i < 2; i++) {
			if (Line_Search(index, Line_Vec, nowTurn)) { Canset = true; }
			Line_Vec.PlusValue(0,1);
		}
		for (int i = 0; i < 2; i++) {
			if (Line_Search(index, Line_Vec, nowTurn)) { Canset = true; }
			Line_Vec.PlusValue(-1,0);
		}
		for (int i = 0; i < 2; i++) {
			if (Line_Search(index, Line_Vec, nowTurn)) { Canset = true; }
			Line_Vec.PlusValue(0, -1);
		}
		return Canset;
	}
	bool Line_Search(vector2 index,vector2 Search_Line,STATE now_Turn)
	{
		//1ライン分探索を行う関数
		vector2 a = index;
		bool CanFlip=false;
		STATE opponent_Stone;
		//現在のターンの状態から探索する状態を算出
		if(now_Turn==STATE::BLACK)
		{
			//黒だったら白
			opponent_Stone = STATE::WHITE;
		}
		else
		{
			//白だったら黒を探す
			opponent_Stone = STATE::BLACK;
		}
		do
		{
			
			index.PlusValue(Search_Line.x, Search_Line.y);
			if (index.x < 0 || index.y < 0) { return false; }
			if (index.x > 9 || index.y > 9) { return false; }
			if(CanFlip&&stone_State[index.x][index.y]==now_Turn) {
				stone_State[a.x][a.y] = STATE::CAN_SET;
				return true;
			}
			if (stone_State[index.x][index.y]==opponent_Stone)
			{
				CanFlip=true;
			}
		} while(CanFlip);
		return false;
	}
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
