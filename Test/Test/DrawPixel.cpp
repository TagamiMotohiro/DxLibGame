#include "DxLib.h"
#include "Vector2.h"
#include "Bord.h"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define BORD_SIZE 560
bool gameEnd=false;
bool BeforeMouseState=false;
Bord bord;
Bord::STATE Turn_Coller = Bord::WHITE;
int white = GetColor(255, 255, 255);
int black = GetColor(0, 0, 0);
vector2 MoucePos;
vector2 pos;
bool MouseGetDown();
void MainLoop();
bool ChackCanSet(vector2 index);
void DrawBord(Bord::STATE now_State[8][8]);
void ChackMousePoint();
void ClickSetStone();
// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	SetMouseDispFlag(true);
	SetBackgroundColor(0,255,0,1);
	while (!gameEnd)
	{
		MainLoop();
	}	
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
void MainLoop()
{
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		gameEnd = true;
	}
	ClearDrawScreen();
	if (MouseGetDown())
	{
		ClickSetStone();
	}
	ChackMousePoint();
	DrawBord(bord.stone_State);
	ScreenFlip();
}
void ClickSetStone()
{
//盤面内をクリックされていた場合
if((MoucePos.x>0 && MoucePos.x<bord.margin * 8) &&
	(MoucePos.y>0 && MoucePos.y<bord.margin * 8)) {
	//押された位置から盤面内のインデックス番号を算出
		vector2 posindex;
		posindex.setpos(MoucePos.x/bord.margin,MoucePos.y/bord.margin);
		if (ChackCanSet(posindex)) {
			//石が置かれていなければ手番の石を置く
			bord.setState(posindex,Turn_Coller );
			if(Turn_Coller==Bord::STATE::BLACK)
			{
				Turn_Coller = Bord::STATE::WHITE;
				return;
			}
			if (Turn_Coller == Bord::STATE::WHITE)
			{
				Turn_Coller = Bord::STATE::BLACK;
				return;
			}
		}
	}
}
bool ChackCanSet(vector2 index)
{
	bool canset = false;
	if (bord.GetState(index)==Bord::NONE)
	{
		canset = true;
	}
	return canset;
}
void DrawBord(Bord::STATE now_State[8][8])
{
	//8*8の盤面を描画
	for (int y=0;y<8;y++) 
	{
		for (int x = 0; x < 8; x++)
		{
			//毎フレーム画を更新しているので線も毎フレーム描画
			DrawLine(bord.margin * x, 0, bord.margin * x, bord.margin*8, black, 1);
			DrawLine(0, bord.margin * y, bord.margin*8, bord.margin * y, black, 1);
			//インデックス内の状態を見て石を描画
			switch (now_State[x][y])
			{
			case Bord::STATE::NONE:
				break;
			case Bord::STATE::BLACK:
				DrawCircle(bord.margin / 2 + (bord.margin * x), bord.margin / 2 + (bord.margin * y), bord.margin / 2, black);
				break;
			case Bord::STATE::WHITE:
				DrawCircle(bord.margin / 2 + (bord.margin * x), bord.margin / 2 + (bord.margin * y), bord.margin / 2, white);
				break;
			}
		}
		
	}
	//蓋の線は別途描画
	DrawLine(bord.margin * 8, 0, bord.margin * 8, bord.margin * 8, black, 1);
	DrawLine(0, bord.margin *8, bord.margin * 8, bord.margin * 8, black, 1);
}
void ChackMousePoint()
{
	//マウス座標チェック(デバッグ用)
	GetMousePoint(&MoucePos.x, &MoucePos.y);
	DrawFormatString(0, 0, white, "Mouse_X=%d::Mouse_Y=%d",MoucePos.x,MoucePos.y);
}
bool MouseGetDown()
{
	bool nowState = false;
	bool Get = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
	bool Changed = Get ^ (BeforeMouseState&Get);
	nowState = Changed ;
	BeforeMouseState = Get;
	return Changed;
}