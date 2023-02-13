#include "DxLib.h"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

bool gameEnd=false;
struct vector2
{
	int x = 0;
	int y = 0;
};
vector2 pos;
void MainLoop();
void Title();
// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	while (!gameEnd)
	{
		MainLoop();
		WaitKey();				// キー入力待ち
	}	
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

void MainLoop()
{
	Title();
	DrawPixel(pos.x,pos.y,GetColor(255,255,255));
	pos.x++;
	pos.y++;
	if (pos.x >= 10)
	{
		gameEnd = true;
	}
}
void Title()
{
	DrawString(240,300,"ここにタイトルを書く",GetColor(255,255,255));
}