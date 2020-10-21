#pragma once
#include "npc.h"
#include "potionShop.h"
class PotionShopNpc : public npc
{
private:
	potionShop* _potionShop;

public:
	virtual HRESULT init(tagPosF pos, string key);
	virtual void update();
	virtual void render();

	virtual void action(string talk);

	potionShop* getPotionShop() { return _potionShop; }
};

