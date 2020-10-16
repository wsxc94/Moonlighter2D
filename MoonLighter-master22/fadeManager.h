#pragma once

typedef struct
{
	FADEKIND kind;
	int fadeUnit;
	int alpha;
	int destAlpha;
	bool isActive;

}tagFade;

class fadeManager
{
private:
	tagFade _fadeInfo;
	int _timer;

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	void fadeInit(int fadeUnit, FADEKIND kind);
	void fadeInit(int fadeUnit, FADEKIND kind, int destAlpha);

};

