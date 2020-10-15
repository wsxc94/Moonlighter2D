#include "stdafx.h"
#include "player.h"

HRESULT player::init()
{
	
	this->imageInit();

	_player.x = 200;
	_player.y = 200;

	_player.speed = 4;
	_player.shadowRc = RectMakeCenter(_player.x, _player.y, 35, 35);
	_player.rc = RectMakeCenter(_player.x, _player.y, 45, 60);

	_player.direction = 0;
	_player.weapon = SHORT_SOWRD;
	_player.index = 0;
	_player.count = 0;
	_player.dashCount = 0;

	_playerAttackBox.rc = RectMakeCenter(10, 10, 10, 10);
	_playerAttackBox.isHit = false;
	_playerAttackTwoBox.rc = RectMakeCenter(10, 10, 10, 10);
	_playerAttackTwoBox.isHit = false;

	_holeAlpha = 255;
	_hitAlpha = 255;

	_place = TOWN_DUNGEON;
	_state = PLAYER_STATE::PLAYER_IDLE;

	_right = false;
	_left = false;
	_up = false;
	_down = false;
	_isShoot = false;
	_isTalk = false;

	_isHit = false;	//맞았냐
	_isDie = false; //죽었냐

	_rollCount = 0;
	_rollIndex = 0;
	_rollJumpPower = 0;
	_rollGravity = 0;

	_attackCount = 0;
	_attackIndex = 0;;

	_playerHp = PLAYERDATA->getInDungeonHp();

	_arrow = new arrow;
	_arrow->init();

	_displayOn = false;

	return S_OK;
}

void player::release()
{
	_arrow->release();
}

void player::update()
{
	this->playerState();
	this->animation(_player.direction);
	this->hitPlayer();
	this->attackRCUpdate();
	_player.shadowRc = RectMakeCenter(_player.x, _player.y, 35, 35);
	_player.rc = RectMakeCenter(_player.x, _player.y - 12, 45, 60);
	_arrow->update();
	this->keyInput();
	this->updateWeaponState();	
}

void player::render(HDC hdc)
{
	POINT pt = CAMERAMANAGER->getRelativeMouse(PointMake(CAMERAMANAGER->getDistanceX(), CAMERAMANAGER->getDistanceY()));

	if (_state != PLAYER_SWIM && _state != PLAYER_USEPENDANT && _state != PLAYER_DIE_PORTAL) CAMERAMANAGER->ZorderAlphaRender(IMAGEMANAGER->findImage("그림자"), _player.y - 1, pt.x - 35, pt.y - 10, 100);
	if (_isShoot)
	{
		_arrow->render(hdc);
	}
	switch (_place)
	{
	case SHOP:
		switch (_state)
		{
		case PLAYER_IDLE:
			_aniTownIdle->ZoderRender(_player.y, pt.x - 60, pt.y - 68);
			break;
		case PLAYER_RUN:
			_aniTownRun->ZoderRender(_player.y, pt.x - 60, pt.y - 68);
			break;
		case PLAYER_ROLL:
			_aniTownRoll->ZoderRender(_player.y, pt.x - 60, pt.y - 68);
			break;
		case PLAYER_ATTACK_SWORD:

			break;
		case PLAYER_ATTACK_BOW:

			break;
		case PLAYER_DIE:


			break;
		}
		break;

	case TOWN_DUNGEON:
		switch (_state)
		{
		case PLAYER_IDLE:
			_aniDgIdle->ZoderRender(_player.y, pt.x - 60, pt.y - 68);
			break;
		case PLAYER_RUN:
			_aniDgRun->ZoderRender(_player.y, pt.x - 60, pt.y - 68);
			break;
		case PLAYER_ROLL:
			_aniDgRoll->ZoderRender(_player.y, pt.x - 60, pt.y - 68);
			break;
		case PLAYER_FALL:
			_aniFall->ZoderAlphaRender(_player.y, pt.x - 60, pt.y - 68, _holeAlpha);
			break;
		case PLAYER_ATTACK_SWORD:
			_aniSword->ZoderRender(_player.y, pt.x - 60, pt.y - 68);
			break;
		case PLAYER_ATTACK_SWORD_SECOND:
			_aniSwordTwo->ZoderRender(_player.y, pt.x - 60, pt.y - 68);
			break;
		case PLAYER_SHILED:
			_aniShiled->ZoderRender(_player.y, pt.x - 60, pt.y - 68);			
			break;
		case PLAYER_TALK:
			_aniDgIdle->ZoderRender(_player.y, pt.x - 60, pt.y - 68);
			break;
		case PLAYER_ATTACK_BOW:
			_aniBow->ZoderRender(_player.y, pt.x - 60, pt.y - 68);
			break;
		case PLAYER_DIE:
			_aniDie->ZoderRender(_player.y, pt.x - 60, pt.y - 68);
			break;
		case PLAYER_DIE_PORTAL:
			_aniDiePortal->ZoderRender(_player.y, pt.x - 128, pt.y - 140);
			break;
		case PLAYER_SWIM:
			_aniSwim->ZoderRender(_player.y, pt.x - 20, pt.y - 15);
			break;
		case HIT_IDLE:
			_aniHit->ZoderAlphaRender(_player.y, pt.x - 60, pt.y - 68, _hitAlpha);
			break;
		case HIT_RUN:
			_aniRunHit->ZoderAlphaRender(_player.y, pt.x - 60, pt.y - 68, _hitAlpha);
			break;
		case HIT_SWORD:
			_aniSwordHit->ZoderAlphaRender(_player.y, pt.x - 60, pt.y - 68, _hitAlpha);
			break;
		case HIT_SWORD_TWO:
			_aniSwordTwoHit->ZoderAlphaRender(_player.y, pt.x - 60, pt.y - 68, _hitAlpha);
			break;
		}
		break;


	}

}

int player::getMaxItemSlot()
{
	return ITEMMENU->getMaxItemSlot();
}

void player::animation(int frameY)
{

	_aniTownIdle->update();
	_aniTownRun->update();
	_aniTownRoll->update();
	_aniDgIdle->update();
	_aniDgRun->update();
	_aniDgRoll->update();
	_aniFall->update();
	_aniSword->update();
	_aniSwordTwo->update();
	_aniShiled->update();
	_aniBow->update();
	_aniDie->update();
	_aniDiePortal->update();
	_aniSwim->update();
	_aniHit->update();
	_aniRunHit->update();
	_aniSwordHit->update();
	_aniSwordTwoHit->update();

	_aniTownIdle->setFrameY(_player.direction);
	_aniTownRun->setFrameY(_player.direction);
	_aniTownRoll->setFrameY(_player.direction);
	_aniDgIdle->setFrameY(_player.direction);
	_aniDgRun->setFrameY(_player.direction);
	_aniDgRoll->setFrameY(_player.direction);
	_aniFall->setFrameY(0);
	_aniSword->setFrameY(_player.direction);
	_aniSwordTwo->setFrameY(_player.direction);
	_aniShiled->setFrameY(_player.direction);
	_aniBow->setFrameY(_player.direction);
	_aniDie->setFrameY(0);
	_aniDiePortal->setFrameY(0);
	_aniSwim->setFrameY(_player.direction);
	_aniHit->setFrameY(_player.direction);
	_aniRunHit->setFrameY(_player.direction);
	_aniSwordHit->setFrameY(_player.direction);
	_aniSwordTwoHit->setFrameY(_player.direction);

}

void player::playerState()
{
	if (!ITEMMENU->getOpenMenu() && !_displayOn)
	{
		switch (_state)
		{
		case PLAYER_IDLE:
			if (_isHit)
			{
				if (this->getKeyMove()) _state = HIT_RUN;
			}
			if (this->getKeyMove()) _state = PLAYER_RUN;
		
			if (INPUT->GetKeyDown(VK_SPACE))
			{
				if (_place == TOWN_DUNGEON)
				{
					SOUNDMANAGER->play("구르기", 0.3f);
				}
				_lastRollX = _player.x;
				_lastRollY = _player.y;
				_holeAlpha = 255;
				_state = PLAYER_ROLL;
				_aniTownRoll->aniRestart();
				_aniDgRoll->aniRestart();
			}

			this->playerAttack();
			this->playerSkill();

			
			if (PLAYERDATA->getInDungeonHp() <= 0)
			{
				PLAYERDATA->setInDungeonHp(0);
				PLAYER->setPlayerState(PLAYER_DIE);
			}
			break;
		case PLAYER_RUN:
			if (!SOUNDMANAGER->isPlaySound("플레이어걷기"))
			{
				SOUNDMANAGER->play("플레이어걷기", 0.3f);
			}
			if (_isHit)
			{
				if (this->getKeyMove()) _state = HIT_RUN;
			}
			if (!this->getKeyMove()) _state = PLAYER_IDLE;
			this->playerMove();
			if (INPUT->GetKeyDown(VK_SPACE))
			{
				if (_place == TOWN_DUNGEON)
				{
					SOUNDMANAGER->play("구르기", 0.3f);
				}
				_lastRollX = _player.x;
				_lastRollY = _player.y;
				_holeAlpha = 255;
				_state = PLAYER_ROLL;
				_aniTownRoll->aniRestart();
				_aniDgRoll->aniRestart();
			}
			this->playerAttack();
			this->playerSkill();
			
			break;

		case PLAYER_ROLL:
			_rollJumpPower = 5.0f;
			_rollGravity = 0.1f;
			_player.dashCount++;
			if(_player.dashCount % 5 == 0)
			EFFECTMANAGER->addParticle("대시이펙트", _player.y, _player.x, _player.y,true,90);
			_rollCount++;
			switch (_player.direction)
			{
			case 0:
				_player.y += _rollJumpPower;
				break;
			case 1:
				_player.y -= _rollJumpPower;
				break;
			case 2:
				_player.x += _rollJumpPower;
				break;
			case 3:
				_player.x -= _rollJumpPower;
				break;
			}

			_rollJumpPower -= _rollGravity;
			if (_aniDgRoll->getAniState() == ANIMATION_END ||
				_aniTownRoll->getAniState() == ANIMATION_END)
			{
				_state = PLAYER_IDLE;
				_rollCount = 0;
				_rollIndex = 0;
				_rollGravity = 0;
				_rollJumpPower = 0;
				_player.dashCount = 0;
			}
			break;

		case PLAYER_FALL:
			_holeAlpha -= 3;
			if (_holeAlpha <= 0)
			{
				_holeAlpha = 255;
				_player.x = _lastRollX;
				_player.y = _lastRollY;
				_state = PLAYER_IDLE;
			
			}
		
			break;

		case PLAYER_ATTACK_SWORD:
			if (_aniSword->getAniState() == ANIMATION_END)
			{
				_state = PLAYER_IDLE;
				if (INPUT->GetKey('J'))
				{
					_state = PLAYER_ATTACK_SWORD_SECOND;
					_aniSwordTwo->aniRestart();
				}
			}
			break;

		case PLAYER_ATTACK_SWORD_SECOND:
			if (_aniSwordTwo->getAniState() == ANIMATION_END)
			{
				_state = PLAYER_IDLE;
			}
			break;
		case PLAYER_SHILED:
			if (INPUT->GetKeyUp('K'))
			{
				_state = PLAYER_IDLE;
			}
			break;
		case PLAYER_ATTACK_BOW:
			if (_aniBow->getAniState() == ANIMATION_END)
			{
				_state = PLAYER_IDLE;
			}
			if (_aniBow->getCurIndex() == 1)
			{
				this->playerPush();
			}

			break;

		case PLAYER_TALK:
			if (this->getKeyMove()) _state = PLAYER_RUN;

			if (INPUT->GetKeyDown(VK_SPACE))
			{
				_lastRollX = _player.x;
				_lastRollY = _player.y;
				_holeAlpha = 255;
				_state = PLAYER_ROLL;
			}
			break;
		case PLAYER_DIE:
			if (_aniDie->getAniState() == ANIMATION_END && !_isDie)
			{
				_aniDie->aniRestart();
				_isDie = true;
			}
			break;
		case PLAYER_DIE_PORTAL:
		
			break;
		case PLAYER_SWIM:
			this->playerMove();
			break;
		case HIT_IDLE:
			if (this->getKeyMove()) _state = HIT_RUN;
			if (!_isHit)
			{
				_state = PLAYER_IDLE;
			}
			break;
		case HIT_RUN:

			if (this->getKeyMove()) _state = HIT_RUN;
			this->playerMove();
			if (!_isHit)
			{
				_state = PLAYER_RUN;
			}
			break;
		case HIT_SWORD:
			if (!_isHit)
			{
				_state = PLAYER_ATTACK_SWORD;
			}
			break;
		case HIT_SWORD_TWO:
			if (!_isHit)
			{
				_state = PLAYER_IDLE;
			}
			break;
		}
	}
}

void player::attackRCUpdate()
{
	switch (_state)
	{
	case PLAYER_ATTACK_SWORD:
		switch (_player.direction)
		{
		case 0:
			_playerAttackBox.rc = RectMakeCenter(_player.x, _player.y + 30, 80, 40);
			break;
		case 1:
			_playerAttackBox.rc = RectMakeCenter(_player.x, _player.y - 35, 80, 40);
			break;
		case 2:
			_playerAttackBox.rc = RectMakeCenter(_player.x + 30, _player.y, 40, 60);
			break;
		case 3:
			_playerAttackBox.rc = RectMakeCenter(_player.x - 30, _player.y, 40, 60);
			break;
		}
		break;
	case PLAYER_ATTACK_SWORD_SECOND:
		switch (_player.direction)
		{
		case 0:
			_playerAttackTwoBox.rc = RectMakeCenter(_player.x, _player.y + 30, 80, 40);
			break;
		case 1:
			_playerAttackTwoBox.rc = RectMakeCenter(_player.x, _player.y - 35, 80, 40);
			break;
		case 2:
			_playerAttackTwoBox.rc = RectMakeCenter(_player.x + 30, _player.y, 40, 60);
			break;
		case 3:
			_playerAttackTwoBox.rc = RectMakeCenter(_player.x - 30, _player.y, 40, 60);
			break;
		}
		break;

	default:
		_playerAttackBox = { 0,0,0,0 };
		_playerAttackBox.isHit = false;
		_playerAttackTwoBox = { 0,0,0,0 };
		_playerAttackTwoBox.isHit = false;
		break;
	}
}

void player::keyInput()
{
	//무기를 바꾸는 함수
	if (INPUT->GetKeyDown('Z'))
	{
		ITEMMENU->getInventory()->switchWeapon();
	}

	//포션을 사용하는 함수
	//아이템 메뉴가 오픈되지 않았을 때만 동작 
	if (!ITEMMENU->getOpenMenu())
	{
		if (INPUT->GetKeyDown('E'))
		{
			//플레이어가 던전에 있고 && 현재 체력이 풀체력이 아닐 때만 사용 
			if (PLAYERDATA->getIsInDungeon() &&
				PLAYERDATA->getInDungeonHp() < PLAYERDATA->getHp())
			{
				SOUNDMANAGER->play("heal", 0.4f);

				ITEMMENU->getInventory()->usePotionEquipped();
			}
		}
	}
}

void player::updateWeaponState()
{
	switch (ITEMMENU->getInventory()->getWeaponEquipped().getItemIdx())
	{
	case 16:
		_player.weapon = SHORT_SOWRD;
		break;

	case 15:
		_player.weapon = BOW;
		break;

	default:
		_player.weapon = SHORT_SOWRD;
		break;
	}
}

void player::npcTalk(bool& isTalk)
{
	if (isTalk) {
		_player.weapon = EMPTY;
	}
	else {
		_player.weapon = SHORT_SOWRD;
	}
}

void player::hitPlayer()
{
	if (!PLAYERDATA->getIsInDungeon()) return; 


	if (_isHit)
	{
		_playerHp = PLAYERDATA->getInDungeonHp();

		_hitAlpha -= 20;
		if (_hitAlpha < 0)
		{
			_hitAlpha = 255;
			_isHit = false;
		}
	}

}

void player::playerMove()
{

	if (INPUT->GetKey('W'))
	{
		_player.y -= _player.speed;
		_player.direction = 1;
		_up = true;
	}
	else {
		_up = false;
	}

	if (INPUT->GetKey('S'))
	{
		_player.y += _player.speed;
		_player.direction = 0;
		_down = true;
	}
	else {
		_down = false;
	}

	if (INPUT->GetKey('A'))
	{
		_player.x -= _player.speed;
		_player.direction = 3;
		_left = true;
	}
	else {
		_left = false;
	}
	if (INPUT->GetKey('D'))
	{
		_player.x += _player.speed;
		_player.direction = 2;
		_right = true;
	}
	else {
		_right = false;
	}
}

void player::playerAttack()
{
	if (INPUT->GetKey('J') && _place == TOWN_DUNGEON)
	{

		switch (_player.weapon)
		{
		case EMPTY:
			_state = PLAYER_TALK;
			break;

		case SHORT_SOWRD:
			_state = PLAYER_ATTACK_SWORD;
			if (!SOUNDMANAGER->isPlaySound("검휘두르기"))
			{
				SOUNDMANAGER->play("검휘두르기", 0.3f);
			}
			_aniSword->aniRestart();
			break;

		case BOW:
			if (!_isShoot)
			{
				_state = PLAYER_ATTACK_BOW;
				if (!SOUNDMANAGER->isPlaySound("화살발사"))
				{
					SOUNDMANAGER->play("화살발사", 0.3f);
				}
				_aniBow->aniRestart();
				_isShoot = true;
			}
			break;
		}
	}
}

void player::playerSkill()
{
	if (INPUT->GetKey('K'))
	{
		switch (_player.weapon)
		{
		case EMPTY:
			break;
		case SHORT_SOWRD:
			_state = PLAYER_SHILED;
			break;
		case BOW:
			break;
		}
	}
}

void player::playerPush()
{
	switch (_player.direction)
	{
	case 0:
		_player.y -= 3;
		break;
	case 1:
		_player.y += 3;
		break;
	case 2:
		_player.x -= 3;
		break;
	case 3:
		_player.x += 3;
		break;
	}
}

bool player::getKeyMove()
{
	if (INPUT->GetKey('W') || INPUT->GetKey('S')
		|| INPUT->GetKey('A')
		|| INPUT->GetKey('D')) return true;
	return false;
}

void player::imageInit()
{
	IMAGEMANAGER->addFrameImage("던전구르기", "Images/플레이어/player_roll_dungeon8X4.bmp", 960, 480, 8, 4);
	IMAGEMANAGER->addFrameImage("던전달리기", "Images/플레이어/player_run_dungeon8X4.bmp", 960, 480, 8, 4);
	IMAGEMANAGER->addFrameImage("던전idle", "Images/플레이어/player_idle_dungeon10X4.bmp", 1200, 480, 10, 4);

	IMAGEMANAGER->addFrameImage("구르기", "Images/플레이어/shop_roll.bmp", 960, 480, 8, 4);
	IMAGEMANAGER->addFrameImage("달리기", "Images/플레이어/shop_run8x4.bmp", 960, 480, 8, 4);
	IMAGEMANAGER->addFrameImage("idle", "Images/플레이어/shop_idle10x4.bmp", 1200, 480, 10, 4);

	IMAGEMANAGER->addFrameImage("죽음", "Images/플레이어/player_die10X1.bmp", 1200, 120, 10, 1);
	IMAGEMANAGER->addFrameImage("구멍빠짐", "Images/플레이어/hole_fall3X1.bmp", 360, 120, 3, 1);
	IMAGEMANAGER->addFrameImage("죽음포탈", "Images/플레이어/teleportOut58x1.bmp", 7424, 128, 58, 1);

	IMAGEMANAGER->addFrameImage("활날리기", "Images/플레이어/bow_attack9X4.bmp", 1080, 480, 9, 4);
	IMAGEMANAGER->addFrameImage("활스킬", "Images/플레이어/bow_skill2X4.bmp", 240, 480, 2, 4);

	IMAGEMANAGER->addFrameImage("숏소드", "Images/플레이어/short_attack6X4.bmp", 720, 480, 6, 4);
	IMAGEMANAGER->addFrameImage("숏소드2연격", "Images/플레이어/short_attack_two5X4.bmp", 600, 480, 5, 4);
	IMAGEMANAGER->addFrameImage("방패", "Images/플레이어/shiled_state1X4.bmp", 120, 480, 1, 4);

	IMAGEMANAGER->addImage("그림자", "Images/플레이어/player_Shadow.bmp", 70, 50, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("온천", "Images/플레이어/swimState10X4.bmp", 500, 208, 10, 4);

	IMAGEMANAGER->addFrameImage("던전idleHIT", "Images/플레이어/player_idle_dungeon_Hit10X4.bmp", 1200, 480, 10, 4);
	IMAGEMANAGER->addFrameImage("던전달리기HIT", "Images/플레이어/player_run_dungeon_Hit8X4.bmp", 960, 480, 8, 4);
	IMAGEMANAGER->addFrameImage("숏소드HIT", "Images/플레이어/short_attack_Hit6X4.bmp", 720, 480, 6, 4);
	IMAGEMANAGER->addFrameImage("숏소드2연격HIT", "Images/플레이어/short_attack_two_Hit5X4.bmp", 600, 480, 5, 4);

	IMAGEMANAGER->addFrameImage("대시이펙트", "Images/플레이어/roll_dust6X1.bmp", 240, 40, 6, 1);

	

	_aniTownIdle = new ::animation;
	_aniTownRun = new ::animation;
	_aniTownRoll = new ::animation;
	_aniDgIdle = new ::animation;
	_aniDgRun = new ::animation;
	_aniDgRoll = new ::animation;
	_aniFall = new ::animation;
	_aniSword = new ::animation;
	_aniSwordTwo = new ::animation;
	_aniShiled = new ::animation;
	_aniBow = new ::animation;
	_aniDie = new ::animation;
	_aniSwim = new ::animation;
	_aniHit = new ::animation;
	_aniRunHit = new ::animation;
	_aniSwordHit = new ::animation;
	_aniSwordTwoHit = new ::animation;
	_aniDiePortal = new ::animation;

	_aniTownIdle->init(IMAGEMANAGER->findImage("idle"), 0, 7, true);
	_aniTownRun->init(IMAGEMANAGER->findImage("달리기"), 0, 5, true);
	_aniTownRoll->init(IMAGEMANAGER->findImage("구르기"), 0, 7);
	_aniTownRoll->aniStop();
	_aniDgIdle->init(IMAGEMANAGER->findImage("던전idle"), 0, 7, true);
	_aniDgRun->init(IMAGEMANAGER->findImage("던전달리기"), 0, 5, true);
	_aniDgRoll->init(IMAGEMANAGER->findImage("던전구르기"), 0, 5);
	_aniDgRoll->aniStop();
	_aniFall->init(IMAGEMANAGER->findImage("구멍빠짐"), 0, 7);
	_aniFall->aniStop();
	_aniSword->init(IMAGEMANAGER->findImage("숏소드"), 0, 5);
	_aniSword->aniStop();
	_aniSwordTwo->init(IMAGEMANAGER->findImage("숏소드2연격"), 0, 5);
	_aniSwordTwo->aniStop();

	_aniShiled->init(IMAGEMANAGER->findImage("방패"), 0, 1);

	_aniBow->init(IMAGEMANAGER->findImage("활날리기"), 0, 5);
	_aniBow->aniStop();
	_aniDie->init(IMAGEMANAGER->findImage("죽음"), 0, 7);
	_aniDie->aniStop();
	_aniSwim->init(IMAGEMANAGER->findImage("온천"), 0, 7);
	_aniHit->init(IMAGEMANAGER->findImage("던전idleHIT"), 0, 7, true);
	_aniRunHit->init(IMAGEMANAGER->findImage("던전달리기HIT"), 0, 5, true);
	_aniSwordHit->init(IMAGEMANAGER->findImage("숏소드HIT"), 0, 5);
	_aniSwordHit->aniStop();
	_aniSwordTwoHit->init(IMAGEMANAGER->findImage("숏소드2연격HIT"), 0, 5);
	_aniSwordTwoHit->aniStop();
	_aniDiePortal->init(IMAGEMANAGER->findImage("죽음포탈"), 0, 7);
	_aniDiePortal->aniStop();

}

