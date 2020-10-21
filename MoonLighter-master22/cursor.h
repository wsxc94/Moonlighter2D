#pragma once

//커서 상태 
enum CURSOR_STATE
{
	CURSOR_IDLE,
	CURSOR_INACTIVE,
	CURSOR_MOVE,
	CURSOR_CLICK,
	CURSOR_PRICE_OFF,
	CURSOR_PRICE_ON,
	CURSOR_SELECT_IDLE,
	CURSOR_SELECT_MOVE
};

//커서 구조체 
typedef struct
{
	CURSOR_STATE state;		//현재 커서의 상태(애니메이션용)
	image *img;				//커서 이미지 
	int idx;				//애니메이션 인덱스X 
	int slotIdx;			//커서가 위치한 슬롯 인덱스
	int clickTime;			//커서가 클릭한 시간
	int destX;				//출력용 X좌표 
	int destY;				//출력용 Y좌표

}cursorUnit;

class cursor 
{
private:
	cursorUnit _cursor;

	int _animTimer;
	int _moveFrameUnit;
	int _clickFrameUnit;

	bool _showCursor;

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	//get 함수 
	cursorUnit getCursor() { return _cursor; }
	CURSOR_STATE getState() { return _cursor.state; }
	image *getImg() { return _cursor.img; }
	int getIdx() { return _cursor.idx; }
	int getSlotIdx() { return _cursor.slotIdx; }
	int getClickTime() { return _cursor.clickTime; }
	bool getShowCursor() { return _showCursor; }
	int getDestX() { return _cursor.destX; }
	int getDestY() { return _cursor.destY; }

	//set 함수 
	void setSlotIdx(int idx) { _cursor.slotIdx = idx; }
	void setClickTime(int clickTime) { _cursor.clickTime = clickTime; }
	void setShowCursor(bool value) { _showCursor = value; }
	void setDestX(int destX) { _cursor.destX = destX; }
	void setDestY(int destY) { _cursor.destY = destY; }
	void setDestPos(int destX, int destY) { _cursor.destX = destX; _cursor.destY = destY; }

	//animation 함수 
	void setCursorState(CURSOR_STATE state);	//커서의 상태 setting 함수(커서 애니메이션 변경에 사용)
	void animation();							//커서 애니메이션 함수 
	void moveAnim();							//커서 이동 애니메이션 
	void clickAnim();							//커서 클릭 애니메이션 
	void selectMoveAnim();						//네,아니오 선택 메뉴의 커서 이동 애니메이션 
};

