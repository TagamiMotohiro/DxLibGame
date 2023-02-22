#include "DxLib.h"
#include "Vector2.h"
#include "Bord.h"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define BORD_SIZE 560
bool gameEnd=false;
Bord bord;
int white = GetColor(255, 255, 255);
vector2 MoucePos;
vector2 pos;
void MainLoop();
void DrawBord();
// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	SetMouseDispFlag(true);
	while (!gameEnd)
	{
		MainLoop();
	}	
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}

void MainLoop()
{
	if (CheckHitKeyAll(DX_CHECKINPUT_KEY)) {
		gameEnd = true;
	}
	DrawBord();
}
void DrawBord()
{
	//8*8�̔Ֆʂ�`��
	for (int y=0;y<9;y++) 
	{
		
		for (int x = 0; x < 9; x++)
		{
			DrawLine(bord.margin * x, 0, bord.margin * x, BORD_SIZE, white, 1);
			DrawLine(0, bord.margin * y, BORD_SIZE, bord.margin * y, white, 1);
		}
	}
}