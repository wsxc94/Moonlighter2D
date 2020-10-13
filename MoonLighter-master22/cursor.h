#pragma once

//커서 상태 
enum CURSOR_STATE
{
	CURSOR_IDLE,
	CURSOR_MOVE,
	CURSOR_CLICK,
	CURSOR_PRICE_OFF,
	CURSOR_PRICE_ON
};

//커서 구조체 
typedef struct
{
	CURSOR_STATE state;		//현재 커서의 상태(애니메이션용)
	image *img;				//커서 이미지 
	int idx;				//애니메이션 인덱스X 
	int slotIdx;			//커서가 위치한 슬롯 인덱스 
	int clickTime;			//커서가 클릭한 시간

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
	image *getImg() { return _cursor.img; }
	int getIdx() { return _cursor.idx; }
	int getSlotIdx() { return _cursor.slotIdx; }
	int getClickTime() { return _cursor.clickTime; }
	bool getShowCursor() { return _showCursor; }

	//set 함수 
	void setSlotIdx(int idx) { _cursor.slotIdx = idx; }
	void setClickTime(int clickTime) { _cursor.clickTime = clickTime; }
	void setShowCursor(bool value) { _showCursor = value; }

	//animation 함수 
	void setCursorState(CURSOR_STATE state);	//커서의 상태 setting 함수(커서 애니메이션 변경에 사용)
	void animation();							//커서 애니메이션 함수 
	void idleAnim();							//커서 idle 애니메이션 
	void moveAnim();							//커서 move 애니메이션 
	void clickAnim();							//커서 click 애니메이션 
};

