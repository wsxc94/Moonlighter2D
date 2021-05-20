#include "stdafx.h"
#include "npc.h"
#include "displayStand.h"

void npc::setshopTargetPos() // �������� npc�� ��ǥ set
{
	// tmp �� 2���� �ش� ���� Ÿ��.

	vector<tagPosF> tmp;
	// 0�� ����
	tmp.push_back(tagPosF(600, 750));
	tmp.push_back(tagPosF(425, 750));
	tmp.push_back(tagPosF(421, 650));
	tmp.push_back(tagPosF(421, 600));
	tmp.push_back(tagPosF(580, 600));
	tmp.push_back(tagPosF(580, 690));

	tmp.push_back(tagPosF(620, 680));
	tmp.push_back(tagPosF(800, 710));
	tmp.push_back(tagPosF(660, 830));

	shop_target.push_back(tmp);

	// 1�� ���� 
	tmp.clear();
	tmp.push_back(tagPosF(670, 780));
	tmp.push_back(tagPosF(610, 740));
	tmp.push_back(tagPosF(563, 655));

	tmp.push_back(tagPosF(610, 740));
	tmp.push_back(tagPosF(640, 680));
	tmp.push_back(tagPosF(800, 710));
	tmp.push_back(tagPosF(660, 830));
	shop_target.push_back(tmp);

	// 2�� ���� 
	tmp.clear();
	tmp.push_back(tagPosF(670, 800));
	tmp.push_back(tagPosF(460, 790));
	tmp.push_back(tagPosF(420, 710));
	tmp.push_back(tagPosF(460, 790));
	tmp.push_back(tagPosF(660, 680));
	tmp.push_back(tagPosF(800, 710));
	tmp.push_back(tagPosF(660, 830));

	shop_target.push_back(tmp);
	//3�� ����
	tmp.clear();
	tmp.push_back(tagPosF(690, 800));
	tmp.push_back(tagPosF(580, 731));
	tmp.push_back(tagPosF(563, 711));
	tmp.push_back(tagPosF(680, 680));
	tmp.push_back(tagPosF(800, 710));
	tmp.push_back(tagPosF(660, 830));
	shop_target.push_back(tmp);

	shop_currentTargetIdx = 0;

	_stop = false;
} // ������ 

HRESULT npc::init(tagPosF pos, string key)
{
	_idx = 0;
	_count = 0;
	_time = 0;
	_angle = 0;
	_targetIdx = 0;
	_Istalk = false;
	_boxidx = 0;
	_boxCnt = 0;

	_key = key;
	_pos = pos;
	_name = key + " : ";

	_speed = 1.0f;

	_target[0] = tagPosF(pos.x + 80, pos.y + 80);
	_target[1] = tagPosF(pos.x - 80, pos.y + 80);
	_target[2] = tagPosF(pos.x + 80, pos.y - 80);
	_target[3] = tagPosF(pos.x - 80, pos.y - 0);

	_stop = false;
	_state = NPC_MOVE;

	_aniNpc = make_unique<animation>();
	_aniNpc->init(IMAGEMANAGER->findImage(_key), 0, 7, true);

	_isBarking = false;

	return S_OK;
}

HRESULT npc::init(tagPosF pos, string key, NPC_MAP NPC_SHOP, int idx, shared_ptr<displayStand> dis)
{
	_peekItemGold = 0;
	_peekItemCnt = 0;
	_idx = 0;
	_count = 0;
	_time = 0;
	_angle = 0;
	_spawnTime = RANDOM->range(200, 1000);
	_Istalk = false;
	_boxidx = 0;
	_boxCnt = 0;

	_key = key;
	_pos = pos;
	_name = key + " : ";

	_speed = 1.0f;
	_delay = false;
	_isActive = false;
	_isSpawn = false;

	setshopTargetPos();

	shop_targetIdx = idx;

	_displayStand = dis;

	_state = NPC_START;

	_peekItemImg = new image;

	_aniNpc = make_unique<animation>();
	_aniNpc->init(IMAGEMANAGER->findImage(_key), 0, 7, true);

	thinkInfo.clear();

	return S_OK;
}

void npc::release()
{
	//SAFE_DELETE(_peekItemImg);
	//SAFE_DELETE(_displayStand);
}

void npc::update()
{
	
	anim(); // ������ ���� �ִϸ��̼��� �����ϴ� �Լ�

	RECT tmp;
	if (IntersectRect(&tmp, &PLAYER->getRect(), &_rc))
		boxAnim();
	else
		_boxidx = 0;

}

void npc::update(NPC_MAP NPC_SHOP)
{

	if (!_isSpawn)
	{
		_time++;
		if (_time % _spawnTime == 0) {
			npcSpawn(); // npc�� �����ɶ� ������ �Ҹ��� �������� ���ִ� �Լ�
			_isSpawn = true;
			_isActive = true;
			_state = NPC_MOVE;
		}
	}
	else {

		anim(); // npc������ ���� �ִϸ��̼��� �ٲ��ִ� �Լ�
		move(NPC_SHOP); // npc �̵� �Լ�
	}
}

void npc::render()
{
	RECT tmp;

	if (IntersectRect(&tmp, &PLAYER->getRect(), &_rc) && !_Istalk)
	{
		CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("npc��ȭ"), 2000, _pos.x + 30, _pos.y - 50, _boxidx, 0);
	}

	if (_state == NPC_STOP)
	{

		_aniNpc->aniStop();
		_aniNpc->ZoderRender(_pos.y + (IMAGEMANAGER->findImage(_key)->getFrameHeight() >> 1), _pos.x, _pos.y);
	}
	else {

		if (_aniNpc->getAniState() == ANIMATION_END) _aniNpc->aniRestart();
		_aniNpc->ZoderRender(_pos.y + (IMAGEMANAGER->findImage(_key)->getFrameHeight() >> 1) , _pos.x, _pos.y);
	}
	
	ShadowPosRender();

	TalkInterfaceRender();
	
	//CAMERAMANAGER->FrameRect(getMemDC(), _rc, RGB(255, 0, 0));
}

void npc::render(NPC_MAP NPC_SHOP)
{
	if (_state == NPC_STOP || _state == NPC_WAIT || _state == NPC_CHECK_PRICE)
	{
		_aniNpc->aniStop();
		_aniNpc->ZoderRender(_pos.y + (IMAGEMANAGER->findImage(_key)->getFrameHeight() >> 1) , _pos.x, _pos.y);

		if (_state == NPC_CHECK_PRICE || _state == NPC_WAIT)
		{
			_aniPriceCheck->ZoderRender(
				_pos.y + IMAGEMANAGER->findImage(_key)->getFrameHeight(),
				_pos.x + IMAGEMANAGER->findImage(_key)->getFrameWidth() / 2,
				_pos.y - IMAGEMANAGER->findImage(_key)->getFrameHeight()/2);
		}
	}
	else if (_state == NPC_MOVE || _state == NPC_ITEM_PICK) {
		
		if (_aniNpc->getAniState() == ANIMATION_END) _aniNpc->aniRestart();
		_aniNpc->ZoderRender(_pos.y + (IMAGEMANAGER->findImage(_key)->getFrameHeight() >> 1) , _pos.x, _pos.y);

		ShadowPosRender();

		if (_state == NPC_ITEM_PICK) {

			CAMERAMANAGER->ZorderRender(_peekItemImg,
				_pos.y,
				_pos.x + _peekItemImg->getWidth()/2,
				_pos.y + _peekItemImg->getHeight() / 4);
		}
	}

	//CAMERAMANAGER->FrameRect(getMemDC(), _rc, RGB(255, 0, 0));
}

void npc::anim() // npc������ ���� �ִϸ��̼��� �ٲ��ִ� �Լ�
{
	_aniNpc->update();

	if ((_state == NPC_MOVE || _state == NPC_ITEM_PICK) && !_stop)
	{
		if (_key != "������" && _key != "������npc") {
			if (RadianToDegree(_angle) >= 225 && RadianToDegree(_angle) <= 330) _aniNpc->setFrameY(0);
			if (RadianToDegree(_angle) >= 45 && RadianToDegree(_angle) < 135) _aniNpc->setFrameY(1);
			if (RadianToDegree(_angle) >= 0 && RadianToDegree(_angle) < 45 || RadianToDegree(_angle) >= 335) {
				if(_key != "���ϰ�����")
				_aniNpc->setFrameY(2);
				else
				_aniNpc->setFrameY(3);
			}
			if (RadianToDegree(_angle) >= 135 && RadianToDegree(_angle) < 225) {
				if (_key != "���ϰ�����")
					_aniNpc->setFrameY(3);
				else
					_aniNpc->setFrameY(2);
			}
		}
		else {
			_aniNpc->setFrameY(0);
		}
	}

}

void npc::boxAnim()
{
	_boxCnt++;

	if (_boxCnt % 10 == 0)
	{

		if (_boxidx < IMAGEMANAGER->findImage("npc��ȭ")->getMaxFrameX())
		{
			_boxidx++;
		}
	}
}

void npc::move()
{
	_time++;

	if (_state == NPC_MOVE) {

		if (_speed == 0)
		{
			_aniNpc->setCurIndex(0);
		}

		if (_time % 120 == 0)
		{
			_speed = 1.0f;
			_stop = false;
			_targetIdx = RANDOM->range(1, 4);

		}

		_angle = getAngle(_pos.x, _pos.y, _target[_targetIdx].x, _target[_targetIdx].y);

		if (getDistance(_pos.x, _pos.y, _target[_targetIdx].x, _target[_targetIdx].y) < 2 && !_stop)
		{
			_targetIdx = RANDOM->range(1, 4);
			_time = 0;
			_speed = 0;
		}

		_pos.x += cosf(_angle) * _speed;
		_pos.y += -sinf(_angle) * _speed / 2;
	}
}

void npc::move(NPC_MAP NPC_SHOP)
{

	if (_state == NPC_STOP) {
	_time++;
		if (_time % 160 == 0) {
			shop_currentTargetIdx++;
			_state = NPC_MOVE;
		}
	}

	else if (_state == NPC_MOVE || _state == NPC_ITEM_PICK)
	{
		_speed = 1.0f;

		_angle = getAngle(_pos.x, _pos.y, shop_target[shop_targetIdx][shop_currentTargetIdx].x,
			shop_target[shop_targetIdx][shop_currentTargetIdx].y);

		_pos.x += cosf(_angle) * _speed;
		_pos.y += -sinf(_angle) * _speed / 2;
	}
	else if (_state == NPC_CHECK_PRICE || _state == NPC_WAIT) {
		PriceCheckAnim();
	}

	DistanceCheck();

}

void npc::DistanceCheck()
{
	if (shop_target[shop_targetIdx][shop_currentTargetIdx].y == 680)
	{
		if (thinkInfo != "�δ�" && thinkInfo != "��û�δ�") {
			shop_currentTargetIdx++;
		}
	}
	if (_key != "�賶�ǿ���" && shop_currentTargetIdx == shop_target[shop_targetIdx].size() - 2) {
		shop_currentTargetIdx++;
	}

	if (getDistance(_pos.x, _pos.y, shop_target[shop_targetIdx][shop_currentTargetIdx].x,
		shop_target[shop_targetIdx][shop_currentTargetIdx].y) < 1)
	{


		if (shop_currentTargetIdx < shop_target[shop_targetIdx].size() && !_delay) {
			_delay = true;

			if (shop_currentTargetIdx == 2) {

				if (shop_targetIdx == 0 || shop_targetIdx == 2) {
					if(_key != "���ϰ�����")
					_aniNpc->setFrameY(2);
					else
					_aniNpc->setFrameY(3);
				}
				else if (shop_targetIdx == 1 || shop_targetIdx == 3)
				{
					if (_key != "���ϰ�����")
					_aniNpc->setFrameY(3);
					else
					_aniNpc->setFrameY(2);
				}
				priceCheck();
			}
			else if (shop_target[shop_targetIdx][shop_currentTargetIdx].y == 680)
			{
				if (thinkInfo == "�δ�" || thinkInfo == "��û�δ�") {
					_state = NPC_WAIT;
					_aniPriceCheck = make_unique<animation>();
					_aniPriceCheck->init(IMAGEMANAGER->findImage("��ٸ�����"), 0, 10, true, false);
				}
				else {
					shop_currentTargetIdx++;
				}

			}
			else if (_key == "�賶�ǿ���" && (shop_target[shop_targetIdx].size() - 2) == shop_currentTargetIdx)
			{
				_state = NPC_STOP;
				_aniNpc->setFrameY(2);
				_time = 0;

			}
			else if (shop_target[shop_targetIdx].size() - 1 == shop_currentTargetIdx)
			{

				_state = NPC_GO_HOME;
				_isActive = false;
				//SOUNDMANAGER->play();
			}
			else {
				shop_currentTargetIdx++;
			}

		}



	}
	else {
		_delay = false;
	}
}

void npc::action(string talk)
{
	_talkRect = RectMake(_pos.x, _pos.y,
		IMAGEMANAGER->findImage(_key)->getFrameWidth() * 2, IMAGEMANAGER->findImage(_key)->getFrameHeight() * 2);

	_nameRect = CAMERAMANAGER->RectMake(_pos.x + 200, _pos.y - 150, 100, 30);
	_textRect = CAMERAMANAGER->RectMake(_pos.x + 200, _pos.y - 120, 200, 200);

	_talk = talk;

	RECT tmp;
	if (!ITEMMENU->getOpenMenu())
	{
		if (IntersectRect(&tmp, &PLAYER->getRect(), &_rc) && INPUT->GetKeyDown('J'))
		{
			if (_Istalk)
			{
				_Istalk = false;
				_stop = false;
				_state = NPC_MOVE;
			}
			else {
				_state = NPC_STOP;
				_Istalk = true;
				_stop = true;
				lookPlayer();
			}
		}
	}

	if (!IntersectRect(&tmp, &PLAYER->getRect(), &_rc))
	{
		_Istalk = false;
		_stop = false;
		_state = NPC_MOVE;
	}

}

void npc::action()
{

	RECT tmp;

	if (IntersectRect(&tmp, &PLAYER->getRect(), &_rc))
		boxAnim();
	else
		_boxidx = 0;

	if (IntersectRect(&tmp, &PLAYER->getRect(), &_rc) && INPUT->GetKeyDown('J'))
	{

		if (!SOUNDMANAGER->isPlaySound("���Ҹ�"))
		{
			SOUNDMANAGER->play("���Ҹ�");
			_isBarking = true;
		}

	}

}

void npc::collision()
{
	if (_key != "������") {
		_rc = RectMake(_pos.x - (IMAGEMANAGER->findImage(_key)->getFrameWidth() >> 1),
			_pos.y - (IMAGEMANAGER->findImage(_key)->getFrameHeight() >> 1),
			IMAGEMANAGER->findImage(_key)->getFrameWidth() * 2,
			IMAGEMANAGER->findImage(_key)->getFrameHeight() * 2);
	}
	else {
		_rc = RectMake(_pos.x - 10, _pos.y - 10,
			IMAGEMANAGER->findImage(_key)->getFrameWidth() + 20, IMAGEMANAGER->findImage(_key)->getFrameHeight() * 2 + 20);
	}

}

void npc::lookPlayer() // �÷��̾ �ٶ󺸰� npc�ִϸ��̼��� �ٲٴ� �Լ�
{
	if (_isBarking)
	{
		_aniNpc->update();
		if (_aniNpc->getAniState() == ANIMATION_END)
		{
			_isBarking = false;
		}
	}


	//�̹��� ���� ������ ������ �������� �޶� �̷��� �س�
	if ((_pos.y < PLAYER->getY() && _pos.y + IMAGEMANAGER->findImage(_key)->getFrameHeight() > PLAYER->getY()) &&
		_pos.x > PLAYER->getX())
	{
		if (_key == "������npc")
			_aniNpc->setFrameY(2);
		else
			_aniNpc->setFrameY(3);

	}
	else if ((_pos.y < PLAYER->getY() && _pos.y + IMAGEMANAGER->findImage(_key)->getFrameHeight() > PLAYER->getY()) &&
		_pos.x < PLAYER->getX())
	{
		if (_key == "������npc")
			_aniNpc->setFrameY(3);
		else
			_aniNpc->setFrameY(2);
	}
	else if (_pos.y > PLAYER->getY())
	{
		_aniNpc->setFrameY(1);
	}
	else {
		_aniNpc->setFrameY(0);
	}

}

void npc::npcSpawn()
{
	/*int rnd = RANDOM->range(0, 1);
	string str = "��������" + to_string(rnd);*/
	string str = "��������1";
	SOUNDMANAGER->play(str, 0.5f);
}

void npc::priceCheck() // ���Ǿ������� ������ ���� �Ǵ��Ѵ�.
{
	thinkInfo.clear();

	if (_displayStand->getDisplayItem()[shop_targetIdx].getPrice() == 0 || _displayStand->getDisplayItem()[shop_targetIdx].getType() == ITEM_EMPTY) {
		shop_currentTargetIdx++;
		_state = NPC_MOVE;
		return;
	}

	for (int i = 0; i < 4; i++) {

		if (_displayStand->getDisplayItem()[shop_targetIdx].getPrice() / _displayStand->getDisplayItem()[shop_targetIdx].getCount()
			< _displayStand->getDisplayItem()[shop_targetIdx].getPriceRange()[i])
		{
			switch (i) // �̰��� ���� ���� �ִϸ��̼� ������ y ���� ���������., ���尡 �޶����� �ؾ���.
			{
			case 0: //���� �δ�
				_aniPriceCheck = make_shared<animation>();
				_aniPriceCheck->init(IMAGEMANAGER->findImage("��û�δ�"), 0, 15);
				thinkInfo = "��û�δ�";
				break;
			case 1: //�δ�
				_aniPriceCheck = make_shared<animation>();
				_aniPriceCheck->init(IMAGEMANAGER->findImage("�δ�"), 0, 15);
				thinkInfo = "�δ�";
				break;
			case 2: // ��δ�
				_aniPriceCheck = make_shared<animation>();
				_aniPriceCheck->init(IMAGEMANAGER->findImage("��δ�"), 0, 15);
				thinkInfo = "��δ�";
				break;
			case 3: default: // ������δ�
				_aniPriceCheck = make_shared<animation>();
				_aniPriceCheck->init(IMAGEMANAGER->findImage("��û��δ�"), 0, 15);
				thinkInfo = "��û��δ�";
				break;
			}
			break;
		}
	}

	if (thinkInfo == "") {
	    _aniPriceCheck = make_shared<animation>();
		_aniPriceCheck->init(IMAGEMANAGER->findImage("��û��δ�"), 0, 15);
		thinkInfo = "��û��δ�";
	}

	_state = NPC_CHECK_PRICE;
}

void npc::PriceCheckAnim()
{
	_aniPriceCheck->update();

	if (_state == NPC_CHECK_PRICE) {
		
		if (_aniPriceCheck->getAniState() == ANIMATION_END) {
			SOUNDMANAGER->play(thinkInfo);
			_state = NPC_ITEM_PICK;
			if (thinkInfo == "�δ�" || thinkInfo == "��û�δ�") {
				ItemGet();
			}
			else {
				_state = NPC_MOVE;
			}
			_stop = false;
			_speed = 1.0f;
			shop_currentTargetIdx++;
		}

	}
}

void npc::ItemGet()
{ 
	_peekItemImg = new image;

	_peekItemImg = _displayStand->getDisplayItem()[shop_targetIdx].getItemImg();
	_peekItemCnt = _displayStand->getDisplayItem()[shop_targetIdx].getCount();
	_peekItemGold = _displayStand->getDisplayItem()[shop_targetIdx].getPrice();

	// ���⼭ ���� ������ ���� ���� �ؾ���.
	_displayStand->deleteDisplayItem(shop_targetIdx);

}

void npc::ItemActive()
{
	// ���ǿ� �������� �ְ� npc�� ��Ȱ��ȭ�Ͻ� npc�� Ȱ��ȭ ��Ŵ
	if (_displayStand->getDisplayItem()[shop_targetIdx].getType() != ITEM_EMPTY && !_isActive)
	{
		_isActive = true;
	}
}

void npc::ShadowPosRender() // �� ���� �� �Լ�ȭ
{
	if (_key == "������npc"){
		CAMERAMANAGER->ZorderAlphaFrameRender(IMAGEMANAGER->findImage("npc�׸���"), 10,
			_pos.x + 1,
			_pos.y + (_aniNpc->getImage()->getFrameHeight() - _aniNpc->getImage()->getFrameHeight() / 3), 0, 0, 80);
	}
	else if(_key == "��" || _key == "����������")
	{
		ShadowPosSet(6, 4, 2);
	}
	else if(_key == "����")
	{
		ShadowPosSet(3, 3, 2);
	}
	else if (_key == "�����")
	{
		ShadowPosSet(3, 8, 2);
	}
	else if (_key == "�賶�ǿ���")
	{
		ShadowPosSet(0, 3, 2);
	}
	else {
		ShadowPosSet(2, 1, 2);
	}
}

void npc::ShadowPosSet(int x , int y , int div)
{
	CAMERAMANAGER->ZorderAlphaFrameRender(IMAGEMANAGER->findImage("npc�׸���"), 10,
		_pos.x + (_aniNpc->getImage()->getFrameWidth() >> div) - x,
		_pos.y + (_aniNpc->getImage()->getFrameHeight() - (_aniNpc->getImage()->getFrameHeight() >> div)) - y, 0, 0, 80);
}

void npc::TalkInterfaceRender()
{
	if (_Istalk)
	{
		char str[256];

		CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("npc��ǳ��"), _pos.x, _pos.y - 200);
		CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage(_key + _illustrator), _pos.x + 10, _pos.y - 220, 1, 1);

		/*hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
			0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
		oldFont = (HFONT)SelectObject(getMemDC(), hFont);
		DeleteObject(oldFont);*/

		SetTextColor(getMemDC(), RGB(67, 42, 10));
		wsprintf(str, _name.c_str(), strlen(str));
		DrawText(getMemDC(), str, -1, &_nameRect, DT_LEFT | DT_WORDBREAK);

		SetTextColor(getMemDC(), RGB(125, 112, 92));
		wsprintf(str, _talk.c_str(), strlen(str));
		DrawText(getMemDC(), str, -1, &_textRect, DT_LEFT | DT_WORDBREAK);

	}
}

npc::~npc()
{
	//SAFE_DELETE(_peekItemImg);
}

