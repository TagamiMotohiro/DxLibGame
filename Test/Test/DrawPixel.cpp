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
// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	while (!gameEnd)
	{
		MainLoop();
		WaitKey();				// �L�[���͑҂�
	}	
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
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
	DrawString(240,300,"�����Ƀ^�C�g��������",GetColor(255,255,255));
}