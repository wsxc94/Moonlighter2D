#pragma once
#include "singletonBase.h"

class dFont;

class damageFont : public singletonBase<damageFont>
{
private:
	
	vector<dFont*> _vFont;
public:
	damageFont() {}
	~damageFont() {}

	HRESULT init(int x, int y, int damage);
	void release();
	void update();
	void render(HDC hdc);

};

class dFont
{
private:
	image* _back;
	float _x;
	float _y;
	int _damage;
	int _time;
	int _alpha;
public:
	dFont() {}
	~dFont() {}

	HRESULT init(int x, int y, int damage);
	void release();
	bool update();
	void render(HDC hdc);
};
