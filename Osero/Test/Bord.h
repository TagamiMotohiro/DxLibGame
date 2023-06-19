#pragma once
#include "Vector2.h"
#include "vector"
#include "Macro.h"
class Bord {
	//5/26�Ƀr�T�C�h�l�ɂ���������FB
	//header�t�@�C���ɏ����𒼏������Ȃ�(�Q�ƌo�H�I�ɏd���Ȃ�̂�)
	//header�t�@�C���ɂ͐錾����������Cpp�t�@�C���ɏ�����u�����Ƃœǂݍ��݂����点��
	//�}�N��(orConst�ϐ�)�����Ǝg���Ă�����
	//�֐��Œl�������Ă���Ƃ��͊ۃR�s�[�ł͂Ȃ�const�Q�Ƃ��g����
	//�t�@�C���쐬��������̂ق��ɏ�����
	//�֐��̍ŏ��ɐ����̃R�����g��������
	//���̎��X�ōŐV�ōœK�ȏ�����������Ă��邩(�֐����Â������Ȃ����Ȃ�)�����厖
	
	//�����̊��z
	//�V���v���Ɏ����̃R�[�h���z������ĂȂ��������⍡�܂ł̃N�Z��C#�߂̃R�[�h�������Ă���Ƃ��낪�U�����ꂽ
	public:
	const int margin = 50;
	//�����΂��u����Ă��邩�����΂��u����Ă��邩���肷��񋓌^
	enum STATE
	{
		NONE = 0,//�����u���Ă��Ȃ�
		WHITE = 1,//�����u����Ă���
		BLACK = 2,//�����u����Ă���
		CAN_SET=3//���̃^�[���΂�u�����Ƃ��ł���
	};
	STATE stone_State[8][8] = { STATE::NONE };
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
				if (GetCanflip(index, nowTurn).size()>0){//�T���p�̊֐����g���Ĉ�ł��Ԃ���悤�������炻���͕Ԃ���ʒu�Ƃ�������
					canSetIndex.push_back(index);//���̈ʒu��Ԃ�
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
	int CountStone(STATE CountState)//�Ֆʓ��̐΂̐����J�E���g
	{
		int result = 0;
		for (int x = 0; x < BORD_EDGE_NUM; x++) {
			for (int y = 0; y < BORD_EDGE_NUM; y++) {
				if (stone_State[x][y] == CountState) {
					result++;
				}
			}
		}
		return result;
	}
	std::vector<vector2> GetCanflip(vector2 Start_Pos/*���ׂ�X�^�[�g�n�_*/,STATE nowTurn/*���݂̃^�[��*/)
	{
		vector2 Line_Vec;
		std::vector<vector2> CanflipList;
		Line_Vec.SetValue(-1,-1);
		//�e�����ɑ΂��Đ΂�Ԃ���z�u�ɂȂ��Ă��邩�T��(8����)
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == 0 && j == 0) { continue; }//(0,0)�͒��ׂ�Ӗ����Ȃ��̂Ŕ�΂�
				vector2 vec;
				vec.SetValue(i,j);
				Line_Search(Start_Pos, vec, nowTurn, CanflipList);
			}
		}
		return CanflipList;
	}
	void Line_Search(vector2 index/*�T���J�n�ʒu*/,vector2 Search�QVector/*�T�����s������(0,1)�Ȃ�����*/,
		STATE nowTurn,std::vector<vector2>& Output/*���ʂ�n���z��(�|�C���^�Q��)*/)
	{
		
		/*Tips:���I�ϒ��z�񓯎m�͗Ⴆ�^�������ł�����ł��Ȃ�*/
		
		STATE opponent_Stone;
		//�����Ă������݂̃^�[���̏�Ԃ���T���΂��Z�o
		if (nowTurn == STATE::BLACK) { opponent_Stone = STATE::WHITE; }
		if (nowTurn == STATE::WHITE) { opponent_Stone = STATE::BLACK; }
		//1���C�����T�����s���֐�
		bool CanFlip=true;
		std::vector<vector2> Canflip_List;//(���u��)�Ԃ����Ƃ��ł���ʒu���̃��X�g
		//���݂̃^�[���̏�Ԃ���T�������Ԃ��Z�o
		while (CanFlip)
		{
			//���݂̒T���ʒu����T�������ɂ��炷
			index.PlusValue(Search�QVector.x, Search�QVector.y);
			if (index.x < 0 || index.y < 0||index.x >= 8 || index.y >= 8) { return; }//�Ֆʔ͈͊O�ɏo�Ă��Ȃ����`�F�b�N
			if (stone_State[index.x][index.y] == opponent_Stone)//�T���ʒu�̐΂�����̂��̂ł�������
			{
				Canflip_List.push_back(index);
			}
			else
			{
				if (stone_State[index.x][index.y] == STATE::NONE||
					stone_State[index.x][index.y] == STATE::CAN_SET ) { return; }//�u���Ă��Ȃ��ꏊ�ɓ��B�����ꍇ���ǕԂ��Ȃ��̂�return
				if (stone_State[index.x][index.y] == nowTurn) { CanFlip = false; }//���^�[���̐F�ɓ��B�����ꍇ���߂Ă���
			}
		}
		for (int i = 0; i < Canflip_List.size(); i++)
		{
			Output.push_back(Canflip_List[i]);//�ŏI�I�Ȍ��ʂ��o�́@�����1���C�����̒T���I��
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
	//�ŏ�����u����Ă���΂��R���X�g���N�^�Őݒ�
	stone_State[4][3] = { STATE::BLACK };
	stone_State[3][4] = { STATE::BLACK };
	stone_State[3][3] = { STATE::WHITE };
	stone_State[4][4] = { STATE::WHITE };
}
