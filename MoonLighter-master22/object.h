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
enum DOORKIND
{
	DOOR_NOMAL,DOOR_BOSS
};
class dgDoor : public gameNode
{
protected:
	DOORKIND _kind;
	bool _isActivate;
	bool _isOpen;
	bool _isClose;
	animation* _open;
	animation* _close;
	RECT _rc;
	int _x, _y;
	string _imgKey;
	string _imgTopKey;
	int _frameY;
public:
	virtual HRESULT init(int x, int y, string key,string topkey, int frameY);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void openPlay();
	virtual void closePlay();
	virtual void setIsActivate(bool isA) { _isActivate = isA; }
	virtual bool getIsActivate() { return _isActivate; }
	virtual RECT getRect() { return _rc; }
	virtual int getId() { return _open->getCurIndex(); }
	virtual DOORKIND getDoorKind() { return _kind; }
};

class dgBossDoor : public dgDoor
{
public:
	HRESULT init(int x, int y, string key, string topkey, int frameY);
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