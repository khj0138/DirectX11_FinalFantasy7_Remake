#include "pch.h"
#include "ScriptMgr.h"

#include "AtkRangeScript.h"
#include "AttackDataScript.h"
#include "BackWeaponScript.h"
#include "BarretLimitAtkEffectScript.h"
#include "BarretLimitAtkScript.h"
#include "BarretMaxFuryEffectScript.h"
#include "BarretMaximumFuryScript.h"
#include "BarretMotionScript.h"
#include "BarretMuzzleEffectScript.h"
#include "BarretNormalAtkEffectScript.h"
#include "BarretNormalAtkScript.h"
#include "BarretSmackDownEffectScript.h"
#include "BarretSmackDownScript.h"
#include "BarretUpperCutEffectScript.h"
#include "BarretUpperCutScript.h"
#include "BarretWeaponScript.h"
#include "BeamScript.h"
#include "BladeBurstScript.h"
#include "BladeScript.h"
#include "BreakEffectScript.h"
#include "ButtonEffectScript.h"
#include "ButtonUIScript.h"
#include "BWeaponHitEffectScript.h"
#include "BWeaponLimitEffectScript.h"
#include "BWeaponShotEffectScript.h"
#include "CameraMoveScript.h"
#include "ChangeLevelScript.h"
#include "CharacterDataScript.h"
#include "CharacterUIScript.h"
#include "CloudMoveScript.h"
#include "CloudWeaponScript.h"
#include "CSlashHitEffectScript.h"
#include "CWeaponFlashScript.h"
#include "CWeaponHitEffectScript.h"
#include "CWeaponSparkScript.h"
#include "DefaultUIScript.h"
#include "EffectScript.h"
#include "ElectricEffectScript.h"
#include "FillUIScript.h"
#include "FindEnemyScript.h"
#include "HealEffectScript.h"
#include "HGFootScript.h"
#include "HomingLaserScript.h"
#include "HundredBulletScript.h"
#include "HundredGunnerRightArmScript.h"
#include "HundredGunnerScript.h"
#include "ImageUIScript.h"
#include "LensEffectScript.h"
#include "MagicFireExplosionScript.h"
#include "MagicFireScript.h"
#include "MagicFireTrailScript.h"
#include "MenuUIScript.h"
#include "ObjectPoolMgr.h"
#include "PlayerCameraScript.h"
#include "PlayerDataScript.h"
#include "PrimaryFireScript.h"
#include "RockScript.h"
#include "SaturationFireScript.h"
#include "SelectUIScript.h"
#include "SoldierScript.h"
#include "SoldierWeaponScript.h"
#include "TargetUIScript.h"
#include "TextUIScript.h"
#include "WorldTextUIScript.h"
namespace ff7r
{
void ScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"AtkRangeScript");
	_vec.push_back(L"AttackDataScript");
	_vec.push_back(L"BackWeaponScript");
	_vec.push_back(L"BarretLimitAtkEffectScript");
	_vec.push_back(L"BarretLimitAtkScript");
	_vec.push_back(L"BarretMaxFuryEffectScript");
	_vec.push_back(L"BarretMaximumFuryScript");
	_vec.push_back(L"BarretMotionScript");
	_vec.push_back(L"BarretMuzzleEffectScript");
	_vec.push_back(L"BarretNormalAtkEffectScript");
	_vec.push_back(L"BarretNormalAtkScript");
	_vec.push_back(L"BarretSmackDownEffectScript");
	_vec.push_back(L"BarretSmackDownScript");
	_vec.push_back(L"BarretUpperCutEffectScript");
	_vec.push_back(L"BarretUpperCutScript");
	_vec.push_back(L"BarretWeaponScript");
	_vec.push_back(L"BeamScript");
	_vec.push_back(L"BladeBurstScript");
	_vec.push_back(L"BladeScript");
	_vec.push_back(L"BreakEffectScript");
	_vec.push_back(L"ButtonEffectScript");
	_vec.push_back(L"ButtonUIScript");
	_vec.push_back(L"BWeaponHitEffectScript");
	_vec.push_back(L"BWeaponLimitEffectScript");
	_vec.push_back(L"BWeaponShotEffectScript");
	_vec.push_back(L"CameraMoveScript");
	_vec.push_back(L"ChangeLevelScript");
	_vec.push_back(L"CharacterDataScript");
	_vec.push_back(L"CharacterUIScript");
	_vec.push_back(L"CloudMoveScript");
	_vec.push_back(L"CloudWeaponScript");
	_vec.push_back(L"CSlashHitEffectScript");
	_vec.push_back(L"CWeaponFlashScript");
	_vec.push_back(L"CWeaponHitEffectScript");
	_vec.push_back(L"CWeaponSparkScript");
	_vec.push_back(L"DefaultUIScript");
	_vec.push_back(L"EffectScript");
	_vec.push_back(L"ElectricEffectScript");
	_vec.push_back(L"FillUIScript");
	_vec.push_back(L"FindEnemyScript");
	_vec.push_back(L"HealEffectScript");
	_vec.push_back(L"HGFootScript");
	_vec.push_back(L"HomingLaserScript");
	_vec.push_back(L"HundredBulletScript");
	_vec.push_back(L"HundredGunnerRightArmScript");
	_vec.push_back(L"HundredGunnerScript");
	_vec.push_back(L"ImageUIScript");
	_vec.push_back(L"LensEffectScript");
	_vec.push_back(L"MagicFireExplosionScript");
	_vec.push_back(L"MagicFireScript");
	_vec.push_back(L"MagicFireTrailScript");
	_vec.push_back(L"MenuUIScript");
	_vec.push_back(L"ObjectPoolMgr");
	_vec.push_back(L"PlayerCameraScript");
	_vec.push_back(L"PlayerDataScript");
	_vec.push_back(L"PrimaryFireScript");
	_vec.push_back(L"RockScript");
	_vec.push_back(L"SaturationFireScript");
	_vec.push_back(L"SelectUIScript");
	_vec.push_back(L"SoldierScript");
	_vec.push_back(L"SoldierWeaponScript");
	_vec.push_back(L"TargetUIScript");
	_vec.push_back(L"TextUIScript");
	_vec.push_back(L"WorldTextUIScript");
}

Script * ScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"AtkRangeScript" == _strScriptName)
		return new AtkRangeScript;
	if (L"AttackDataScript" == _strScriptName)
		return new AttackDataScript;
	if (L"BackWeaponScript" == _strScriptName)
		return new BackWeaponScript;
	if (L"BarretLimitAtkEffectScript" == _strScriptName)
		return new BarretLimitAtkEffectScript;
	if (L"BarretLimitAtkScript" == _strScriptName)
		return new BarretLimitAtkScript;
	if (L"BarretMaxFuryEffectScript" == _strScriptName)
		return new BarretMaxFuryEffectScript;
	if (L"BarretMaximumFuryScript" == _strScriptName)
		return new BarretMaximumFuryScript;
	if (L"BarretMotionScript" == _strScriptName)
		return new BarretMotionScript;
	if (L"BarretMuzzleEffectScript" == _strScriptName)
		return new BarretMuzzleEffectScript;
	if (L"BarretNormalAtkEffectScript" == _strScriptName)
		return new BarretNormalAtkEffectScript;
	if (L"BarretNormalAtkScript" == _strScriptName)
		return new BarretNormalAtkScript;
	if (L"BarretSmackDownEffectScript" == _strScriptName)
		return new BarretSmackDownEffectScript;
	if (L"BarretSmackDownScript" == _strScriptName)
		return new BarretSmackDownScript;
	if (L"BarretUpperCutEffectScript" == _strScriptName)
		return new BarretUpperCutEffectScript;
	if (L"BarretUpperCutScript" == _strScriptName)
		return new BarretUpperCutScript;
	if (L"BarretWeaponScript" == _strScriptName)
		return new BarretWeaponScript;
	if (L"BeamScript" == _strScriptName)
		return new BeamScript;
	if (L"BladeBurstScript" == _strScriptName)
		return new BladeBurstScript;
	if (L"BladeScript" == _strScriptName)
		return new BladeScript;
	if (L"BreakEffectScript" == _strScriptName)
		return new BreakEffectScript;
	if (L"ButtonEffectScript" == _strScriptName)
		return new ButtonEffectScript;
	if (L"ButtonUIScript" == _strScriptName)
		return new ButtonUIScript;
	if (L"BWeaponHitEffectScript" == _strScriptName)
		return new BWeaponHitEffectScript;
	if (L"BWeaponLimitEffectScript" == _strScriptName)
		return new BWeaponLimitEffectScript;
	if (L"BWeaponShotEffectScript" == _strScriptName)
		return new BWeaponShotEffectScript;
	if (L"CameraMoveScript" == _strScriptName)
		return new CameraMoveScript;
	if (L"ChangeLevelScript" == _strScriptName)
		return new ChangeLevelScript;
	if (L"CharacterDataScript" == _strScriptName)
		return new CharacterDataScript;
	if (L"CharacterUIScript" == _strScriptName)
		return new CharacterUIScript;
	if (L"CloudMoveScript" == _strScriptName)
		return new CloudMoveScript;
	if (L"CloudWeaponScript" == _strScriptName)
		return new CloudWeaponScript;
	if (L"CSlashHitEffectScript" == _strScriptName)
		return new CSlashHitEffectScript;
	if (L"CWeaponFlashScript" == _strScriptName)
		return new CWeaponFlashScript;
	if (L"CWeaponHitEffectScript" == _strScriptName)
		return new CWeaponHitEffectScript;
	if (L"CWeaponSparkScript" == _strScriptName)
		return new CWeaponSparkScript;
	if (L"DefaultUIScript" == _strScriptName)
		return new DefaultUIScript;
	if (L"EffectScript" == _strScriptName)
		return new EffectScript;
	if (L"ElectricEffectScript" == _strScriptName)
		return new ElectricEffectScript;
	if (L"FillUIScript" == _strScriptName)
		return new FillUIScript;
	if (L"FindEnemyScript" == _strScriptName)
		return new FindEnemyScript;
	if (L"HealEffectScript" == _strScriptName)
		return new HealEffectScript;
	if (L"HGFootScript" == _strScriptName)
		return new HGFootScript;
	if (L"HomingLaserScript" == _strScriptName)
		return new HomingLaserScript;
	if (L"HundredBulletScript" == _strScriptName)
		return new HundredBulletScript;
	if (L"HundredGunnerRightArmScript" == _strScriptName)
		return new HundredGunnerRightArmScript;
	if (L"HundredGunnerScript" == _strScriptName)
		return new HundredGunnerScript;
	if (L"ImageUIScript" == _strScriptName)
		return new ImageUIScript;
	if (L"LensEffectScript" == _strScriptName)
		return new LensEffectScript;
	if (L"MagicFireExplosionScript" == _strScriptName)
		return new MagicFireExplosionScript;
	if (L"MagicFireScript" == _strScriptName)
		return new MagicFireScript;
	if (L"MagicFireTrailScript" == _strScriptName)
		return new MagicFireTrailScript;
	if (L"MenuUIScript" == _strScriptName)
		return new MenuUIScript;
	if (L"ObjectPoolMgr" == _strScriptName)
		return new ObjectPoolMgr;
	if (L"PlayerCameraScript" == _strScriptName)
		return new PlayerCameraScript;
	if (L"PlayerDataScript" == _strScriptName)
		return new PlayerDataScript;
	if (L"PrimaryFireScript" == _strScriptName)
		return new PrimaryFireScript;
	if (L"RockScript" == _strScriptName)
		return new RockScript;
	if (L"SaturationFireScript" == _strScriptName)
		return new SaturationFireScript;
	if (L"SelectUIScript" == _strScriptName)
		return new SelectUIScript;
	if (L"SoldierScript" == _strScriptName)
		return new SoldierScript;
	if (L"SoldierWeaponScript" == _strScriptName)
		return new SoldierWeaponScript;
	if (L"TargetUIScript" == _strScriptName)
		return new TargetUIScript;
	if (L"TextUIScript" == _strScriptName)
		return new TextUIScript;
	if (L"WorldTextUIScript" == _strScriptName)
		return new WorldTextUIScript;
	return nullptr;
}

Script * ScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ATKRANGESCRIPT:
		return new AtkRangeScript;
		break;
	case (UINT)SCRIPT_TYPE::ATTACKDATASCRIPT:
		return new AttackDataScript;
		break;
	case (UINT)SCRIPT_TYPE::BACKWEAPONSCRIPT:
		return new BackWeaponScript;
		break;
	case (UINT)SCRIPT_TYPE::BARRETLIMITATKEFFECTSCRIPT:
		return new BarretLimitAtkEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::BARRETLIMITATKSCRIPT:
		return new BarretLimitAtkScript;
		break;
	case (UINT)SCRIPT_TYPE::BARRETMAXFURYEFFECTSCRIPT:
		return new BarretMaxFuryEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::BARRETMAXIMUMFURYSCRIPT:
		return new BarretMaximumFuryScript;
		break;
	case (UINT)SCRIPT_TYPE::BARRETMOTIONSCRIPT:
		return new BarretMotionScript;
		break;
	case (UINT)SCRIPT_TYPE::BARRETMUZZLEEFFECTSCRIPT:
		return new BarretMuzzleEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::BARRETNORMALATKEFFECTSCRIPT:
		return new BarretNormalAtkEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::BARRETNORMALATKSCRIPT:
		return new BarretNormalAtkScript;
		break;
	case (UINT)SCRIPT_TYPE::BARRETSMACKDOWNEFFECTSCRIPT:
		return new BarretSmackDownEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::BARRETSMACKDOWNSCRIPT:
		return new BarretSmackDownScript;
		break;
	case (UINT)SCRIPT_TYPE::BARRETUPPERCUTEFFECTSCRIPT:
		return new BarretUpperCutEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::BARRETUPPERCUTSCRIPT:
		return new BarretUpperCutScript;
		break;
	case (UINT)SCRIPT_TYPE::BARRETWEAPONSCRIPT:
		return new BarretWeaponScript;
		break;
	case (UINT)SCRIPT_TYPE::BEAMSCRIPT:
		return new BeamScript;
		break;
	case (UINT)SCRIPT_TYPE::BLADEBURSTSCRIPT:
		return new BladeBurstScript;
		break;
	case (UINT)SCRIPT_TYPE::BLADESCRIPT:
		return new BladeScript;
		break;
	case (UINT)SCRIPT_TYPE::BREAKEFFECTSCRIPT:
		return new BreakEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::BUTTONEFFECTSCRIPT:
		return new ButtonEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::BUTTONUISCRIPT:
		return new ButtonUIScript;
		break;
	case (UINT)SCRIPT_TYPE::BWEAPONHITEFFECTSCRIPT:
		return new BWeaponHitEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::BWEAPONLIMITEFFECTSCRIPT:
		return new BWeaponLimitEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::BWEAPONSHOTEFFECTSCRIPT:
		return new BWeaponShotEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CHANGELEVELSCRIPT:
		return new ChangeLevelScript;
		break;
	case (UINT)SCRIPT_TYPE::CHARACTERDATASCRIPT:
		return new CharacterDataScript;
		break;
	case (UINT)SCRIPT_TYPE::CHARACTERUISCRIPT:
		return new CharacterUIScript;
		break;
	case (UINT)SCRIPT_TYPE::CLOUDMOVESCRIPT:
		return new CloudMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CLOUDWEAPONSCRIPT:
		return new CloudWeaponScript;
		break;
	case (UINT)SCRIPT_TYPE::CSLASHHITEFFECTSCRIPT:
		return new CSlashHitEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::CWEAPONFLASHSCRIPT:
		return new CWeaponFlashScript;
		break;
	case (UINT)SCRIPT_TYPE::CWEAPONHITEFFECTSCRIPT:
		return new CWeaponHitEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::CWEAPONSPARKSCRIPT:
		return new CWeaponSparkScript;
		break;
	case (UINT)SCRIPT_TYPE::DEFAULTUISCRIPT:
		return new DefaultUIScript;
		break;
	case (UINT)SCRIPT_TYPE::EFFECTSCRIPT:
		return new EffectScript;
		break;
	case (UINT)SCRIPT_TYPE::ELECTRICEFFECTSCRIPT:
		return new ElectricEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::FILLUISCRIPT:
		return new FillUIScript;
		break;
	case (UINT)SCRIPT_TYPE::FINDENEMYSCRIPT:
		return new FindEnemyScript;
		break;
	case (UINT)SCRIPT_TYPE::HEALEFFECTSCRIPT:
		return new HealEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::HGFOOTSCRIPT:
		return new HGFootScript;
		break;
	case (UINT)SCRIPT_TYPE::HOMINGLASERSCRIPT:
		return new HomingLaserScript;
		break;
	case (UINT)SCRIPT_TYPE::HUNDREDBULLETSCRIPT:
		return new HundredBulletScript;
		break;
	case (UINT)SCRIPT_TYPE::HUNDREDGUNNERRIGHTARMSCRIPT:
		return new HundredGunnerRightArmScript;
		break;
	case (UINT)SCRIPT_TYPE::HUNDREDGUNNERSCRIPT:
		return new HundredGunnerScript;
		break;
	case (UINT)SCRIPT_TYPE::IMAGEUISCRIPT:
		return new ImageUIScript;
		break;
	case (UINT)SCRIPT_TYPE::LENSEFFECTSCRIPT:
		return new LensEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::MAGICFIREEXPLOSIONSCRIPT:
		return new MagicFireExplosionScript;
		break;
	case (UINT)SCRIPT_TYPE::MAGICFIRESCRIPT:
		return new MagicFireScript;
		break;
	case (UINT)SCRIPT_TYPE::MAGICFIRETRAILSCRIPT:
		return new MagicFireTrailScript;
		break;
	case (UINT)SCRIPT_TYPE::MENUUISCRIPT:
		return new MenuUIScript;
		break;
	case (UINT)SCRIPT_TYPE::OBJECTPOOLMGR:
		return new ObjectPoolMgr;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERCAMERASCRIPT:
		return new PlayerCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERDATASCRIPT:
		return new PlayerDataScript;
		break;
	case (UINT)SCRIPT_TYPE::PRIMARYFIRESCRIPT:
		return new PrimaryFireScript;
		break;
	case (UINT)SCRIPT_TYPE::ROCKSCRIPT:
		return new RockScript;
		break;
	case (UINT)SCRIPT_TYPE::SATURATIONFIRESCRIPT:
		return new SaturationFireScript;
		break;
	case (UINT)SCRIPT_TYPE::SELECTUISCRIPT:
		return new SelectUIScript;
		break;
	case (UINT)SCRIPT_TYPE::SOLDIERSCRIPT:
		return new SoldierScript;
		break;
	case (UINT)SCRIPT_TYPE::SOLDIERWEAPONSCRIPT:
		return new SoldierWeaponScript;
		break;
	case (UINT)SCRIPT_TYPE::TARGETUISCRIPT:
		return new TargetUIScript;
		break;
	case (UINT)SCRIPT_TYPE::TEXTUISCRIPT:
		return new TextUIScript;
		break;
	case (UINT)SCRIPT_TYPE::WORLDTEXTUISCRIPT:
		return new WorldTextUIScript;
		break;
	}
	return nullptr;
}

const wchar_t * ScriptMgr::GetScriptName(Script * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ATKRANGESCRIPT:
		return L"AtkRangeScript";
		break;

	case SCRIPT_TYPE::ATTACKDATASCRIPT:
		return L"AttackDataScript";
		break;

	case SCRIPT_TYPE::BACKWEAPONSCRIPT:
		return L"BackWeaponScript";
		break;

	case SCRIPT_TYPE::BARRETLIMITATKEFFECTSCRIPT:
		return L"BarretLimitAtkEffectScript";
		break;

	case SCRIPT_TYPE::BARRETLIMITATKSCRIPT:
		return L"BarretLimitAtkScript";
		break;

	case SCRIPT_TYPE::BARRETMAXFURYEFFECTSCRIPT:
		return L"BarretMaxFuryEffectScript";
		break;

	case SCRIPT_TYPE::BARRETMAXIMUMFURYSCRIPT:
		return L"BarretMaximumFuryScript";
		break;

	case SCRIPT_TYPE::BARRETMOTIONSCRIPT:
		return L"BarretMotionScript";
		break;

	case SCRIPT_TYPE::BARRETMUZZLEEFFECTSCRIPT:
		return L"BarretMuzzleEffectScript";
		break;

	case SCRIPT_TYPE::BARRETNORMALATKEFFECTSCRIPT:
		return L"BarretNormalAtkEffectScript";
		break;

	case SCRIPT_TYPE::BARRETNORMALATKSCRIPT:
		return L"BarretNormalAtkScript";
		break;

	case SCRIPT_TYPE::BARRETSMACKDOWNEFFECTSCRIPT:
		return L"BarretSmackDownEffectScript";
		break;

	case SCRIPT_TYPE::BARRETSMACKDOWNSCRIPT:
		return L"BarretSmackDownScript";
		break;

	case SCRIPT_TYPE::BARRETUPPERCUTEFFECTSCRIPT:
		return L"BarretUpperCutEffectScript";
		break;

	case SCRIPT_TYPE::BARRETUPPERCUTSCRIPT:
		return L"BarretUpperCutScript";
		break;

	case SCRIPT_TYPE::BARRETWEAPONSCRIPT:
		return L"BarretWeaponScript";
		break;

	case SCRIPT_TYPE::BEAMSCRIPT:
		return L"BeamScript";
		break;

	case SCRIPT_TYPE::BLADEBURSTSCRIPT:
		return L"BladeBurstScript";
		break;

	case SCRIPT_TYPE::BLADESCRIPT:
		return L"BladeScript";
		break;

	case SCRIPT_TYPE::BREAKEFFECTSCRIPT:
		return L"BreakEffectScript";
		break;

	case SCRIPT_TYPE::BUTTONEFFECTSCRIPT:
		return L"ButtonEffectScript";
		break;

	case SCRIPT_TYPE::BUTTONUISCRIPT:
		return L"ButtonUIScript";
		break;

	case SCRIPT_TYPE::BWEAPONHITEFFECTSCRIPT:
		return L"BWeaponHitEffectScript";
		break;

	case SCRIPT_TYPE::BWEAPONLIMITEFFECTSCRIPT:
		return L"BWeaponLimitEffectScript";
		break;

	case SCRIPT_TYPE::BWEAPONSHOTEFFECTSCRIPT:
		return L"BWeaponShotEffectScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CameraMoveScript";
		break;

	case SCRIPT_TYPE::CHANGELEVELSCRIPT:
		return L"ChangeLevelScript";
		break;

	case SCRIPT_TYPE::CHARACTERDATASCRIPT:
		return L"CharacterDataScript";
		break;

	case SCRIPT_TYPE::CHARACTERUISCRIPT:
		return L"CharacterUIScript";
		break;

	case SCRIPT_TYPE::CLOUDMOVESCRIPT:
		return L"CloudMoveScript";
		break;

	case SCRIPT_TYPE::CLOUDWEAPONSCRIPT:
		return L"CloudWeaponScript";
		break;

	case SCRIPT_TYPE::CSLASHHITEFFECTSCRIPT:
		return L"CSlashHitEffectScript";
		break;

	case SCRIPT_TYPE::CWEAPONFLASHSCRIPT:
		return L"CWeaponFlashScript";
		break;

	case SCRIPT_TYPE::CWEAPONHITEFFECTSCRIPT:
		return L"CWeaponHitEffectScript";
		break;

	case SCRIPT_TYPE::CWEAPONSPARKSCRIPT:
		return L"CWeaponSparkScript";
		break;

	case SCRIPT_TYPE::DEFAULTUISCRIPT:
		return L"DefaultUIScript";
		break;

	case SCRIPT_TYPE::EFFECTSCRIPT:
		return L"EffectScript";
		break;

	case SCRIPT_TYPE::ELECTRICEFFECTSCRIPT:
		return L"ElectricEffectScript";
		break;

	case SCRIPT_TYPE::FILLUISCRIPT:
		return L"FillUIScript";
		break;

	case SCRIPT_TYPE::FINDENEMYSCRIPT:
		return L"FindEnemyScript";
		break;

	case SCRIPT_TYPE::HEALEFFECTSCRIPT:
		return L"HealEffectScript";
		break;

	case SCRIPT_TYPE::HGFOOTSCRIPT:
		return L"HGFootScript";
		break;

	case SCRIPT_TYPE::HOMINGLASERSCRIPT:
		return L"HomingLaserScript";
		break;

	case SCRIPT_TYPE::HUNDREDBULLETSCRIPT:
		return L"HundredBulletScript";
		break;

	case SCRIPT_TYPE::HUNDREDGUNNERRIGHTARMSCRIPT:
		return L"HundredGunnerRightArmScript";
		break;

	case SCRIPT_TYPE::HUNDREDGUNNERSCRIPT:
		return L"HundredGunnerScript";
		break;

	case SCRIPT_TYPE::IMAGEUISCRIPT:
		return L"ImageUIScript";
		break;

	case SCRIPT_TYPE::LENSEFFECTSCRIPT:
		return L"LensEffectScript";
		break;

	case SCRIPT_TYPE::MAGICFIREEXPLOSIONSCRIPT:
		return L"MagicFireExplosionScript";
		break;

	case SCRIPT_TYPE::MAGICFIRESCRIPT:
		return L"MagicFireScript";
		break;

	case SCRIPT_TYPE::MAGICFIRETRAILSCRIPT:
		return L"MagicFireTrailScript";
		break;

	case SCRIPT_TYPE::MENUUISCRIPT:
		return L"MenuUIScript";
		break;

	case SCRIPT_TYPE::OBJECTPOOLMGR:
		return L"ObjectPoolMgr";
		break;

	case SCRIPT_TYPE::PLAYERCAMERASCRIPT:
		return L"PlayerCameraScript";
		break;

	case SCRIPT_TYPE::PLAYERDATASCRIPT:
		return L"PlayerDataScript";
		break;

	case SCRIPT_TYPE::PRIMARYFIRESCRIPT:
		return L"PrimaryFireScript";
		break;

	case SCRIPT_TYPE::ROCKSCRIPT:
		return L"RockScript";
		break;

	case SCRIPT_TYPE::SATURATIONFIRESCRIPT:
		return L"SaturationFireScript";
		break;

	case SCRIPT_TYPE::SELECTUISCRIPT:
		return L"SelectUIScript";
		break;

	case SCRIPT_TYPE::SOLDIERSCRIPT:
		return L"SoldierScript";
		break;

	case SCRIPT_TYPE::SOLDIERWEAPONSCRIPT:
		return L"SoldierWeaponScript";
		break;

	case SCRIPT_TYPE::TARGETUISCRIPT:
		return L"TargetUIScript";
		break;

	case SCRIPT_TYPE::TEXTUISCRIPT:
		return L"TextUIScript";
		break;

	case SCRIPT_TYPE::WORLDTEXTUISCRIPT:
		return L"WorldTextUIScript";
		break;

	}
	return nullptr;
}}