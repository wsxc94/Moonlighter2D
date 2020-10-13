#pragma once

enum ARROWDIRECTION
{
	ARROW_DOWN,
	ARROW_UP,
	ARROW_RIGHT,
	ARROW_LEFT
};

struct tagShadowArrow
{
	RECT rc;
	float x, y;
	int alpha;
};

class arrow
{
private:
	ARROWDIRECTION _arrowDirection;

	RECT _rc;
	int	_speed;
	int _alpha;
	float _x, _y;
	bool _isSkill;

	vector<tagShadowArrow> _shadowArrow;


public:

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	void animation();
	void shootArrow();
	void moveArrow();

	ARROWDIRECTION getArrowDirection() { return _arrowDirection; }
	RECT getRect() { return _rc; }
	int getAlpha() { return _alpha; }

	vector<tagShadowArrow> getShadowArrow() { return _shadowArrow; }
	void setShadowArrow(vector<tagShadowArrow> shadowArrow) { _shadowArrow = shadowArrow; }
	
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	void setAlpha(int alpha) { _alpha = alpha; }


};

