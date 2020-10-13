#include "stdafx.h"
#include "npc.h"

void npc::setshopTargetPos()
{
	vector<tagPosF> tmp;
	// 0번 좌판
	tmp.push_back(tagPosF(600, 750));
	tmp.push_back(tagPosF(425, 750));
	tmp.push_back(tagPosF(421, 650));
	tmp.push_back(tagPosF(421, 600));
	tmp.push_back(tagPosF(580, 600));
	tmp.push_back(tagPosF(580, 690));

	tmp.push_back(tagPosF(620, 680));
	tmp.push_back(tagPosF(660, 830));

	shop_target.push_back(tmp);
	// 1번 좌판 670 780 , 610 ,740 ,570 670
	tmp.clear();
	tmp.push_back(tagPosF(670, 780));
	tmp.push_back(tagPosF(610, 740));
	tmp.push_back(tagPosF(563, 655));
	tmp.push_back(tagPosF(610, 740));

	tmp.push_back(tagPosF(640, 680));
	tmp.push_back(tagPosF(660, 830));
	shop_target.push_back(tmp);

	// 2번 좌판 
	tmp.clear();
	tmp.push_back(tagPosF(670, 800));
	tmp.push_back(tagPosF(460, 790));
	tmp.push_back(tagPosF(420, 710));
	tmp.push_back(tagPosF(460, 790));

	tmp.push_back(tagPosF(660, 680));
	tmp.push_back(tagPosF(660, 830));
	shop_target.push_back(tmp);
	//3번 좌판
	tmp.clear();
	tmp.push_back(tagPosF(690, 800));
	tmp.push_back(tagPosF(580, 731));
	tmp.push_back(tagPosF(563, 711));
	tmp.push_back(tagPosF(680, 680));
	tmp.push_back(tagPosF(660, 830));
	shop_target.push_back(tmp);

	shop_targetIdx = RANDOM->range(0, 3);

	shop_currentTargetIdx = 0;
	_stop = false;
}

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

	_aniNpc = new animation;
	_aniNpc->init(IMAGEMANAGER->findImage(_key), 0, 7, true);

	return S_OK;
}

HRESULT npc::init(tagPosF pos, string key, NPC_MAP NPC_SHOP)
{
	_idx = 0;
	_count = 0;
	_time = 0;
	_angle = 0;
	
	_Istalk = false;
	_boxidx = 0;
	_boxCnt = 0;

	_key = key;
	_pos = pos;
	_name = key + " : ";

	_speed = 1.0f;
	_delay = false;

	setshopTargetPos();

	_aniNpc = new animation;
	_aniNpc->init(IMAGEMANAGER->findImage(_key), 0, 7, true);

	return S_OK;
}

void npc::release()
{
}

void npc::update()
{
	anim();

	RECT tmp;
	if (IntersectRect(&tmp, &PLAYER->getRect(), &_rc))
	boxAnim();
	else
	_boxidx = 0;

}

void npc::update(NPC_MAP NPC_SHOP)
{
	anim();
}

void npc::render()
{
	RECT tmp;
	
		if (IntersectRect(&tmp, &PLAYER->getRect(), &_rc) && !_Istalk)
		{
			//CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("npc대화"), _pos.x + 30, _pos.y - 50, _boxidx, 0);
			CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("npc대화"), 2000, _pos.x + 30, _pos.y - 50, _boxidx, 0);
		}
		
		if (_stop)
		{
			//CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage(_key), _pos.x, _pos.y,
			//	0, IMAGEMANAGER->findImage(_key)->getFrameY());
			// 영훈이형 z오더를 했더니 IMAGEMANAGER->getFrameX() 이게안되서 제가 애니메이션으로 바꿨습니다.....죄송합니다ㅠㅠ
			_aniNpc->aniStop();
			_aniNpc->ZoderRender(_pos.y + IMAGEMANAGER->findImage(_key)->getFrameHeight() / 2, _pos.x, _pos.y);
		}
		else {
			//CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage(_key), _pos.x, _pos.y,
			//	IMAGEMANAGER->findImage(_key)->getFrameX(), IMAGEMANAGER->findImage(_key)->getFrameY());
			if (_aniNpc->getAniState() == ANIMATION_END) _aniNpc->aniRestart();
			_aniNpc->ZoderRender(_pos.y + IMAGEMANAGER->findImage(_key)->getFrameHeight() / 2, _pos.x, _pos.y);
		}

		if (_Istalk)
		{
			char str[256];

			CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("npc말풍선"), _pos.x, _pos.y - 200);
			CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage(_key + _illustrator), _pos.x + 10, _pos.y - 220, 1, 1);

			hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
				0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
			oldFont = (HFONT)SelectObject(getMemDC(), hFont);
			DeleteObject(oldFont);

			SetTextColor(getMemDC(), RGB(67, 42, 10));
			wsprintf(str, _name.c_str(), strlen(str));
			DrawText(getMemDC(), str, -1, &_nameRect, DT_LEFT | DT_WORDBREAK);
			
			SetTextColor(getMemDC(), RGB(125, 112, 92));
			wsprintf(str, _talk.c_str(), strlen(str));
			DrawText(getMemDC(), str, -1, &_textRect, DT_LEFT | DT_WORDBREAK);

		}
	    // CAMERAMANAGER->Rectangle(getMemDC(), _rc);
}

void npc::render(NPC_MAP NPC_SHOP)
{
	if (_stop)
	{
		//CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage(_key), _pos.x, _pos.y,
		//	0, IMAGEMANAGER->findImage(_key)->getFrameY());
		// 영훈이형 z오더를 했더니 IMAGEMANAGER->getFrameX() 이게안되서 제가 애니메이션으로 바꿨습니다.....죄송합니다ㅠㅠ
		_aniNpc->aniStop();
		_aniNpc->ZoderRender(_pos.y + IMAGEMANAGER->findImage(_key)->getFrameHeight() / 2, _pos.x, _pos.y);
	}
	else {
		//CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage(_key), _pos.x, _pos.y,
		//	IMAGEMANAGER->findImage(_key)->getFrameX(), IMAGEMANAGER->findImage(_key)->getFrameY());
		if (_aniNpc->getAniState() == ANIMATION_END) _aniNpc->aniRestart();
		_aniNpc->ZoderRender(_pos.y + IMAGEMANAGER->findImage(_key)->getFrameHeight() / 2, _pos.x, _pos.y);
	}
}

void npc::anim()
{
	/*_count++;
	if (_count % 7 == 0)
	{
		_idx++;
		if(RadianToDegree(_angle) >= 225 && RadianToDegree(_angle) <= 360) IMAGEMANAGER->findImage(_key)->setFrameY(0);
		if(RadianToDegree(_angle) >= 45 && RadianToDegree(_angle) < 135) IMAGEMANAGER->findImage(_key)->setFrameY(1);
		if(RadianToDegree(_angle) >= 0 && RadianToDegree(_angle) < 45) IMAGEMANAGER->findImage(_key)->setFrameY(2);
		if(RadianToDegree(_angle) >= 135 && RadianToDegree(_angle) < 225) IMAGEMANAGER->findImage(_key)->setFrameY(3);

		if (_idx > IMAGEMANAGER->findImage(_key)->getMaxFrameX())
		{
			_idx = 0;
		}

		IMAGEMANAGER->findImage(_key)->setFrameX(_idx);
	}*/

	_aniNpc->update();
	if (!_stop)
	{
		if (_key != "에리스" && _key != "강아지npc") {
			if (RadianToDegree(_angle) >= 225 && RadianToDegree(_angle) <= 360) _aniNpc->setFrameY(0);
			if (RadianToDegree(_angle) >= 45 && RadianToDegree(_angle) < 135) _aniNpc->setFrameY(1);
			if (RadianToDegree(_angle) >= 0 && RadianToDegree(_angle) < 45) _aniNpc->setFrameY(2);
			if (RadianToDegree(_angle) >= 135 && RadianToDegree(_angle) < 225) _aniNpc->setFrameY(3);
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

		if (_boxidx < IMAGEMANAGER->findImage("npc대화")->getMaxFrameX())
		{
			_boxidx++;
		}
	}
}

void npc::move()
{
	_time++;

	

	if (!_stop) {

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
			_stop = true;

		}

		_pos.x += cosf(_angle) * _speed;
		_pos.y += -sinf(_angle) * _speed / 2;
	}
}

void npc::move(NPC_MAP NPC_SHOP)
{
	_time++;

	if (!_stop)
	{
		_angle = getAngle(_pos.x, _pos.y, shop_target[shop_targetIdx][shop_currentTargetIdx].x, 
			shop_target[shop_targetIdx][shop_currentTargetIdx].y);
	}
	
	if (getDistance(_pos.x, _pos.y, shop_target[shop_targetIdx][shop_currentTargetIdx].x,
		shop_target[shop_targetIdx][shop_currentTargetIdx].y) < 1)
	{
		
		if (shop_currentTargetIdx < shop_target[shop_targetIdx].size() - 1 && !_delay) {
			_delay = true;
			shop_currentTargetIdx++;

		}

	}
	else {
		_delay = false;
	}
	

	_pos.x += cosf(_angle) * _speed;
	_pos.y += -sinf(_angle) * _speed / 2;
}

void npc::action(string talk)
{
	_talkRect = RectMake(_pos.x, _pos.y,
		IMAGEMANAGER->findImage(_key)->getFrameWidth() * 2, IMAGEMANAGER->findImage(_key)->getFrameHeight() * 2);

	_nameRect = CAMERAMANAGER->RectMake(_pos.x + 200, _pos.y - 150, 100, 30);
	_textRect = CAMERAMANAGER->RectMake(_pos.x + 200, _pos.y - 120, 200, 200);

	_talk = talk;

	RECT tmp;
	if (IntersectRect(&tmp, &PLAYER->getRect(), &_rc) && INPUT->GetKeyDown('J'))
	{
		
		if (_Istalk)
		{
			_Istalk = false;
			_stop = false;
		}
		else {
			_Istalk = true;
			_stop = true;
			lookPlayer();
		}
	}

	if (!IntersectRect(&tmp, &PLAYER->getRect(), &_rc))
	{
		_Istalk = false;
		_stop = false;
	}

}

void npc::collision()
{
	if (_key != "에리스") {
		_rc = RectMake(_pos.x, _pos.y,
			IMAGEMANAGER->findImage(_key)->getFrameWidth(), IMAGEMANAGER->findImage(_key)->getFrameHeight());
	}
	else {
		_rc = RectMake(_pos.x, _pos.y,
			IMAGEMANAGER->findImage(_key)->getFrameWidth(), IMAGEMANAGER->findImage(_key)->getFrameHeight()*2);
	}

}

void npc::lookPlayer()
{
	_aniNpc->update();

	if ((_pos.y < PLAYER->getY() && _pos.y + IMAGEMANAGER->findImage(_key)->getFrameHeight() > PLAYER->getY()) &&
		_pos.x > PLAYER->getX())
	{
		if(_key == "강아지npc")
		_aniNpc->setFrameY(2);
		else 
		_aniNpc->setFrameY(3);
		
	}
	else if ((_pos.y < PLAYER->getY() && _pos.y + IMAGEMANAGER->findImage(_key)->getFrameHeight() > PLAYER->getY()) &&
		_pos.x < PLAYER->getX())
	{
		if (_key == "강아지npc")
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
