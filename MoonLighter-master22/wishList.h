#pragma once
#include "item.h"
#define WISHLISTPOSX	232
#define WISHLISTPOSY	130

class wishList
{
private:
	POINT _wishListPos;		//위시리스트 위치 
	float _menuMoveSpeed;	//메뉴가 이동하는 속도 

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	POINT getWishListPos() { return _wishListPos; }					//위시리스트 위치값 get함수
	void setWishListPosX(int posX) { _wishListPos.x = posX; }		//위시리스트 x좌표 set함수
	void setWishListPosY(int posY) { _wishListPos.y = posY; }		//위시리스트 y좌표 set함수 
	void initPos();
	void moveWishListLeft(int destPos);								//위시리스트를 destPos까지 좌측으로 이동시킴						
	void moveWishListRight(int destPos);							//위시리스트를 destPos까지 우측으로 이동시킴
};

