#include "DxLib.h"
#include "Vector2.h"
#include "Bord.h"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define BORD_SIZE 560
bool gameEnd=false;
bool BeforeMouseState=false;
Bord bord;
int white = GetColor(255, 255, 255);
int black = GetColor(0, 0, 0);
vector2 MoucePos;
vector2 pos;
bool MouseGetDown();
void MainLoop();
void DrawBord(Bord::STATE now_State[8][8]);
void SetStone();
void ChackMousePoint();
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
		if((MoucePos.x>0 && MoucePos.x<bord.margin * 8) &&
		(MoucePos.y>0 && MoucePos.y<bord.margin * 8)) {
			vector2 posindex;
			posindex.x = MoucePos.x / bord.margin;
			posindex.y = MoucePos.y / bord.margin;
			bord.setState(posindex,Bord::STATE::BLACK);
		}
	}
	ChackMousePoint();
	DrawBord(bord.stone_State);
	ScreenFlip();
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
	//石を置くときの式
	//マスごとの間隔/2+(マスごとの間隔*インデックス)
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