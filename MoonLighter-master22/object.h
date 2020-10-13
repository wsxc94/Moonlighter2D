#pragma once
#include "gameNode.h"

class object : public gameNode
{
protected:
	RECT _rc;
	string _imgKey;
	
public:

	virtual HRESULT init(int x, int y,string key);
	virtual void release();
	virtual void update();
	virtual void render();

};

class dgDoor : public gameNode
{
private:
	bool _isActivate;
	bool _isOpen;
	bool _isClose;
	animation* _open;
	animation* _close;
	RECT _rc;
	string _imgKey;
public:
	HRESULT init(int x, int y, string key, int frameY);
	void release();
	void update();
	void render();

	void openPlay();
	void closePlay();
	void setIsActivate(bool isA) { _isActivate = isA; }
	bool getIsActivate() { return _isActivate; }
	RECT getRect() { return _rc; }
	int getId() { return _open->getCurIndex(); }
};

class dgChest : public object
{
private:
	animation* _open;
public:
	HRESULT init(int x, int y, string key);
	void release();
	void update();
	void render();

	void openPlay();
};