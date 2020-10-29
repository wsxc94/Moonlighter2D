#include "stdafx.h"
#include "bossGolemKing.h"

HRESULT bossGolemKing::init(int x, int y)
{
	//애니메이션 초기화
	_aniBossUp = new animation;
	_aniBossUp->init(IMAGEMANAGER->findImage("bossUp"), 0, 7, false);
	_aniBossDead1 = new animation;
	_aniBossDead1->init(IMAGEMANAGER->findImage("bossDead1"), 0, 5, false);
	_aniBossDead2 = new animation;
	_aniBossDead2->init(IMAGEMANAGER->findImage("bossDead2"), 0, 5, false);
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
	_hpRed->init("semiBossHpBarFront(red)", "semiBossHpBarBack", BOSS_MAXHP);
	_hpWhite = new progressBar;
	_hpWhite->init("semiBossHpBarFront(white)", "semiBossHpBarBack", BOSS_MAXHP);
	_hpBar = _hpRed;


	_hp = 0;
	_attackCool = RANDOM->range(150,300);
	_patternCool = 150;


	_golemState = GOLEMKINGSTATE::BS_INIT;
	_golemAni = GOLEMANISTATE::ANI_BOSSUP;
	_pattern = BOSSPATTERN::EMPTY;

	//보스 xy 좌표
	_x = x;
	_y = y;
	_bossRC = RectMakeCenter(_x - 10, _y + 85, 250, 150);
	_hitCount = 0;


	_golemHand.ani = nullptr;

	_isAttackSoundPlay = false;
	_isDeadSoundPlay = false;
	_isDead = false;
	_isHit = false;
	_coliSkillAroow = false;
	_isFirst = true;
	

	//아이템 초기화
	//아이템 초기화
	_isItemDrop = true;
	_itemIndexSize = 11;
	_itemDropSize = RANDOM->range(3, 10);
	_itemIndex = new int[_itemIndexSize];
	_itemNum = new int[_itemDropSize];

	//나올수 있는 아이템인덱스 초기화
	for (int i = 0; i < 10; i++)
	{
		_itemIndex[i] = i + 1;
	}

	// 드랍될 아이템의 인덱스를 랜덤으로 지정
	for (int i = 0; i < _itemDropSize; i++)
	{
		_itemNum[i] = _itemIndex[RANDOM->range(_itemIndexSize)];
	}

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
	//체력 0되면 죽여!
	if (_hp <= 0 && _golemState != GOLEMKINGSTATE::BS_INIT && _golemState != GOLEMKINGSTATE::BS_DEAD)
	{
		this->changeAniState(GOLEMANISTATE::ANI_DEAD1);
		_golemState = GOLEMKINGSTATE::BS_DEAD;
		_vRock.clear();

	}
	//충돌처리
	this->collisionPlayer();
	this->hitUpdate();

	//상태에따른 업데이트
	switch (_golemState)
	{
	case GOLEMKINGSTATE::BS_INIT:
		_hp += 10;
		if (_hp > BOSS_MAXHP)
		{
			_hp = BOSS_MAXHP;
			_golemState = GOLEMKINGSTATE::BS_IDLE;
			CAMERAMANAGER->ChangePivot(PLAYER->getX(), PLAYER->getY(), 20);
		}
		else
		CAMERAMANAGER->ChangePivot(_x, _y, 10);
		if ((CAMERAMANAGER->getPivotX() == _x || CAMERAMANAGER->getPivotY() == _y) && !CAMERAMANAGER->getIsShake())
		{
			//사운드 재생
			if (!SOUNDMANAGER->isPlaySound("bossInit"))
				SOUNDMANAGER->play("bossInit", 1.f);
			CAMERAMANAGER->setShake(5, 30, 2);
		}
		break;
	case GOLEMKINGSTATE::BS_IDLE:

		switch (_pattern)
		{
		case BOSSPATTERN::PATTERN1:
			if (_isFirst)
				_golemState = GOLEMKINGSTATE::BS_ROCK_ROUND;
			else
			{
				if (_patternCool >= 0)
					_patternCool--;
				else
				{
					_golemState = GOLEMKINGSTATE::BS_FIST;
					this->changeAniState(GOLEMANISTATE::ANI_FISTSHOOT1);
				}
			}
			break;
		case BOSSPATTERN::PATTERN2:
			if (_isFirst)
				_golemState = GOLEMKINGSTATE::BS_ROCK_SHOOT;
			else
			{
				if (_patternCool >= 0)
					_patternCool--;
				else
				{
				_golemState = GOLEMKINGSTATE::BS_FIST;
				this->changeAniState(GOLEMANISTATE::ANI_FISTSHOOT1);
				}
			}
			break;
		case BOSSPATTERN::PATTERN3:
			if (_isFirst)
				_golemState = GOLEMKINGSTATE::BS_ROCK_ROUND;
			else
			{
				if (_patternCool >= 0)
					_patternCool--;
				else
				{
					_golemState = GOLEMKINGSTATE::BS_HAND;
					this->changeAniState(GOLEMANISTATE::ANI_HANDSHOOTSTART);
				}
			}
			break;
		case BOSSPATTERN::PATTERN4:
			if (_isFirst)
				_golemState = GOLEMKINGSTATE::BS_ROCK_SHOOT;
			else
			{
				if (_patternCool >= 0)
					_patternCool--;
				else
				{
					_golemState = GOLEMKINGSTATE::BS_HAND;
					this->changeAniState(GOLEMANISTATE::ANI_HANDSHOOTSTART);
				}
			}
			break;
		case BOSSPATTERN::EMPTY:
			_attackCool--;

			//공격쿨타임이 끝나면 공격~
			if (_attackCool <= 0)
			{
				this->initAttack();
			}
			break;
		default:
			break;
		}
		

		break;
	case  GOLEMKINGSTATE::BS_FIST:
		this->bsFistUpdate();
		break;
	case GOLEMKINGSTATE::BS_ROCK_SHOOT:	// 대각선 돌맹이
		this->bsRockShootUpdate();
		break;
	case GOLEMKINGSTATE::BS_ROCK_ROUND: // 둥그런 돌맹이
		this->bsRockRoundUpdate();
	
		break;
	case GOLEMKINGSTATE::BS_HAND:
		this->bsHandUpdate();
		break;
	case GOLEMKINGSTATE::BS_DEAD:
		if (!_isDeadSoundPlay)
		{
			SOUNDMANAGER->play("bossDead");
			_isDeadSoundPlay = true;
		}
		if (_vAni[(int)_golemAni]->getAniState() == ANIMATION_END)
		{
			if (_golemAni == GOLEMANISTATE::ANI_DEAD1)
			{
				this->changeAniState(GOLEMANISTATE::ANI_DEAD2);
			}
			if (_golemAni == GOLEMANISTATE::ANI_DEAD2)
			{
				_isDead = true;
				_isItemDrop = false;
			}
		}
		break;
	}

	//돌맹이들 업데이트
	this->vRockUpdate();
	this->collisionRock();

}

void bossGolemKing::render()
{
	if(_golemAni == GOLEMANISTATE::ANI_DEAD2)
		_vAni[(int)_golemAni]->ZorderStretchRender(_y, _x, _y + 78, 3.f);
	else
	_vAni[(int)_golemAni]->ZorderStretchRender(_y, _x, _y, 3.f);
	//스크롤 렌더
	if(_scroll->getAniState() == ANIMATION_PLAY)
	_scroll->stretchRender(getMemDC(), WINSIZEX / 2, WINSIZEY - 150, 2.f);
	//체력바 렌더
	if (!_isDead)
	{
		_hpBar->render(WINSIZEX / 2, WINSIZEY - 50);
		//이름 렌더
		RECT txtRC = RectMakeCenter((CAMERAMANAGER->getRect().right + CAMERAMANAGER->getRect().left) / 2
			, CAMERAMANAGER->getRect().bottom - 70, 300, 40);
		HFONT hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
			0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
		CAMERAMANAGER->ZorderDrawText("골 렘 왕", 2000, txtRC, hFont, RGB(255, 255, 255), DT_CENTER | DT_WORDBREAK | DT_VCENTER);
	}
	//CAMERAMANAGER->FrameRect(getMemDC(), _bossRC, RGB(255, 0, 0));

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
		break;
	case GOLEMKINGSTATE::BS_ROCK_ROUND:
		break;
	case GOLEMKINGSTATE::BS_HAND:
		CAMERAMANAGER->ZorderStretchRender(IMAGEMANAGER->findImage("그림자"), 0, _golemHand.x, _golemHand.y, _golemHand.shadowScale);
		_golemHand.ani->ZorderStretchRender(_golemHand.y, _golemHand.x, _golemHand.hight,3.f);
		//CAMERAMANAGER->FrameRect(getMemDC(), _golemHand.rc, RGB(255, 0, 0));
		break;
	}

	//돌맹이 렌더
	for (int i = 0; i < _vRock.size(); i++)
	{
		CAMERAMANAGER->ZorderStretchRender(IMAGEMANAGER->findImage("그림자"), 0, _vRock[i].x, _vRock[i].y, _vRock[i].shadowScale);
		int cx = _vRock[i].x - _vRock[i].img->getWidth() / 2;
		int cy = _vRock[i].hight - _vRock[i].img->getHeight() / 2;
		CAMERAMANAGER->ZorderRender(_vRock[i].img, _vRock[i].y, cx, cy);
		//CAMERAMANAGER->FrameRect(getMemDC(), _vRock[i].colRC, RGB(255, 0, 0));
	}

	//textOut(getMemDC(), 10, 200, to_string((int)_golemState).c_str(), to_string((int)_golemState).size());
}

void bossGolemKing::changeAniState(GOLEMANISTATE state)
{
	_golemAni = state;
	_vAni[(int)_golemAni]->aniRestart();
}

void bossGolemKing::initAttack()
{
	if (_vGolemAttack.size() == 0)
		this->initVGolemAttack();

	if (_vRock.size() > 0) _vRock.clear();

	switch (_vGolemAttack.back())
	{
	case BOSSPATTERN::PATTERN1:
		_bossFist.ani = new animation;
		_bossFist.ani->initReverse(IMAGEMANAGER->findImage("bossFist"), 0, 7, false, true);
		_bossFist.angle = RANDOM->range(DEGREE(240), DEGREE(300)) + DEGREE(90);
		_bossFist.x = _x - 70;
		_bossFist.y = _y + 30;
		_bossFist.count = 5;
		_bossFist.isMoveLeft = (bool)RANDOM->range(2);
		_bossFist.isHit = false;
		break;
	case BOSSPATTERN::PATTERN2:
		_bossFist.ani = new animation;
		_bossFist.ani->initReverse(IMAGEMANAGER->findImage("bossFist"), 0, 7, false, true);
		_bossFist.angle = RANDOM->range(DEGREE(240), DEGREE(300)) + DEGREE(90);
		_bossFist.x = _x - 70;
		_bossFist.y = _y + 30;
		_bossFist.count = 5;
		_bossFist.isMoveLeft = (bool)RANDOM->range(2);
		_bossFist.isHit = false;
		break;
	case BOSSPATTERN::PATTERN3:
		this->initGolemHand();
		break;
	case BOSSPATTERN::PATTERN4:
		//손 초기황
		this->initGolemHand();
		break;
	default:
		break;
	}

	
	_isFirst = true;
	_attackCool = RANDOM->range(150, 300);
	_patternCool = 150;
	_pattern = _vGolemAttack.back();
	_isAttackSoundPlay = false;

	//현재상태 제거
	_vGolemAttack.pop_back();
}

void bossGolemKing::collisionPlayer()
{
	int damage = RANDOM->range(PLAYERDATA->getAtk() - 2, PLAYERDATA->getAtk() + 2);
	RECT temp;
	if (IntersectRect(&temp, &PLAYER->getPlayerAttackBox().rc, &_bossRC))
	{
		if (PLAYER->getPlayerAttackBox().isHit == false && _golemState != GOLEMKINGSTATE::BS_DEAD)
		{
			EFFECTMANAGER->addEffect("공격이펙트", 2000,
				(_bossRC.right + _bossRC.left) / 2,
				(_bossRC.bottom + _bossRC.top) / 2, PLAYER->getPlayerDirection(), 10);
			SOUNDMANAGER->play("화살맞음", 0.5f);
			DAMAGEFONT->init(_x, _y - 30, damage);
			_hp -= damage;
			_isHit = true;
			PLAYER->setPlayerAttackBoxHit(true);
		}
	}

	if (IntersectRect(&temp, &PLAYER->getPlayerAttackTwoBox().rc, &_bossRC))
	{
		if (PLAYER->getPlayerAttackTwoBox().isHit == false && _golemState != GOLEMKINGSTATE::BS_DEAD)
		{
			EFFECTMANAGER->addEffect("공격이펙트", 2000,
				(_bossRC.right + _bossRC.left) / 2,
				(_bossRC.bottom + _bossRC.top) / 2, PLAYER->getPlayerDirection(), 10);
			SOUNDMANAGER->play("화살맞음", 0.5f);
			DAMAGEFONT->init(_x, _y - 30, damage);
			_hp -= damage;
			_isHit = true;
			PLAYER->setPlayerAttackTwoBoxHit(true);
		}
	}

	if (IntersectRect(&temp, &PLAYER->getArrow()->getRect(), &_bossRC) && PLAYER->getArrow()->getIsShoot())
	{
		if (!_coliSkillAroow)
		{
			_coliSkillAroow = true;
			EFFECTMANAGER->addEffect("화살이펙트", (_bossRC.bottom + _bossRC.top) / 2 + 3,
				(_bossRC.right + _bossRC.left) / 2,
				(_bossRC.bottom + _bossRC.top) / 2, PLAYER->getPlayerDirection(), 10);
			DAMAGEFONT->init(_x, _y - 30, damage);

			SOUNDMANAGER->play("화살맞음", 0.5f);

			if (PLAYER->getSkill())
			{
				_hp -= 10 + damage;
				_isHit = true;
			}
			else
			{
				_hp -= damage;
				_isHit = true;
				PLAYER->getArrow()->setIsShoot(false);
			}
		}
	}
	else
	{
		_coliSkillAroow = false;
	}
}

void bossGolemKing::hitUpdate()
{
	if (!_isHit) return;

	_hitCount++;

	if (_hitCount < 10)
	{
		_hpBar = _hpWhite;
	}
	if (_hitCount < 20)
	{
		_hpBar = _hpRed;
	}
	if (_hitCount > 20)
	{
		_hitCount = 0;
		_isHit = false;
		_hpBar = _hpRed;
	}


}

void bossGolemKing::initVGolemAttack()
{
	_vGolemAttack.push_back(BOSSPATTERN::PATTERN1);
	_vGolemAttack.push_back(BOSSPATTERN::PATTERN2);
	_vGolemAttack.push_back(BOSSPATTERN::PATTERN3);
	_vGolemAttack.push_back(BOSSPATTERN::PATTERN4);
	this->suffleVGolemAttack();
}

void bossGolemKing::suffleVGolemAttack()
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
	if(!_golemHand.ani)
	_golemHand.ani = new animation;
	_golemHand.ani->init(IMAGEMANAGER->findImage("bossHand"), 0, 7, false);
	_golemHand.shadowScale = 0.2f;
	_golemHand.x = PLAYER->getX();
	_golemHand.y = PLAYER->getY() + 25;
	_golemHand.rc = RectMakeCenter(_golemHand.x, _golemHand.hight + 20, 100, 50);
	_golemHand.hight = CAMERAMANAGER->getRect().top - 200;
	_golemHand.count = 200;
	_golemHand.atkCount = 0;
	_golemHand.state = HANDSTATE::HAND_INIT;
	_golemHand.isHit = false;
	_golemHand.speed = 30;
}

void bossGolemKing::bsFistUpdate()
{
	
	if (_vAni[(int)_golemAni]->getAniState() == ANIMATION_END && _golemAni == GOLEMANISTATE::ANI_FISTSHOOT1)
	{
		this->changeAniState(GOLEMANISTATE::ANI_FISTSHOOT2);
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
				_bossFist.angle -= DEGREE(3);
				if (_bossFist.angle <= DEGREE(270))
				{
					_bossFist.isMoveLeft = false;
					_bossFist.isHit = false;
					_bossFist.count--;
				}
			}
			else if (!_bossFist.isMoveLeft)
			{
				_bossFist.angle += DEGREE(3);
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
			this->changeAniState(GOLEMANISTATE::ANI_FISTSHOOT3);
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
		if (sDist + eDist <= tDist + 5.f && sDist + eDist >= tDist - 10.f && _bossFist.isHit == false && PLAYER->getPlayerState() != PLAYER_ROLL)
		{
			//영빈아 여기다 충돌처리쓰면 된다
			DAMAGEFONT->init(PLAYER->getX(), PLAYER->getY(), FISTDAMAGE);
			_bossFist.isHit = true;
			PLAYERDATA->setInDungeonHp(PLAYERDATA->getInDungeonHp() - FISTDAMAGE);
			PLAYER->setPlayerState(HIT_IDLE);
			PLAYER->setHit(true);

			if (PLAYERDATA->getInDungeonHp() <= 0)
			{
				RESULTENEMY* em = new RESULTENEMY;
				em->attack = new animation;
				em->attack->init(_bossFist.ani->getImage(), 0, 7, true);
				em->frameY = 0;
				em->scale = 0.5f;
				PLAYERDATA->setKillEnemy(em);
			}
		}
	}
	//아이들 상태로 바꾸기
	if (_golemAni == GOLEMANISTATE::ANI_FISTSHOOT3)
	{
		//멈춰있던팔 재생
		_bossFist.ani->update();
		if (_bossFist.ani->getAniState() == ANIMATION_PAUSE)
		{
			_bossFist.ani->aniPlay();
		}

		if (_bossFist.ani->getAniState() == ANIMATION_END)
		{
			_pattern = BOSSPATTERN::EMPTY;
			_golemAni = GOLEMANISTATE::ANI_IDLE;
			_golemState = GOLEMKINGSTATE::BS_IDLE;
			_vRock.clear();
		}
	}
	
}

void bossGolemKing::bsRockShootUpdate()
{
	for (int p = 0; p < 2; p++)
	{
		float angle;
		if (p == 0)
			angle = DEGREE(290);
		else
			angle = DEGREE(250);
		for (int i = 0; i < 20; i++)
		{
			string str = "bossRock" + to_string(RANDOM->range(4));
			tagRock _rock;
			_rock.img = IMAGEMANAGER->findImage(str);
			if(p == 0)
			_rock.x = _x - 500 + cosf(angle) * (40 * i);
			else
				_rock.x = _x + 500 + cosf(angle) * (40 * i);
			_rock.y = _y - sinf(angle) * (40 * i);
			_rock.hight = -100;
			_rock.time = 0;
			_rock.shadowScale = 0.5f;
			_rock.isHit = false;
			_vRock.push_back(_rock);
		}
	}
	
	if (_isFirst)
		_isFirst = false;
	_golemState = GOLEMKINGSTATE::BS_IDLE;
}

void bossGolemKing::bsRockRoundUpdate()
{
	for (int p = 1; p <= 2; p++)
	{
		for (int i = 0; i <= 20 * p; i++)
		{
			float angle = PI / (20 * p);
			string str = "bossRock" + to_string(RANDOM->range(4));
			tagRock _rock;
			_rock.img = IMAGEMANAGER->findImage(str);
			_rock.x = _x - 50 + cosf(angle * i + PI) * (400 * p);
			_rock.y = _y - sinf(angle * i + PI) * (400 * p);
			_rock.hight = -100;
			_rock.time = 0;
			_rock.shadowScale = 0.5f;
			_rock.isHit = false;
			_vRock.push_back(_rock);
		}
	}
	
	if (_isFirst)
		_isFirst = false;
	_golemState = GOLEMKINGSTATE::BS_IDLE;
}

void bossGolemKing::bsHandUpdate()
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
			DAMAGEFONT->init(PLAYER->getX(), PLAYER->getY(), HANDDAMAGE);
			PLAYERDATA->setInDungeonHp(PLAYERDATA->getInDungeonHp() - HANDDAMAGE);
			PLAYER->setPlayerState(HIT_IDLE);
			PLAYER->setHit(true);
			_golemHand.isHit = true;

			if (PLAYERDATA->getInDungeonHp() <= 0)
			{
				RESULTENEMY* em = new RESULTENEMY;
				em->attack = new animation;
				em->attack->init(_golemHand.ani->getImage(), 0, 7, true);
				em->frameY = 0;
				em->scale = 3.f;
				PLAYERDATA->setKillEnemy(em);
			}
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
			//따라가는 속도
			_golemHand.x += cosf(angle) * 4.f;
			_golemHand.y -= sinf(angle) * 4.f;
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
			if (_golemHand.ani->getCurIndex() < 15)
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
			if (_golemHand.atkCount < 3)
			{
				_golemHand.state = HANDSTATE::HAND_INIT;
				_golemHand.count = 200;
				_golemHand.atkCount++;
				_golemHand.isHit = false;
				_golemHand.ani->aniRestart();
				_golemHand.ani->aniStop();
			}
			else if(_golemHand.atkCount == 3)
			{
				if(_golemAni != GOLEMANISTATE::ANI_HANDSHOOTEND)
				this->changeAniState(GOLEMANISTATE::ANI_HANDSHOOTEND);
				if (_vAni[(int)_golemAni]->getAniState() == ANIMATION_END)
				{
					_pattern = BOSSPATTERN::EMPTY;
					_golemAni = GOLEMANISTATE::ANI_IDLE;
					_golemState = GOLEMKINGSTATE::BS_IDLE;
					_vRock.clear();
				}
			}
			
		}
		//그림자 크기 줄이기
		_golemHand.shadowScale -= 0.04f;
		if (_golemHand.shadowScale < 0.2f) _golemHand.shadowScale = 0.2f;
		_golemHand.rc = { 0,0,0,0 };
		break;
	}
}

void bossGolemKing::vRockUpdate()
{
	//돌맹이 밑으로 내려오는 업데이트
	for (int i = 0; i < _vRock.size(); i++)
	{
		_vRock[i].hight += 10;
		if (_vRock[i].hight >= _vRock[i].y)
		{
			//충돌용렉트생성
			_vRock[i].colRC = RectMakeCenter(_vRock[i].x, _vRock[i].y + 10, 60, 60);
			_vRock[i].hight = _vRock[i].y;
			//바닥 도착하면 타이머 시작
			_vRock[i].time++;
			if (_vRock[i].time < 30)
			{
				_vRock[i].rc = RectMakeCenter(_vRock[i].x, _vRock[i].y + 10, 60, 60);
			}
			else _vRock[i].rc = { 0,0,0,0 };
		}

		RECT temp;
		if (IntersectRect(&temp, &PLAYER->getRect(), &_vRock[i].rc) && !_vRock[i].isHit && PLAYER->getPlayerState() != PLAYER_ROLL)
		{
			DAMAGEFONT->init(PLAYER->getX(), PLAYER->getY(), ROCKDAMAGE);
			PLAYERDATA->setInDungeonHp(PLAYERDATA->getInDungeonHp() - ROCKDAMAGE);
			PLAYER->setPlayerState(HIT_IDLE);
			PLAYER->setHit(true);
			_vRock[i].isHit = true;
			if (PLAYERDATA->getInDungeonHp() <= 0)
			{
				RESULTENEMY* em = new RESULTENEMY;
				em->attack = new animation;
				em->attack->init(_vRock[i].img, 0, 7, true);
				em->frameY = 0;
				em->scale = 1.f;
				PLAYERDATA->setKillEnemy(em);
			}
		}
	}
}

void bossGolemKing::collisionRock()
{
	for (int i = 0; i < _vRock.size(); i++)
	{
		RECT temp;
		if (IntersectRect(&temp, &_vRock[i].colRC, &PLAYER->getShadowRect()))
		{
			int wid = temp.right - temp.left;
			int hei = temp.bottom - temp.top;
			int pWid = (PLAYER->getShadowRect().right - PLAYER->getShadowRect().left) / 2;
			int phei = (PLAYER->getShadowRect().bottom - PLAYER->getShadowRect().top) / 2;

			if (wid > hei) // 위아래
			{
				if (temp.top == PLAYER->getShadowRect().top) // 아래
				{
					PLAYER->setY(_vRock[i].colRC.bottom + phei);
				}
				else  // 위
				{
					PLAYER->setY(_vRock[i].colRC.top - phei);
				}
			}
			else  // 양옆
			{
				if (temp.left == PLAYER->getShadowRect().left) // 오른쪽
				{
					PLAYER->setX(_vRock[i].colRC.right + pWid);
				}
				else // 왼쪽
				{
					PLAYER->setX(_vRock[i].colRC.left - phei);
				}
			}
			
		}
	}
}
