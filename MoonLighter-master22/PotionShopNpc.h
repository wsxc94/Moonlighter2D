#pragma once
#include "npc.h"
#include "potionShop.h"
class PotionShopNpc : public npc
{
private:
	potionShop* _potionShop;

public:
	HRESULT init(tagPosF pos, string key);

	void update();
	void render();
	void action(string talk);

	potionShop* getPotionShop() { return _potionShop; }

	PotionShopNpc() {}
	virtual ~PotionShopNpc() {}
};

