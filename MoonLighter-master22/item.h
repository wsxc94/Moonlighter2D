#pragma once
#include "stdafx.h"
enum CSVITEMTYPE {
	CSV_NOMAL,
	CSV_POTION,
	CSV_WEAPON
};
enum NomalItem {
	eNomalItem_count,
	eNomalItem_itemKey,
	eNomalItem_nameKey,
	eNomalItem_desKey,
	eNomalItem_name,
	eNomalItem_itemIdx,
	eNomalItem_cheapPrice,
	eNomalItem_reasonablePrice,
	eNomalItem_expensivePrice,
	eNomalItem_outrageousPrice,
	eNomalItem_maxCount
};
enum WeaponItem
{
	eWeaponItem_count,
	eWeaponItem_itemKey,
	eWeaponItem_nameKey,
	eWeaponItem_desKey,
	eWeaponItem_name,
	eWeaponItem_itemIdx,
	eWeaponItem_hpValue,
	eWeaponItem_atkValue,
	eWeaponItem_defValue,
	eWeaponItem_spdValue,
	eWeaponItem_cheapPrice,
	eWeaponItem_reasonablePrice,
	eWeaponItem_expensivePrice,
	eWeaponItem_outrageousPrice,
	eWeaponItem_maxCount,
};
enum PotionItem
{
	ePotionItem_count,
	ePotionItem_itemKey,
	ePotionItem_nameKey,
	ePotionItem_desKey,
	ePotionItem_name,
	ePotionItem_itemIdx,
	ePotionItem_potionValue,
	ePotionItem_cheapPrice,
	ePotionItem_reasonablePrice,
	ePotionItem_expensivePrice,
	ePotionItem_outrageousPrice,
	ePotionItem_maxCount
};

class Item {
public:
	int getCount() const { return count; }
	void setCount(int value) { count = value; }

	string getItemKey() const { return itemKey; }
	void setItemKey(string key) { itemKey = key; }

	string getNameKey() const { return nameKey; }
	void setNameKey(string key) { nameKey = key; }

	string getDesKey() const { return desKey; }
	void setDesKey(string key) { desKey = key; }

	string getName() const { return name; }
	void setName(string key) { name = key; }

	int getItemIdx() const { return itemIdx; }
	void setItemIdx(int value) { itemIdx = value; }

	int getCheapPrice() const { return cheapPrice; }
	void setCheapPrice(int value) { cheapPrice = value; }

	int getReasonablePrice() const { return reasonablePrice; }
	void setReasonablePrice(int value) { reasonablePrice = value; }

	int getExpensivePrice() const { return expensivePrice; }
	void setExpensivePrice(int value) { expensivePrice = value; }

	int getOutrageousPrice() const { return outrageousPrice; }
	void setOutrageousPrice(int value) { outrageousPrice = value; }

	int getMaxCount() const { return maxCount; }
	void setMaxCount(int value) { maxCount = value; }

	int gethpValue() const { return hpValue; }
	void sethpValue(int value) { hpValue = value; }

	int getatkValue() const { return atkValue; }
	void setatkValue(int value) { atkValue = value; }

	int getdefValue() const { return defValue; }
	void setdefValue(int value) { defValue = value; }

	int getspdValue() const { return spdValue; }
	void setspdValue(int value) { spdValue = value; }

	int getpotionValue() const { return potionValue; }
	void setpotionValue(int value) { potionValue = value; }

	void FillData(std::vector<string>& In, CSVITEMTYPE& _type) {
		switch (_type)
		{
		case CSV_NOMAL:
			setNomalData(In);
			break;
		case CSV_POTION:
			setPotionData(In);
			break;
		case CSV_WEAPON:
			setWeaponData(In);
			break;
		default:
			break;
		}
	}
private:
	int count;
	string itemKey;
	string nameKey;
	string desKey;
	string name;
	int itemIdx;
	int cheapPrice;
	int reasonablePrice;
	int expensivePrice;
	int outrageousPrice;
	int maxCount;
	int hpValue;
	int atkValue;
	int defValue;
	int spdValue;
	int potionValue;

	void setNomalData(vector<string>& In);
	void setPotionData(vector<string>& In);
	void setWeaponData(vector<string>& In);
};