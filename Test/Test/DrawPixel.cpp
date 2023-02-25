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
// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	SetMouseDispFlag(true);
	SetBackgroundColor(0,255,0,1);
	while (!gameEnd)
	{
		MainLoop();
	}	
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
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
	//8*8�̔Ֆʂ�`��
	for (int y=0;y<8;y++) 
	{
		for (int x = 0; x < 8; x++)
		{
			//���t���[������X�V���Ă���̂Ő������t���[���`��
			DrawLine(bord.margin * x, 0, bord.margin * x, bord.margin*8, black, 1);
			DrawLine(0, bord.margin * y, bord.margin*8, bord.margin * y, black, 1);
			//�C���f�b�N�X���̏�Ԃ����Đ΂�`��
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
	//�W�̐��͕ʓr�`��
	DrawLine(bord.margin * 8, 0, bord.margin * 8, bord.margin * 8, black, 1);
	DrawLine(0, bord.margin *8, bord.margin * 8, bord.margin * 8, black, 1);
	//�΂�u���Ƃ��̎�
	//�}�X���Ƃ̊Ԋu/2+(�}�X���Ƃ̊Ԋu*�C���f�b�N�X)
}
void ChackMousePoint()
{
	//�}�E�X���W�`�F�b�N(�f�o�b�O�p)
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