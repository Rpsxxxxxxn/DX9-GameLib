//*************************************************************
//
// GameManager.h
//
// ゲーム作成に必要なデータを管理します
//
// Date: 2019/07/30 更新
//
//*************************************************************

#pragma once

//=============================================================
// インクルード
//=============================================================
#include "../../Library/Collision/CollisionCheck.h"
#include "../../Library/Collision/Sphere.h"
#include "../../Library/Collision/Obb.h"

#include "../../Library/Graphics/Camera/Camera.h"
#include "../../Library/Graphics/Light/DirectionLight.h"
#include "../../Library/Graphics/Line/Line.h"
#include "../../Library/Graphics/Billboard/Billboard.h"
#include "../../Library/Graphics/Model/XFileModel.h"

#include "../../Library/Utility/Math/Math.h"
#include "../../Library/Utility/Math/Matrix.h"
#include "../../Library/Utility/Math/Vector.h"
#include "../../Library/Utility/Math/Calculate.h"
#include "../../Library/Utility/Sprite/SpriteAnimation.h"
#include "../../Library/Utility/Sprite/SpriteValue.h"
#include "../../Library/Utility/Singleton.h"
#include "../../Library/Utility/IntervalTimer.h"
#include "../../Library/Utility/DebugText.h"

#include "GameObject.h"

//=============================================================
// 名前空間
//=============================================================
namespace FileConst
{
	enum
	{
		MODEL_PLAYER,
		MODEL_SWORD,
		MODEL_SHIELD,
		MODEL_ENEMY,
		MODEL_SKYDOME,
		MODEL_DEMOBOSS,
		MODEL_EASYBOSS,
		MODEL_NORMALBOSS,
		MODEL_HARDBOSS,
		MODEL_FIELD1,
		MODEL_FIELD2,
		MODEL_FIELD3,
		MODEL_FIELD4,
		MODEL_FIELD5,
		MODEL_FIELD6,
		MODEL_FIELD7,
		TEXTURE_FADE,
		TEXTURE_NUMBER,
		TEXTURE_TIME_DOT,
		TEXTURE_STATUS_BACK,
		TEXTURE_STATUS_FRONT,
		TEXTURE_HP,
		TEXTURE_STAMINA,
		TEXTURE_LOCKON,
		TEXTURE_SHOW_DAMAGE,
		TEXTURE_ITEM,
		TEXTURE_MINIMAP,
		TEXTURE_MINIMAP_ENEMY,
		TEXTURE_MINIMAP_BOSS,
		TEXTURE_RECOVERY_BD,
		TEXTURE_RECOVERY_BAR,
		TEXTURE_ENEMY_HP_BACK,
		TEXTURE_ENEMY_HP_BAR,
		TEXTURE_ENEMY_HP_FRONT,
		TEXTURE_BOSS_HP_BAR,
		TEXTURE_BOSS_HP_FRONT,
		TEXTURE_START,
		TEXTURE_EXIT,
		TEXTURE_CURSOR,
		TEXTURE_SWORD_LOGO,
		TEXTURE_MASTER_LOGO,
		TEXTURE_SWORD,
		TEXTURE_PAUSE_LOGO,
		TEXTURE_PAUSE_BACK,
		TEXTURE_PAUSE_TITLE,
		TEXTURE_MISSION_BACK,
		TEXTURE_MISSION_ENEMY,
		TEXTURE_MISSION_BOSS,
		TEXTURE_STAGE_BACK,
		TEXTURE_STAGE_FIRST,
		TEXTURE_STAGE_SECOND,
		TEXTURE_STAGE_FINAL,
		TEXTURE_STAGE_CLEAR,
		TEXTURE_GAME_CLEAR,
		TEXTURE_GAME_OVER,
		TEXTURE_ATTACK_KEY,
		TEXTURE_GUIDE_KEY,
		TEXTURE_RESULT_KEY,
		TEXTURE_TITLE_KEY,
		TEXTURE_RESULT_BACK,
		TEXTURE_RESULT_LOGO,
		TEXTURE_HALF_LINE,
		TEXTURE_RESULT_TIME,
		TEXTURE_RESULT_RANK,
		TEXTURE_RESULT_NUM,
		BGM_GAME_FIELD1,
		BGM_GAME_FIELD2,
		BGM_GAME_FIELD3,
		BGM_RESULT,
		BGM_TITLE,
		SE_DAMAGE1,
		SE_DAMAGE2,
		SE_GUARD,
		SE_SWORD_SLASH,
		SE_CLEAR,
		SE_SELECT,
		SE_ENTER,
	};
}