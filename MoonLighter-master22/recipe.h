#pragma once
#include "itemManager.h"

typedef struct
{
	gameItem item;
	int itemIdx;
	int count;
	int possession;

}materialUnit;

class material
{
private:
	materialUnit _material;

public:
	//재료 초기화 
	HRESULT init(int itemIdx, int count);
	void release();
	void update();
	void render(HDC hdc);

	//get함수 
	gameItem getItem() { return _material.item; }
	int getCount() { return _material.count; }
	int updatePossession();

	//set함수 
	void setCount(int count) { _material.count = count; }
	void setPossession(int count) { _material.possession = count; }

};

class recipe
{
private:
	typedef vector<material*> vMaterial;
	vMaterial _vMaterial;

private:
	char _materialName[64];

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc, int destX, int destY);

	//get함수 
	vector<material*> getMeterial() { return _vMaterial; }

	void addMaterial(int itemIdx, int count);		//레시피에 재료 추가하기 
	bool checkMaterialOX(int index);				//해당 재료의 조건충족여부 확인
	bool checkRequirements();						//전체 재료의 조건충족여부 확인 
	int getMaxProduceBasedOnMaterial();				//소지 중인 재료를 기반으로 최대제조개수 구하기 
	const char *getShortMaterialName();				//부족한 재료의 이름 받기 

	//렌더함수 
	void recipeIconRender(HDC hdc, int destX, int destY);
	void recipeTextRender(HDC hdc, int destX, int destY);
	void recipeOXIconRender(HDC hdc, int destX, int destY);
	void recipeCountRender(HDC hdc, int destX, int destY);
	void countRender(HDC hdc, int destX, int destY, int count);
};

