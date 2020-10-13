#pragma once
struct tagArrowSkill
{
	RECT rc;
	float x, y;
	int speed;
};
struct tagShiled
{
	RECT rc;
	float x, y;
	bool isColi;
};
enum SKILLSTATE
{
	SHILED,
	BOW_CHARGE
};
class skill
{
private:
	tagArrowSkill _arrowSkill;
	tagShiled _shiled;

	SKILLSTATE _state;

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	void animation();
	void skillset();

};

