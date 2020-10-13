#include "stdafx.h"
#include "wishList.h"

HRESULT wishList::init()
{
	//위시리스트메뉴 위치 초기화 
	initPos();

	_menuMoveSpeed = 90.f;

	return S_OK;
}

void wishList::release()
{
}

void wishList::update()
{
}

void wishList::render(HDC hdc)
{
	IMAGEMANAGER->findImage("menu_wishlist")->render(hdc, _wishListPos.x, _wishListPos.y);
}

void wishList::initPos()
{
	_wishListPos.x = -IMAGEMANAGER->findImage("menu_wishlist")->getWidth();
	_wishListPos.y = WISHLISTPOSY;
}

void wishList::moveWishListLeft(int destPos)
{
	_wishListPos.x -= _menuMoveSpeed;

	if (_wishListPos.x <= destPos)
	{
		_wishListPos.x = destPos;
	}
}

void wishList::moveWishListRight(int destPos)
{
	_wishListPos.x += _menuMoveSpeed;

	if (_wishListPos.x >= destPos)
	{
		_wishListPos.x = destPos;
	}
}
