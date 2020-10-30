#pragma once
#include "stdafx.h"

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

	void FillData(std::vector<string>& In) {
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
};