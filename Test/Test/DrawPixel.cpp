#include "DxLib.h"
#include "vector"
#include "Vector2.h"
#include "Bord.h"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define BORD_SIZE 400
#define BORD_EDGE_NUM 8//盤面の端っこの配列番号(9*9なので[8]がMAX)
#define TEXT_WINDOW_POSX 500//テキストウィンドウの位置X
#define TEXT_WINDOW_POSY 150//テキストウィンドウの位置Y
bool gameEnd=false;
bool BeforeMouseState=false;
Bord bord;
Bord::STATE NowTurn_State = Bord::WHITE;
int white = GetColor(255, 255, 255);
int black = GetColor(0, 0, 0);
vector2 MoucePos;
vector2 pos;
std::vector<vector2> CansetPosAllay;
bool turnSkiped=false;//ターンが一度スキップされたか

//関数プロト宣言
void CanSetAllay_Reset();
bool MouseGetDown();
void lateTurn();
void MainLoop();
bool CheckCanSet(vector2 pos);
void DrawBord(Bord::STATE now_State[BORD_EDGE_NUM][BORD_EDGE_NUM]);
void ChackMousePoint();
void ClickSetStone();
void GameSet();
// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	SetMouseDispFlag(true);
	SetBackgroundColor(0,0,0,1);
	CanSetAllay_Reset();
	while (!gameEnd)
	{
		MainLoop();
	}
	GameSet();
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
void MainLoop()
{
	if (CheckHitKey(KEY_INPUT_ESCAPE)){
		//esc押したら強制終了
		gameEnd = true;
	}
	ClearDrawScreen();
	ChackMousePoint();
	if (MouseGetDown())
	{
		ClickSetStone();
	}
	if (NowTurn_State == Bord::STATE::WHITE) { DrawString(TEXT_WINDOW_POSX,TEXT_WINDOW_POSY,"白のターンです",white); }
	if (NowTurn_State == Bord::STATE::BLACK) { DrawString(TEXT_WINDOW_POSX,TEXT_WINDOW_POSY,"黒のターンです",white); }
	DrawBord(bord.stone_State);
	ScreenFlip();
}
void ClickSetStone()
{
//盤面内をクリックされていた場合
if((MoucePos.x>0 && MoucePos.x<bord.margin * BORD_EDGE_NUM) &&
	(MoucePos.y>0 && MoucePos.y<bord.margin * BORD_EDGE_NUM)) {
	//押された位置から盤面内のインデックス番号を算出
		vector2 posindex;
		posindex.SetValue(MoucePos.x/bord.margin,MoucePos.y/bord.margin);
		if (CheckCanSet(posindex)){
			//条件がそろって入れば手番の石を置く
			bord.setState(posindex,NowTurn_State);
			bord.flip(bord.GetCanflip(posindex,NowTurn_State),NowTurn_State);
			lateTurn();
			CanSetAllay_Reset();
		}
	}
}
bool CheckCanSet(vector2 pos) {
	for (int i = 0; i < CansetPosAllay.size(); i++)
	{
		if (pos.x == CansetPosAllay[i].x && pos.y == CansetPosAllay[i].y)
		{
			return true;
		}
	}
	return false;
}
void lateTurn()
{
	//ターンを経過させる
	if (NowTurn_State == Bord::STATE::BLACK)
	{
		NowTurn_State = Bord::STATE::WHITE;
		return;
	}
	if (NowTurn_State == Bord::STATE::WHITE)
	{
		NowTurn_State = Bord::STATE::BLACK;
		return;
	}
}
void DrawBord(Bord::STATE now_State[8][8])
{
	//8*8の盤面を描画
	DrawBox(0, 0, BORD_SIZE + 1, BORD_SIZE + 1, GetColor(0, 255, 0),true);
	for (int y=0;y<BORD_EDGE_NUM;y++) 
	{
		for (int x = 0; x < BORD_EDGE_NUM; x++)
		{
			//毎フレーム画を更新しているので線も毎フレーム描画
			DrawLine(bord.margin * x, 0, bord.margin * x, bord.margin*BORD_EDGE_NUM, black, 1);
			DrawLine(0, bord.margin * y, bord.margin*BORD_EDGE_NUM, bord.margin * y, black, 1);
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
			case Bord::STATE::CAN_SET:
				DrawCircle(bord.margin / 2 + (bord.margin * x), bord.margin / 2 + (bord.margin * y), bord.margin / 2, GetColor(255,0,0));
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
bool MouseGetDown()//MouseのGetDownがなかったので作った
{
	bool nowState = false;
	bool Get = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
	bool Changed = Get ^ (BeforeMouseState&Get);
	nowState = Changed ;
	BeforeMouseState = Get;
	return Changed;
}
void CanSetAllay_Reset()
{
	CansetPosAllay = bord.ReturnCanSetindex(NowTurn_State);
	if (CansetPosAllay.size() <= 0)
	{
		if (turnSkiped == true) { gameEnd = true; return; }//さっきもスキップされていたらどっちも置けないので試合終了
		turnSkiped = true;//スキップされた判定をONに
		lateTurn();//ターンを経過させてリセット
		CanSetAllay_Reset();//再度この関数を呼び出す
		return;
	}
	turnSkiped = false;
}
void GameSet() {//ゲーム終了後の処理
	DrawFormatString(TEXT_WINDOW_POSX,TEXT_WINDOW_POSY,white,"ゲーム終了");
	WaitKey();
}