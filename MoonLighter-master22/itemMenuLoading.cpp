#include "stdafx.h"
#include "itemMenuLoading.h"

HRESULT itemMenuLoading::init()
{
	_loading = new loading;
	_loading->init();

	this->loadingImage();
	this->loadingSound();

	PLAYERDATA->setIsActivate(false);

	return S_OK;
}

void itemMenuLoading::release()
{
	if (_loading)
	{
		_loading->release();
		SAFE_DELETE(_loading);
	}
}

void itemMenuLoading::update()
{
	_loading->update();
	if (_loading->loadingDone())
	{
		ITEMMENU->init();
		SCENEMANAGER->loadScene("타운로딩");
	}
}

void itemMenuLoading::render()
{
	_loading->render();
}

void itemMenuLoading::loadingImage()
{
	//아이템메뉴 이미지 초기화 
	_loading->loadImage("menu_calendar", "Images/ItemMenu/menu_calendar.bmp", 1002, 592, true, RGB(255, 0, 255));
	_loading->loadImage("menu_wishlist", "Images/ItemMenu/menu_wishlist.bmp", 818, 518, true, RGB(255, 0, 255));
	_loading->loadImage("menu_inventory", "Images/ItemMenu/menu_inventory.bmp", 1044, 598, true, RGB(255, 0, 255));
	_loading->loadImage("menu_inventory_inDungeon", "Images/ItemMenu/menu_inventory_inDungeon.bmp", 1044, 598, true, RGB(255, 0, 255));
	_loading->loadImage("menu_notebook", "Images/ItemMenu/menu_notebook.bmp", 827, 574, true, RGB(255, 0, 255));
	_loading->loadImage("btn_qe", "Images/ItemMenu/btn_qe.bmp", 300, 38, true, RGB(255, 0, 255));
	_loading->loadFrameImage("toggle_calendar", "Images/ItemMenu/toggle_calendar.bmp", 92, 50, 2, 1);
	_loading->loadFrameImage("toggle_wishlist", "Images/ItemMenu/toggle_wishlist.bmp", 92, 50, 2, 1);
	_loading->loadFrameImage("toggle_inventory", "Images/ItemMenu/toggle_inventory.bmp", 92, 50, 2, 1);
	_loading->loadFrameImage("toggle_notebook", "Images/ItemMenu/toggle_notebook.bmp", 92, 50, 2, 1);
	_loading->loadFrameImage("icon_euip", "Images/ItemMenu/icon_euip.bmp", 64, 30, 2, 1);
	_loading->loadFrameImage("icon_equip_idx", "Images/ItemMenu/icon_equip_idx.bmp", 60, 30, 2, 1);
	_loading->loadFrameImage("icon_equip_idx_empty", "Images/ItemMenu/icon_equip_idx_empty.bmp", 60, 30, 2, 1);
	_loading->loadImage("nameBase", "Images/ItemMenu/nameBase.bmp", 310, 34, true, RGB(255, 0, 255));
	_loading->loadImage("grabBase", "Images/ItemMenu/grabBase.bmp", 72, 72, true, RGB(255, 0, 255));
	_loading->loadImage("item_slot", "Images/ItemMenu/item_slot.bmp", 60, 60, true, RGB(255, 0, 255));
	_loading->loadImage("inventory_selectBubble", "Images/ItemMenu/inventory_selectBubble.bmp", 220, 144, true, RGB(255, 0, 255));
	_loading->loadFrameImage("select_yes", "Images/ItemMenu/select_yes.bmp", 312, 78, 4, 1);
	_loading->loadFrameImage("select_no", "Images/ItemMenu/select_no.bmp", 312, 78, 4, 1);
	_loading->loadImage("bag_pendant", "Images/ItemMenu/bag_pendant.bmp", 70, 48, true, RGB(255, 0, 255));
	_loading->loadImage("bag_emblem", "Images/ItemMenu/bag_emblem.bmp", 68, 66, true, RGB(255, 0, 255));

	//커서 이미지 초기화 
	_loading->loadFrameImage("cursor_move", "Images/ItemMenu/cursor_move.bmp", 304, 76, 4, 1);
	_loading->loadFrameImage("cursor_click", "Images/ItemMenu/cursor_click.bmp", 380, 76, 5, 1);
	_loading->loadFrameImage("cursor_price", "Images/Shop/cursor_price.bmp", 408, 94, 2, 1);
	_loading->loadFrameImage("cursor_select", "Images/ItemMenu/cursor_select.bmp", 510, 102, 5, 1);

	//아이템 이미지 초기화 
	_loading->loadImage("풍부한 젤리", "Images/Item/item_richJelly.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("맹독성 젤리", "Images/Item/item_venomJelly.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("강화 수정", "Images/Item/item_crystal.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("덩굴", "Images/Item/item_vine.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("이빨석", "Images/Item/item_teethStone.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("뿌리", "Images/Item/item_root.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("쇠막대", "Images/Item/item_ironBar.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("주물 잔해", "Images/Item/item_foundryRests.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("망가진 검", "Images/Item/item_brokenSword.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("천", "Images/Item/item_fabric.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("경화 강철", "Images/Item/item_hardenedSteel.bmp", 48, 48, true, RGB(255, 0, 255));

	_loading->loadImage("훈련용 단검", "Images/Item/item_trainingShortSword.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("훈련용 활", "Images/Item/item_trainingBow.bmp", 48, 48, true, RGB(255, 0, 255));

	_loading->loadImage("HP 물약Ⅰ", "Images/Item/item_hpPotion1.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("HP 물약Ⅱ", "Images/Item/item_hpPotion2.bmp", 48, 48, true, RGB(255, 0, 255));

	//아이템 이름 이미지 초기화
	_loading->loadImage("name_richJelly", "Images/Item/name_richJelly.bmp", 160, 24);
	_loading->loadImage("name_venomJelly", "Images/Item/name_venomJelly.bmp", 160, 24);
	_loading->loadImage("name_crystal", "Images/Item/name_crystal.bmp", 160, 24);
	_loading->loadImage("name_vine", "Images/Item/name_vine.bmp", 160, 24);
	_loading->loadImage("name_teethStone", "Images/Item/name_teethStone.bmp", 160, 24);
	_loading->loadImage("name_root", "Images/Item/name_root.bmp", 160, 24);
	_loading->loadImage("name_ironBar", "Images/Item/name_ironBar.bmp", 160, 24);
	_loading->loadImage("name_foundryRests", "Images/Item/name_foundryRests.bmp", 160, 24);
	_loading->loadImage("name_brokenSword", "Images/Item/name_brokenSword.bmp", 160, 24);
	_loading->loadImage("name_fabric", "Images/Item/name_fabric.bmp", 160, 24);
	_loading->loadImage("name_hardenedSteel", "Images/Item/name_hardenedSteel.bmp", 160, 24);

	_loading->loadImage("name_trainingShortSword", "Images/Item/name_trainingShortSword.bmp", 160, 24);
	_loading->loadImage("name_trainingBow", "Images/Item/name_trainingBow.bmp", 160, 24);

	_loading->loadImage("name_hpPotion1", "Images/Item/name_hpPotion1.bmp", 160, 24);
	_loading->loadImage("name_hpPotion2", "Images/Item/name_hpPotion2.bmp", 160, 24);

	//아이템 설명 이미지 초기화 
	_loading->loadImage("des_richJelly", "Images/Item/des_richJelly.bmp", 304, 66);
	_loading->loadImage("des_venomJelly", "Images/Item/des_venomJelly.bmp", 314, 42);
	_loading->loadImage("des_crystal", "Images/Item/des_crystal.bmp", 280, 20);
	_loading->loadImage("des_vine", "Images/Item/des_vine.bmp", 292, 42);
	_loading->loadImage("des_teethStone", "Images/Item/des_teethStone.bmp", 310, 42);
	_loading->loadImage("des_root", "Images/Item/des_root.bmp", 316, 42);
	_loading->loadImage("des_ironBar", "Images/Item/des_ironBar.bmp", 312, 64);
	_loading->loadImage("des_foundryRests", "Images/Item/des_foundryRests.bmp", 278, 64);
	_loading->loadImage("des_brokenSword", "Images/Item/des_brokenSword.bmp", 276, 64);
	_loading->loadImage("des_fabric", "Images/Item/des_fabric.bmp", 306, 64);
	_loading->loadImage("des_hardenedSteel", "Images/Item/des_hardenedSteel.bmp", 270, 42);

	_loading->loadImage("des_trainingShortSword", "Images/Item/des_trainingShortSword.bmp", 286, 64);
	_loading->loadImage("des_trainingBow", "Images/Item/des_trainingBow.bmp", 306, 66);

	_loading->loadImage("des_hpPotion1", "Images/Item/des_hpPotion1.bmp", 310, 42);
	_loading->loadImage("des_hpPotion2", "Images/Item/des_hpPotion2.bmp", 304, 42);

	//인터페이스 이미지 초기화 
	_loading->loadImage("interface_key", "Images/Interface/interface_key.bmp", 174, 152, true, RGB(255, 0, 255));
	_loading->loadImage("icon_coin", "Images/Interface/icon_coin.bmp", 18, 18, true, RGB(255, 0, 255));
	_loading->loadImage("icon_goldPouch", "Images/Interface/icon_goldPouch.bmp", 70, 70, true, RGB(255, 0, 255));
	_loading->loadImage("icon_bag", "Images/Interface/icon_bag.bmp", 84, 84, true, RGB(255, 0, 255));
	_loading->loadImage("icon_hp", "Images/Interface/icon_hp.bmp", 56, 40, true, RGB(255, 0, 255));
	_loading->loadImage("hp_barBack", "Images/Interface/hp_barBack.bmp", 130, 24, true, RGB(255, 0, 255));
	_loading->loadImage("hp_barFront", "Images/Interface/hp_barFront.bmp", 130, 24, true, RGB(255, 0, 255));
	_loading->loadFrameImage("weapon_slot", "Images/Interface/weapon_slot.bmp", 212, 70, 2, 1);
	_loading->loadFrameImage("weapon_frontIdx", "Images/Interface/weapon_frontIdx.bmp", 60, 30, 2, 1);
	_loading->loadFrameImage("weapon_backIdx", "Images/Interface/weapon_backIdx.bmp", 60, 30, 2, 1);

	//상점 이미지 초기화 
	_loading->loadImage("menu_shopInventory", "Images/Shop/menu_shopInventory.bmp", 482, 544, true, RGB(255, 0, 255));
	_loading->loadImage("menu_display", "Images/Shop/menu_display.bmp", 500, 464, true, RGB(255, 0, 255));
	_loading->loadImage("arrow_price", "Images/Shop/arrow_price.bmp", 14, 46, true, RGB(255, 0, 255));

	//포션상점 이미지 초기화 
	_loading->loadImage("shop_nameBase", "Images/PotionShop/shop_nameBase.bmp", 402, 88, true, RGB(255, 0, 255));
	_loading->loadImage("menu_potion", "Images/PotionShop/menu_potion.bmp", 1124, 680, true, RGB(255, 0, 255));
	_loading->loadImage("icon_potionKeyE", "Images/PotionShop/icon_potionKeyE.bmp", 68, 144, true, RGB(255, 0, 255));
	_loading->loadImage("slot_base", "Images/PotionShop/slot_base.bmp", 60, 60, true, RGB(255, 0, 255));
	_loading->loadImage("shop_bubble", "Images/PotionShop/shop_bubble.bmp", 500, 160, true, RGB(255, 0, 255));
	_loading->loadFrameImage("shop_ox", "Images/PotionShop/shop_ox.bmp", 76, 60, 2, 1);

	//숫자(흰색) 이미지 초기화 
	_loading->loadImage("0", "Images/Number/0.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("1", "Images/Number/1.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("2", "Images/Number/2.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("3", "Images/Number/3.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("4", "Images/Number/4.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("5", "Images/Number/5.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("6", "Images/Number/6.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("7", "Images/Number/7.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("8", "Images/Number/8.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("9", "Images/Number/9.bmp", 10, 14, true, RGB(255, 0, 255));

	//숫자(검정) 이미지 초기화 
	_loading->loadImage("0_black", "Images/Number/0_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("1_black", "Images/Number/1_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("2_black", "Images/Number/2_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("3_black", "Images/Number/3_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("4_black", "Images/Number/4_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("5_black", "Images/Number/5_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("6_black", "Images/Number/6_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("7_black", "Images/Number/7_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("8_black", "Images/Number/8_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("9_black", "Images/Number/9_black.bmp", 10, 14, true, RGB(255, 0, 255));

	//숫자(초록색) 이미지 초기화 
	_loading->loadImage("0_green", "Images/Number/0_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("1_green", "Images/Number/1_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("2_green", "Images/Number/2_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("3_green", "Images/Number/3_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("4_green", "Images/Number/4_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("5_green", "Images/Number/5_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("6_green", "Images/Number/6_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("7_green", "Images/Number/7_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("8_green", "Images/Number/8_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("9_green", "Images/Number/9_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("x_green", "Images/Number/x_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("0_gray", "Images/Number/0_gray.bmp", 10, 14, true, RGB(255, 0, 255));

	//작은 숫자(흰색) 이미지 초기화 
	_loading->loadImage("0_white_small", "Images/Number/0_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("1_white_small", "Images/Number/1_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("2_white_small", "Images/Number/2_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("3_white_small", "Images/Number/3_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("4_white_small", "Images/Number/4_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("5_white_small", "Images/Number/5_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("6_white_small", "Images/Number/6_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("7_white_small", "Images/Number/7_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("8_white_small", "Images/Number/8_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("9_white_small", "Images/Number/9_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("slash_white_small", "Images/Number/slash_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
}

void itemMenuLoading::loadingSound()
{
	//아이템메뉴 사운드 
	_loading->loadSound("openInven", "Sound/UI&inventorySound/openInven.wav");
	_loading->loadSound("moveMenu", "Sound/UI&inventorySound/moveMenu.wav");
	_loading->loadSound("cursor_move", "Sound/UI&inventorySound/cursor_move.wav");
	_loading->loadSound("cursor_pick", "Sound/UI&inventorySound/cursor_pick.wav");
	_loading->loadSound("cursor_drop", "Sound/UI&inventorySound/cursor_drop.wav");
	_loading->loadSound("cursor_error", "Sound/UI&inventorySound/cursor_error.wav");

	//상점(포션,대장간) 사운드
	_loading->loadSound("witch_Open", "Sound/witch&blacksmith/witch_open.wav");
}
