#include "stdafx.h"
#include "item.h"

void Item::setNomalData(vector<string>& In)
{
	setCount(atoi(In[eNomalItem_count].c_str()));
	setItemKey(In[eNomalItem_itemKey].c_str());
	setNameKey(In[eNomalItem_nameKey].c_str());
	setDesKey(In[eNomalItem_desKey].c_str());
	setName(In[eNomalItem_name].c_str());
	setItemIdx(atoi(In[eNomalItem_itemIdx].c_str()));
	setCheapPrice(atoi(In[eNomalItem_cheapPrice].c_str()));
	setReasonablePrice(atoi(In[eNomalItem_reasonablePrice].c_str()));
	setExpensivePrice(atoi(In[eNomalItem_expensivePrice].c_str()));
	setOutrageousPrice(atoi(In[eNomalItem_outrageousPrice].c_str()));
	setMaxCount(atoi(In[eNomalItem_maxCount].c_str()));
}

void Item::setPotionData(vector<string>& In)
{
	setCount(atoi(In[ePotionItem_count].c_str()));
	setItemKey(In[ePotionItem_itemKey].c_str());
	setNameKey(In[ePotionItem_nameKey].c_str());
	setDesKey(In[ePotionItem_desKey].c_str());
	setName(In[ePotionItem_name].c_str());
	setItemIdx(atoi(In[ePotionItem_itemIdx].c_str()));
	setpotionValue(atoi(In[ePotionItem_potionValue].c_str()));
	setCheapPrice(atoi(In[ePotionItem_cheapPrice].c_str()));
	setReasonablePrice(atoi(In[ePotionItem_reasonablePrice].c_str()));
	setExpensivePrice(atoi(In[ePotionItem_expensivePrice].c_str()));
	setOutrageousPrice(atoi(In[ePotionItem_outrageousPrice].c_str()));
	setMaxCount(atoi(In[ePotionItem_maxCount].c_str()));
}

void Item::setWeaponData(vector<string>& In)
{
	setCount(atoi(In[eWeaponItem_count].c_str()));
	setItemKey(In[eWeaponItem_itemKey].c_str());
	setNameKey(In[eWeaponItem_nameKey].c_str());
	setDesKey(In[eWeaponItem_desKey].c_str());
	setName(In[eWeaponItem_name].c_str());
	setItemIdx(atoi(In[eWeaponItem_itemIdx].c_str()));

	sethpValue(atoi(In[eWeaponItem_hpValue].c_str()));
	setatkValue(atoi(In[eWeaponItem_atkValue].c_str()));
	setdefValue(atoi(In[eWeaponItem_defValue].c_str()));
	setspdValue(atoi(In[eWeaponItem_spdValue].c_str()));

	setCheapPrice(atoi(In[eWeaponItem_cheapPrice].c_str()));
	setReasonablePrice(atoi(In[eWeaponItem_reasonablePrice].c_str()));
	setExpensivePrice(atoi(In[eWeaponItem_expensivePrice].c_str()));
	setOutrageousPrice(atoi(In[eWeaponItem_outrageousPrice].c_str()));
	setMaxCount(atoi(In[eWeaponItem_maxCount].c_str()));
}
