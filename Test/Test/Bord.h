#pragma once
#include "Vector2.h"
class Bord {
	public:
	//�����΂��u����Ă��邩�����΂��u����Ă��邩���肷��񋓌^
	enum STATE
	{
		NONE = 0,
		WHITE = 1,
		BLACK = 2
	};
	const int margin = 50;
	STATE stone_State[8][8] ={STATE::NONE};
	void setState(vector2 index,STATE ChangeState)
	{
		stone_State[index.x][index.y] = ChangeState;
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
