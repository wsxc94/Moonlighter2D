#pragma once
#define CALENDARPOSX	130
#define CALENDARPOSY	92

class calendar
{
private:
	POINT _calendarPos;		//캘린더메뉴 위치 
	float _menuMoveSpeed;	//메뉴가 이동하는 속도 

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	POINT getCalendarPos() { return _calendarPos; }					//캘린더 위치값 get함수
	void setCalendarPosX(int posX) { _calendarPos.x = posX; }		//캘린더 x좌표 set함수
	void setCalendarPosY(int posY) { _calendarPos.y = posY; }		//캘린더 y좌표 set함수 
	void initPos();													//캘린더 위치값 초기화 
	void moveCalendarLeft(int destPos);								//캘린더를 destPos까지 좌측으로 이동시킴						
	void moveCalendarRight(int destPos);							//캘린더를 destPos까지 우측으로 이동시킴

};

