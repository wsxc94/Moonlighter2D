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
	_isTalk = false;

	_isHit = false;	//맞았냐
	_isDie = false; //죽었냐
	_isReturn = false;

	_isSkill = false;

	_rollCount = 0;
	_rollIndex = 0;
	_rollJumpPower = 0;
	_rollAngle = 0;

	_skillCount = 0;
	_skillIndex = 0;

	_attackCount = 0;
	_attackIndex = 0;;

	_playerHp = PLAYERDATA->getInDungeonHp();

	_arrow = make_shared<arrow>();
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
	this->arrowSkillSet();
	this->setPlayerRoll();
	this->playerState();
	this->animation(_player.direction);
	this->hitPlayer();
	this->attackRCUpdate();
	_player.shadowRc = RectMakeCenter(_player.x, _player.y, 35, 35);
	_player.rc = RectMakeCenter(_player.x, _player.y - 12, 45, 60);
	_arrow->update();
	this->keyInput();
}

void player::render(HDC hdc)
{
	if (_state != PLAYER_SWIM && _state != PLAYER_DIE_PORTAL) CAMERAMANAGER->ZorderAlphaRender(IMAGEMANAGER->findImage("그림자"), _player.y - 1, _player.x - 18, _player.y + 7, 100);
	if(_arrow->getIsShoot())
	_arrow->render(hdc);
	switch (_place)
	{
	case SHOP:
		switch (_state)
		{
		case PLAYER_IDLE:
			_aniTownIdle->ZoderRender(_player.y, _player.x - 60, _player.y - 68);
			break;
		case PLAYER_RUN:
			_aniTownRun->ZoderRender(_player.y, _player.x - 60, _player.y - 68);
			break;
		case PLAYER_ROLL:
			_aniTownRoll->ZoderRender(_player.y, _player.x - 60, _player.y - 68);
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
			_aniDgIdle->ZoderRender(_player.y, _player.x - 60, _player.y - 68);
			break;
		case PLAYER_RUN:
			_aniDgRun->ZoderRender(_player.y, _player.x - 60, _player.y - 68);
			break;
		case PLAYER_ROLL:
			_aniDgRoll->ZoderRender(_player.y, _player.x - 60, _player.y - 68);
			break;
		case PLAYER_FALL:
			_aniFall->ZoderAlphaRender(_player.y, _player.x - 60, _player.y - 68, _holeAlpha);
			break;
		case PLAYER_ATTACK_SWORD:
			_aniSword->ZoderRender(_player.y, _player.x - 60, _player.y - 68);
			break;
		case PLAYER_ATTACK_SWORD_SECOND:
			_aniSwordTwo->ZoderRender(_player.y, _player.x - 60, _player.y - 68);
			break;
		case PLAYER_SHILED:
			_aniShiled->ZoderRender(_player.y, _player.x - 60, _player.y - 68);
			break;
		case PLAYER_TALK:
			_aniDgIdle->ZoderRender(_player.y, _player.x - 60, _player.y - 68);
			break;
		case PLAYER_ATTACK_BOW:
			_aniBow->ZoderRender(_player.y, _player.x - 60, _player.y - 68);
			break;
		case BOW_CHARGE:
			_bowCharge->ZoderRender(_player.y, _player.x - 60, _player.y - 68);
			break;
		case PLAYER_DIE:
			_aniDie->ZoderRender(_player.y, _player.x - 60, _player.y - 68);
			break;
		case PLAYER_DIE_PORTAL:
			_aniDiePortal->ZorderStretchRender(_player.y, _player.x, _player.y - 20,2.f);
			break;
		case PLAYER_SWIM:
			_aniSwim->ZoderRender(_player.y, _player.x - 20, _player.y - 15);
			break;
		case HIT_IDLE:
			_aniHit->ZoderAlphaRender(_player.y, _player.x - 60, _player.y - 68, _hitAlpha);
			break;
		case HIT_RUN:
			_aniRunHit->ZoderAlphaRender(_player.y, _player.x - 60, _player.y - 68, _hitAlpha);
			break;
		case HIT_SWORD:
			_aniSwordHit->ZoderAlphaRender(_player.y, _player.x - 60, _player.y - 68, _hitAlpha);
			break;
		case HIT_SWORD_TWO:
			_aniSwordTwoHit->ZoderAlphaRender(_player.y, _player.x - 60, _player.y - 68, _hitAlpha);
			break;
		}
		break;


	}

	/*TextOut(hdc, 10, 200 + 30, ("right " + to_string(_right)).c_str(), ("right " + to_string(_right)).size());
	TextOut(hdc, 10, 200 + 60, ("left	" + to_string(_left)).c_str(), ("left " + to_string(_left)).size());
	TextOut(hdc, 10, 200 + 90, ("up " + to_string(_up)).c_str(), ("up " + to_string(_up)).size());
	TextOut(hdc, 10, 200 + 120, ("down " + to_string(_down)).c_str(), ("down " + to_string(_down)).size());*/

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
	_bowCharge->update();
	

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
	_bowCharge->setFrameY(_player.direction);
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
					SOUNDMANAGER->play("구르기", 0.2f);
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

		case PLAYER_RUN:
			if (!SOUNDMANAGER->isPlaySound("플레이어걷기"))
			{
				SOUNDMANAGER->play("플레이어걷기", 0.15f);
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
			_player.dashCount++;
			if(_player.dashCount % 5 == 0)
			EFFECTMANAGER->addParticle("대시이펙트", _player.y, _player.x, _player.y,true,90);
			_rollCount++;
			
			_player.x += cosf(_rollAngle) * _rollJumpPower;
			_player.y -= sinf(_rollAngle) * _rollJumpPower;


			if (_aniDgRoll->getAniState() == ANIMATION_END ||
				_aniTownRoll->getAniState() == ANIMATION_END)
			{
				_state = PLAYER_IDLE;
				_rollCount = 0;
				_rollIndex = 0;
				_rollJumpPower = 0;
				_player.dashCount = 0;
				
				switch (_aniTownRoll->getFrameY())
				{
				case 0: // 아래
					_rollAngle = DEGREE(270);
					break;
				case 1: // 위
					_rollAngle = DEGREE(90);
					break;
				case 2: // 오
					_rollAngle = DEGREE(0);
					break;
				case 3: // 왼
					_rollAngle = DEGREE(180);
					break;
				default:
					break;
				}
				_right = false;
				_left = false;
				_up = false;
				_down = false;
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
					SOUNDMANAGER->play("검휘두르기", 0.3f);
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
			if (INPUT->GetKey('W'))
			{
				_player.y -= 1;
				_player.direction = 1;
				_up = true;
			}
			else {
				_up = false;
			}

			if (INPUT->GetKey('S'))
			{
				_player.y += 1;
				_player.direction = 0;
				_down = true;
			}
			else {
				_down = false;
			}

			if (INPUT->GetKey('A'))
			{
				_player.x -=  1;
				_player.direction = 3;
				_left = true;
			}
			else {
				_left = false;
			}
			if (INPUT->GetKey('D'))
			{
				_player.x += 1;
				_player.direction = 2;
				_right = true;
			}
			else {
				_right = false;
			}
			if (INPUT->GetKeyUp('K'))
			{
				_state = PLAYER_IDLE;
			}
			break;
		case BOW_CHARGE:
			if (INPUT->GetKeyUp('K'))
			{
				if (_isSkill)
				{
					_arrow->setIsShoot(true);
				}
				_state = PLAYER_IDLE;
				_skillCount = 0;
				if (!SOUNDMANAGER->isPlaySound("화살발사"))
				{
					SOUNDMANAGER->play("화살발사", 0.3f);
				}
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
			this->playerMove();
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
			if (_aniDiePortal->getAniState() == ANIMATION_END)
			{
				if (!_isReturn)
				{
					_aniDiePortal->aniRestart();
					_isReturn = true;
				}
				else
				{
					_isReturn = false;
					_state = PLAYER_IDLE;
				}
			}		
		
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
		updateWeaponState();
		ITEMMENU->getInventory()->updateStatus();
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
	case TRAININGSWORD_IDX:
		_player.weapon = SHORT_SOWRD;
		break;

	case TRAININGBOW_IDX:
		_player.weapon = BOW;
		break;

	//default:
	//	_player.weapon = SHORT_SOWRD;
	//	break;
	}
}

void player::npcTalk(bool& isTalk)
{
	if (isTalk) 
	{
		_player.weapon = EMPTY;
	}
	else
	{
		this->updateWeaponState();
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

			break;
		case SHORT_SOWRD:
			_state = PLAYER_ATTACK_SWORD;
			SOUNDMANAGER->play("검휘두르기", 0.3f);
			_aniSword->aniRestart();
			break;

		case BOW:
			if (!_arrow->getIsShoot() && _state != PLAYER_ATTACK_BOW)
			{
				_state = PLAYER_ATTACK_BOW;
				if (!SOUNDMANAGER->isPlaySound("화살발사"))
				{
					SOUNDMANAGER->play("화살발사", 0.3f);
				}
				_aniBow->aniRestart();
				_arrow->setIsShoot(true);
			}
			break;
		}
		
	}
	
}

void player::playerSkill()
{
	if (INPUT->GetKey('K') && _place == TOWN_DUNGEON)
	{
		_bowCharge->aniRestart();
		switch (_player.weapon)
		{
		case EMPTY:
			break;
		case SHORT_SOWRD:
			_state = PLAYER_SHILED;
			break;
		case BOW:
			if (!SOUNDMANAGER->isPlaySound("화살스킬") && !_arrow->getIsShoot())
			{
				SOUNDMANAGER->play("화살스킬", 0.3f);
			}
			if (!_arrow->getIsShoot())
			{
				_state = BOW_CHARGE;
			}
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

void player::arrowSkillSet()
{
	
	if (_state == BOW_CHARGE)
	{
		_skillCount++;
	}
	if (_skillCount > 50)
	{
		_bowCharge->aniStop();
		_isSkill = true;
	}

}

void player::setPlayerRoll()
{
	//오른쪽은 0
	//왼쪽은 PI
	//위는 pi/2
	//아래는 PI/2 *3
	//우상 pi /4 ,dw
	//좌상 pi/4 * 3
	//우하 pi / 4 *3 +pi
	//좌하 pi /4 + pi
	if (_right)
	{
		if (_up)
		{
			_rollAngle = PI / 4;
		}
		else if (_down)
		{
			_rollAngle = (PI / 4 * 3) + PI;
		}
		else
		{
			_rollAngle = 0;
		}
	}
	else if (_left)
	{
		if (_up)
		{
			_rollAngle = PI / 4 * 3;
		}
		else if (_down)
		{
			_rollAngle = PI / 4 + PI;
		}
		else
		{
			_rollAngle = PI;
		}
	}
	else if (_up)
	{
		_rollAngle = PI / 2;
	}
	else if (_down)
	{
		_rollAngle = PI / 2 * 3;
	}

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

	IMAGEMANAGER->addFrameImage("활날리기", "Images/플레이어/bow_attack5X4.bmp", 600, 480, 5, 4);
	IMAGEMANAGER->addFrameImage("활스킬", "Images/플레이어/bow_skill2X4.bmp", 240, 480, 2, 4);

	IMAGEMANAGER->addFrameImage("숏소드", "Images/플레이어/short_attack5X4.bmp", 600, 480, 5, 4);
	IMAGEMANAGER->addFrameImage("숏소드2연격", "Images/플레이어/short_attack_two4X4.bmp", 480, 480, 4, 4);
	IMAGEMANAGER->addFrameImage("방패", "Images/플레이어/shiled_state1X4.bmp", 120, 480, 1, 4);

	IMAGEMANAGER->addFrameImage("그림자", "Images/npc/npc_shadow.bmp", 36, 20, 1, 1);
	IMAGEMANAGER->addFrameImage("온천", "Images/플레이어/swimState10X4.bmp", 500, 208, 10, 4);

	IMAGEMANAGER->addFrameImage("던전idleHIT", "Images/플레이어/player_idle_dungeon_Hit10X4.bmp", 1200, 480, 10, 4);
	IMAGEMANAGER->addFrameImage("던전달리기HIT", "Images/플레이어/player_run_dungeon_Hit8X4.bmp", 960, 480, 8, 4);
	IMAGEMANAGER->addFrameImage("숏소드HIT", "Images/플레이어/short_attack_Hit6X4.bmp", 720, 480, 6, 4);
	IMAGEMANAGER->addFrameImage("숏소드2연격HIT", "Images/플레이어/short_attack_two_Hit5X4.bmp", 600, 480, 5, 4);

	IMAGEMANAGER->addFrameImage("대시이펙트", "Images/플레이어/roll_dust6X1.bmp", 240, 40, 6, 1);

	_aniTownIdle = make_unique<::animation>();
	_aniTownRun = make_unique<::animation>();
	_aniTownRoll = make_unique<::animation>();
	_aniDgIdle = make_unique<::animation>();
	_aniDgRun = make_unique<::animation>();
	_aniDgRoll = make_unique<::animation>();
	_aniFall = make_unique<::animation>();
	_aniSword = make_unique<::animation>();
	_aniSwordTwo = make_unique<::animation>();
	_aniBow = make_unique<::animation>();
	_aniDie = make_unique<::animation>();
	_aniSwim = make_unique<::animation>();
	_aniShiled = make_unique<::animation>();
	_bowCharge = make_unique<::animation>();
	_aniHit = make_unique<::animation>();
	_aniRunHit = make_unique<::animation>();
	_aniSwordHit = make_unique<::animation>();
	_aniSwordTwoHit = make_unique<::animation>();
	_aniDiePortal = make_unique<::animation>();

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
	_bowCharge->init(IMAGEMANAGER->findImage("활스킬"), 0, 3, true);
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

unique_ptr<::animation>& player::getAnimation()
{
	switch (_state)
	{
	case PLAYER_IDLE:
		break;
	case PLAYER_RUN:
		break;
	case PLAYER_ROLL:
		break;
	case PLAYER_ATTACK_SWORD:
		break;
	case PLAYER_ATTACK_SWORD_SECOND:
		break;
	case PLAYER_ATTACK_BOW:
		break;
	case PLAYER_SHILED:
		break;
	case BOW_CHARGE:
		break;
	case PLAYER_TALK:
		break;
	case PLAYER_DIE:
		return _aniDie;
		break;
	case PLAYER_DIE_PORTAL:
		break;
	case PLAYER_FALL:
		break;
	case PLAYER_SWIM:
		break;
	case HIT_IDLE:
		break;
	case HIT_RUN:
		break;
	case HIT_SWORD:
		break;
	case HIT_SWORD_TWO:
		break;
	}
	return _aniDgIdle;
}

