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
				if (GetCanflip(index, nowTurn).size()>0){//探索用の関数を使って一つでも返せるようだったらそこは返せる位置ということ
					canSetIndex.push_back(index);//その位置を返す
					stone_State[index.x][index.y] = STATE::CAN_SET;
				}
			}
		}
		return canSetIndex;
	}
	void flip(std::vector<vector2> FlipStone,STATE FlipState)
	{
		for (int i = 0; i < FlipStone.size(); i++)
		{
			stone_State[FlipStone[i].x][FlipStone[i].y] = FlipState;
		}
	}
	std::vector<vector2> GetCanflip(vector2 Start_Pos/*調べるスタート地点*/,STATE nowTurn)
	{
		vector2 Line_Vec;
		std::vector<vector2> CanflipList;
		Line_Vec.SetValue(-1,-1);
		//各方向に対して石を返せる配置になっているか探索(8方向)
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == 0 && j == 0) { continue; }//(0,0)は調べる意味がないので飛ばす
				vector2 vec;
				vec.SetValue(i,j);
				Line_Search(Start_Pos, vec, nowTurn, CanflipList);
			}
		}
		return CanflipList;
	}
	void Line_Search(vector2 index/*探索開始位置*/,vector2 Search＿Vector/*探索を行う線上(0,1)なら上方向*/,
		STATE now_Turn,std::vector<vector2>& Output/*結果を渡す配列(ポインタ参照)*/)
	{

		/*Tips:動的可変長配列同士は例え型が同じでも代入できない*/

		//1ライン分探索を行う関数
		bool CanFlip=true;
		STATE opponent_Stone;
		std::vector<vector2> Canflip_List;//(仮置き)返すことができる位置情報のリスト
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
		while (CanFlip)
		{
			//現在の探索位置から探索方向にずらす
			index.PlusValue(Search＿Vector.x, Search＿Vector.y);
			if (index.x < 0 || index.y < 0||index.x > 9 || index.y > 9) { return; }//盤面範囲外に出ていないかチェック
			if (stone_State[index.x][index.y] == opponent_Stone)//探索位置の石が相手のものであったら
			{
				Canflip_List.push_back(index);
			}
			else
			{
				if (stone_State[index.x][index.y] == STATE::NONE) { return; }//置いていない場所に到達した場合結局返せないのでreturn
				if (stone_State[index.x][index.y] == now_Turn) { CanFlip = false; }//自ターンの色に到達した場合挟めている
			}
		}
		for (int i = 0; i < Canflip_List.size(); i++)
		{
			Output.push_back(Canflip_List[i]);//最終的な結果を出力　これで1ライン分の探索終了
		}
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
