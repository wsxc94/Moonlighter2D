#pragma once

typedef struct
{
	image *img;
	int animTimer;
	int frameUnit;
	int idx;
	int selectIdx;

}selectMenuUnit;

class selectMenu 
{
private:
	selectMenuUnit _selectMenu;

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc, int destX, int destY);

	//get함수 
	image *getImg() { return _selectMenu.img; }
	int getIdx() { return _selectMenu.idx; }
	int getSelectIdx() { return _selectMenu.selectIdx; }

	//set함수 
	void setSelectIdx(int idx) { _selectMenu.selectIdx = idx; }

	//애니메이션 함수
	void setMenuState(int selectIdx);
	void animation();
	void yesAnim();
	void noAnim();


};

