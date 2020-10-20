#include "stdafx.h"
#include "bossGolemKing.h"

HRESULT bossGolemKing::init(int x, int y)
{
	//애니메이션 초기화
	_aniBossUp = new animation;
	_aniBossUp->init(IMAGEMANAGER->findImage("bossUp"), 0, 7, false);
	_aniBossDead1 = new animation;
	_aniBossDead1->init(IMAGEMANAGER->findImage("bossDead1"), 0, 7, false);
	_aniBossDead2 = new animation;
	_aniBossDead2->init(IMAGEMANAGER->findImage("bossDead2"), 0, 7, false);
	/*_aniBossFistShoot = new animation;
	_aniBossFistShoot->init(IMAGEMANAGER->findImage("bossFistShoot"), 0, 7, false);*/
	_aniBossHandShootStart = new animation;
	_aniBossHandShootStart->init(IMAGEMANAGER->findImage("bossHandShootStart"), 0, 7, false);
	_aniBossHandShootEnd = new animation;
	_aniBossHandShootEnd->init(IMAGEMANAGER->findImage("bossHandShootEnd"), 0, 7, false);
	_aniIdle = new animation;
	_aniIdle->init(IMAGEMANAGER->findImage("bossIdle"), 0, 50, true);
	_scroll = new animation;
	_scroll->init(IMAGEMANAGER->findImage("golemScroll"), 0, 9);

	_aniBossFistShoot1 = new animation;
	_aniBossFistShoot2 = new animation;
	_aniBossFistShoot3 = new animation;
	vector<POINT> pt;

	for (int i = 0; i < 17; i++)
	{
		pt.push_back(PointMake(i, 0));
	}
	_aniBossFistShoot1->initArray(pt, IMAGEMANAGER->findImage("bossFistShoot"), 7);
	pt.clear();
	for (int i = 0; i < 22; i++)
	{
		pt.push_back(PointMake(i, 1));
	}
	_aniBossFistShoot2->initArray(pt, IMAGEMANAGER->findImage("bossFistShoot"), 7);
	pt.clear();
	for (int i = 0; i < 16; i++)
	{
		pt.push_back(PointMake(i, 2));
	}
	_aniBossFistShoot3->initArray(pt, IMAGEMANAGER->findImage("bossFistShoot"), 7);

	//벡터에 담기
	_vAni.push_back(_aniBossUp);					
	_vAni.push_back(_aniBossDead1);					
	_vAni.push_back(_aniBossDead2);					
	_vAni.push_back(_aniBossFistShoot1);				
	_vAni.push_back(_aniBossFistShoot2);				
	_vAni.push_back(_aniBossFistShoot3);				
	_vAni.push_back(_aniBossHandShootStart);		
	_vAni.push_back(_aniBossHandShootEnd);
	_vAni.push_back(_aniIdle);

	//체력바
	_hpRed = new progressBar;
	_hpRed->init("semiBossHpBarFront(red)", "semiBossHpBarBack", 1000);
	_hpWhite = new progressBar;
	_hpWhite->init("semiBossHpBarFront(white)", "semiBossHpBarBack", 1000);
	_hpBar = _hpRed;


	_hp = 0;
	//_attackCool = RANDOM->range(300,500);
	_attackCool = 200;

	_golemState = GOLEMKINGSTATE::BS_INIT;
	_golemAni = GOLEMANISTATE::ANI_BOSSUP;

	//보스 xy 좌표
	_x = x;
	_y = y;

	//돌공격 카운트 초기화
	_rockFireTime = 0;
	_rockFireCount = 0;


	return S_OK;
}

void bossGolemKing::release()
{
}

void bossGolemKing::update()
{
	//스크롤 업뎃
	_scroll->update();
	//상태에따른 애니메이션 업데이트
	_vAni[(int)_golemAni]->update();
	//체력바 업데이트
	_hpRed->update(_hp);
	_hpWhite->update(_hp);

	//상태에따른 업데이트
	switch (_golemState)
	{
	case GOLEMKINGSTATE::BS_INIT:
		_hp += 5;
		if (_hp > 1000)
		{
			_hp = 1000;
			_golemState = GOLEMKINGSTATE::BS_IDLE;
		}
		CAMERAMANAGER->ChangePivot(_x, _y, 10);
		if ((CAMERAMANAGER->getPivotX() == _x || CAMERAMANAGER->getPivotY() == _y) && !CAMERAMANAGER->getIsShake())
		{
			CAMERAMANAGER->setShake(5, 30, 2);
		}
		break;
	case GOLEMKINGSTATE::BS_IDLE:
		_attackCool--;

		//공격패턴들이 없으면 채워줘라
		if (_vGolemAttack.size() == 0)
			this->initGolemAttack();

		//공격쿨타임이 끝나면 공격~
		if (_attackCool <= 0)
		{
			_attackCool = RANDOM->range(300, 500);
			_golemState = GOLEMKINGSTATE::BS_FIST;
			//손 초기황
			this->initGolemHand();
			//돌슛 초기황
			_rockShootAngle[0] = RANDOM->range(DEGREE(240), DEGREE(300));
			_rockShootAngle[1] = _rockShootAngle[0] + DEGREE(5);
			_rockShootAngle[2] = _rockShootAngle[0] + DEGREE(10);
			//주먹질 초기화
			_bossFist.ani = new animation;
			_bossFist.ani->initReverse(IMAGEMANAGER->findImage("bossFist"), 0, 7, false, true);
			_bossFist.angle = RANDOM->range(DEGREE(240), DEGREE(300))+DEGREE(90);
			_bossFist.x = _x - 70;
			_bossFist.y = _y + 30;
			_bossFist.count = RANDOM->range(4,7);
			_bossFist.isMoveLeft = (bool)RANDOM->range(2);
			_bossFist.isHit = false;

			_golemAni = GOLEMANISTATE::ANI_FISTSHOOT1;
		}

		break;
	case  GOLEMKINGSTATE::BS_FIST:
	{
		if (_vAni[(int)_golemAni]->getAniState() == ANIMATION_END)
		{
			_golemAni = GOLEMANISTATE::ANI_FISTSHOOT2;
		}
		if (_golemAni == GOLEMANISTATE::ANI_FISTSHOOT2)
		{
			_bossFist.ani->update();
			//팔을 뻗고 애니메이션 퍼즈
			if (_bossFist.ani->getCurIndex() == _bossFist.ani->getAniList().size() / 2)
			{
				_bossFist.ani->aniPause();
			}
			//팔 흔들기
			if (_bossFist.ani->getAniState() == ANIMATION_PAUSE)
			{
				if (_bossFist.isMoveLeft)
				{
					_bossFist.angle -= DEGREE(1);
					if (_bossFist.angle <= DEGREE(270))
					{
						_bossFist.isMoveLeft = false;
						_bossFist.isHit = false;
						_bossFist.count--;
					}
				}
				else if (!_bossFist.isMoveLeft)
				{
					_bossFist.angle += DEGREE(1);
					if (_bossFist.angle > DEGREE(360 + 90))
					{
						_bossFist.isMoveLeft = true;
						_bossFist.isHit = false;
						_bossFist.count--;
					}
				}
			}
			//공격끝났으면 다음 애니메이션으로
			if (_bossFist.count <= 0)
			{
				_golemAni = GOLEMANISTATE::ANI_FISTSHOOT3;
			}
		}
		//플레이어와 충돌처리
		if (_golemAni == GOLEMANISTATE::ANI_FISTSHOOT2)
		{
			//플레잉어 선점충돌
			float bx = _bossFist.x - 20;
			float by = _bossFist.y;
			float x = bx + cosf(_bossFist.angle - DEGREE(90)) * 555;
			float y = by - sinf(_bossFist.angle - DEGREE(90)) * 555;
			float sDist = getDistance(bx, by, PLAYER->getX(), PLAYER->getY());
			float eDist = getDistance(x, y, PLAYER->getX(), PLAYER->getY());
			float tDist = getDistance(bx, by, x, y);
			if (sDist + eDist <= tDist + 5.f && sDist + eDist >= tDist - 10.f && _bossFist.isHit == false)
			{
				//영빈아 여기다 충돌처리쓰면 된다
				_bossFist.isHit = true;
				PLAYERDATA->setInDungeonHp(PLAYERDATA->getInDungeonHp() - 10);
				PLAYER->setPlayerState(HIT_IDLE);
				PLAYER->setHit(true);
			}
		}
		//아이들 상태로 바꾸기
		if (_golemAni == GOLEMANISTATE::ANI_FISTSHOOT3)
		{
			//멈춰있던팔 재생
			if (_bossFist.ani->getAniState() == ANIMATION_PAUSE)
				_bossFist.ani->aniPlay();

			if (_bossFist.ani->getAniState() == ANIMATION_END)
			{
				_golemAni = GOLEMANISTATE::ANI_IDLE;
				_golemState = GOLEMKINGSTATE::BS_IDLE;
			}
		}
	}
		break;
	case GOLEMKINGSTATE::BS_ROCK_SHOOT:
		_rockFireTime++;

		//발사시간이 되면 발사해라
		if (_rockFireTime % 20 == 0)
		{
			tagRock rock[3];
			for (int i = 0; i < 3; i++)
			{
				string str = "bossRock" + to_string(RANDOM->range(4));
				rock[i].img = IMAGEMANAGER->findImage(str);
				rock[i].x = _x + cosf(_rockShootAngle[i]) * (100 + _rockFireTime * 5);
				rock[i].y = _y - sinf(_rockShootAngle[i]) * (100 + _rockFireTime * 5);
				rock[i].hight = -100;
				rock[i].time = 0;
				rock[i].shadowScale = 0.5f;
				rock[i].isHit = false;
				_vRock.push_back(rock[i]);
			}
			_rockFireCount++;
		}
		if (_rockFireCount > 8)
		{
			_rockFireCount = 0;
			_rockFireTime = 0;
			_golemState = GOLEMKINGSTATE::BS_IDLE;
		}

		break;
	case GOLEMKINGSTATE::BS_ROCK_ROUND:
		_rockFireTime++;
		
		if (_rockFireTime % 100 == 0)
		{
			int j = RANDOM->range(11);
			for (int i = 0; i <= 10; i++)
			{
				if (i == j) continue;
				float angle = PI / 10;
				string str = "bossRock" + to_string(RANDOM->range(4));
				tagRock _rock;
				_rock.img = IMAGEMANAGER->findImage(str);
				_rock.x = _x - 50 + cosf(angle * i + PI) * (100+_rockFireTime * 3);
				_rock.y = _y - sinf(angle * i + PI) * (100+_rockFireTime * 3);
				_rock.hight = -100;
				_rock.time = 0;
				_rock.shadowScale = 0.5f;
				_rock.isHit = false;
				_vRock.push_back(_rock);
			}
			//공격횟수 ++
			_rockFireCount++;
		}
		if (_rockFireCount >= 2)
		{
			_rockFireCount = 0;
			_rockFireTime = 0;
			_golemState = GOLEMKINGSTATE::BS_IDLE;
		}
		break;
	case GOLEMKINGSTATE::BS_HAND:
	{
		//패턴 시작할때 한번만 초기화해주고
		if (_golemAni != GOLEMANISTATE::ANI_HANDSHOOTSTART && _golemAni != GOLEMANISTATE::ANI_HANDSHOOTEND)
		{
			_golemAni = GOLEMANISTATE::ANI_HANDSHOOTSTART;
			_vAni[(int)_golemAni]->aniRestart();
		}
		//애니메이션 업데
		_golemHand.ani->update();
		//플레이어랑 충돌처리		구르기제외
		RECT temp;
		if (IntersectRect(&temp, &PLAYER->getRect(), &_golemHand.rc) && !_golemHand.isHit && PLAYER->getPlayerState() != PLAYER_ROLL)
		{

			if (PLAYER->getPlayerState() == PLAYER_SHILED)
			{
				PLAYER->playerPush();
			}
			else
			{
				PLAYERDATA->setInDungeonHp(PLAYERDATA->getInDungeonHp() - 10);
				PLAYER->setPlayerState(HIT_IDLE);
				PLAYER->setHit(true);
				_golemHand.isHit = true;
			}
		}

		switch (_golemHand.state)
		{
		case HANDSTATE::HAND_INIT:
		{
			//항상 카메라밖에있어라
			_golemHand.hight = CAMERAMANAGER->getRect().top - 200;

			//플레이어 위치를 따라가게해준다
			float dist = getDistance(_golemHand.x, _golemHand.y, PLAYER->getX(), PLAYER->getY() + 25);
			float angle = getAngle(_golemHand.x, _golemHand.y, PLAYER->getX(), PLAYER->getY() + 25);
			if (dist > 0.5f)
			{
				_golemHand.x += cosf(angle) * 1.5f;
				_golemHand.y -= sinf(angle) * 1.5f;
			}
			//일정시간 지나면 떨구자
			_golemHand.count--;
			if (_golemHand.count <= 0)
			{
				_golemHand.count = 0;
				_golemHand.state = HANDSTATE::HAND_FALL;
			}

			if (_golemHand.ani->getCurIndex() == 1)
				_golemHand.ani->aniPause();
		}
			break;
		case HANDSTATE::HAND_FALL:
			//그림자 크기키우기
			_golemHand.shadowScale += 0.04f;
			if (_golemHand.shadowScale > 1.f) _golemHand.shadowScale = 1.f;

			//손 위로올려
			_golemHand.hight += _golemHand.speed;
			if (_golemHand.hight >= _golemHand.y - 35)
			{
				//위치고정
				_golemHand.hight = _golemHand.y - 35;
				//애니메이션 재생
				if(_golemHand.ani->getCurIndex() < 15)
				_golemHand.ani->aniPlay();
				else _golemHand.ani->aniPause();				
				//위치에 도착했으면 일정시간있다 올려버려
				_golemHand.count++;
				if (_golemHand.count > 140)
				{
					_golemHand.count = 0;
					_golemHand.state = HANDSTATE::HAND_UP;
				}
				//충돌용 렉트 잠깐 만들어라
				if (_golemHand.count < 30)
					_golemHand.rc = RectMakeCenter(_golemHand.x, _golemHand.hight + 20, 100, 50);
				else _golemHand.rc = { 0,0,0,0 };
			}
			break;
		case HANDSTATE::HAND_UP:
			//손 위로올리기
			_golemHand.hight -= _golemHand.speed;
			//애니메이션 재생
			_golemHand.ani->aniPlay();
			//카메라 밖으로 나갔으면 상태변경
			if (_golemHand.hight < CAMERAMANAGER->getRect().top - 200)
			{
				_golemAni = GOLEMANISTATE::ANI_HANDSHOOTEND;
				if (_vAni[(int)_golemAni]->getAniState() == ANIMATION_END)
				{
					_golemAni = GOLEMANISTATE::ANI_IDLE;
					_golemState = GOLEMKINGSTATE::BS_IDLE;
				}
			}
			//그림자 크기 줄이기
			_golemHand.shadowScale -= 0.04f;
			if (_golemHand.shadowScale < 0.2f) _golemHand.shadowScale = 0.2f;
			_golemHand.rc = { 0,0,0,0 };
			break;
		default:
			break;
		}


	}
		break;

	}

	//돌맹이 밑으로 내려오는 업데이트
	for (int i = 0; i < _vRock.size();)
	{
		_vRock[i].hight += 10;
		if (_vRock[i].hight >= _vRock[i].y)
		{
			_vRock[i].hight = _vRock[i].y;
			//바닥 도착하면 타이머 시작
			_vRock[i].time++;
			if (_vRock[i].time < 30)
			{

				_vRock[i].rc = RectMakeCenter(_vRock[i].x, _vRock[i].y + 10, 60 , 60);
			}
			else _vRock[i].rc = { 0,0,0,0 };
		}

		RECT temp;
		if (IntersectRect(&temp, &PLAYER->getRect(), &_vRock[i].rc) && !_vRock[i].isHit)
		{
			PLAYERDATA->setInDungeonHp(PLAYERDATA->getInDungeonHp() - 10);
			PLAYER->setPlayerState(HIT_IDLE);
			PLAYER->setHit(true);
			_vRock[i].isHit = true;
		}

		if (_vRock[i].time > 600)
		{
			_vRock.erase(_vRock.begin() + i);
		}
		else i++;
	}


}

void bossGolemKing::render()
{
	_vAni[(int)_golemAni]->ZorderStretchRender(_y, _x, _y, 3.f);
	CAMERAMANAGER->Rectangle(getMemDC(), RectMakeCenter(_x, _y, 10, 10));
	//스크롤 렌더
	if(_scroll->getAniState() == ANIMATION_PLAY)
	_scroll->stretchRender(getMemDC(), WINSIZEX / 2, WINSIZEY - 150, 2.f);
	//체력바 렌더
	_hpBar->render(WINSIZEX / 2, WINSIZEY - 50);
	//이름 렌더
	RECT txtRC = RectMakeCenter(WINSIZEX / 2, WINSIZEY - 70, 300, 40);
	HFONT hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
	CAMERAMANAGER->ZorderDrawText("골 렘 왕", 2000, txtRC, hFont, RGB(255, 255, 255), DT_CENTER | DT_WORDBREAK | DT_VCENTER);

	switch (_golemState)
	{
	case GOLEMKINGSTATE::BS_INIT:
		break;
	case GOLEMKINGSTATE::BS_IDLE:
		break;
	case GOLEMKINGSTATE::BS_FIST:
		if(_golemAni != GOLEMANISTATE::ANI_FISTSHOOT1)
		_bossFist.ani->ZorderRotateStretchRender(1999, _bossFist.x, _bossFist.y, _bossFist.angle, 1.5f);
		break;
	case GOLEMKINGSTATE::BS_ROCK_SHOOT:
		LineMake(getMemDC(), _x, _y, _x + cosf(_rockShootAngle[0]) * 1000, _y - sinf(_rockShootAngle[0]) * 1000);
		LineMake(getMemDC(), _x, _y, _x + cosf(_rockShootAngle[1]) * 1000, _y - sinf(_rockShootAngle[1]) * 1000);
		LineMake(getMemDC(), _x, _y, _x + cosf(_rockShootAngle[2]) * 1000, _y - sinf(_rockShootAngle[2]) * 1000);
		break;
	case GOLEMKINGSTATE::BS_ROCK_ROUND:
		break;
	case GOLEMKINGSTATE::BS_HAND:
		CAMERAMANAGER->ZorderStretchRender(IMAGEMANAGER->findImage("그림자"), 0, _golemHand.x, _golemHand.y, _golemHand.shadowScale);
		_golemHand.ani->ZorderStretchRender(_golemHand.y, _golemHand.x, _golemHand.hight,3.f);
		CAMERAMANAGER->FrameRect(getMemDC(), _golemHand.rc, RGB(255, 0, 0));
		break;
	}

	//돌맹이 렌더
	for (int i = 0; i < _vRock.size(); i++)
	{
		CAMERAMANAGER->ZorderStretchRender(IMAGEMANAGER->findImage("그림자"), 0, _vRock[i].x, _vRock[i].y, _vRock[i].shadowScale);
		int cx = _vRock[i].x - _vRock[i].img->getWidth() / 2;
		int cy = _vRock[i].hight - _vRock[i].img->getHeight() / 2;
		CAMERAMANAGER->ZorderRender(_vRock[i].img, _vRock[i].y, cx, cy);
		CAMERAMANAGER->FrameRect(getMemDC(), _vRock[i].rc, RGB(255, 0, 0));
	}

	textOut(getMemDC(), 10, 200, to_string((int)_golemState).c_str(), to_string((int)_golemState).size());
}

void bossGolemKing::bossAtkUpdate()
{
}

void bossGolemKing::initGolemAttack()
{
	_vGolemAttack.push_back(GOLEMKINGSTATE::BS_FIST);
	_vGolemAttack.push_back(GOLEMKINGSTATE::BS_HAND);
	switch (RANDOM->range(2))
	{
	case 0:
		_vGolemAttack.push_back(GOLEMKINGSTATE::BS_ROCK_ROUND);
		break;
	case 1:
		_vGolemAttack.push_back(GOLEMKINGSTATE::BS_ROCK_SHOOT);
		break;
	}
}

void bossGolemKing::suffleGolemAttack()
{
	for (int i = 0; i < 100; i++)
	{
		int a = RANDOM->range((int)_vGolemAttack.size());
		int b = RANDOM->range((int)_vGolemAttack.size());
		swap(_vGolemAttack[a], _vGolemAttack[b]);
	}
}

void bossGolemKing::initGolemHand()
{
	_golemHand.ani = new animation;
	_golemHand.ani->init(IMAGEMANAGER->findImage("bossHand"), 0, 7, false);
	_golemHand.shadowScale = 0.2f;
	_golemHand.x = PLAYER->getX();
	_golemHand.y = PLAYER->getY() + 25;
	_golemHand.rc = RectMakeCenter(_golemHand.x, _golemHand.hight + 20, 100, 50);
	_golemHand.hight = CAMERAMANAGER->getRect().top - 200;
	_golemHand.count = 200;
	_golemHand.state = HANDSTATE::HAND_INIT;
	_golemHand.isHit = false;
	_golemHand.speed = 10;
}
