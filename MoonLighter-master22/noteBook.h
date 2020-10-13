#pragma once
#include "item.h"
#define NOTEBOOKPOSX	230
#define NOTEBOOKPOSY	98

class noteBook
{
private:
	POINT _noteBookPos;		//노트북메뉴 위치 
	float _menuMoveSpeed;	//메뉴가 이동하는 속도 

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	POINT getNoteBookPos() { return _noteBookPos; }					//노트북 위치값 get함수
	void setNoteBookPosX(int posX) { _noteBookPos.x = posX; }		//노트북 x좌표 set함수
	void setNoteBookPosY(int posY) { _noteBookPos.y = posY; }		//노트북 y좌표 set함수 
	void initPos();
	void moveNoteBookLeft(int destPos);								//노트북을 destPos까지 좌측으로 이동시킴
	void moveNoteBookRight(int destPos);							//노트북을 destPos까지 우측으로 이동시킴
};


