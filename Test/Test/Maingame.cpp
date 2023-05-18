#include "DxLib.h"
#include "vector"
#include "Vector2.h"
#include "Bord.h"
#include <random>
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define BORD_SIZE 400
#define BORD_EDGE_NUM 8//�Ֆʂ̒[�����̔z��ԍ�(9*9�Ȃ̂�[8]��MAX)
#define TEXT_WINDOW_POSX 500//�e�L�X�g�E�B���h�E�̈ʒuX
#define TEXT_WINDOW_POSY 150//�e�L�X�g�E�B���h�E�̈ʒuY
#define NPC_THINKTIME_MAX 500//NPC���l���鎞��(1=1�t���[��)
bool gameEnd=false;//�Q�[�����I���������ǂ���
bool BeforeMouseState=false;//�}�E�X��ԊǗ��p
Bord bord;//�ՖʊǗ��N���X�̃C���X�^���X
Bord::STATE NowTurn_State = Bord::WHITE;//���ǂ���̃^�[����

//NPC���l���n�߂Ă���o�߂�������
int NPCthinkTime=0;

//�F�擾�p
int white = GetColor(255, 255, 255);
int black = GetColor(0, 0, 0);

//�}�E�X�ʒu�m�F�p
vector2 MoucePos;

//�z�u�ł���ʒu�����X�g�����ĕ��ׂ�����
std::vector<vector2> CansetPosAllay;

bool turnSkiped=false;//�^�[������x�X�L�b�v���ꂽ��
//�֐��v���g�錾
void CanSetAllay_Reset();
bool MouseGetDown();
void lateTurn();
void MainLoop();
bool CheckCanSet(vector2 pos);
void DrawBord(Bord::STATE now_State[BORD_EDGE_NUM][BORD_EDGE_NUM]);
void ChackMousePoint();
void ClickSetStone();
void EnemyTurn();
void GameSet();
int GetRandom(int min,int max);
// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	SetMouseDispFlag(true);
	SetBackgroundColor(0,0,0,1);
	CanSetAllay_Reset();
	while (!gameEnd)
	{
		MainLoop();
	}
	GameSet();
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}
void MainLoop()
{
	if (CheckHitKey(KEY_INPUT_ESCAPE)){
		//esc�������狭���I��
		gameEnd = true;
	}
	if (NowTurn_State == Bord::STATE::BLACK)//�������̃^�[����������
	{
		//�G�̃^�[��
		EnemyTurn();
	}
	else
	{
		//�����łȂ��ꍇ�v���C���[�̃^�[��
		//�N���b�N�Ő΂�u��
		ClearDrawScreen();
		ChackMousePoint();
		if (MouseGetDown())
		{
			ClickSetStone();
		}
	}
	
	if (NowTurn_State == Bord::STATE::WHITE) { DrawString(TEXT_WINDOW_POSX,TEXT_WINDOW_POSY,"���̃^�[���ł�",white); }
	if (NowTurn_State == Bord::STATE::BLACK) { DrawString(TEXT_WINDOW_POSX,TEXT_WINDOW_POSY,"���̃^�[���ł�",white); }
	DrawBord(bord.stone_State);
	ScreenFlip();
}
void ClickSetStone()
{
//�Ֆʓ����N���b�N����Ă����ꍇ
if((MoucePos.x>0 && MoucePos.x<bord.margin * BORD_EDGE_NUM) &&
	(MoucePos.y>0 && MoucePos.y<bord.margin * BORD_EDGE_NUM)) {
	//�����ꂽ�ʒu����Ֆʓ��̃C���f�b�N�X�ԍ����Z�o
		vector2 posindex;
		posindex.SetValue(MoucePos.x/bord.margin,MoucePos.y/bord.margin);
		if (CheckCanSet(posindex)){
			//������������ē���Ύ�Ԃ̐΂�u��
			bord.setState(posindex,NowTurn_State);
			bord.flip(bord.GetCanflip(posindex,NowTurn_State),NowTurn_State);
			lateTurn();
			CanSetAllay_Reset();
		}
	}
}
bool CheckCanSet(vector2 pos) {
	//�N���b�N�Ŏw�肳�ꂽ�ʒu�ԍ��Ɠ����ʒu�ԍ����Z�b�g�\��ԂɂȂ��Ă�����true
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
	//�^�[�����o�߂�����
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
void EnemyTurn()
{
	//�l�����Ԃ��v���X
	NPCthinkTime++;
	//�l�����Ԃ�MAX�ɂȂ�����
	if (NPCthinkTime>=NPC_THINKTIME_MAX) {
		NPCthinkTime = 0;
		//�ǂ�������ɐ΂�u��
		vector2 decisionPos = CansetPosAllay[GetRandom(0,CansetPosAllay.size()-1)];
		bord.setState(decisionPos,NowTurn_State);
		bord.flip(bord.GetCanflip(decisionPos,NowTurn_State), NowTurn_State);
		//�^�[���I���̏���
		lateTurn();
		CanSetAllay_Reset();
	}
}
void CanSetAllay_Reset()
{
	CansetPosAllay = bord.ReturnCanSetindex(NowTurn_State);
	if (CansetPosAllay.size() <= 0)
	{
		if (turnSkiped == true) { gameEnd = true; return; }//���������X�L�b�v����Ă�����ǂ������u���Ȃ��̂Ŏ����I��
		turnSkiped = true;//�X�L�b�v���ꂽ�����ON��
		lateTurn();//�^�[�����o�߂����ă��Z�b�g
		CanSetAllay_Reset();//�ēx���̊֐����Ăяo��
		return;
	}
	turnSkiped = false;
}
void DrawBord(Bord::STATE now_State[8][8])
{
	//8*8�̔Ֆʂ�`��
	DrawBox(0, 0, BORD_SIZE + 1, BORD_SIZE + 1, GetColor(0, 255, 0),true);
	for (int y=0;y<BORD_EDGE_NUM;y++) 
	{
		for (int x = 0; x < BORD_EDGE_NUM; x++)
		{
			//���t���[������X�V���Ă���̂Ő������t���[���`��
			DrawLine(bord.margin * x, 0, bord.margin * x, bord.margin*BORD_EDGE_NUM, black, 1);
			DrawLine(0, bord.margin * y, bord.margin*BORD_EDGE_NUM, bord.margin * y, black, 1);
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
			case Bord::STATE::CAN_SET:
				DrawCircle(bord.margin / 2 + (bord.margin * x), bord.margin / 2 + (bord.margin * y), bord.margin / 2, GetColor(255,0,0));
				break;
			}
		}
		
	}
	//�W�̐��͕ʓr�`��
	DrawLine(bord.margin * 8, 0, bord.margin * 8, bord.margin * 8, black, 1);
	DrawLine(0, bord.margin *8, bord.margin * 8, bord.margin * 8, black, 1);
}
void GameSet() {//�Q�[���I����̏���
	DrawFormatString(TEXT_WINDOW_POSX,TEXT_WINDOW_POSY,white,"�Q�[���I��");
	WaitKey();
}
bool MouseGetDown()//Mouse��GetDown���Ȃ������̂ō����
{
	bool nowState = false;
	bool Get = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
	bool Changed = Get ^ (BeforeMouseState&Get);
	nowState = Changed ;
	BeforeMouseState = Get;
	return Changed;
}
void ChackMousePoint()
{
	//�}�E�X���W�`�F�b�N(�f�o�b�O�p)
	GetMousePoint(&MoucePos.x, &MoucePos.y);
	DrawFormatString(0, 0, white, "Mouse_X=%d::Mouse_Y=%d",MoucePos.x,MoucePos.y);
}
int GetRandom(int min,int max)
{
	std::random_device randomdevice;
	//�����_�������f�o�C�X��������
	std::mt19937 mt(randomdevice());
	//�����Z���k�E�c�C�X�^�[�G���W����������
	std::uniform_int_distribution<int> ditribution(min,max);
	//�����̕��z��ݒ�
	return ditribution(mt);
	//�l��Ԃ�
}