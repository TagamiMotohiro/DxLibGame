#pragma once

class vector2
{
public:
	int x;
	int y;
	vector2();
	~vector2();
	void setpos(int inputX,int inputY)
	{
	x = inputX;
	y = inputY;
	}
private:
		
};
	
vector2::vector2()
{
	//�R���X�g���N�^�ŏ�����
	this->x = 0;
	this->y = 0;
}

vector2::~vector2()
{
}
