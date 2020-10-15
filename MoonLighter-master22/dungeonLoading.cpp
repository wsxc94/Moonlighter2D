#include "stdafx.h"
#include "dungeonLoading.h"

HRESULT dungeonLoading::init()
{
	_loading = new loading;
	_loading->init();

	this->loadImage();
	this->loadSound();

	return S_OK;
}

void dungeonLoading::release()
{
	if (_loading)
	{
		_loading->release();
		SAFE_DELETE(_loading);
	}
}

void dungeonLoading::update()
{
	_loading->update();
	if (_loading->loadingDone())
	{
		SCENEMANAGER->loadScene("노말던전");
	}
}

void dungeonLoading::render()
{
	_loading->render();
}

void dungeonLoading::loadImage()
{
	_loading->loadImage("dungeonBack", "Images/맵툴/dungeon_default37X21.bmp", 1280, 720);
	_loading->loadImage("dungeonSpa", "Images/맵툴/dungeon_spa.bmp", 1280, 720);
	_loading->loadImage("dungeonSpaPixel", "Images/맵툴/dungeon_spaPixcelCollision.bmp", 1280, 720);
	_loading->loadFrameImage("minimapEntranceH", "Images/던전/minimapEntranceH.bmp", 20, 12, 2, 2);
	_loading->loadFrameImage("minimapEntranceW", "Images/던전/minimapEntranceW.bmp", 12, 20, 2, 2);
	_loading->loadFrameImage("minimap", "Images/던전/minimap.bmp", 172, 48, 2, 1);
	_loading->loadFrameImage("golemScroll", "Images/던전/골렘던전스크롤11.bmp", 2277, 60, 11, 1);

	_loading->loadFrameImage("던전돌", "Images/맵툴/던전돌.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전돌2", "Images/맵툴/던전돌2.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전돌3", "Images/맵툴/던전돌3.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전돌4", "Images/맵툴/던전돌4.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전돌5", "Images/맵툴/던전돌5.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("나무뿌리", "Images/맵툴/나무뿌리.bmp", 105, 84, 1, 1);
	_loading->loadFrameImage("나무뿌리2", "Images/맵툴/나무뿌리2.bmp", 84, 126, 1, 1);
	_loading->loadFrameImage("던전텐트", "Images/맵툴/던전텐트.bmp", 280, 210, 1, 1);
	_loading->loadFrameImage("던전책", "Images/맵툴/던전책.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전의자", "Images/맵툴/던전의자.bmp", 70, 70, 1, 1);
	_loading->loadFrameImage("던전통", "Images/맵툴/던전통.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전보물상자", "Images/맵툴/던전보물상자14.bmp", 420, 38, 14, 1);

	_loading->loadFrameImage("던전타일", "Images/맵툴/던전타일.bmp", 175, 175, SAMPLETILEX, SAMPLETILEY);
	_loading->loadFrameImage("잔디타일", "Images/맵툴/잔디타일.bmp", 175, 175, SAMPLETILEX, SAMPLETILEY);
	_loading->loadFrameImage("쩌리타일", "Images/맵툴/쩌리타일2.bmp", 175, 175, SAMPLETILEX, SAMPLETILEY);

	_loading->loadFrameImage("babySlime", "Images/몬스터/babySlime9x4(2).bmp", 576, 256, 9, 4);
	_loading->loadFrameImage("bigSlime", "Images/몬스터/BigSlime12x2.bmp", 984, 526, 12, 2);
	_loading->loadFrameImage("골렘공격", "Images/몬스터/GolemAttack13x4.bmp", 2470, 760, 13, 4);
	_loading->loadFrameImage("골렘전사", "Images/몬스터/GolemMove8x4.bmp", 1520, 760, 8, 4);
	_loading->loadFrameImage("red골렘공격", "Images/몬스터/RedGolemAttack13x4.bmp", 2470, 760, 13, 4);
	_loading->loadFrameImage("red골렘전사", "Images/몬스터/RedGolemMove8x4.bmp", 1520, 760, 8, 4);
	_loading->loadFrameImage("골렘공격피격", "Images/몬스터/GolemAttackHit13x4.bmp", 2470, 760, 13, 4);
	_loading->loadFrameImage("골렘전사피격", "Images/몬스터/GolemMoveHit8x4.bmp", 1520, 760, 8, 4);
	_loading->loadFrameImage("항아리", "Images/몬스터/pot11x4.bmp", 519, 235, 11, 4);
	_loading->loadFrameImage("항아리Hit", "Images/몬스터/pot11x4Hit.bmp", 519, 235, 11, 4);
	_loading->loadFrameImage("항아리총알H", "Images/몬스터/potBulletH.bmp", 11, 34, 1, 2);
	_loading->loadFrameImage("항아리총알W", "Images/몬스터/potBulletW.bmp", 17, 22, 1, 2);
	_loading->loadFrameImage("해골전사", "Images/몬스터/skullHammerMove14X4.bmp", 2982, 688, 14, 4);
	_loading->loadFrameImage("해골전사피격", "Images/몬스터/skullHammerMoveHit14X4.bmp", 2982, 688, 14, 4);
	_loading->loadFrameImage("해골공격", "Images/몬스터/skullHammerAttack22X4.bmp", 4686, 688, 22, 4);
	_loading->loadFrameImage("해골공격피격", "Images/몬스터/skullHammerAttackHit22X4.bmp", 4686, 688, 22, 4);
	_loading->loadFrameImage("가스맨", "Images/몬스터/gasManMove12.bmp", 1488, 472, 12, 4);
	_loading->loadFrameImage("가스맨피격", "Images/몬스터/gasManMoveHit12.bmp", 1488, 472, 12, 4);
	_loading->loadFrameImage("가스맨공격", "Images/몬스터/gasManAttack10.bmp", 1240, 472, 10, 4);
	_loading->loadFrameImage("가스맨공격피격", "Images/몬스터/gasManAttackHit10.bmp", 1240, 472, 10, 4);
	_loading->loadFrameImage("가스화살W", "Images/몬스터/gasArrowW.bmp", 48, 5, 2, 1);
	_loading->loadFrameImage("가스화살H", "Images/몬스터/gasArrowH.bmp", 10, 24, 2, 1);
	_loading->loadFrameImage("enemyDie", "Images/몬스터/enemyDie11.bmp", 1320, 136, 11, 1);
	_loading->loadFrameImage("potDie", "Images/몬스터/potDie7(2).bmp", 602, 116, 7, 1);

	_loading->loadFrameImage("skeletonMove", "Images/몬스터/bossSkeletonMove14X4.bmp", 2324, 621, 14, 4);
	_loading->loadFrameImage("skeletonMoveHitRed", "Images/몬스터/bossSkeletonMove14X4HitRed.bmp", 2324, 621, 14, 4);
	_loading->loadFrameImage("skeletonMoveHitWhite", "Images/몬스터/bossSkeletonMove14X4HitWhite.bmp", 2324, 621, 14, 4);
	_loading->loadFrameImage("skeletonAttackSword", "Images/몬스터/bossSkeletonSword22X4.bmp", 3652, 621, 22, 4);
	_loading->loadFrameImage("skeletonAttackSwordHitRed", "Images/몬스터/bossSkeletonSword22X4HitRed.bmp", 3652, 621, 22, 4);
	_loading->loadFrameImage("skeletonAttackSwordHitWhite", "Images/몬스터/bossSkeletonSword22X4HitWhite.bmp", 3652, 621, 22, 4);
	_loading->loadFrameImage("skeletonAttackHammer", "Images/몬스터/bossSkeletonHammer20X4.bmp", 3320, 621, 20, 4);
	_loading->loadFrameImage("skeletonAttackHammerHitRed", "Images/몬스터/bossSkeletonHammer20X4HitRed.bmp", 3320, 621, 20, 4);
	_loading->loadFrameImage("skeletonAttackHammerHitWhite", "Images/몬스터/bossSkeletonHammer20X4HitWhite.bmp", 3320, 621, 20, 4);
	_loading->loadFrameImage("hammerWave1", "Images/몬스터/hammerWave(2)8.bmp", 1848, 251, 8, 1);
	_loading->loadFrameImage("skeletonBlade", "Images/몬스터/bossSkeletonBlade8.bmp", 1040, 297, 8, 1);

	_loading->loadImage("enemyProgressBarFront", "Images/던전/enemyProgressBarFront.bmp", 120, 10);
	_loading->loadImage("enemyProgressBarBack", "Images/던전/enemyProgressBarback.bmp", 124, 14);
	_loading->loadImage("semiBossHpBarFront(red)", "Images/몬스터/semiBossHpFront(red).bmp", 927, 17, true, RGB(255, 0, 255));
	_loading->loadImage("semiBossHpBarFront(white)", "Images/몬스터/semiBossHpFront(white).bmp", 927, 17, true, RGB(255, 0, 255));
	_loading->loadImage("semiBossHpBarBack", "Images/몬스터/semiBossHpBack.bmp", 947, 23, true, RGB(255, 0, 255));



	_loading->loadFrameImage("던전문(위아래)", "Images/던전/dungeonDoorUD6.bmp", 1050, 212, 6, 2);
	_loading->loadFrameImage("던전문(양옆)", "Images/던전/dungeonDoorLR6.bmp", 630, 350, 6, 2);
	_loading->loadFrameImage("던전문뚜껑(위아래)", "Images/던전/dungeonDoorTopUD.bmp", 175, 212, 1, 2);
	_loading->loadFrameImage("던전문뚜껑(양옆)", "Images/던전/dungeonDoorTopLR.bmp", 105, 350, 1, 2);
	_loading->loadFrameImage("보스문(위아래)", "Images/던전/bossDoorH17.bmp", 1632, 54,17,1);
	_loading->loadFrameImage("보스문(양옆)", "Images/던전/bossDoorW17.bmp", 918, 192,17,2);
	_loading->loadFrameImage("보스문뚜껑(양옆)", "Images/던전/bossDoorTopW17.bmp", 54, 192,1,2);
	_loading->loadFrameImage("보스문뚜껑(위아래)", "Images/던전/bossDoorTopH17.bmp", 96, 54,1,1);

	//플레이어 이팩트
	_loading->loadFrameImage("화살이펙트", "Images/플레이어/bowEffect.bmp", 240, 40, 3, 1);
	_loading->loadFrameImage("공격이펙트", "Images/플레이어/swordEffect 3X1.bmp", 240, 40, 3, 1);

}

void dungeonLoading::loadSound()
{
	_loading->loadSound("dungeonBGM", "Sound/던전/dungeonBGM.mp3", true, true);
	_loading->loadSound("spaRoomBGM", "Sound/던전/poolRoomBGM.mp3", true, true);
	_loading->loadSound("bossRoomBGM", "Sound/던전/bossRoomBGM.mp3", true, true);


	_loading->loadSound("DungeonDoor", "Sound/door.wav");
	_loading->loadSound("bossDoor", "Sound/bossDoor.wav");
	_loading->loadSound("golemAttack", "Sound/에너미/골렘/골렘공격.wav");
	_loading->loadSound("golemHit", "Sound/에너미/골렘/골렘맞음.wav");
	_loading->loadSound("potFire", "Sound/에너미/폿/총알쏘기.wav");
	_loading->loadSound("potReload", "Sound/에너미/폿/총알장전.wav");
	_loading->loadSound("potBulletPop", "Sound/에너미/폿/총알터짐.wav");
	_loading->loadSound("skullDie", "Sound/에너미/보스/golem_dungeon_king_golem_roar2-resources.assets-4536.wav");
	_loading->loadSound("skullAttackSword", "Sound/에너미/해골/skeletonAttackSword.wav");
	_loading->loadSound("skullAttackHammer", "Sound/에너미/해골/skeletonAttackHammer.wav");
	_loading->loadSound("skullHit", "Sound/에너미/해골/skeletonStep1.wav");
	_loading->loadSound("skullHit2", "Sound/에너미/해골/skeletonStep2.wav");
	_loading->loadSound("skullHit3", "Sound/에너미/해골/skeletonStep3.wav");
	_loading->loadSound("skeletonWave", "Sound/에너미/해골/wave.wav");
	_loading->loadSound("gasManHit", "Sound/에너미/슬라임/슬라임맞음.wav");
	_loading->loadSound("golemDie", "Sound/에너미/골렘/골렘사망.wav");
	_loading->loadSound("enemyDie", "Sound/에너미/슬라임/슬라임죽음.wav");
	_loading->loadSound("gasArrowPop", "Sound/에너미/가스맨/gasArrowPop.wav");

}
