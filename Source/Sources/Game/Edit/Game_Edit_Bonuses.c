
//<<>-<>>---------------------------------------------------------------------()
/*
	Edition | Bonus permanents
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "GameEdit.h"
#include "Dialogs.h"
#include "Requests.h"
#include "Utils.h"
#include "Texts.h"

extern APPLICATION	App;

static GAMEEDITPAGE	BonusesPages[] = {
					{ GAME_PAGE_BONUS_TARGET, TEXT_BONUS_TITLE_TARGET, TEXT_BONUS_INFO_TARGET, 7000 },
					{ GAME_PAGE_BONUS_LIST, TEXT_BONUS_TITLE_LIST, TEXT_BONUS_INFO_LIST, 7001 },
					{ GAME_PAGE_BONUS_VALUE, TEXT_BONUS_TITLE_VALUE, TEXT_BONUS_INFO_VALUE, 7002 },
					{ GAME_PAGE_BONUS_DAMAGETYPE, TEXT_BONUS_TITLE_DAMAGETYPE, TEXT_BONUS_INFO_DAMAGETYPE, 7003 },
					{ GAME_PAGE_BONUS_REFLECTION, TEXT_BONUS_TITLE_REFLECT, TEXT_BONUS_INFO_REFLECT, 7004 },
					{ GAME_PAGE_BONUS_SKILLS, TEXT_BONUS_TITLE_SKILLS, TEXT_BONUS_INFO_SKILLS, 7005 },
					{ GAME_PAGE_BONUS_COLORS, TEXT_BONUS_TITLE_COLORS, TEXT_BONUS_INFO_COLORS, 7006 },
					{ 0 }
				};

static GAMEBONUS	Bonuses[] = {
					{ (WCHAR *)-1,		TEXT_BONUS_PRIMARIES,		BONUS_FLAG_TITLE|BONUS_FLAG_TARGET_ALL },
					{ L"Strength",		TEXT_CHR_FOR,			BONUS_FLAG_INT|BONUS_FLAG_TARGET_ALL },
					{ L"Finesse",		TEXT_CHR_DEX,			BONUS_FLAG_INT|BONUS_FLAG_TARGET_ALL },
					{ L"Intelligence",	TEXT_CHR_INT,			BONUS_FLAG_INT|BONUS_FLAG_TARGET_ALL },
					{ L"Constitution",	TEXT_CHR_CON,			BONUS_FLAG_INT|BONUS_FLAG_TARGET_ALL },
					{ L"Memory",		TEXT_CHR_MEM,			BONUS_FLAG_INT|BONUS_FLAG_TARGET_ALL },
					{ L"Wits",		TEXT_CHR_PER,			BONUS_FLAG_INT|BONUS_FLAG_TARGET_ALL },
					{ (WCHAR *)-1,		TEXT_BONUS_OTHERS,		BONUS_FLAG_TITLE|BONUS_FLAG_TARGET_ALL },
					{ L"ItemColor",		TEXT_BONUS_COLORS,		BONUS_FLAG_COLORS|BONUS_FLAG_TARGET_ALL },
					{ L"DamageBoost",	TEXT_BONUS_DAMAGEBOOST,		BONUS_FLAG_INT|BONUS_FLAG_PERCENT|BONUS_FLAG_TARGET_WEAPON },
					{ L"MagicArmorValue",	TEXT_BONUS_ARMOR_MAGICAL,	BONUS_FLAG_INT|BONUS_FLAG_TARGET_ARMOR|BONUS_FLAG_TARGET_SHIELD },
					{ L"ArmorValue",	TEXT_BONUS_ARMOR_PHYSICAL,	BONUS_FLAG_INT|BONUS_FLAG_TARGET_ARMOR|BONUS_FLAG_TARGET_SHIELD },
					{ L"CriticalChance",	TEXT_BONUS_CRITICALCHANCE,	BONUS_FLAG_INT|BONUS_FLAG_PERCENT|BONUS_FLAG_TARGET_ALL },
					{ L"Accuracy",		TEXT_BONUS_ACCURACY,		BONUS_FLAG_INT|BONUS_FLAG_PERCENT|BONUS_FLAG_TARGET_ALL },
					{ L"Dodge",		TEXT_BONUS_DODGE,		BONUS_FLAG_INT|BONUS_FLAG_PERCENT|BONUS_FLAG_TARGET_ALL },
					{ L"Vitality",		TEXT_BONUS_VITALITY,		BONUS_FLAG_INT|BONUS_FLAG_TARGET_ALL },
					{ L"DamageType",	TEXT_BONUS_DAMAGETYPE,		BONUS_FLAG_DAMAGETYPE|BONUS_FLAG_TARGET_WEAPON },
					{ L"MinDamage",		TEXT_BONUS_MINDAMAGE,		BONUS_FLAG_INT|BONUS_FLAG_TARGET_WEAPON },
					{ L"MaxDamage",		TEXT_BONUS_MAXDAMAGE,		BONUS_FLAG_INT|BONUS_FLAG_TARGET_WEAPON },
					{ L"CleaveAngle",	TEXT_BONUS_CLEAVEANGLE,		BONUS_FLAG_INT|BONUS_FLAG_DEGREES|BONUS_FLAG_TARGET_WEAPON },
					{ L"CleavePercentage",	TEXT_BONUS_CLEAVEPERCENTAGE,	BONUS_FLAG_FLOAT|BONUS_FLAG_PERCENT|BONUS_FLAG_SPECIAL1|BONUS_FLAG_TARGET_WEAPON },
					{ L"WeaponRange",	TEXT_BONUS_WEAPONRANGE,		BONUS_FLAG_FLOAT|BONUS_FLAG_METERS|BONUS_FLAG_TARGET_WEAPON },
					{ L"LifeSteal",		TEXT_BONUS_LIFESTEAL,		BONUS_FLAG_INT|BONUS_FLAG_PERCENT|BONUS_FLAG_TARGET_WEAPON },
					{ L"Blocking",		TEXT_BONUS_BLOCKING,		BONUS_FLAG_INT|BONUS_FLAG_PERCENT|BONUS_FLAG_TARGET_SHIELD },
					{ L"Movement",		TEXT_BONUS_MOVEMENT,		BONUS_FLAG_INT|BONUS_FLAG_SPECIAL2|BONUS_FLAG_TARGET_ALL },
					{ L"Initiative",	TEXT_BONUS_INITIATIVE,		BONUS_FLAG_INT|BONUS_FLAG_TARGET_ALL },
					{ L"HasReflection",	TEXT_BONUS_HASREFLECTION,	BONUS_FLAG_REFLECTION|BONUS_FLAG_TARGET_ALL },
					{ L"Skills",		TEXT_BONUS_SKILL,		BONUS_FLAG_SKILL|BONUS_FLAG_TARGET_ALL },
					{ (WCHAR *)-1,		TEXT_BONUS_ABILITIES,		BONUS_FLAG_TITLE|BONUS_FLAG_TARGET_ALL },
					{ L"DualWielding",	TEXT_ABILITIES_DUALWIELD,	BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"TwoHanded",		TEXT_ABILITIES_TWOHANDED,	BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"Ranged",		TEXT_ABILITIES_RANGED,		BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"SingleHanded",	TEXT_ABILITIES_ONEHANDED,	BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"PainReflection",	TEXT_ABILITIES_PAINREFLECTION,	BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"Leadership",	TEXT_ABILITIES_LEADERSHIP,	BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"Perseverance",	TEXT_ABILITIES_PERSEVERANCE,	BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"WarriorLore",	TEXT_ABILITIES_WARFARE,		BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"AirSpecialist",	TEXT_ABILITIES_AIR,		BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"RangerLore",	TEXT_ABILITIES_HUNTING,		BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"RogueLore",		TEXT_ABILITIES_ROGUES,		BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"EarthSpecialist",	TEXT_ABILITIES_EARTH,		BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"WaterSpecialist",	TEXT_ABILITIES_WATER,		BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"Summoning",		TEXT_ABILITIES_SUMMONING,	BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"Polymorph",		TEXT_ABILITIES_POLYMORPH,	BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"Necromancy",	TEXT_ABILITIES_DEATH,		BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"FireSpecialist",	TEXT_ABILITIES_FIRE,		BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"Luck",		TEXT_ABILITIES_LUCK,		BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"Barter",		TEXT_ABILITIES_BARTERING,	BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"Persuasion",	TEXT_ABILITIES_PERSUASION,	BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"Loremaster",	TEXT_ABILITIES_LOREMASTER,	BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"Telekinesis",	TEXT_ABILITIES_TELEKINESIS,	BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"Thievery",		TEXT_ABILITIES_LOCKPICKING,	BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ L"Sneaking",		TEXT_ABILITIES_SNEAKING,	BONUS_FLAG_INT|BONUS_FLAG_ATTRIBUTE|BONUS_FLAG_TARGET_ALL },
					{ (WCHAR *)-1,		TEXT_BONUS_UTILITY,		BONUS_FLAG_TITLE|BONUS_FLAG_TARGET_ALL },
					{ L"RuneSlots",		TEXT_BONUS_RUNESLOTS,		BONUS_FLAG_INT|BONUS_FLAG_TARGET_ALL },
					{ L"Value",		TEXT_BONUS_VALUE,		BONUS_FLAG_INT|BONUS_FLAG_TARGET_ALL },
					{ (WCHAR *)-1,		TEXT_BONUS_RESISTANCES,		BONUS_FLAG_TITLE|BONUS_FLAG_TARGET_ALL },
					{ L"PiercingResistance",TEXT_BONUS_PIERCINGRESISTANCE,	BONUS_FLAG_INT|BONUS_FLAG_PERCENT|BONUS_FLAG_TARGET_ALL },
					{ L"PhysicalResistance",TEXT_BONUS_PHYSICALRESISTANCE,	BONUS_FLAG_INT|BONUS_FLAG_PERCENT|BONUS_FLAG_TARGET_ALL },
					{ L"FireResistance",	TEXT_BONUS_FIRERESISTANCE,	BONUS_FLAG_INT|BONUS_FLAG_PERCENT|BONUS_FLAG_TARGET_ALL },
					{ L"WaterResistance",	TEXT_BONUS_WATERRESISTANCE,	BONUS_FLAG_INT|BONUS_FLAG_PERCENT|BONUS_FLAG_TARGET_ALL },
					{ L"EarthResistance",	TEXT_BONUS_EARTHRESISTANCE,	BONUS_FLAG_INT|BONUS_FLAG_PERCENT|BONUS_FLAG_TARGET_ALL },
					{ L"AirResistance",	TEXT_BONUS_AIRRESISTANCE,	BONUS_FLAG_INT|BONUS_FLAG_PERCENT|BONUS_FLAG_TARGET_ALL },
					{ L"PoisonResistance",	TEXT_BONUS_POISONRESISTANCE,	BONUS_FLAG_INT|BONUS_FLAG_PERCENT|BONUS_FLAG_TARGET_ALL },
					{ 0 }
				};

static GAMESKILL	Skills[] = {
					{ L"Cone_CorrosiveSpray", NULL, 1, 2, 540, TEXT_ABILITIES_EARTH },
					{ L"Cone_Flamebreath", NULL, 0, 1, 313, TEXT_ABILITIES_SPECIAL },
					{ L"Cone_GroundSmash", NULL, 0, 2, 364, TEXT_ABILITIES_WARFARE },
					{ L"Cone_Shatter", NULL, 0, 4, 390, TEXT_ABILITIES_WATER },
					{ L"Cone_SilencingStare", NULL, 0, 2, 392, TEXT_ABILITIES_DEATH },
					{ L"Cone_SteamLance", NULL, 2, 2, 330, TEXT_ABILITIES_WATER },
					{ L"Jump_CloakAndDagger", NULL, 0, 1, 112, TEXT_ABILITIES_ROGUES },
					{ L"Jump_PhoenixDive", NULL, 0, 1, 325, TEXT_ABILITIES_WARFARE },
					{ L"Jump_TacticalRetreat", NULL, 0, 1, 106, TEXT_ABILITIES_HUNTING },
					{ L"MultiStrike_BlinkStrike", NULL, 0, 2, 370, TEXT_ABILITIES_WARFARE },
					{ L"MultiStrike_Vault", NULL, 0, 1, 343, TEXT_ABILITIES_ROGUES },
					{ L"ProjectileStrike_DazingBolt", NULL, 0, 3, 448, TEXT_ABILITIES_AIR },
					{ L"ProjectileStrike_HailAttack", NULL, 3, 4, 172, TEXT_ABILITIES_WATER },
					{ L"ProjectileStrike_HailStrike", NULL, 0, 3, 353, TEXT_ABILITIES_WATER },
					{ L"ProjectileStrike_MeteorShower", NULL, 3, 4, 578, TEXT_ABILITIES_FIRE },
					{ L"ProjectileStrike_RainOfArrows", NULL, 3, 3, 581, TEXT_ABILITIES_HUNTING },
					{ L"Projectile_AcidSpores", NULL, 2, 3, 54, TEXT_ABILITIES_EARTH },
					{ L"Projectile_ArrowSpray", NULL, 1, 3, 315, TEXT_ABILITIES_HUNTING },
					{ L"Projectile_BallisticShot", NULL, 0, 2, 483, TEXT_ABILITIES_HUNTING },
					{ L"Projectile_BouncingShield", NULL, 0, 2, 472, TEXT_ABILITIES_WARFARE },
					{ L"Projectile_ChainHeal", NULL, 0, 2, 369, TEXT_ABILITIES_WATER },
					{ L"Projectile_ChainLightning", NULL, 1, 3, 12, TEXT_ABILITIES_AIR },
					{ L"Projectile_Chloroform", NULL, 0, 1, 318, TEXT_ABILITIES_ROGUES },
					{ L"Projectile_DeployMassTraps", NULL, 1, 3, 509, TEXT_ABILITIES_FIRE },
					{ L"Projectile_DimensionalBolt", NULL, 0, 2, 405, TEXT_ABILITIES_SUMMONING },
					{ L"Projectile_DustBlast", NULL, 1, 3, 555, TEXT_ABILITIES_EARTH },
					{ L"Projectile_FanOfKnives", NULL, 1, 3, 488, TEXT_ABILITIES_ROGUES },
					{ L"Projectile_Fireball", NULL, 0, 2, 355, TEXT_ABILITIES_FIRE },
					{ L"Projectile_FlamingDaggers", NULL, 0, 2, 362, TEXT_ABILITIES_FIRE },
					{ L"Projectile_IceFan", NULL, 0, 3, 452, TEXT_ABILITIES_WATER },
					{ L"Projectile_InfectiousFlame", NULL, 2, 3, 579, TEXT_ABILITIES_FIRE },
					{ L"Projectile_LaunchBomber", NULL, 0, 2, 458, TEXT_ABILITIES_ROGUES },
					{ L"Projectile_LaunchExplosiveTrap", NULL, 0, 1, 510, TEXT_ABILITIES_FIRE },
					{ L"Projectile_LaunchOilBlob", NULL, 0, 2, 484, TEXT_ABILITIES_POLYMORPH },
					{ L"Projectile_LightningBolt", NULL, 0, 2, 357, TEXT_ABILITIES_AIR },
					{ L"Projectile_Mark", NULL, 0, 1, 350, TEXT_ABILITIES_HUNTING },
					{ L"Projectile_Multishot", NULL, 0, 3, 95, TEXT_ABILITIES_HUNTING },
					{ L"Projectile_PiercingShot", NULL, 0, 2, 317, TEXT_ABILITIES_HUNTING },
					{ L"Projectile_PinDown", NULL, 0, 3, 379, TEXT_ABILITIES_HUNTING },
					{ L"Projectile_PoisonDartStart", NULL, 0, 2, 64, TEXT_ABILITIES_EARTH },
					{ L"Projectile_PyroclasticEruption", NULL, 3, 3, 455, TEXT_ABILITIES_EARTH },
					{ L"Projectile_PyroclasticRock", NULL, 0, 2, 312, TEXT_ABILITIES_EARTH },
					{ L"Projectile_Ricochet", NULL, 0, 2, 100, TEXT_ABILITIES_HUNTING },
					{ L"Projectile_SkyShot", NULL, 0, 2, 482, TEXT_ABILITIES_HUNTING },
					{ L"Projectile_Snipe", NULL, 0, 3, 351, TEXT_ABILITIES_HUNTING },
					{ L"Projectile_Superconductor", NULL, 0, 3, 447, TEXT_ABILITIES_AIR },
					{ L"Projectile_ThrowDust", NULL, 0, 2, 542, TEXT_ABILITIES_EARTH },
					{ L"Projectile_ThrowingKnife", NULL, 0, 2, 342, TEXT_ABILITIES_ROGUES },
					{ L"Quake_Earthquake", NULL, 0, 3, 55, TEXT_ABILITIES_EARTH },
					{ L"Rain_Blood", NULL, 0, 2, 346, TEXT_ABILITIES_DEATH },
					{ L"Rain_Water", NULL, 0, 1, 180, TEXT_ABILITIES_WATER },
					{ L"Rush_BatteringRam", NULL, 0, 2, 324, TEXT_ABILITIES_WARFARE },
					{ L"Shout_Adrenaline", NULL, 0, 0, 110, TEXT_ABILITIES_ROGUES },
					{ L"Shout_Apotheosis", NULL, 3, 2, 544, TEXT_ABILITIES_POLYMORPH },
					{ L"Shout_BlindingRadiance", NULL, 0, 2, 335, TEXT_ABILITIES_AIR },
					{ L"Shout_BoneCage", NULL, 0, 1, 456, TEXT_ABILITIES_DEATH },
					{ L"Shout_BreakTheShackles", NULL, 1, 0, 429, TEXT_ABILITIES_SPECIAL },
					{ L"Shout_BreathingBubble", NULL, 0, 1, 526, TEXT_ABILITIES_AIR },
					{ L"Shout_BullHorns", NULL, 0, 0, 402, TEXT_ABILITIES_POLYMORPH },
					{ L"Shout_ChameleonSkin", NULL, 0, 1, 314, TEXT_ABILITIES_POLYMORPH },
					{ L"Shout_CloseTheDoor", NULL, 1, 2, 486, TEXT_ABILITIES_SUMMONING },
					{ L"Shout_Contamination", NULL, 0, 1, 336, TEXT_ABILITIES_EARTH },
					{ L"Shout_Cryotherapy", NULL, 0, 1, 531, TEXT_ABILITIES_WATER },
					{ L"Shout_DeflectiveBarrier", NULL, 0, 2, 473, TEXT_ABILITIES_WARFARE },
					{ L"Shout_ElectricFence", NULL, 2, 2, 334, TEXT_ABILITIES_AIR },
					{ L"Shout_EvasiveAura", NULL, 1, 2, 545, TEXT_ABILITIES_AIR },
					{ L"Shout_FavourableWind", NULL, 0, 1, 333, TEXT_ABILITIES_AIR },
					{ L"Shout_FireBrand", NULL, 0, 1, 438, TEXT_ABILITIES_FIRE },
					{ L"Shout_FlamingSkin", NULL, 1, 1, 496, TEXT_ABILITIES_FIRE },
					{ L"Shout_FlamingTongues", NULL, 0, 1, 439, TEXT_ABILITIES_FIRE },
					{ L"Shout_FleshSacrifice", NULL, 0, 0, 338, TEXT_ABILITIES_SPECIAL },
					{ L"Shout_FrostAura", NULL, 0, 1, 449, TEXT_ABILITIES_WATER },
					{ L"Shout_GlobalCooling", NULL, 0, 1, 365, TEXT_ABILITIES_WATER },
					{ L"Shout_GuardianAngel", NULL, 0, 2, 394, TEXT_ABILITIES_WARFARE },
					{ L"Shout_HealingTears", NULL, 0, 1, 535, TEXT_ABILITIES_WATER },
					{ L"Shout_IceBreaker", NULL, 0, 1, 538, TEXT_ABILITIES_WATER },
					{ L"Shout_IceSkin", NULL, 1, 1, 534, TEXT_ABILITIES_POLYMORPH },
					{ L"Shout_Ignition", NULL, 0, 1, 319, TEXT_ABILITIES_FIRE },
					{ L"Shout_InnerDemon", NULL, 1, 1, 560, TEXT_ABILITIES_SUMMONING },
					{ L"Shout_InspireStart", NULL, 0, 1, 158, TEXT_ABILITIES_SPECIAL },
					{ L"Shout_JellyfishSkin", NULL, 1, 1, 525, TEXT_ABILITIES_POLYMORPH },
					{ L"Shout_MassBreathingBubbles", NULL, 1, 1, 527, TEXT_ABILITIES_AIR },
					{ L"Shout_MassCleanseWounds", NULL, 1, 2, 536, TEXT_ABILITIES_WATER },
					{ L"Shout_MassCorpseExplosion", NULL, 1, 1, 499, TEXT_ABILITIES_FIRE },
					{ L"Shout_MassCryotherapy", NULL, 1, 2, 530, TEXT_ABILITIES_WATER },
					{ L"Shout_MassOilyCarapace", NULL, 1, 2, 559, TEXT_ABILITIES_EARTH },
					{ L"Shout_MedusaHead", NULL, 0, 2, 502, TEXT_ABILITIES_POLYMORPH },
					{ L"Shout_MendMetal", NULL, 0, 1, 459, TEXT_ABILITIES_EARTH },
					{ L"Shout_NecromancerTotems", NULL, 3, 2, 453, TEXT_ABILITIES_DEATH },
					{ L"Shout_OilyCarapace", NULL, 0, 1, 558, TEXT_ABILITIES_EARTH },
					{ L"Shout_PlayDead", NULL, 0, 0, 501, TEXT_ABILITIES_SPECIAL },
					{ L"Shout_PoisonWave", NULL, 0, 2, 464, TEXT_ABILITIES_EARTH },
					{ L"Shout_PoisonousSkin", NULL, 1, 1, 553, TEXT_ABILITIES_POLYMORPH },
					{ L"Shout_ReactiveArmor", NULL, 0, 2, 478, TEXT_ABILITIES_EARTH },
					{ L"Shout_RecoverArmour", NULL, 0, 2, 375, TEXT_ABILITIES_SPECIAL },
					{ L"Shout_ShedSkin", NULL, 1, 1, 398, TEXT_ABILITIES_POLYMORPH },
					{ L"Shout_SiphonPoison", NULL, 0, 1, 457, TEXT_ABILITIES_EARTH },
					{ L"Shout_SmokeCover", NULL, 0, 1, 548, TEXT_ABILITIES_AIR },
					{ L"Shout_SparkingSwings", NULL, 0, 1, 514, TEXT_ABILITIES_FIRE },
					{ L"Shout_SpiderLegs", NULL, 0, 0, 515, TEXT_ABILITIES_POLYMORPH },
					{ L"Shout_SpiritForm_Lucian", NULL, 0, 2, 1015, TEXT_ABILITIES_SOURCE },
					{ L"Shout_SpiritVision", NULL, 0, 1, 1015, TEXT_ABILITIES_SOURCE },
					{ L"Shout_SteelSkin", NULL, 0, 2, 401, TEXT_ABILITIES_POLYMORPH },
					{ L"Shout_Supernova", NULL, 0, 3, 440, TEXT_ABILITIES_FIRE },
					{ L"Shout_Taunt", NULL, 0, 2, 166, TEXT_ABILITIES_WARFARE },
					{ L"Shout_ThickOfTheFight", NULL, 1, 2, 395, TEXT_ABILITIES_WARFARE },
					{ L"Shout_VacuumAura", NULL, 1, 2, 522, TEXT_ABILITIES_AIR },
					{ L"Shout_VampiricHungerAura", NULL, 1, 1, 533, TEXT_ABILITIES_WATER },
					{ L"Shout_VenomCoating", NULL, 0, 1, 556, TEXT_ABILITIES_EARTH },
					{ L"Shout_VenomousAura", NULL, 1, 2, 557, TEXT_ABILITIES_EARTH },
					{ L"Shout_Whirlwind", NULL, 0, 2, 323, TEXT_ABILITIES_WARFARE },
					{ L"Shout_Wings", NULL, 0, 1, 397, TEXT_ABILITIES_POLYMORPH },
					{ L"Storm_Blood", NULL, 3, 4, 507, TEXT_ABILITIES_DEATH },
					{ L"Storm_Ethereal", NULL, 3, 4, 466, TEXT_ABILITIES_SUMMONING },
					{ L"Storm_Lightning", NULL, 3, 4, 582, TEXT_ABILITIES_AIR },
					{ L"Summon_ArtilleryPlant", NULL, 1, 2, 352, TEXT_ABILITIES_EARTH },
					{ L"Summon_BonePile", NULL, 0, 2, 476, TEXT_ABILITIES_DEATH },
					{ L"Summon_Cat", NULL, 0, 2, 383, TEXT_ABILITIES_SUMMONING },
					{ L"Summon_Condor", NULL, 0, 2, 524, TEXT_ABILITIES_SUMMONING },
					{ L"Summon_FireSlug", NULL, 1, 1, 460, TEXT_ABILITIES_FIRE },
					{ L"Summon_Incarnate", NULL, 0, 2, 417, TEXT_ABILITIES_SUMMONING },
					{ L"Summon_PlanarGateway", NULL, 2, 0, 467, TEXT_ABILITIES_SUMMONING },
					{ L"Summon_TotemFromSurface", NULL, 0, 2, 414, TEXT_ABILITIES_SUMMONING },
					{ L"Target_AcidInfusion", NULL, 2, 1, 495, TEXT_ABILITIES_SUMMONING },
					{ L"Target_Apportation", NULL, 0, 1, 461, TEXT_ABILITIES_AIR },
					{ L"Target_ArcaneStitch", NULL, 1, 3, 465, TEXT_ABILITIES_WATER },
					{ L"Target_BlackShroud", NULL, 1, 1, 454, TEXT_ABILITIES_DEATH },
					{ L"Target_Bless", NULL, 1, 1, 368, TEXT_ABILITIES_SOURCE },
					{ L"Target_BlessedSmokeCloud", NULL, 2, 1, 547, TEXT_ABILITIES_AIR },
					{ L"Target_BloatedCorpse", NULL, 0, 1, 468, TEXT_ABILITIES_DEATH },
					{ L"Target_BloodBubble", NULL, 0, 1, 345, TEXT_ABILITIES_DEATH },
					{ L"Target_BurnMyEyes", NULL, 0, 1, 354, TEXT_ABILITIES_FIRE },
					{ L"Target_Cannibalize", NULL, 0, 1, 337, TEXT_ABILITIES_SUMMONING },
					{ L"Target_Challenge", NULL, 0, 0, 471, TEXT_ABILITIES_WARFARE },
					{ L"Target_Charm", NULL, 0, 3, 583, TEXT_ABILITIES_SUMMONING },
					{ L"Target_ChickenTouch", NULL, 0, 2, 399, TEXT_ABILITIES_POLYMORPH },
					{ L"Target_CleanseWounds", NULL, 0, 2, 537, TEXT_ABILITIES_WATER },
					{ L"Target_Condense", NULL, 0, 1, 360, TEXT_ABILITIES_EARTH },
					{ L"Target_CorpseExplosion", NULL, 0, 1, 498, TEXT_ABILITIES_FIRE },
					{ L"Target_CorrosiveTouch", NULL, 0, 1, 539, TEXT_ABILITIES_EARTH },
					{ L"Target_CorruptedBlade", NULL, 0, 3, 469, TEXT_ABILITIES_ROGUES },
					{ L"Target_CripplingBlow", NULL, 0, 2, 149, TEXT_ABILITIES_WARFARE },
					{ L"Target_CryogenicStasis", NULL, 0, 2, 393, TEXT_ABILITIES_WATER },
					{ L"Target_Curse", NULL, 0, 1, 366, TEXT_ABILITIES_SOURCE },
					{ L"Target_CursedElectricInfusion", NULL, 2, 1, 529, TEXT_ABILITIES_SUMMONING },
					{ L"Target_DaggersDrawn", NULL, 2, 4, 114, TEXT_ABILITIES_ROGUES },
					{ L"Target_DeathWish", NULL, 0, 2, 445, TEXT_ABILITIES_DEATH },
					{ L"Target_DeathsDoor", NULL, 0, 3, 554, TEXT_ABILITIES_DEATH },
					{ L"Target_DecayingTouch", NULL, 0, 2, 347, TEXT_ABILITIES_DEATH },
					{ L"Target_DemonicBargain", NULL, 2, 1, 550, TEXT_ABILITIES_POLYMORPH },
					{ L"Target_DemonicStare", NULL, 1, 2, 400, TEXT_ABILITIES_SPECIAL },
					{ L"Target_DualWieldingAttack", NULL, 0, 2, 603, TEXT_ABILITIES_SPECIAL },
					{ L"Target_ElectricInfusion", NULL, 0, 1, 420, TEXT_ABILITIES_SUMMONING },
					{ L"Target_ElementalArrowheads", NULL, 0, 1, 485, TEXT_ABILITIES_HUNTING },
					{ L"Target_EnemyCripplingBlow_Shambling_Boss", NULL, 0, 2, 149, TEXT_ABILITIES_WARFARE },
					{ L"Target_Enrage", NULL, 0, 2, 340, TEXT_ABILITIES_WARFARE },
					{ L"Target_Equalize", NULL, 0, 2, 549, TEXT_ABILITIES_POLYMORPH },
					{ L"Target_EvasiveManeuver", NULL, 0, 1, 546, TEXT_ABILITIES_AIR },
					{ L"Target_Farsight", NULL, 1, 1, 505, TEXT_ABILITIES_HUNTING },
					{ L"Target_Fatality", NULL, 3, 2, 580, TEXT_ABILITIES_ROGUES },
					{ L"Target_FireInfusion", NULL, 0, 1, 424, TEXT_ABILITIES_SUMMONING },
					{ L"Target_FireWhip", NULL, 0, 3, 436, TEXT_ABILITIES_FIRE },
					{ L"Target_Fireblood", NULL, 0, 1, 361, TEXT_ABILITIES_FIRE },
					{ L"Target_FirstAid", NULL, 0, 1, 93, TEXT_ABILITIES_HUNTING },
					{ L"Target_FlamingCrescendo", NULL, 0, 1, 450, TEXT_ABILITIES_FIRE },
					{ L"Target_Flurry", NULL, 2, 4, 513, TEXT_ABILITIES_WARFARE },
					{ L"Target_Fortify", NULL, 0, 1, 358, TEXT_ABILITIES_EARTH },
					{ L"Target_FrostyShell", NULL, 0, 1, 326, TEXT_ABILITIES_WATER },
					{ L"Target_GagOrder", NULL, 0, 2, 506, TEXT_ABILITIES_ROGUES },
					{ L"Target_GraspOfTheStarved", NULL, 2, 2, 446, TEXT_ABILITIES_DEATH },
					{ L"Target_Harmony", NULL, 0, 2, 413, TEXT_ABILITIES_SUMMONING },
					{ L"Target_Haste", NULL, 0, 1, 79, TEXT_ABILITIES_FIRE },
					{ L"Target_HeavyAttack", NULL, 0, 3, 376, TEXT_ABILITIES_SPECIAL },
					{ L"Target_IceInfusion", NULL, 2, 1, 508, TEXT_ABILITIES_SUMMONING },
					{ L"Target_Infect", NULL, 0, 3, 348, TEXT_ABILITIES_DEATH },
					{ L"Target_KneeBreaker", NULL, 0, 2, 396, TEXT_ABILITIES_ROGUES },
					{ L"Target_MaddeningSong", NULL, 1, 1, 433, TEXT_ABILITIES_SPECIAL },
					{ L"Target_MassSabotage", NULL, 1, 2, 511, TEXT_ABILITIES_FIRE },
					{ L"Target_MasterOfSparks", NULL, 1, 1, 497, TEXT_ABILITIES_FIRE },
					{ L"Target_MosquitoSwarm", NULL, 0, 2, 344, TEXT_ABILITIES_DEATH },
					{ L"Target_NecrofireInfusion", NULL, 2, 1, 500, TEXT_ABILITIES_SUMMONING },
					{ L"Target_Overpower", NULL, 3, 2, 339, TEXT_ABILITIES_WARFARE },
					{ L"Target_PetrifyingTouch", NULL, 0, 1, 332, TEXT_ABILITIES_SPECIAL },
					{ L"Target_PoisonInfusion", NULL, 0, 1, 421, TEXT_ABILITIES_SUMMONING },
					{ L"Target_PowerInfusion", NULL, 0, 1, 415, TEXT_ABILITIES_SUMMONING },
					{ L"Target_PressureSpike", NULL, 0, 1, 462, TEXT_ABILITIES_AIR },
					{ L"Target_RangedInfusion", NULL, 0, 1, 416, TEXT_ABILITIES_SUMMONING },
					{ L"Target_ReactionShot", NULL, 0, 2, 481, TEXT_ABILITIES_HUNTING },
					{ L"Target_Restoration", NULL, 0, 1, 184, TEXT_ABILITIES_WATER },
					{ L"Target_RockSpikes", NULL, 0, 2, 327, TEXT_ABILITIES_EARTH },
					{ L"Target_Sabotage", NULL, 0, 1, 512, TEXT_ABILITIES_FIRE },
					{ L"Target_SerratedEdge", NULL, 0, 2, 320, TEXT_ABILITIES_ROGUES },
					{ L"Target_ShacklesOfPain", NULL, 0, 1, 349, TEXT_ABILITIES_DEATH },
					{ L"Target_ShadowInfusion", NULL, 0, 1, 475, TEXT_ABILITIES_SUMMONING },
					{ L"Target_ShockingTouch", NULL, 0, 2, 23, TEXT_ABILITIES_AIR },
					{ L"Target_SingleHandedAttack", NULL, 0, 1, 377, TEXT_ABILITIES_SPECIAL },
					{ L"Target_SleepingArms", NULL, 0, 2, 380, TEXT_ABILITIES_ROGUES },
					{ L"Target_SoulMate", NULL, 0, 1, 470, TEXT_ABILITIES_SUMMONING },
					{ L"Target_SourceVampirism", NULL, 0, 1, 367, TEXT_ABILITIES_SOURCE },
					{ L"Target_SpontaneousCombustion", NULL, 0, 2, 363, TEXT_ABILITIES_FIRE },
					{ L"Target_Squall", NULL, 1, 2, 434, TEXT_ABILITIES_SPECIAL },
					{ L"Target_StripResistance", NULL, 0, 2, 516, TEXT_ABILITIES_POLYMORPH },
					{ L"Target_Supercharge", NULL, 0, 1, 412, TEXT_ABILITIES_SUMMONING },
					{ L"Target_TentacleLash", NULL, 0, 2, 403, TEXT_ABILITIES_POLYMORPH },
					{ L"Target_TerrifyingCruelty", NULL, 0, 3, 487, TEXT_ABILITIES_ROGUES },
					{ L"Target_TimeWarp", NULL, 1, 2, 435, TEXT_ABILITIES_SPECIAL },
					{ L"Target_VacuumTouch", NULL, 0, 1, 528, TEXT_ABILITIES_AIR },
					{ L"Target_VampiricHunger", NULL, 0, 1, 532, TEXT_ABILITIES_WATER },
					{ L"Target_Vaporize", NULL, 0, 1, 1003, TEXT_ABILITIES_AIR },
					{ L"Target_WarpInfusion", NULL, 0, 1, 474, TEXT_ABILITIES_SUMMONING },
					{ L"Target_WaterInfusion", NULL, 0, 1, 423, TEXT_ABILITIES_SUMMONING },
					{ L"Target_Windwalker", NULL, 0, 1, 310, TEXT_ABILITIES_AIR },
					{ L"Target_WinterBlast", NULL, 0, 2, 331, TEXT_ABILITIES_WATER },
					{ L"Target_WormTremor", NULL, 0, 3, 479, TEXT_ABILITIES_EARTH },
					{ L"Teleportation_FreeFall", NULL, 0, 2, 309, TEXT_ABILITIES_AIR },
					{ L"Teleportation_LastRites", NULL, 0, 3, 391, TEXT_ABILITIES_DEATH },
					{ L"Teleportation_Netherswap", NULL, 0, 1, 20, TEXT_ABILITIES_AIR },
					{ L"Teleportation_SwapGround", NULL, 0, 1, 564, TEXT_ABILITIES_POLYMORPH },
					{ L"Tornado_Air", NULL, 0, 2, 27, TEXT_ABILITIES_AIR },
					{ L"Wall_LivingWall", NULL, 0, 2, 477, TEXT_ABILITIES_EARTH },
					{ L"Zone_LaserRay", NULL, 0, 3, 437, TEXT_ABILITIES_FIRE },
					{ 0 }
				};

static GAMEITEMCOLORS	ItemColors[] =	{

					{ L"Autunite", NULL, { 0xffe651, 0x918648, 0xfff3b7 } },
					{ L"Burnished Bronze", NULL, { 0xad724b, 0x76503c, 0xdba776 } },
					{ L"Chalcocite", NULL, { 0x00aac5, 0x00768e, 0xa4d7e2 } },
					{ L"Cobalt", NULL, { 0x2a4fd7, 0x273b89, 0x637fb7 } },
					{ L"Droxlerite", NULL, { 0xad46c0, 0x471450, 0xb381bd } },
					{ L"Gold", NULL, { 0xe2bb00, 0x8b6d00, 0xffdb7c } },
					{ L"Limonite", NULL, { 0xc95c18, 0x853820, 0xd4957a } },
					{ L"Malachite", NULL, { 0x24ac65, 0x13613c, 0x6ebb7b } },
					{ L"Mercury", NULL, { 0x909090, 0x5b5b5b, 0xc7c7c7 } },
					{ L"Pure Mythril", NULL, { 0xffffff, 0xa4a4a4, 0xffffff } },
					{ L"Red Ore", NULL, { 0xa81010, 0x590000, 0xeb7474 } },
					{ L"Tenebrium", NULL, { 0x2d2035, 0x5a4868, 0x575757 } },
					{ L"Venom Stone", NULL, { 0x4a9c08, 0x155019, 0x84925a } },
					{ L"Cotton Candy", NULL, { 0xe674bf, 0xab2d71, 0xffc2ea } },
					{ L"Grumio's Petrolium Brand", NULL, { 0x9dd92c, 0x229b23, 0xe2d95b } },
					{ L"Grumio's Favorite Triade", NULL, { 0xcc3a1c, 0x586aba, 0xcab945 } },
					{ L"ArenaLobby", NULL, { 0xad724b, 0x76503c, 0xdba776 } },
					{ L"ArenaRed", NULL, { 0x851212, 0x970000, 0x414141 } },
					{ L"ArenaBlue", NULL, { 0x375f92, 0x2184ff, 0xffd392 } },
					{ L"ArenaGreen", NULL, { 0x5a6e3d, 0x46a600, 0xffd392 } },
					{ L"ArenaYellow", NULL, { 0xa68a59, 0x785628, 0xffd392 } },
					{ L"Battlemage", NULL, { 0x3b1e1e, 0xa78965, 0xceac95 } },
					{ L"Cleric", NULL, { 0xad6b36, 0x0165a1, 0xe9e9e9 } },
					{ L"Enchanter", NULL, { 0x3d3329, 0x97611b, 0xdaa550 } },
					{ L"Fighter", NULL, { 0xffd392, 0xa13901, 0xc0c0c0 } },
					{ L"Inquisitor", NULL, { 0x2c2d34, 0xb8a88f, 0xf2dcc3 } },
					{ L"Knight", NULL, { 0x877067, 0x9c1616, 0xc0c0c0 } },
					{ L"Metamorph", NULL, { 0xc39a70, 0x591c1c, 0xac9d80 } },
					{ L"Ranger", NULL, { 0x876d61, 0x254739, 0xeec07b } },
					{ L"Rogue", NULL, { 0x602931, 0x3b5572, 0xaaabb8 } },
					{ L"Shadowblade", NULL, { 0x3c2121, 0x82282b, 0xc8a79b } },
					{ L"Conjurer", NULL, { 0x96774a, 0x625271, 0xb2ada5 } },
					{ L"Wayfarer", NULL, { 0x402905, 0x72716f, 0x72716f } },
					{ L"Witch", NULL, { 0x182333, 0x541616, 0xedcc81 } },
					{ L"Wizard", NULL, { 0x334865, 0x16232d, 0xedcc81 } },
					{ L"White Prisoner Robe", NULL, { 0xaeaea2, 0xaeaea2, 0xaeaea2 } },
					{ L"StarterArmorLeatherEarly", NULL, { 0x594230, 0xBE835A, 0xFDD280 } },
					{ L"StarterArmorLeatherLate", NULL, { 0x7B6351, 0x6491B8, 0xC0C0C0 } },
					{ L"StarterArmorChainmailEarly", NULL, { 0x6D503A, 0x580813, 0xC0C0C0 } },
					{ L"StarterArmorChainmailLate", NULL, { 0x916A4E, 0x6491B8, 0xC0C0C0 } },
					{ L"LeatherCommonEarly", NULL, { 0x8D5534, 0x4B5E0D, 0xC0C0C0 } },
					{ L"LeatherRareEarly", NULL, { 0x644B28, 0x255641, 0xFDD280 } },
					{ L"LeatherCommonLate", NULL, { 0xB28E77, 0x6491B8, 0xF1DBA6 } },
					{ L"LeatherRareLate", NULL, { 0x592F17, 0x0F699D, 0xF1DBA6 } },
					{ L"ChainCommonEarly", NULL, { 0xB8A596, 0x4B5E0D, 0xb2ada5 } },
					{ L"ChainRareEarly", NULL, { 0xad6b36, 0x0165a1, 0xC0C0C0 } },
					{ L"ChainCommonLate", NULL, { 0x654938, 0x255641, 0xC0C0C0 } },
					{ L"ChainRareLate", NULL, { 0x4a3a36, 0x4E80AB, 0xf1e1d7 } },
					{ L"ScaleCommonEarly", NULL, { 0x897261, 0x671F17, 0xC0C0C0 } },
					{ L"ScaleRareEarly", NULL, { 0x523B2D, 0x284C7F, 0xD1D1D1 } },
					{ L"ScaleCommonLate", NULL, { 0x745239, 0x2B2020, 0xC2AD9D } },
					{ L"ScaleRareLate", NULL, { 0x232808, 0x173D72, 0xF3E0B4 } },
					{ L"PlateCommonEarly", NULL, { 0xB8A596, 0x8B2E2E, 0xC0C0C0 } },
					{ L"PlateRareEarly", NULL, { 0xC0C0C0, 0x34758D, 0xc0c0c0 } },
					{ L"PlateCommonLate", NULL, { 0xF1DBA6, 0x368995, 0xB8A596 } },
					{ L"PlateRareLate", NULL, { 0xFDD280, 0x1D523B, 0xC0C0C0 } },
					{ L"MageCommon", NULL, { 0x86583d, 0x33302c, 0xc0c0c0 } },
					{ L"MageUncommon", NULL, { 0x556464, 0x51383e, 0xedcc81 } },
					{ L"MageRare", NULL, { 0x394351, 0x16232d, 0xedcc81 } },
					{ L"MageEpic", NULL, { 0x182333, 0x541616, 0xedcc81 } },
					{ L"MageMagic", NULL, { 0x7d0202, 0xeda73a, 0xbe835a } },
					{ L"MagisterRobe", NULL, { 0x8B2E2E, 0x8B2E2E, 0x8B2E2E } },
					{ L"MageStarterRobeCommon", NULL, { 0x0d2972, 0x5e508d, 0xc0c0c0 } },
					{ L"MageStarterRobeRare", NULL, { 0x5e508d, 0xc79536, 0xc0c0c0 } },
					{ L"DefaultGray", NULL, { 0xb6b6b6, 0x6a5e3e, 0xacacac } },
					{ L"LizardsCommon", NULL, { 0x8a5b46, 0xeec07b, 0xa3a3a3 } },
					{ L"LizardsUncommon", NULL, { 0xcdb887, 0x9f8fae, 0xd6d6d6 } },
					{ L"LizardsRare", NULL, { 0x674035, 0xd3754f, 0x8fb4b3 } },
					{ L"LizardsEpic", NULL, { 0x473c3c, 0xd6d6d6, 0x5d3f42 } },
					{ L"LizardsCommonGem", NULL, { 0xb80000, 0xeec07b, 0xa3a3a3 } },
					{ L"LizardsUncommonGem", NULL, { 0x8a857e, 0x9f8fae, 0xb3bdcd } },
					{ L"LizardsRareGem", NULL, { 0x18c3ee, 0xcfa595, 0xb3bfbf } },
					{ L"LizardsEpicGem", NULL, { 0xc696f8, 0xd6d6d6, 0x6e3e42 } },
					{ L"LizardsShieldACommon", NULL, { 0x644c43, 0xd20000, 0xbebebe } },
					{ L"LizardsShieldAUncommon", NULL, { 0x806446, 0x020aa8, 0xeec07b } },
					{ L"LizardsShieldARare", NULL, { 0xbebebe, 0x30cde9, 0xa9714c } },
					{ L"LizardsShieldAEpic", NULL, { 0xbebebe, 0xff7800, 0x775b5b } },
					{ L"LizardsBowAShieldCCommon", NULL, { 0x605246, 0x9c9c9c, 0x523a24 } },
					{ L"LizardsBowAShieldCUncommon", NULL, { 0x6f533e, 0xeec07b, 0x441616 } },
					{ L"LizardsBowAShieldCRare", NULL, { 0x482d17, 0xc0a997, 0x1e1c1c } },
					{ L"LizardsBowAShieldCEpic", NULL, { 0x5f0606, 0xb2cac9, 0x5f0606 } },
					{ L"LizardsStaffACommon", NULL, { 0x7c715a, 0xa80202, 0xb4b4b4 } },
					{ L"LizardsStaffAUncommon", NULL, { 0xbebebe, 0x25297f, 0xeec07b } },
					{ L"LizardsStaffARare", NULL, { 0x628998, 0x0095a7, 0x8d8d8d } },
					{ L"LizardsStaffAEpic", NULL, { 0x6c3f2f, 0x710c9f, 0xdfc773 } },
					{ L"LizardsStaffBCommon", NULL, { 0x796a57, 0xae0505, 0xcbc390 } },
					{ L"LizardsStaffBUncommon", NULL, { 0x305090, 0x1c1a7f, 0xa5ae97 } },
					{ L"LizardsStaffBRare", NULL, { 0x00c8cf, 0x00898e, 0xd9855b } },
					{ L"LizardsStaffBEpic", NULL, { 0xd5b9b2, 0x640d97, 0x232428 } },
					{ L"LizardsUnique01", NULL, { 0x37452e, 0xfae9c0, 0xa06015 } },
					{ L"LizardsUnique01Gem", NULL, { 0x0c36bf, 0xfae9c0, 0xa06015 } },
					{ L"LizardsUnique02", NULL, { 0x5f4e13, 0x4a7a76, 0x796464 } },
					{ L"LizardsUnique02Gem", NULL, { 0x17e7d1, 0x4a7a76, 0x796464 } },
					{ L"LizardsShieldCCommon", NULL, { 0xbfbfbf, 0xbfbfbf, 0x664728 } },
					{ L"LizardsShieldCUncommon", NULL, { 0xd6d6d6, 0xefd0a3, 0x815639 } },
					{ L"LizardsShieldCRare", NULL, { 0xd6d6d6, 0xad896f, 0x895e4a } },
					{ L"LizardsShieldCEpic", NULL, { 0xd6d6d6, 0xecce87, 0x553530 } },
					{ L"LizardsStaffAWizard", NULL, { 0x3e281d, 0x84d9de, 0xd3c097 } },
					{ L"LizardsRogue", NULL, { 0x70564e, 0x57515e, 0xb4b8ba } },
					{ L"LizardsKnight", NULL, { 0x289c04, 0x7d573d, 0xc5bbb6 } },
					{ L"LizardsShadowblade", NULL, { 0x2d2121, 0x692323, 0xceb9b0 } },
					{ L"DwarvesCommon", NULL, { 0xd2a767, 0x9f1616, 0xc3c2c2 } },
					{ L"DwarvesUncommon", NULL, { 0x5a4e48, 0x1b8135, 0xdadae3 } },
					{ L"DwarvesRare", NULL, { 0xac6d4e, 0xf99246, 0x899998 } },
					{ L"DwarvesEpic", NULL, { 0xc8bb9f, 0x900c0c, 0x6f6661 } },
					{ L"DwarvesRogue", NULL, { 0x3f3d42, 0xd58c44, 0xc6d2cd } },
					{ L"DwarvesShadowblade", NULL, { 0x6e5245, 0x54376f, 0xc9d2da } },
					{ L"HumansCommon", NULL, { 0x71562e, 0x7b402d, 0xc9c9c9 } },
					{ L"HumansUncommon", NULL, { 0x796f54, 0x756f4d, 0xbebebe } },
					{ L"HumansRare", NULL, { 0x58493a, 0x6d6653, 0x9497aa } },
					{ L"HumansEpic", NULL, { 0xd4a569, 0x684313, 0xf4e9da } },
					{ L"HumansStaffACommon", NULL, { 0x891b1b, 0x29231f, 0x96714a } },
					{ L"HumansStaffAUncommon", NULL, { 0x20a138, 0x77786e, 0x46442f } },
					{ L"HumansStaffARare", NULL, { 0x285bb9, 0x8e7664, 0x4f2f20 } },
					{ L"HumansStaffAEpic", NULL, { 0xf58f0b, 0xe0d1a4, 0x4c130e } },
					{ L"HumansStaffBCommon", NULL, { 0xf9debd, 0x57cee0, 0xdfcec0 } },
					{ L"HumansStaffBUncommon", NULL, { 0xd39f4c, 0x20a138, 0xd6adad } },
					{ L"HumansStaffBRare", NULL, { 0x696670, 0x285bb9, 0xedc8b2 } },
					{ L"HumansStaffBEpic", NULL, { 0xd4a569, 0xf58f0b, 0xe7eeb7 } },
					{ L"HumansStaffWitch", NULL, { 0xd09d61, 0xc11414, 0xc7ac97 } },
					{ L"HumansBowCommon", NULL, { 0xb6b6b6, 0x5c4a36, 0x5e3419 } },
					{ L"HumansBowUncommon", NULL, { 0xeec07b, 0x5c4a36, 0x705639 } },
					{ L"HumansBowRare", NULL, { 0xdebfa1, 0x4e4133, 0x9e7d59 } },
					{ L"HumansBowEpic", NULL, { 0xffe9d4, 0x584742, 0x936641 } },
					{ L"HumansDaggerBCommon", NULL, { 0xeec07b, 0x5c452b, 0x989898 } },
					{ L"HumansDaggerBUncommon", NULL, { 0x74706d, 0x886649, 0xeeccaf } },
					{ L"HumansDaggerBRare", NULL, { 0xb26f33, 0x483b2f, 0xa2c3c7 } },
					{ L"HumansDaggerBEpic", NULL, { 0x8e4848, 0x483232, 0xe1e1e1 } },
					{ L"ElvesCommon", NULL, { 0x937058, 0x5e7b35, 0x009ca2 } },
					{ L"ElvesUncommon", NULL, { 0x70624c, 0x118570, 0x417b3b } },
					{ L"ElvesRare", NULL, { 0x564333, 0x18692d, 0x58558c } },
					{ L"ElvesEpic", NULL, { 0x512a1c, 0x456e10, 0xe87505 } },
					{ L"ElvesUnique01", NULL, { 0x907462, 0x5e7b35, 0xa0a8a8 } },
					{ L"ElvesWizard", NULL, { 0x564333, 0x18692d, 0x6c98ce } },
					{ L"ElvesShieldCommon", NULL, { 0x009ca2, 0x5e7b35, 0x937058 } },
					{ L"ElvesShieldUncommon", NULL, { 0x00a204, 0x3c6d1e, 0x70624c } },
					{ L"ElvesShieldRare", NULL, { 0x4f7ecd, 0x318045, 0x7a5c43 } },
					{ L"ElvesShieldEpic", NULL, { 0xe87505, 0x456e10, 0x512a1c } },
					{ L"BeltCommon", NULL, { 0x8a5b46, 0xd20c0c, 0x979797 } },
					{ L"BeltUncommon", NULL, { 0x664e43, 0x213fbd, 0xdec183 } },
					{ L"BeltRare", NULL, { 0x5a512f, 0x1193b8, 0xc1c1c1 } },
					{ L"BeltEpic", NULL, { 0x3c2820, 0x119f39, 0xc4722a } },
					{ L"BeltUnique01", NULL, { 0x6a5544, 0x7d22b2, 0xedbda5 } },
					{ L"ElvesStaffBCommon", NULL, { 0x2e7fdb, 0x674524, 0x90a953 } },
					{ L"ElvesStaffBUncommon", NULL, { 0x17c528, 0x3f7417, 0xca6a6a } },
					{ L"ElvesStaffBRare", NULL, { 0xd20000, 0x778f2d, 0x6a9cca } },
					{ L"ElvesStaffBEpic", NULL, { 0xf5e81d, 0x07a9c1, 0xd87f16 } },
					{ L"ElvesStaffUnique01", NULL, { 0x009ca2, 0x094746, 0xb67314 } },
					{ L"ElvesStaffUnique02", NULL, { 0xc0ac05, 0x146d48, 0x262741 } },
					{ L"ElvesFighter", NULL, { 0x544227, 0x456e10, 0xa36400 } },
					{ L"ElvesShieldFighter", NULL, { 0xa36400, 0x456e10, 0x544227 } },
					{ L"RedCape", NULL, { 0x7a695f, 0x972e2e, 0xdbae6b } },
					{ L"SaheilaRewardSpear", NULL, { 0x9c7f6d, 0xded1c0, 0xd61818 } },
					{ L"OriginBeast", NULL, { 0x402905, 0x72716f, 0x72716f } },
					{ L"OriginSebille", NULL, { 0x424242, 0x424242, 0x424242 } },
					{ L"OriginLohse", NULL, { 0x25262b, 0x32395d, 0xb0a28b } },
					{ L"OriginRedPrince", NULL, { 0xc39a70, 0x43418, 0xac9d80 } },
					{ L"OriginIfan", NULL, { 0x201a1a, 0x45444f, 0x8a8785 } },
					{ L"OriginIfanPants", NULL, { 0x201a1a, 0x4a3327, 0x8a8785 } },
					{ L"OriginFane", NULL, { 0x25262b, 0x54181f, 0xf8e5cf } },
					{ L"MagisterWhite", NULL, { 0xddd4c4, 0x535353, 0xedcc81 } },
					{ NULL, NULL, { 0 } }
				};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initialisation							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// La fonction effectue une copie de pBonus (si non NULL)
// Elle retourne cette copie (éventuellement modifiée) ou NULL en cas d'erreur.
// La structure pointée par pBonus n'est JAMAIS modifiée.

GAMEEDITBONUS* Game_Bonus(HWND hWnd, GAMEEDITBONUS *pEditBonus, WCHAR *pszObject)
{
	GAMEEDITBONUSCONTEXT*	pBonusContext;
	GAMEEDITBONUS*		pNewEditBonus;
	PROPSHEETHEADER*	psh;
	PROPSHEETPAGE*		psp;
	int			iNumPages;
	int			iResult;
	int			i;

	pNewEditBonus = NULL;
	psh = NULL;
	psp = NULL;
	iResult = -1;

	//--- Alloue les structures ---

	pBonusContext = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMEEDITBONUSCONTEXT));
	if (!pBonusContext)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		goto Done;
		}

	pBonusContext->pEdit = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMEEDITBONUS));
	if (!pBonusContext->pEdit)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		goto Done;
		}

	psh = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(PROPSHEETHEADER));
	if (!psh)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		goto Done;
		}

	for (i = 0, iNumPages = 0; BonusesPages[i].uPageID != 0; i++, iNumPages++);

	psp = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(PROPSHEETPAGE)*iNumPages);
	if (!psp)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		goto Done;
		}

	//--- Initialisations ---

	if (!Game_BonusCopy(pBonusContext->pEdit,pEditBonus))
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		goto Done;
		}

	pBonusContext->pszObject = pszObject;

	//--- Création des pages ---

	for (i = 0; BonusesPages[i].uPageID != 0; i++)
		{
		GAMEEDITPAGECONTEXT*	ctx;

		ctx = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMEEDITPAGECONTEXT));
		if (!ctx)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			goto Done;
			}

		ctx->bonus.pContext = pBonusContext;
		ctx->uPageID = BonusesPages[i].uPageID;
		ctx->pszInfo = Locale_GetText(BonusesPages[i].uInfoID);

		psp[i].dwSize = sizeof(PROPSHEETPAGE);
		psp[i].dwFlags = PSP_USETITLE;
		psp[i].hInstance = App.hInstance;
		psp[i].pszTemplate = MAKEINTRESOURCE(BonusesPages[i].uResID);
		psp[i].pszTitle = Locale_GetText(BonusesPages[i].uTitleID);
		psp[i].pfnDlgProc = (DLGPROC)Game_BonusProc;
		psp[i].lParam = (LPARAM)ctx;
		}

	//--- Affichage de la boîte de dialogue ---

	psh->dwSize = sizeof(PROPSHEETHEADER);
	psh->dwFlags = PSH_PROPSHEETPAGE|PSH_USEICONID|PSH_NOAPPLYNOW|PSH_WIZARD;
	psh->hwndParent = hWnd;
	psh->hInstance = App.hInstance;
	psh->pszIcon = MAKEINTRESOURCE(1);
	psh->nPages = iNumPages;
	psh->ppsp = psp;

	iResult = PropertySheet(psh);
	if (iResult)
		{
		pNewEditBonus = pBonusContext->pEdit;
		pBonusContext->pEdit = NULL;
		}

Done:	for (i = 0; Skills[i].pszId != NULL; i++) if (Skills[i].pszName != NULL) { HeapFree(App.hHeap,0,Skills[i].pszName); Skills[i].pszName = NULL; };
	for (i = 0; ItemColors[i].pszId != NULL; i++) if (ItemColors[i].pszName != NULL) { HeapFree(App.hHeap,0,ItemColors[i].pszName); ItemColors[i].pszName = NULL; };
	if (psh) HeapFree(App.hHeap,0,psh);
	if (psp)
		{
		for (i = 0; BonusesPages[i].uPageID != 0; i++) if (psp[i].lParam) HeapFree(App.hHeap,0,(void *)psp[i].lParam);
		HeapFree(App.hHeap,0,psp);
		}
	if (pBonusContext)
		{
		Locale_Unload(LOCALE_TYPE_MISC,(void **)&pBonusContext->pLocale,NULL);
		Game_BonusRelease(pBonusContext->pEdit,FALSE);
		HeapFree(App.hHeap,0,pBonusContext);
		}

	if (iResult == -1) Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
	return(pNewEditBonus);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Boîte de dialogue						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

BOOL CALLBACK Game_BonusProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	PROPSHEETPAGE*	psp;

	if (uMsgId == WM_MEASUREITEM)
		{
		if (Dialog_ViewComboMeasureItem(190,(MEASUREITEMSTRUCT *)lParam)) return(TRUE);
		((MEASUREITEMSTRUCT *)lParam)->itemWidth = 0;
		((MEASUREITEMSTRUCT *)lParam)->itemHeight = App.Font.uFontHeight+4;
		if (((MEASUREITEMSTRUCT *)lParam)->itemHeight < 24+4) ((MEASUREITEMSTRUCT *)lParam)->itemHeight = 24+4;
		return(TRUE);
		}

	if (uMsgId == WM_INITDIALOG)
		{
		GAMEEDITPAGECONTEXT*	ctx;
		RECT			rcDialog;
		int			Height;

		ctx = (GAMEEDITPAGECONTEXT *)((PROPSHEETPAGE *)lParam)->lParam;
		Height = Dialog_GetInfoHeight(hDlg,100,ctx->pszInfo);
		GetWindowRect(hDlg,&rcDialog);
		// PropSheet dialogs need more work to be resized...
		//SetWindowPos(hDlg,NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top+Height,SWP_NOZORDER|SWP_NOMOVE);
		GetWindowRect(GetDlgItem(hDlg,100),&rcDialog);
		SetWindowPos(GetDlgItem(hDlg,100),NULL,0,0,rcDialog.right-rcDialog.left,Height,SWP_NOZORDER|SWP_NOMOVE);
		SetWindowLongPtr(hDlg,DWLP_USER,(LONG_PTR)lParam);
		SendMessage(hDlg,PSM_SETWIZBUTTONS,(WPARAM)PSWIZB_NEXT,0);

		switch(ctx->uPageID)
			{
			case GAME_PAGE_BONUS_TARGET:
				Dialog_OffsetY(hDlg,108,Height);
				Dialog_OffsetY(hDlg,208,Height);
				Dialog_OffsetY(hDlg,200,Height);
				Dialog_OffsetY(hDlg,201,Height);
				Dialog_OffsetY(hDlg,202,Height);
				Dialog_OffsetY(hDlg,777,Height);
				SetDlgItemText(hDlg,200,Locale_GetText(TEXT_BONUS_TYPE_ARMOR));
				SetDlgItemText(hDlg,201,Locale_GetText(TEXT_BONUS_TYPE_WEAPON));
				SetDlgItemText(hDlg,202,Locale_GetText(TEXT_BONUS_TYPE_SHIELD));
				Game_EditValueSizeObject(hDlg,777);
				break;

			case GAME_PAGE_BONUS_LIST:
				Dialog_OffsetY(hDlg,200,Height);
				GetWindowRect(GetDlgItem(hDlg,200),&rcDialog);
				MapWindowPoints(NULL,hDlg,(POINT *)&rcDialog,2);
				rcDialog.bottom -= Height;
				SetWindowPos(GetDlgItem(hDlg,200),NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top,SWP_NOZORDER|SWP_NOMOVE);
				break;

			case GAME_PAGE_BONUS_VALUE:
				Dialog_OffsetY(hDlg,200,Height);
				Dialog_OffsetY(hDlg,201,Height);
				break;

			case GAME_PAGE_BONUS_REFLECTION:
				Dialog_OffsetY(hDlg,200,Height);
				Dialog_OffsetY(hDlg,201,Height);
				Dialog_OffsetY(hDlg,208,Height);
			case GAME_PAGE_BONUS_DAMAGETYPE:
				Dialog_OffsetY(hDlg,211,Height);
				Dialog_OffsetY(hDlg,212,Height);
				Dialog_OffsetY(hDlg,216,Height);
				Dialog_OffsetY(hDlg,217,Height);
				Dialog_OffsetY(hDlg,218,Height);
				Dialog_OffsetY(hDlg,219,Height);
				Dialog_OffsetY(hDlg,220,Height);
				break;

			case GAME_PAGE_BONUS_SKILLS:
				Dialog_OffsetY(hDlg,180,Height);
				Dialog_OffsetY(hDlg,190,Height);
				Dialog_OffsetY(hDlg,200,Height);
				GetWindowRect(GetDlgItem(hDlg,200),&rcDialog);
				MapWindowPoints(NULL,hDlg,(POINT *)&rcDialog,2);
				rcDialog.bottom -= Height;
				SetWindowPos(GetDlgItem(hDlg,200),NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top,SWP_NOZORDER|SWP_NOMOVE);
				break;

			case GAME_PAGE_BONUS_COLORS:
				Dialog_OffsetY(hDlg,200,Height);
				GetWindowRect(GetDlgItem(hDlg,200),&rcDialog);
				MapWindowPoints(NULL,hDlg,(POINT *)&rcDialog,2);
				rcDialog.bottom -= Height;
				SetWindowPos(GetDlgItem(hDlg,200),NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top,SWP_NOZORDER|SWP_NOMOVE);
				break;
			}

		return(TRUE);
		}

	psp = (PROPSHEETPAGE *)GetWindowLongPtr(hDlg,DWLP_USER);
	if (!psp) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			if (wParam == 100)
				{
				Dialog_DrawInfo(((GAMEEDITPAGECONTEXT *)psp->lParam)->pszInfo,(DRAWITEMSTRUCT *)lParam,BF_RECT);
				return(TRUE);
				}
			else if (wParam == 900)
				{
				Game_BonusDrawSeparator(Locale_GetText(TEXT_BONUS_RESULT),(DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			else if (wParam == 901)
				{
				Game_BonusDrawResult((GAMEEDITPAGECONTEXT *)psp->lParam,(DRAWITEMSTRUCT *)lParam);
				return(TRUE);
				}
			switch(((GAMEEDITPAGECONTEXT *)psp->lParam)->uPageID)
				{
				case GAME_PAGE_BONUS_TARGET:
					if (wParam == 777)
						{
						Game_EditValueDrawObject(((GAMEEDITPAGECONTEXT *)psp->lParam)->bonus.pContext->pszObject,NULL,(DRAWITEMSTRUCT *)lParam);
						return(TRUE);
						}
					else if (wParam == 108 || wParam == 208)
						{
						Game_BonusDrawSeparator(Locale_GetText(wParam == 108?TEXT_BONUS_TARGET:TEXT_BONUS_TARGETTYPE),(DRAWITEMSTRUCT *)lParam);
						return(TRUE);
						}
					break;
				case GAME_PAGE_BONUS_LIST:
					if (wParam == 200)
						{
						Game_BonusDraw((DRAWITEMSTRUCT *)lParam);
						return(TRUE);
						}
					break;
				case GAME_PAGE_BONUS_VALUE:
					if (wParam == 201)
						{
						Dialog_DrawLabel(Locale_GetText(((GAMEEDITPAGECONTEXT *)psp->lParam)->bonus.pContext->pSelected->uLocaleID),(DRAWITEMSTRUCT *)lParam,DT_RIGHT);
						return(TRUE);
						}
					break;
				case GAME_PAGE_BONUS_REFLECTION:
					if (wParam == 201)
						{
						Dialog_DrawLabel(Locale_GetText(TEXT_BONUS_REFLECTIONPERCENT),(DRAWITEMSTRUCT *)lParam,DT_RIGHT);
						return(TRUE);
						}
					else if (wParam == 208)
						{
						Game_BonusDrawSeparator(Locale_GetText(TEXT_BONUS_TITLE_DAMAGETYPE),(DRAWITEMSTRUCT *)lParam);
						return(TRUE);
						}
					break;
				case GAME_PAGE_BONUS_SKILLS:
					if (wParam == 190)
						{
						Dialog_ViewComboDrawItem((DRAWITEMSTRUCT *)lParam);
						return(TRUE);
						}
					break;
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case LBN_DBLCLK:
					SendMessage(GetParent(hDlg),PSM_PRESSBUTTON,(WPARAM)PSBTN_NEXT,0);
					return(TRUE);
				case LBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 190:
							Dialog_ViewComboChanged(hDlg,190,200,&App.Config.uSkillsView);
							return(TRUE);
						case 200:
							SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,0,(LPARAM)PSWIZB_BACK|(Game_BonusSetNextPage(hDlg,FALSE,(GAMEEDITPAGECONTEXT *)psp->lParam) != -1?PSWIZB_NEXT:0));
							return(TRUE);
						}
					break;
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 200:
							SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,0,(LPARAM)PSWIZB_BACK|(Game_BonusSetNextPage(hDlg,FALSE,(GAMEEDITPAGECONTEXT *)psp->lParam) != -1?PSWIZB_FINISH:PSWIZB_DISABLEDFINISH));
							InvalidateRect(GetDlgItem(hDlg,901),NULL,FALSE);
							return(TRUE);
						}
					break;
				case BN_CLICKED:
					switch(((GAMEEDITPAGECONTEXT *)psp->lParam)->uPageID)
						{
						case GAME_PAGE_BONUS_DAMAGETYPE:
							Game_BonusDamageType(hDlg,((GAMEEDITPAGECONTEXT *)psp->lParam)->bonus.pContext->pEdit->value.szVal,GAME_DAMAGETYPE_STORE);
							InvalidateRect(GetDlgItem(hDlg,901),NULL,FALSE);
							return(TRUE);
						case GAME_PAGE_BONUS_REFLECTION:
							Game_BonusDamageType(hDlg,((GAMEEDITPAGECONTEXT *)psp->lParam)->bonus.pContext->pEdit->reflection.szDamageType,GAME_DAMAGETYPE_STORE);
							InvalidateRect(GetDlgItem(hDlg,901),NULL,FALSE);
							return(TRUE);
						case GAME_PAGE_BONUS_SKILLS:
							switch(LOWORD(wParam))
								{
								case 180:
									App.Config.bSkillsGroups = (IsDlgButtonChecked(hDlg,180) == BST_CHECKED?TRUE:FALSE);
									SendDlgItemMessage(hDlg,200,LVM_ENABLEGROUPVIEW,(WPARAM)App.Config.bSkillsGroups,0);
									return(TRUE);
								}
							break;
						}
					break;
				}
			break;

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case UDN_DELTAPOS:
					Game_BonusEditMove(hDlg,((NMHDR *)lParam)->idFrom,((NM_UPDOWN *)lParam)->iDelta > 0?-1:1,(GAMEEDITPAGECONTEXT *)psp->lParam);
					return(TRUE);
				case LVN_ITEMACTIVATE:
					SendMessage(GetParent(hDlg),PSM_PRESSBUTTON,(WPARAM)PSBTN_FINISH,0);
					return(TRUE);
				case LVN_ITEMCHANGED:
					SendMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,0,(LPARAM)PSWIZB_BACK|(Game_BonusSetNextPage(hDlg,FALSE,(GAMEEDITPAGECONTEXT *)psp->lParam) != -1?PSWIZB_FINISH:PSWIZB_DISABLEDFINISH));
					return(TRUE);
				case PSN_SETACTIVE:
					SetWindowLongPtr(hDlg,DWLP_MSGRESULT,Game_BonusActivate(hDlg,(GAMEEDITPAGECONTEXT *)psp->lParam)?0:-1);
					return(TRUE);
				case PSN_WIZBACK:
					SetWindowLongPtr(hDlg,DWLP_MSGRESULT,Game_BonusSetPrevPage(hDlg,(GAMEEDITPAGECONTEXT *)psp->lParam));
					return(TRUE);
				case PSN_WIZNEXT:
					SetWindowLongPtr(hDlg,DWLP_MSGRESULT,Game_BonusSetNextPage(hDlg,TRUE,(GAMEEDITPAGECONTEXT *)psp->lParam));
					return(TRUE);
				case PSN_WIZFINISH:
					SetWindowLongPtr(hDlg,DWLP_MSGRESULT,(Game_BonusSetNextPage(hDlg,TRUE,(GAMEEDITPAGECONTEXT *)psp->lParam) == -1)?TRUE:FALSE);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}

//--- Sous-classe d'un contrôle d'édition ---

// dwRefData can be one of the following value: BONUS_FLAG_INT, BONUS_FLAG_UINT, BONUS_FLAG_FLOAT

LRESULT CALLBACK Game_BonusEditSubclass(HWND hWnd, UINT uMsgId, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch(uMsgId)
		{
		case WM_PASTE: {
			WCHAR	szText[BONUS_NUMBER_MAX_LEN+1];
			LRESULT	lResult;

			lResult = DefSubclassProc(hWnd,uMsgId,wParam,lParam);
			SendMessage(hWnd,WM_GETTEXT,(WPARAM)BONUS_NUMBER_MAX_LEN+1,(LPARAM)szText);
			Game_BonusCleanNumber(szText,dwRefData == BONUS_FLAG_FLOAT,dwRefData == BONUS_FLAG_FLOAT || dwRefData == BONUS_FLAG_INT);
			SendMessage(hWnd,WM_SETTEXT,0,(LPARAM)szText);
			return(lResult);
			}

		case WM_CHAR:
			if (wParam == 8) // Suppr
				break;
			else if (wParam == 3) // Ctrl C
				break;
			else if (wParam == 22) // Ctrl V
				break;
			else if (wParam == 26) // Ctrl Z
				break;

			else if (wParam == 1) // Ctrl A
				{
				SendMessage(hWnd,EM_SETSEL,(WPARAM)0,(LPARAM)-1);
				return(0);
				}

			else if (wParam >= 48 && wParam <= 57) // 0-9
				{
				WCHAR	szSign[2];
				int	iStartSel;
				int	iEndSel;

				SendMessage(hWnd,WM_GETTEXT,(WPARAM)2,(LPARAM)szSign);
				if (szSign[0] != L'-') break; // String does NOT start with "-", OK
				SendMessage(hWnd,EM_GETSEL,(WPARAM)&iStartSel,(LPARAM)&iEndSel);
				if (!iStartSel && iEndSel) break; // Selection(0,y) where y > 0 ["-" will be discarded if any], OK
				if (iStartSel) break; // Selection(x,?) where x > 0, OK
				return(0);
				}

			else if (wParam == 46) // .
				{
				if (dwRefData == BONUS_FLAG_FLOAT)
					{
					WCHAR	szText[BONUS_NUMBER_MAX_LEN+1];
					int	iStartSel;
					int	iEndSel;
					int	i,b;

					SendMessage(hWnd,WM_GETTEXT,(WPARAM)BONUS_NUMBER_MAX_LEN+1,(LPARAM)szText);
					SendMessage(hWnd,EM_GETSEL,(WPARAM)&iStartSel,(LPARAM)&iEndSel);
					if (!wcschr(szText,L'.')) // No "." in the string
						{
						if (szText[0] != L'-') break; // No "-" at the beginning, OK
						if (iStartSel) break; // Selection(x,?) where X > 0, OK
						return(0);
						}
					for (i = iStartSel, b = 0; i != iEndSel; i++)
						{
						if (szText[i] == '.')
							{
							b = 1; // "." is inside the selection, it will be discarded
							break;
							}
						}
					if (b) break;
					}
				}

			else if (wParam == 45) // -
				{
				if (dwRefData == BONUS_FLAG_INT || dwRefData == BONUS_FLAG_FLOAT)
					{
					WCHAR	szSign[2];
					int	iStartSel;
					int	iEndSel;

					SendMessage(hWnd,EM_GETSEL,(WPARAM)&iStartSel,(LPARAM)&iEndSel);
					SendMessage(hWnd,WM_GETTEXT,(WPARAM)2,(LPARAM)szSign);
					if (szSign[0] != L'-') // String does NOT start with "-"
						{
						if (!iStartSel) break; // Selection(0-?), OK
						return(0);
						}
					if (!iStartSel && iEndSel) break; // Selection(0-y) where y > 0 ["-" will be discarded if any], OK
					}
				}

			return(0);

		case WM_NCDESTROY:
			RemoveWindowSubclass(hWnd,Game_BonusEditSubclass,uIdSubclass);
			break;
		}

	return(DefSubclassProc(hWnd,uMsgId,wParam,lParam));
}


// «»»» Navigation ««««««««««««««««««««««««««««««««««««««««««««««««««««««»

//--- Activation de la page précédente ---

LONG_PTR Game_BonusSetPrevPage(HWND hDlg, GAMEEDITPAGECONTEXT *ctx)
{
	switch(ctx->uPageID)
		{
		case GAME_PAGE_BONUS_TARGET:
			break;

		case GAME_PAGE_BONUS_LIST:
			return(Game_BonusGetPage(GAME_PAGE_BONUS_TARGET));

		case GAME_PAGE_BONUS_VALUE:
		case GAME_PAGE_BONUS_DAMAGETYPE:
		case GAME_PAGE_BONUS_REFLECTION:
		case GAME_PAGE_BONUS_SKILLS:
		case GAME_PAGE_BONUS_COLORS:
			return(Game_BonusGetPage(GAME_PAGE_BONUS_LIST));
		}

	return(-1);
}


//--- Activation de la page suivante ---

LONG_PTR Game_BonusSetNextPage(HWND hDlg, BOOL bStore, GAMEEDITPAGECONTEXT *ctx)
{
	switch(ctx->uPageID)
		{
		case GAME_PAGE_BONUS_TARGET:
			ctx->bonus.pContext->uNewListMask = 0;
			if (IsDlgButtonChecked(hDlg,200) == BST_CHECKED) ctx->bonus.pContext->uNewListMask = BONUS_FLAG_TARGET_ARMOR;
			else if (IsDlgButtonChecked(hDlg,201) == BST_CHECKED) ctx->bonus.pContext->uNewListMask = BONUS_FLAG_TARGET_WEAPON;
			else if (IsDlgButtonChecked(hDlg,202) == BST_CHECKED) ctx->bonus.pContext->uNewListMask = BONUS_FLAG_TARGET_SHIELD;
			if (ctx->bonus.pContext->uNewListMask) return(0);
			break;

		case GAME_PAGE_BONUS_LIST: {
			UINT uSelected = SendDlgItemMessage(hDlg,200,LB_GETCURSEL,0,0);
			if (uSelected == LB_ERR) break;
			ctx->bonus.pContext->pSelected = (GAMEBONUS *)SendDlgItemMessage(hDlg,200,LB_GETITEMDATA,(WPARAM)uSelected,0);
			if (ctx->bonus.pContext->pSelected == (GAMEBONUS *)-1) break;
			if (bStore) { if (!Game_BonusSetInfos(ctx->bonus.pContext->pEdit,ctx->bonus.pContext->pSelected)) break; }
			if (ctx->bonus.pContext->pSelected->uFlags&(BONUS_FLAG_INT|BONUS_FLAG_UINT|BONUS_FLAG_FLOAT)) return(Game_BonusGetPage(GAME_PAGE_BONUS_VALUE));
			if (ctx->bonus.pContext->pSelected->uFlags&(BONUS_FLAG_DAMAGETYPE)) return(Game_BonusGetPage(GAME_PAGE_BONUS_DAMAGETYPE));
			if (ctx->bonus.pContext->pSelected->uFlags&(BONUS_FLAG_REFLECTION)) return(Game_BonusGetPage(GAME_PAGE_BONUS_REFLECTION));
			if (ctx->bonus.pContext->pSelected->uFlags&(BONUS_FLAG_SKILL)) return(Game_BonusGetPage(GAME_PAGE_BONUS_SKILLS));
			if (ctx->bonus.pContext->pSelected->uFlags&(BONUS_FLAG_COLORS)) return(Game_BonusGetPage(GAME_PAGE_BONUS_COLORS));
			} break;

		case GAME_PAGE_BONUS_VALUE: {
			WCHAR	szTemp[BONUS_NUMBER_MAX_LEN+1];

			SendDlgItemMessage(hDlg,200,WM_GETTEXT,(WPARAM)BONUS_NUMBER_MAX_LEN+1,(LPARAM)szTemp);
			Game_BonusSetVal(ctx->bonus.pContext->pEdit,(ctx->bonus.pContext->pSelected->uFlags&BONUS_FLAG_FLOAT)?BONUS_FLAG_FLOAT:BONUS_FLAG_INT,szTemp);
			if (Game_BonusIsEmptyNumber(szTemp)) break;
			} return(0);

		case GAME_PAGE_BONUS_DAMAGETYPE: {
			WCHAR	szDamageType[4];

			Game_BonusDamageType(hDlg,szDamageType,GAME_DAMAGETYPE_STORE);
			Game_BonusSetVal(ctx->bonus.pContext->pEdit,BONUS_FLAG_UINT,szDamageType);
			} return(0);

		case GAME_PAGE_BONUS_REFLECTION: {
			WCHAR	szPercentage[BONUS_NUMBER_MAX_LEN+1];
			WCHAR	szDamageType[4];

			SendDlgItemMessage(hDlg,200,WM_GETTEXT,(WPARAM)BONUS_NUMBER_MAX_LEN+1,(LPARAM)szPercentage);
			Game_BonusDamageType(hDlg,szDamageType,GAME_DAMAGETYPE_STORE);
			Game_BonusSetVal(ctx->bonus.pContext->pEdit,BONUS_FLAG_REFLECTION,TRUE,szPercentage,szDamageType);
			if (Game_BonusIsEmptyNumber(szPercentage)) break;
			} return(0);

		case GAME_PAGE_BONUS_SKILLS:
			if (SendDlgItemMessage(hDlg,200,LVM_GETSELECTEDCOUNT,0,0))
				{
				LVITEM		lvItem;

				lvItem.iItem = SendDlgItemMessage(hDlg,200,LVM_GETNEXTITEM,-1,LVNI_SELECTED);
				if (lvItem.iItem != -1)
					{
					lvItem.mask = LVIF_PARAM;
					lvItem.iSubItem = 0;
					lvItem.lParam = 0;
					if (SendDlgItemMessage(hDlg,200,LVM_GETITEM,0,(LPARAM)&lvItem))
						{
						if (!lvItem.lParam) break;
						if (bStore)
							{
							WCHAR*	pszId;
							WCHAR*	pszName;
							pszId = Misc_StrCpyAlloc(((GAMESKILL *)lvItem.lParam)->pszId);
							pszName = Misc_StrCpyAlloc(((GAMESKILL *)lvItem.lParam)->pszName);
							if (!pszId)
								{
								if (pszId) HeapFree(App.hHeap,0,pszId);
								if (pszName) HeapFree(App.hHeap,0,pszName);
								SetLastError(ERROR_NOT_ENOUGH_MEMORY);
								Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
								break;
								}
							Game_BonusSetVal(ctx->bonus.pContext->pEdit,BONUS_FLAG_SKILL,pszId,pszName);
							}
						return(0);
						}
					}
				}
			break;

		case GAME_PAGE_BONUS_COLORS:
			if (SendDlgItemMessage(hDlg,200,LVM_GETSELECTEDCOUNT,0,0))
				{
				LVITEM		lvItem;

				lvItem.iItem = SendDlgItemMessage(hDlg,200,LVM_GETNEXTITEM,-1,LVNI_SELECTED);
				if (lvItem.iItem != -1)
					{
					lvItem.mask = LVIF_PARAM;
					lvItem.iSubItem = 0;
					lvItem.lParam = 0;
					if (SendDlgItemMessage(hDlg,200,LVM_GETITEM,0,(LPARAM)&lvItem))
						{
						if (!lvItem.lParam) break;
						if (bStore)
							{
							WCHAR*	pszId;
							WCHAR*	pszName;
							pszId = Misc_StrCpyAlloc(((GAMEITEMCOLORS *)lvItem.lParam)->pszId);
							pszName = Misc_StrCpyAlloc(((GAMEITEMCOLORS *)lvItem.lParam)->pszName);
							if (!pszId)
								{
								if (pszId) HeapFree(App.hHeap,0,pszId);
								if (pszName) HeapFree(App.hHeap,0,pszName);
								SetLastError(ERROR_NOT_ENOUGH_MEMORY);
								Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
								break;
								}
							Game_BonusSetVal(ctx->bonus.pContext->pEdit,BONUS_FLAG_COLORS,pszId,pszName);
							}
						return(0);
						}
					}
				}
			break;
		}

	return(-1);
}

//--- Recherche l'identifiant d'une page ---

LONG_PTR Game_BonusGetPage(UINT uPageID)
{
	UINT	i;

	for (i = 0; BonusesPages[i].uPageID != 0; i++)
		if (BonusesPages[i].uPageID == uPageID) return((LONG_PTR)BonusesPages[i].uResID);

	return(-1);
}


// «»»» Activation d'une page «««««««««««««««««««««««««««««««««««««««««««»

int Game_BonusActivate(HWND hDlg, GAMEEDITPAGECONTEXT *ctx)
{
	switch(ctx->uPageID)
		{
		case GAME_PAGE_BONUS_TARGET:
			if (!ctx->bPageSet)
				{
				UINT	uType;
				UINT64	uFlags;

				uType = Game_GetItemType(ctx->bonus.pContext->pszObject);
				uFlags = Game_GetItemFlags(ctx->bonus.pContext->pszObject);
				if (uType == DATA_TYPE_IS_WEAPON) CheckDlgButton(hDlg,201,BST_CHECKED);
				else if (uType == DATA_TYPE_IS_ARMOR && uFlags&FILTER_SHIELDS) CheckDlgButton(hDlg,202,BST_CHECKED);
				else CheckDlgButton(hDlg,200,BST_CHECKED);

				ctx->bPageSet = TRUE;
				}
			PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,0,(LPARAM)PSWIZB_NEXT);
			break;

		case GAME_PAGE_BONUS_LIST:
			if (ctx->bonus.pContext->uNewListMask != ctx->bonus.pContext->uListMask)
				{
				SendDlgItemMessage(hDlg,200,LB_RESETCONTENT,0,0);
				ctx->bPageSet = FALSE;
				ctx->bonus.pContext->uListMask = ctx->bonus.pContext->uNewListMask;
				}
			if (!ctx->bPageSet)
				{
				LRESULT	lResult;
				int	i;

				for (i = 0; Bonuses[i].pszId != NULL; i ++)
					{
					if (!(Bonuses[i].uFlags&ctx->bonus.pContext->uListMask)) continue;
					lResult = SendDlgItemMessage(hDlg,200,LB_ADDSTRING,0,(LPARAM)&Bonuses[i]);
					if (lResult == LB_ERR)
						{
						Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
						return(0);
						}
					if (Bonuses[i].pszId == (WCHAR *)-1) continue;
					if (!ctx->bonus.pContext->pEdit->pszId) continue;
					if (wcscmp(Bonuses[i].pszId,ctx->bonus.pContext->pEdit->pszId)) continue;
					SendDlgItemMessage(hDlg,200,LB_SETCURSEL,(WPARAM)lResult,0);
					}

				ctx->bPageSet = TRUE;
				}
			PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,0,(LPARAM)PSWIZB_BACK|(Game_BonusSetNextPage(hDlg,FALSE,ctx) != -1?PSWIZB_NEXT:0));
			break;

		case GAME_PAGE_BONUS_VALUE:
			if (!ctx->bPageSet)
				{
				ctx->bonus.hwndCtrl = CreateWindowEx(0,UPDOWN_CLASS,NULL,WS_CHILD|WS_VISIBLE|UDS_ALIGNRIGHT|UDS_ARROWKEYS|UDS_NOTHOUSANDS,0,0,0,0,hDlg,(HMENU)202,App.hInstance,0);
				if (!ctx->bonus.hwndCtrl)
					{
					Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
					return(0);
					}
				SendMessage(ctx->bonus.hwndCtrl,UDM_SETBUDDY,(WPARAM)GetDlgItem(hDlg,200),0);
				SendDlgItemMessage(hDlg,200,EM_LIMITTEXT,(WPARAM)BONUS_NUMBER_MAX_LEN,0);
				ctx->bPageSet = TRUE;
				}
			Game_BonusCleanNumber(ctx->bonus.pContext->pEdit->value.szVal,ctx->bonus.pContext->pSelected->uFlags&BONUS_FLAG_FLOAT,ctx->bonus.pContext->pSelected->uFlags&(BONUS_FLAG_FLOAT|BONUS_FLAG_INT));
			SetDlgItemText(hDlg,200,ctx->bonus.pContext->pEdit->value.szVal);
			PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,0,(LPARAM)PSWIZB_BACK|(Game_BonusSetNextPage(hDlg,FALSE,ctx) != -1?PSWIZB_FINISH:PSWIZB_DISABLEDFINISH));
			SetWindowSubclass(GetDlgItem(hDlg,200),Game_BonusEditSubclass,0,(ctx->bonus.pContext->pSelected->uFlags&BONUS_FLAG_FLOAT)?BONUS_FLAG_FLOAT:((ctx->bonus.pContext->pSelected->uFlags&BONUS_FLAG_UINT)?BONUS_FLAG_UINT:BONUS_FLAG_INT));
			break;

		case GAME_PAGE_BONUS_REFLECTION:
			if (!ctx->bPageSet)
				{
				ctx->bonus.hwndCtrl = CreateWindowEx(0,UPDOWN_CLASS,NULL,WS_CHILD|WS_VISIBLE|UDS_ALIGNRIGHT|UDS_ARROWKEYS|UDS_NOTHOUSANDS,0,0,0,0,hDlg,(HMENU)202,App.hInstance,0);
				if (!ctx->bonus.hwndCtrl)
					{
					Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
					return(0);
					}
				SendMessage(ctx->bonus.hwndCtrl,UDM_SETBUDDY,(WPARAM)GetDlgItem(hDlg,200),0);
				SendDlgItemMessage(hDlg,200,EM_LIMITTEXT,(WPARAM)BONUS_NUMBER_MAX_LEN,0);
				Game_BonusDamageType(hDlg,ctx->bonus.pContext->pEdit->reflection.szDamageType,GAME_DAMAGETYPE_SETUP);
				ctx->bPageSet = TRUE;
				}
			Game_BonusCleanNumber(ctx->bonus.pContext->pEdit->reflection.szPercentage,FALSE,FALSE);
			SetDlgItemText(hDlg,200,ctx->bonus.pContext->pEdit->reflection.szPercentage);
			PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,0,(LPARAM)PSWIZB_BACK|(Game_BonusSetNextPage(hDlg,FALSE,ctx) != -1?PSWIZB_FINISH:PSWIZB_DISABLEDFINISH));
			SetWindowSubclass(GetDlgItem(hDlg,200),Game_BonusEditSubclass,0,BONUS_FLAG_UINT);
			break;

		case GAME_PAGE_BONUS_DAMAGETYPE:
			if (!ctx->bPageSet)
				{
				Game_BonusDamageType(hDlg,ctx->bonus.pContext->pEdit->value.szVal,GAME_DAMAGETYPE_SETUP);
				ctx->bPageSet = TRUE;
				}
			PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,0,(LPARAM)PSWIZB_BACK|PSWIZB_FINISH);
			break;

		case GAME_PAGE_BONUS_SKILLS:
			if (!ctx->bPageSet)
				{
				CheckDlgButton(hDlg,180,App.Config.bSkillsGroups?BST_CHECKED:BST_UNCHECKED);
				SendDlgItemMessage(hDlg,180,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_VIEW_GROUPS));
				if (!Dialog_ViewComboInit(hDlg,190,App.Config.uSkillsView,2,LV_VIEW_ICON,LV_VIEW_TILE))
					{
					Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
					return(0);
					}
				if (!Game_BonusInitSkills(hDlg,200,ctx)) return(0);
				ctx->bPageSet = TRUE;
				}
			PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,0,(LPARAM)PSWIZB_BACK|(Game_BonusSetNextPage(hDlg,FALSE,ctx) != -1?PSWIZB_FINISH:PSWIZB_DISABLEDFINISH));
			break;

		case GAME_PAGE_BONUS_COLORS:
			if (!ctx->bPageSet)
				{
				ctx->bPageSet = TRUE;
				if (!Game_BonusInitColors(hDlg,200,ctx)) return(0);
				}
			PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,0,(LPARAM)PSWIZB_BACK|(Game_BonusSetNextPage(hDlg,FALSE,ctx) != -1?PSWIZB_FINISH:PSWIZB_DISABLEDFINISH));
			break;
		}

	return(1);
}

//--- Tri des couleurs ---

int Game_BonusSortColors(LPARAM lFirstColor, LPARAM lSecondColor, LPARAM _unused)
{
	WCHAR*	pszText1;
	WCHAR*	pszText2;
	int	iResult;

	pszText1 = ((GAMEITEMCOLORS *)lFirstColor)->pszName?((GAMEITEMCOLORS *)lFirstColor)->pszName:((GAMEITEMCOLORS *)lFirstColor)->pszId;
	pszText2 = ((GAMEITEMCOLORS *)lSecondColor)->pszName?((GAMEITEMCOLORS *)lSecondColor)->pszName:((GAMEITEMCOLORS *)lSecondColor)->pszId;
	iResult = CompareStringEx(LOCALE_NAME_SYSTEM_DEFAULT,LINGUISTIC_IGNORECASE|SORT_DIGITSASNUMBERS,pszText1,-1,pszText2,-1,NULL,NULL,0);
	if (iResult == CSTR_LESS_THAN) return(-1);
	if (iResult == CSTR_GREATER_THAN) return(1);
	return(0);
}


//--- Initialisation de la liste des couleurs ---

int Game_BonusInitColors(HWND hDlg, UINT uCtlID, GAMEEDITPAGECONTEXT *ctx)
{
	LVITEM		lvItem;
	HIMAGELIST	hImageList = NULL;
	HDC		hDC = NULL;
	HDC		hdcIcon = NULL;
	HBITMAP		hbitmapIcon = NULL;
	HBITMAP		hbitmapMask = NULL;
	HBITMAP		hbitmapDefault = NULL;
	ICONINFO	IconInfo;
	HPEN		hPen;
	HBRUSH		hBrush;
	HICON		hIcon;
	RECT		rcIcon;
	GAMEITEMCOLORS*	pSelected;
	int		i,j,X;

	SendDlgItemMessage(hDlg,uCtlID,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_DOUBLEBUFFER,LVS_EX_DOUBLEBUFFER);

	if (!Locale_Load(hDlg,szLangPath,App.Config.pszLocaleName,LOCALE_TYPE_MISC,(void **)&ctx->bonus.pContext->pLocale,NULL))
		goto Error;

	//--- Images
	hImageList = ImageList_Create(52,52,ILC_COLOR32,sizeof(ItemColors)/sizeof(GAMEITEMCOLORS),0);
	if (!hImageList) goto Error;

	//--- Selected item
	pSelected = NULL;
	if (ctx->bonus.pContext->pEdit->uType == BONUS_FLAG_COLORS)
		{
		if (ctx->bonus.pContext->pEdit->colors.pszId)
			{
			for (i = 0; ItemColors[i].pszId != NULL; i++)
				{
				if (!wcscmp(ctx->bonus.pContext->pEdit->colors.pszId,ItemColors[i].pszId))
					{
					pSelected = &ItemColors[i];
					break;
					}
				}
			}
		}

	//--- Icons
	rcIcon.left = 0;
	rcIcon.right = 52;
	rcIcon.top = 0;
	rcIcon.bottom = 52;
	hDC = GetDC(hDlg);
	if (hDC)
		{
		hdcIcon = CreateCompatibleDC(hDC);
		if (hdcIcon)
			{
			hbitmapIcon = CreateCompatibleBitmap(hDC,52,52);
			hbitmapMask = CreateBitmap(52,52,1,1,NULL);
			if (!hbitmapIcon || !hbitmapMask) goto Error;
			}
		else goto Error;
		}
	else goto Error;

	//--- Items
	for (i = 0; ItemColors[i].pszId != NULL; i++)
		{
		//--- Icon
		hbitmapDefault = SelectObject(hdcIcon,hbitmapIcon);
		FillRect(hdcIcon,&rcIcon,GetStockObject(BLACK_BRUSH));
		//FillRect(hdcIcon,&rcIcon,GetSysColorBrush(COLOR_WINDOW));
		//DrawIconEx(hdcIcon,0,0,App.hIcons[APP_ICON_BACKGROUND],52,52,0,NULL,DI_NORMAL);
		for (X = 4, j = 0; j != 3; j++)
			{
			COLORREF crColor = ItemColors[i].colors[j];
			crColor = ((crColor&0x000000FF)<<16) | (crColor&0x0000FF00) | ((crColor&0x00FF0000)>>16);
			hPen = CreatePen(PS_SOLID,1,crColor);
			if (hPen)
				{
				hBrush = CreateSolidBrush(crColor);
				if (hBrush)
					{
					HPEN hpenDef = SelectObject(hdcIcon,hPen);
					HBRUSH hbrushDef = SelectObject(hdcIcon,hBrush);
					RoundRect(hdcIcon,X,4,X+12,48,4,4);
					SelectObject(hdcIcon,hbrushDef);
					SelectObject(hdcIcon,hpenDef);
					DeleteObject(hBrush);
					}
				DeleteObject(hPen);
				}
			X += 16;
			}
		//DrawIconEx(hdcIcon,2,2,App.hIcons[APP_ICON_FRAME],48,48,0,NULL,DI_NORMAL);
		SelectObject(hdcIcon,hbitmapDefault);
		IconInfo.fIcon = TRUE;
		IconInfo.xHotspot = 0;
		IconInfo.yHotspot = 0;
		IconInfo.hbmMask = hbitmapMask;
		IconInfo.hbmColor = hbitmapIcon;
		hIcon = CreateIconIndirect(&IconInfo);
		if (!hIcon) goto Error;
		//--- Item
		if (!ItemColors[i].pszName) Locale_QueryID(ctx->bonus.pContext->pLocale->db,szDataBaseColors,ItemColors[i].pszId,NULL,&ItemColors[i].pszName,1);
		lvItem.mask = LVIF_IMAGE|LVIF_PARAM|LVIF_TEXT;
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.pszText = ItemColors[i].pszName?ItemColors[i].pszName:ItemColors[i].pszId;
		lvItem.iImage = ImageList_ReplaceIcon(hImageList,-1,hIcon);
		lvItem.lParam = (LPARAM)&ItemColors[i];
		if (pSelected == &ItemColors[i])
			{
			lvItem.mask |= LVIF_STATE;
			lvItem.state = LVIS_SELECTED;
			lvItem.stateMask = LVIS_SELECTED;
			}
		DestroyIcon(hIcon);
		if (lvItem.iImage == -1 || SendDlgItemMessage(hDlg,uCtlID,LVM_INSERTITEM,0,(LPARAM)&lvItem) == -1) goto Error;
		}

	DeleteObject(hbitmapIcon);
	DeleteObject(hbitmapMask);
	DeleteDC(hdcIcon);
	ReleaseDC(hDlg,hDC);

	SendDlgItemMessage(hDlg,uCtlID,LVM_SETIMAGELIST,(WPARAM)LVSIL_NORMAL,(LPARAM)hImageList);
	SendDlgItemMessage(hDlg,uCtlID,LVM_SORTITEMS,(WPARAM)0,(LPARAM)Game_BonusSortColors);
	Locale_Unload(LOCALE_TYPE_MISC,(void **)&ctx->bonus.pContext->pLocale,NULL);
	return(1);

	//--- Error ---

Error:	Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);

	if (hbitmapIcon) DeleteObject(hbitmapIcon);
	if (hbitmapMask) DeleteObject(hbitmapMask);
	if (hdcIcon) DeleteDC(hdcIcon);
	if (hDC) ReleaseDC(hDlg,hDC);

	SendDlgItemMessage(hDlg,uCtlID,LVM_DELETEALLITEMS,0,0);
	if (hImageList) ImageList_Destroy(hImageList);
	Locale_Unload(LOCALE_TYPE_MISC,(void **)&ctx->bonus.pContext->pLocale,NULL);
	return(0);
}


//--- Initialisation de la liste des skills ---

int Game_BonusInitSkills(HWND hDlg, UINT uCtlID, GAMEEDITPAGECONTEXT *ctx)
{
	LVCOLUMN	lvColumn;
	LVGROUP		lvGroup;
	LVITEM		lvItem;
	LVTILEINFO	lvTileInfo;
	LVTILEVIEWINFO	lvTileView;
	HIMAGELIST	hImageList = NULL;
	HINSTANCE	hSkillsIcons = NULL;
	HICON		hIcon;
	GAMESKILL*	pSelected;
	WCHAR		szName[16];
	UINT		uColumns[2];
	static UINT	uSkillsColumns[] = { TEXT_BONUS_SKILL_COLUMN_NAME, TEXT_BONUS_SKILL_COLUMN_SCHOOL, TEXT_BONUS_SKILL_COLUMN_POINTS, 0 };
	static UINT	uGroupsTitles[] = { TEXT_ABILITIES_WARFARE, TEXT_ABILITIES_AIR, TEXT_ABILITIES_HUNTING, TEXT_ABILITIES_ROGUES, TEXT_ABILITIES_EARTH, TEXT_ABILITIES_WATER, TEXT_ABILITIES_SUMMONING, TEXT_ABILITIES_POLYMORPH, TEXT_ABILITIES_DEATH, TEXT_ABILITIES_FIRE, TEXT_ABILITIES_SOURCE, TEXT_ABILITIES_SPECIAL, 0 };
	int		i,j,k;

	SendDlgItemMessage(hDlg,uCtlID,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT|LVS_EX_DOUBLEBUFFER,LVS_EX_FULLROWSELECT|LVS_EX_DOUBLEBUFFER);

	if (!Locale_Load(hDlg,szLangPath,App.Config.pszLocaleName,LOCALE_TYPE_MISC,(void **)&ctx->bonus.pContext->pLocale,NULL))
		goto Error;

	//--- Images
	hImageList = ImageList_Create(64,64,ILC_COLOR32|ILC_MASK,sizeof(Skills)/sizeof(GAMESKILL),0);
	if (!hImageList) goto Error;

	//--- Icons
	hSkillsIcons = LoadLibraryEx(szSkillsIconsPath,NULL,DONT_RESOLVE_DLL_REFERENCES|LOAD_LIBRARY_AS_DATAFILE|LOAD_WITH_ALTERED_SEARCH_PATH);
	if (!hSkillsIcons) goto Error;

	//--- Columns
	for (i = 0; uSkillsColumns[i] != 0; i++)
		{
		lvColumn.mask = LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.cx = 0;
		lvColumn.pszText = Locale_GetText(uSkillsColumns[i]);
		lvColumn.cchTextMax = wcslen(lvColumn.pszText);
		lvColumn.iOrder = i;
		if (SendDlgItemMessage(hDlg,uCtlID,LVM_INSERTCOLUMN,i,(LPARAM)&lvColumn) == -1) goto Error;
		}

	//--- Selected item
	pSelected = NULL;
	if (ctx->bonus.pContext->pEdit->uType == BONUS_FLAG_SKILL)
		{
		if (ctx->bonus.pContext->pEdit->skill.pszId)
			{
			for (i = 0; Skills[i].pszId != NULL; i++)
				{
				if (!wcscmp(ctx->bonus.pContext->pEdit->skill.pszId,Skills[i].pszId))
					{
					pSelected = &Skills[i];
					break;
					}
				}
			}
		}

	//--- Groups
	for (i = 0; uGroupsTitles[i] != 0; i++)
		{
		lvGroup.cbSize = sizeof(LVGROUP);
		lvGroup.mask = LVGF_HEADER|LVGF_GROUPID|LVGF_STATE;
		lvGroup.pszHeader = Locale_GetText(uGroupsTitles[i]);
		lvGroup.iGroupId = uGroupsTitles[i];
		lvGroup.stateMask = lvGroup.state = LVGS_COLLAPSIBLE|LVGS_COLLAPSED;
		if (pSelected)
			{
			if (pSelected->uSchoolLocaleID == uGroupsTitles[i]) lvGroup.state &= ~LVGS_COLLAPSED;
			}
		if (SendDlgItemMessage(hDlg,uCtlID,LVM_INSERTGROUP,(WPARAM)-1,(LPARAM)&lvGroup) == -1) goto Error;
		}

	//--- Items
	for (i = 0; Skills[i].pszId != NULL; i++)
		{
		//--- Icon
		wsprintf(szName,L"SI_%.4u",Skills[i].uIconID);
		hIcon = LoadImage(hSkillsIcons,szName,IMAGE_ICON,64,64,LR_DEFAULTCOLOR|LR_SHARED);
		if (!hIcon) goto Error;
		//--- Item
		if (!Skills[i].pszName) Locale_QueryID(ctx->bonus.pContext->pLocale->db,szDataBaseSkills,Skills[i].pszId,NULL,&Skills[i].pszName,1);
		lvItem.mask = LVIF_IMAGE|LVIF_PARAM|LVIF_TEXT|LVIF_GROUPID;
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.pszText = Skills[i].pszName?Skills[i].pszName:Skills[i].pszId;
		lvItem.iImage = ImageList_ReplaceIcon(hImageList,-1,hIcon);
		lvItem.iGroupId = Skills[i].uSchoolLocaleID;
		lvItem.lParam = (LPARAM)&Skills[i];
		if (pSelected == &Skills[i])
			{
			lvItem.mask |= LVIF_STATE;
			lvItem.state = LVIS_SELECTED;
			lvItem.stateMask = LVIS_SELECTED;
			}
		if (lvItem.iImage == -1 || SendDlgItemMessage(hDlg,uCtlID,LVM_INSERTITEM,0,(LPARAM)&lvItem) == -1) goto Error;
		//--- School
		lvItem.iSubItem = 1;
		lvItem.pszText = Locale_GetText(Skills[i].uSchoolLocaleID);
		if (SendDlgItemMessage(hDlg,uCtlID,LVM_SETITEMTEXT,(WPARAM)lvItem.iItem,(LPARAM)&lvItem) == 0) goto Error;
		//--- Points
		for (j = 0; j != Skills[i].uSourcePoints; j++)
			{
			if (j > 3) break;
			szName[j] = sSourcePoint;
			}
		if (j) szName[j++] = L' ';
		for (k = 0; k != Skills[i].uActionPoints; k++)
			{
			if (k > 10) break;
			szName[j++] = sActionPoint;
			}
		szName[j] = 0;
		lvItem.iSubItem = 2;
		lvItem.pszText = szName;
		if (SendDlgItemMessage(hDlg,uCtlID,LVM_SETITEMTEXT,(WPARAM)lvItem.iItem,(LPARAM)&lvItem) == 0) goto Error;
		//--- Tile info
		uColumns[0] = 1;
		uColumns[1] = 2;
		lvTileInfo.cbSize = sizeof(LVTILEINFO);
		lvTileInfo.iItem = lvItem.iItem;
		lvTileInfo.cColumns = 2;
		lvTileInfo.puColumns = uColumns;
		lvTileInfo.piColFmt = NULL;
		if (SendDlgItemMessage(hDlg,uCtlID,LVM_SETTILEINFO,0,(LPARAM)&lvTileInfo) == FALSE) goto Error;
		}
	FreeLibrary(hSkillsIcons);

	lvTileView.cbSize = sizeof(LVTILEVIEWINFO);
	lvTileView.dwMask = LVTVIM_COLUMNS;
	lvTileView.dwFlags = LVTVIF_AUTOSIZE;
	lvTileView.cLines = 3;
	SendDlgItemMessage(hDlg,uCtlID,LVM_SETTILEVIEWINFO,0,(LPARAM)&lvTileView);

	ctx->bonus.uSort = DATA_SORT_SKILL_SCHOOL;
	SendDlgItemMessage(hDlg,uCtlID,LVM_SETIMAGELIST,(WPARAM)LVSIL_NORMAL,(LPARAM)hImageList);
	SendDlgItemMessage(hDlg,uCtlID,LVM_SETICONSPACING,0,(LPARAM)MAKELONG(64+40,64+App.Font.uFontHeight*2+20));
	SendDlgItemMessage(hDlg,uCtlID,LVM_SETVIEW,(WPARAM)App.Config.uSkillsView,(LPARAM)0);
	SendDlgItemMessage(hDlg,uCtlID,LVM_ENABLEGROUPVIEW,(WPARAM)App.Config.bSkillsGroups,0);
	SendDlgItemMessage(hDlg,uCtlID,LVM_SORTITEMS,(WPARAM)ctx->bonus.uSort,(LPARAM)Game_BonusSortSkills);
	Locale_Unload(LOCALE_TYPE_MISC,(void **)&ctx->bonus.pContext->pLocale,NULL);
	return(1);

	//--- Error ---

Error:	Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
	SendDlgItemMessage(hDlg,uCtlID,LVM_DELETEALLITEMS,0,0);
	if (hSkillsIcons) FreeLibrary(hSkillsIcons);
	if (hImageList) ImageList_Destroy(hImageList);
	Locale_Unload(LOCALE_TYPE_MISC,(void **)&ctx->bonus.pContext->pLocale,NULL);
	return(0);
}


// «»»» Tri des compétences «««««««««««««««««««««««««««««««««««««««««««««»

int Game_BonusSortSkills(LPARAM lFirstSkill, LPARAM lSecondSkill, LPARAM lSort)
{
	int	iResult = 0;

	switch(lSort)
		{
		case DATA_SORT_ID:
		case DATA_SORT_TEXT:
			iResult = Game_BonusSortSkillsCmp((GAMESKILL *)lFirstSkill,(GAMESKILL *)lSecondSkill,(UINT)DATA_SORT_TEXT);
			break;
		case DATA_SORT_SKILL_SCHOOL:
			iResult = Game_BonusSortSkillsCmp((GAMESKILL *)lFirstSkill,(GAMESKILL *)lSecondSkill,(UINT)lSort);
			if (iResult == 0) iResult = Game_BonusSortSkillsCmp((GAMESKILL *)lFirstSkill,(GAMESKILL *)lSecondSkill,(UINT)DATA_SORT_TEXT);
			break;
		case DATA_SORT_SKILL_POINTS:
			iResult = Game_BonusSortSkillsCmp((GAMESKILL *)lFirstSkill,(GAMESKILL *)lSecondSkill,(UINT)lSort);
			if (iResult == 0) iResult = Game_BonusSortSkillsCmp((GAMESKILL *)lFirstSkill,(GAMESKILL *)lSecondSkill,(UINT)DATA_SORT_TEXT);
			break;
		}

	return(iResult);
}

//--- Routine de comparaison ---

int Game_BonusSortSkillsCmp(GAMESKILL *pFirstSkill, GAMESKILL *pSecondSkill, UINT uSort)
{
	WCHAR*	pszText1;
	WCHAR*	pszText2;
	int	iResult = 0;

	switch(uSort)
		{
		case DATA_SORT_TEXT:
			pszText1 = pFirstSkill->pszName?pFirstSkill->pszName:pFirstSkill->pszId;
			pszText2 = pSecondSkill->pszName?pSecondSkill->pszName:pSecondSkill->pszId;
			iResult = CompareStringEx(LOCALE_NAME_SYSTEM_DEFAULT,LINGUISTIC_IGNORECASE|SORT_DIGITSASNUMBERS,pszText1,-1,pszText2,-1,NULL,NULL,0);
			if (iResult == CSTR_LESS_THAN) return(-1);
			if (iResult == CSTR_GREATER_THAN) return(1);
			break;
		case DATA_SORT_SKILL_SCHOOL:
			pszText1 = Locale_GetText(pFirstSkill->uSchoolLocaleID);
			pszText2 = Locale_GetText(pSecondSkill->uSchoolLocaleID);
			iResult = CompareStringEx(LOCALE_NAME_SYSTEM_DEFAULT,LINGUISTIC_IGNORECASE|SORT_DIGITSASNUMBERS,pszText1,-1,pszText2,-1,NULL,NULL,0);
			if (iResult == CSTR_LESS_THAN) return(-1);
			if (iResult == CSTR_GREATER_THAN) return(1);
			break;
		case DATA_SORT_SKILL_POINTS:
			if (pFirstSkill->uSourcePoints < pSecondSkill->uSourcePoints) return(-1);
			if (pFirstSkill->uSourcePoints > pSecondSkill->uSourcePoints) return(1);
			if (pFirstSkill->uActionPoints < pSecondSkill->uActionPoints) return(-1);
			if (pFirstSkill->uActionPoints > pSecondSkill->uActionPoints) return(1);
			break;
		}

	return(0);
}


// «»»» Affichage des bonus «««««««««««««««««««««««««««««««««««««««««««««»

void Game_BonusDraw(DRAWITEMSTRUCT *pDraw)
{
	GAMEBONUS*	pBonus;
	HPEN		hPen;
	HPEN		hDefPen;
	HFONT		hFont;
	WCHAR*		pszText;
	RECT		rcText;
	COLORREF	crText;
	int		iBack;

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush((pDraw->itemState&ODS_SELECTED)?COLOR_HIGHLIGHT:COLOR_WINDOW));
	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&pDraw->rcItem);

	pBonus = (GAMEBONUS *)pDraw->itemData;
	if (!pBonus) return;
	if (pBonus == (GAMEBONUS *)-1) return;

	hFont = SelectObject(pDraw->hDC,App.Font.hFont);
	crText = SetTextColor(pDraw->hDC,GetSysColor((pDraw->itemState&ODS_SELECTED)?COLOR_HIGHLIGHTTEXT:COLOR_WINDOWTEXT));
	iBack = SetBkMode(pDraw->hDC,TRANSPARENT);
	hPen = NULL;
	pszText = Locale_GetText(pBonus->uLocaleID);

	CopyRect(&rcText,&pDraw->rcItem);
	rcText.left += 4;
	rcText.right -= 4;
	if (pBonus->uFlags&BONUS_FLAG_TITLE)
		{
		hPen = CreatePen(PS_SOLID,1,GetSysColor((pDraw->itemState&ODS_SELECTED)?COLOR_HIGHLIGHTTEXT:COLOR_WINDOWTEXT));
		if (hPen)
			{
			hDefPen = SelectObject(pDraw->hDC,hPen);
			MoveToEx(pDraw->hDC,rcText.left,rcText.top+(rcText.bottom-rcText.top)/2,NULL);
			LineTo(pDraw->hDC,rcText.left+10,rcText.top+(rcText.bottom-rcText.top)/2);
			rcText.left += 14;
			}
		}
	DrawText(pDraw->hDC,pszText,-1,&rcText,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
	if (hPen)
		{
		SIZE	Size;

		GetTextExtentPoint32(pDraw->hDC,pszText,wcslen(pszText),&Size);
		rcText.left += 4+Size.cx;
		MoveToEx(pDraw->hDC,rcText.left,rcText.top+(rcText.bottom-rcText.top)/2,NULL);
		LineTo(pDraw->hDC,rcText.right,rcText.top+(rcText.bottom-rcText.top)/2);
		SelectObject(pDraw->hDC,hDefPen);
		DeleteObject(hPen);
		}

	SetBkMode(pDraw->hDC,iBack);
	SetTextColor(pDraw->hDC,crText);
	SelectObject(pDraw->hDC,hFont);
	return;
}


// «»»» Affichage d'un séparateur «««««««««««««««««««««««««««««««««««««««»

void Game_BonusDrawSeparator(WCHAR *pszText, DRAWITEMSTRUCT *pDraw)
{
	HFONT		hFont;
	RECT		rcDraw;
	COLORREF	crText;
	int		iBack;

	hFont = SelectObject(pDraw->hDC,App.Font.hFont);
	crText = SetTextColor(pDraw->hDC,GetSysColor(COLOR_BTNTEXT));
	iBack = SetBkMode(pDraw->hDC,TRANSPARENT);

	CopyRect(&rcDraw,&pDraw->rcItem);
	rcDraw.top = (pDraw->rcItem.bottom-pDraw->rcItem.top-2)/2;
	rcDraw.bottom = rcDraw.top+2;
	DrawEdge(pDraw->hDC,&rcDraw,BDR_SUNKENOUTER,BF_TOP|BF_BOTTOM);

	if (pszText)
		{
		SIZE	Size;

		GetTextExtentPoint32(pDraw->hDC,pszText,wcslen(pszText),&Size);
		CopyRect(&rcDraw,&pDraw->rcItem);
		rcDraw.left += 10;
		rcDraw.right = rcDraw.left+Size.cx+8;
		FillRect(pDraw->hDC,&rcDraw,GetSysColorBrush(COLOR_BTNFACE));
		CopyRect(&rcDraw,&pDraw->rcItem);
		rcDraw.left += 14;
		rcDraw.right -= 14;
		DrawState(pDraw->hDC,NULL,NULL,(LPARAM)pszText,0,rcDraw.left,rcDraw.top+(rcDraw.bottom-rcDraw.top-Size.cy)/2,rcDraw.right-rcDraw.left,rcDraw.bottom-rcDraw.top,DST_TEXT|DSS_DISABLED);
		}

	SetBkMode(pDraw->hDC,iBack);
	SetTextColor(pDraw->hDC,crText);
	SelectObject(pDraw->hDC,hFont);
	return;
}


// «»»» Aperçu du résultat ««««««««««««««««««««««««««««««««««««««««««««««»

void Game_BonusDrawResult(GAMEEDITPAGECONTEXT *ctx, DRAWITEMSTRUCT *pDraw)
{
	HFONT		hFont;
	RECT		rcDraw;
	WCHAR*		pszText;
	COLORREF	crText;
	int		iBack;

	hFont = SelectObject(pDraw->hDC,App.Font.hFont);
	crText = SetTextColor(pDraw->hDC,GetSysColor(COLOR_WINDOWTEXT));
	iBack = SetBkMode(pDraw->hDC,TRANSPARENT);

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(COLOR_WINDOW));
	DrawEdge(pDraw->hDC,&pDraw->rcItem,BDR_SUNKENOUTER,BF_RECT);

	pszText = Game_BonusGetDisplayText(ctx->bonus.pContext->pEdit);
	if (pszText)
		{
		CopyRect(&rcDraw,&pDraw->rcItem);
		rcDraw.left += 8;
		rcDraw.top += 2;
		rcDraw.right -= 8;
		rcDraw.bottom -= 2;
		DrawText(pDraw->hDC,pszText,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		LocalFree(pszText);
		}

	SetBkMode(pDraw->hDC,iBack);
	SetTextColor(pDraw->hDC,crText);
	SelectObject(pDraw->hDC,hFont);
}


// «»»» Modifie la valeur des nombres «««««««««««««««««««««««««««««««««««»

void Game_BonusEditMove(HWND hDlg, UINT uCtrlId, int iIncrease, GAMEEDITPAGECONTEXT *ctx)
{
	WCHAR	szNumber[BONUS_NUMBER_MAX_LEN+1];

	switch(ctx->uPageID)
		{
		case GAME_PAGE_BONUS_VALUE:
			switch(uCtrlId)
				{
				case 202:
					SendDlgItemMessage(hDlg,200,WM_GETTEXT,(WPARAM)BONUS_NUMBER_MAX_LEN+1,(LPARAM)szNumber);
					Game_BonusIncreaseNumber(szNumber,iIncrease,ctx->bonus.pContext->pSelected->uFlags&BONUS_FLAG_FLOAT,ctx->bonus.pContext->pSelected->uFlags&(BONUS_FLAG_FLOAT|BONUS_FLAG_INT));
					SendDlgItemMessage(hDlg,200,WM_SETTEXT,0,(LPARAM)szNumber);
					SendDlgItemMessage(hDlg,200,EM_SETSEL,(WPARAM)0,(LPARAM)-1);
					break;
				}
			break;
		case GAME_PAGE_BONUS_REFLECTION:
			switch(uCtrlId)
				{
				case 202:
					SendDlgItemMessage(hDlg,200,WM_GETTEXT,(WPARAM)BONUS_NUMBER_MAX_LEN+1,(LPARAM)szNumber);
					Game_BonusIncreaseNumber(szNumber,iIncrease,FALSE,FALSE);
					SendDlgItemMessage(hDlg,200,WM_SETTEXT,0,(LPARAM)szNumber);
					SendDlgItemMessage(hDlg,200,EM_SETSEL,(WPARAM)0,(LPARAM)-1);
					break;
				}
			break;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions de gestion						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Texte d'affichage d'un bonus ««««««««««««««««««««««««««««««««««««»

WCHAR* Game_BonusGetDisplayText(GAMEEDITBONUS *pEditBonus)
{
	WCHAR		szNumber[BONUS_NUMBER_MAX_LEN+1];
	WCHAR*		pszText;
	WCHAR*		pszFmt;
	DWORD_PTR	vl[3];

	pszFmt = NULL;
	vl[0] = (DWORD_PTR)(pEditBonus->uLocaleID?Locale_GetText(pEditBonus->uLocaleID):pEditBonus->pszId);

	if (pEditBonus->uFlags&BONUS_FLAG_DAMAGETYPE)
		{
		pszFmt = szBonusDamageTypeFmt;
		vl[1] = (DWORD_PTR)Game_BonusDamageType(NULL,pEditBonus->value.szVal,GAME_DAMAGETYPE_DISPLAY);
		}
	else if (pEditBonus->uFlags&BONUS_FLAG_REFLECTION)
		{
		if (pEditBonus->reflection.bIsSet)
			{
			pszFmt = Locale_GetText(TEXT_REFLECTION_ACTIVATED);
			vl[1] = (DWORD_PTR)pEditBonus->reflection.szPercentage;
			vl[2] = (DWORD_PTR)Game_BonusDamageType(NULL,pEditBonus->reflection.szDamageType,GAME_DAMAGETYPE_DISPLAY);
			}
		else
			{
			pszFmt = Locale_GetText(TEXT_REFLECTION_DEACTIVATED);
			}
		}
	else if (pEditBonus->uFlags&BONUS_FLAG_SKILL)
		{
		pszFmt = szBonusSkillFmt;
		vl[1] = (DWORD_PTR)(pEditBonus->skill.pszName?pEditBonus->skill.pszName:pEditBonus->skill.pszId);
		}
	else if (pEditBonus->uFlags&BONUS_FLAG_COLORS)
		{
		pszFmt = szBonusColorsFmt;
		vl[1] = (DWORD_PTR)(pEditBonus->colors.pszName?pEditBonus->colors.pszName:pEditBonus->colors.pszId);
		}
	else if (pEditBonus->uFlags&BONUS_FLAG_PERCENT)
		{
		wcscpy(szNumber,pEditBonus->value.szVal);
		if (pEditBonus->uFlags&BONUS_FLAG_SPECIAL1) Game_BonusShiftNumber(szNumber,FALSE);
		else if (pEditBonus->uFlags&BONUS_FLAG_SPECIAL2) Game_BonusShiftNumber(szNumber,TRUE);
		Game_BonusCleanFloat(szNumber);
		pszFmt = szBonusPercentFmt;
		vl[1] = (DWORD_PTR)szNumber;
		vl[2] = (DWORD_PTR)Game_BonusGetPositiveSign(szNumber);
		}
	else if (pEditBonus->uFlags&BONUS_FLAG_DEGREES)
		{
		wcscpy(szNumber,pEditBonus->value.szVal);
		if (pEditBonus->uFlags&BONUS_FLAG_SPECIAL1) Game_BonusShiftNumber(szNumber,FALSE);
		else if (pEditBonus->uFlags&BONUS_FLAG_SPECIAL2) Game_BonusShiftNumber(szNumber,TRUE);
		Game_BonusCleanFloat(szNumber);
		pszFmt = szBonusDegreesFmt;
		vl[1] = (DWORD_PTR)szNumber;
		vl[2] = (DWORD_PTR)Game_BonusGetPositiveSign(szNumber);
		}
	else if (pEditBonus->uFlags&BONUS_FLAG_METERS)
		{
		wcscpy(szNumber,pEditBonus->value.szVal);
		if (pEditBonus->uFlags&BONUS_FLAG_SPECIAL1) Game_BonusShiftNumber(szNumber,FALSE);
		else if (pEditBonus->uFlags&BONUS_FLAG_SPECIAL2) Game_BonusShiftNumber(szNumber,TRUE);
		Game_BonusCleanFloat(szNumber);
		pszFmt = szBonusMetersFmt;
		vl[1] = (DWORD_PTR)szNumber;
		vl[2] = (DWORD_PTR)Game_BonusGetPositiveSign(szNumber);
		}
	else
		{
		wcscpy(szNumber,pEditBonus->value.szVal);
		if (pEditBonus->uFlags&BONUS_FLAG_SPECIAL1) Game_BonusShiftNumber(szNumber,FALSE);
		else if (pEditBonus->uFlags&BONUS_FLAG_SPECIAL2) Game_BonusShiftNumber(szNumber,TRUE);
		Game_BonusCleanFloat(szNumber);
		pszFmt = szBonusFmt;
		vl[1] = (DWORD_PTR)szNumber;
		vl[2] = (DWORD_PTR)Game_BonusGetPositiveSign(szNumber);
		}

	if (!pszFmt) return(NULL);

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,pszFmt,0,0,(WCHAR *)&pszText,1,(va_list *)vl))
		return(pszText);

	return(NULL);
}


// «»»» Détermine le type de dégâts «««««««««««««««««««««««««««««««««««««»

WCHAR* Game_BonusDamageType(HWND hDlg, WCHAR *pszDamageType, UINT uAction)
{
	UINT	uDamageType;

	switch(uAction)
		{
		case GAME_DAMAGETYPE_SETUP:
			SendDlgItemMessage(hDlg,211,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_DAMAGE_PHYSICAL));
			SendDlgItemMessage(hDlg,212,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_DAMAGE_PIERCING));
			SendDlgItemMessage(hDlg,216,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_DAMAGE_FIRE));
			SendDlgItemMessage(hDlg,217,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_DAMAGE_AIR));
			SendDlgItemMessage(hDlg,218,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_DAMAGE_WATER));
			SendDlgItemMessage(hDlg,219,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_DAMAGE_EARTH));
			SendDlgItemMessage(hDlg,220,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_DAMAGE_POISON));
			uDamageType = wcstol(pszDamageType,NULL,10);
			if (!((uDamageType >= 1 && uDamageType <= 2) || (uDamageType >= 6 && uDamageType <= 10))) uDamageType = 1;
			CheckDlgButton(hDlg,uDamageType+210,BST_CHECKED);
			break;
		case GAME_DAMAGETYPE_STORE:
			pszDamageType[0] = 0;
			for (uDamageType = 1; uDamageType != 11; uDamageType++)
				{
				if (uDamageType > 2 && uDamageType < 6) continue;
				if (IsDlgButtonChecked(hDlg,uDamageType+210) != BST_CHECKED) continue;
				wsprintf(pszDamageType,L"%.1u",uDamageType);
				break;
				}
			if (!wcslen(pszDamageType))
				{
				pszDamageType[0] = L'1';
				pszDamageType[1] = 0;
				}
			break;
		case GAME_DAMAGETYPE_DISPLAY:
			uDamageType = TEXT_DAMAGE_PHYSICAL;
			if (pszDamageType)
				{
				switch(wcstol(pszDamageType,NULL,10))
					{
					case 1:	uDamageType = TEXT_DAMAGE_PHYSICAL;
						break;
					case 2:	uDamageType = TEXT_DAMAGE_PIERCING;
						break;
					case 6:	uDamageType = TEXT_DAMAGE_FIRE;
						break;
					case 7:	uDamageType = TEXT_DAMAGE_AIR;
						break;
					case 8:	uDamageType = TEXT_DAMAGE_WATER;
						break;
					case 9:	uDamageType = TEXT_DAMAGE_EARTH;
						break;
					case 10:uDamageType = TEXT_DAMAGE_POISON;
						break;
					}
				}
			return(Locale_GetText(uDamageType));
			break;
		}

	return(NULL);
}


// «»»» Retrouve un bonus par son identifiant «««««««««««««««««««««««««««»

GAMEBONUS* Game_BonusGetFromID(WCHAR *pszId)
{
	int	i;

	if (!pszId) return(NULL);

	for (i = 0; Bonuses[i].pszId != 0; i++)
		{
		if (Bonuses[i].pszId == (WCHAR *)-1) continue;
		if (wcscmp(Bonuses[i].pszId,pszId)) continue;
		return(&Bonuses[i]);
		}

	return(NULL);
}


// «»»» Modifie les informations d'un bonux ««««««««««««««««««««««««««««»

int Game_BonusSetInfos(GAMEEDITBONUS *pEditBonus, GAMEBONUS *pBonus)
{
	if (pBonus->pszId == (WCHAR *)-1) return(1);

	if (pEditBonus->pszId) HeapFree(App.hHeap,0,pEditBonus->pszId);

	pEditBonus->pszId = Misc_StrCpyAlloc(pBonus->pszId);
	pEditBonus->uLocaleID = pBonus->uLocaleID;
	pEditBonus->uFlags = pBonus->uFlags;

	return(pEditBonus->pszId?1:0);
}


// «»»» Modifie une valeur ««««««««««««««««««««««««««««««««««««««««««««««»

//!\ Pointers are copied

int Game_BonusSetVal(GAMEEDITBONUS *pEditBonus, UINT uType, ...)
{
	WCHAR*		pszValue;
	va_list		vl;

	va_start(vl,uType);

	pEditBonus->uType = uType;
	switch(uType)
		{
		case BONUS_FLAG_INT:
			pEditBonus->value.szVal[0] = 0;
			pszValue = va_arg(vl,WCHAR *);
			if (pszValue) wcsncpy(pEditBonus->value.szVal,pszValue,BONUS_NUMBER_MAX_LEN);
			Game_BonusCleanNumber(pEditBonus->value.szVal,FALSE,TRUE);
			break;
		case BONUS_FLAG_UINT:
			pEditBonus->value.szVal[0] = 0;
			pszValue = va_arg(vl,WCHAR *);
			if (pszValue) wcsncpy(pEditBonus->value.szVal,pszValue,BONUS_NUMBER_MAX_LEN);
			Game_BonusCleanNumber(pEditBonus->value.szVal,FALSE,FALSE);
			break;
		case BONUS_FLAG_FLOAT:
			pEditBonus->value.szVal[0] = 0;
			pszValue = va_arg(vl,WCHAR *);
			if (pszValue) wcsncpy(pEditBonus->value.szVal,pszValue,BONUS_NUMBER_MAX_LEN);
			Game_BonusCleanNumber(pEditBonus->value.szVal,TRUE,TRUE);
			break;
		case BONUS_FLAG_REFLECTION:
			pEditBonus->reflection.bIsSet = va_arg(vl,BOOL);
			pEditBonus->reflection.szPercentage[0] = 0;
			pszValue = va_arg(vl,WCHAR *);
			if (pszValue) wcsncpy(pEditBonus->reflection.szPercentage,pszValue,BONUS_NUMBER_MAX_LEN);
			pEditBonus->reflection.szDamageType[0] = 0;
			pszValue = va_arg(vl,WCHAR *);
			if (pszValue) wcsncpy(pEditBonus->reflection.szDamageType,pszValue,BONUS_NUMBER_MAX_LEN);
			Game_BonusCleanNumber(pEditBonus->reflection.szPercentage,FALSE,FALSE);
			Game_BonusCleanNumber(pEditBonus->reflection.szDamageType,FALSE,FALSE);
			break;
		case BONUS_FLAG_SKILL:
			if (pEditBonus->skill.pszId) HeapFree(App.hHeap,0,pEditBonus->skill.pszId);
			if (pEditBonus->skill.pszName) HeapFree(App.hHeap,0,pEditBonus->skill.pszName);
			pEditBonus->skill.pszId = va_arg(vl,WCHAR*);
			pEditBonus->skill.pszName = va_arg(vl,WCHAR*);
			break;
		case BONUS_FLAG_COLORS:
			if (pEditBonus->colors.pszId) HeapFree(App.hHeap,0,pEditBonus->colors.pszId);
			if (pEditBonus->colors.pszName) HeapFree(App.hHeap,0,pEditBonus->colors.pszName);
			pEditBonus->colors.pszId = va_arg(vl,WCHAR*);
			pEditBonus->colors.pszName = va_arg(vl,WCHAR*);
			break;
		}

	va_end(vl);
	return(1);
}


// «»»» Copie d'un bonus ««««««««««««««««««««««««««««««««««««««««««««««««»

int Game_BonusCopy(GAMEEDITBONUS *pEditBonusDest, GAMEEDITBONUS *pEditBonusSrc)
{
	if (!pEditBonusSrc) return(1);

	Game_BonusRelease(pEditBonusDest,TRUE);

	pEditBonusDest->uType = 0;
	pEditBonusDest->uFlags = pEditBonusSrc->uFlags;

	pEditBonusDest->pszId = Misc_StrCpyAlloc(pEditBonusSrc->pszId);
	if (!pEditBonusDest->pszId) return(0);
	pEditBonusDest->uLocaleID = pEditBonusSrc->uLocaleID;

	pEditBonusDest->uType = pEditBonusSrc->uType;
	switch(pEditBonusSrc->uType)
		{
		case BONUS_FLAG_INT:
		case BONUS_FLAG_UINT:
		case BONUS_FLAG_FLOAT:
			wcsncpy(pEditBonusDest->value.szVal,pEditBonusSrc->value.szVal,BONUS_NUMBER_MAX_LEN);
			break;
		case BONUS_FLAG_REFLECTION:
			pEditBonusDest->reflection.bIsSet = pEditBonusSrc->reflection.bIsSet;
			wcsncpy(pEditBonusDest->reflection.szPercentage,pEditBonusSrc->reflection.szPercentage,BONUS_NUMBER_MAX_LEN);
			wcsncpy(pEditBonusDest->reflection.szDamageType,pEditBonusSrc->reflection.szDamageType,BONUS_NUMBER_MAX_LEN);
			break;
		case BONUS_FLAG_SKILL:
			pEditBonusDest->skill.pszId = Misc_StrCpyAlloc(pEditBonusSrc->skill.pszId);
			pEditBonusDest->skill.pszName = Misc_StrCpyAlloc(pEditBonusSrc->skill.pszName);
			if (!pEditBonusDest->skill.pszId) return(0);
			break;
		case BONUS_FLAG_COLORS:
			pEditBonusDest->colors.pszId = Misc_StrCpyAlloc(pEditBonusSrc->colors.pszId);
			pEditBonusDest->colors.pszName = Misc_StrCpyAlloc(pEditBonusSrc->colors.pszName);
			if (!pEditBonusDest->colors.pszId) return(0);
			break;
		}

	return(1);
}


// «»»» Libération des bonus ««««««««««««««««««««««««««««««««««««««««««««»

//--- Libération de tous les bonus ---

void Game_BonusReleaseAll(NODE *pBonusList)
{
	GAMEEDITBONUS*	pEditBonus;

	for (pEditBonus = (GAMEEDITBONUS *)pBonusList->next; pEditBonus != NULL; pEditBonus = (GAMEEDITBONUS *)pEditBonus->node.next)
		Game_BonusRelease(pEditBonus,TRUE);

	List_ReleaseMemory(pBonusList);
	return;
}

//--- Libération d'un bonus ---

void Game_BonusRelease(GAMEEDITBONUS *pEditBonus, BOOL bCleanUpOnly)
{
	if (!pEditBonus) return;

	if (pEditBonus->pszId) HeapFree(App.hHeap,0,pEditBonus->pszId);

	switch(pEditBonus->uType)
		{
		case BONUS_FLAG_INT:
		case BONUS_FLAG_UINT:
		case BONUS_FLAG_FLOAT:
			if (bCleanUpOnly) ZeroMemory(pEditBonus->value.szVal,BONUS_NUMBER_MAX_LEN);
			break;
		case BONUS_FLAG_REFLECTION:
			if (bCleanUpOnly)
				{
				ZeroMemory(pEditBonus->reflection.szPercentage,BONUS_NUMBER_MAX_LEN);
				ZeroMemory(pEditBonus->reflection.szDamageType,BONUS_NUMBER_MAX_LEN);
				}
			break;
		case BONUS_FLAG_SKILL:
			if (pEditBonus->skill.pszId) HeapFree(App.hHeap,0,pEditBonus->skill.pszId);
			if (pEditBonus->skill.pszName) HeapFree(App.hHeap,0,pEditBonus->skill.pszName);
			if (bCleanUpOnly)
				{
				pEditBonus->skill.pszId = NULL;
				pEditBonus->skill.pszName = NULL;
				}
			break;
		case BONUS_FLAG_COLORS:
			if (pEditBonus->colors.pszId) HeapFree(App.hHeap,0,pEditBonus->colors.pszId);
			if (pEditBonus->colors.pszName) HeapFree(App.hHeap,0,pEditBonus->colors.pszName);
			if (bCleanUpOnly)
				{
				pEditBonus->colors.pszId = NULL;
				pEditBonus->colors.pszName = NULL;
				}
			break;
		}

	if (bCleanUpOnly)
		{
		pEditBonus->pszId = NULL;
		pEditBonus->uLocaleID = 0;
		pEditBonus->uType = 0;
		pEditBonus->uFlags = 0;
		}

	if (!bCleanUpOnly) HeapFree(App.hHeap,0,pEditBonus);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Manipulation des chaînes						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Incrémente un nombre ««««««««««««««««««««««««««««««««««««««««««««»

void Game_BonusIncreaseNumber(WCHAR *pszNumber, int iIncrease, BOOL bFloat, BOOL bSigned)
{
	WCHAR	szNumber[BONUS_NUMBER_MAX_LEN+1];

	if (bFloat)
		{
		double fNum = wcstod(pszNumber,NULL);
		fNum += (double)iIncrease/100.0f;
		if (fNum > (double)BONUS_NUMBER_MAX) fNum = (double)BONUS_NUMBER_MAX;
		if (fNum < (double)BONUS_NUMBER_MIN) fNum = (double)BONUS_NUMBER_MIN;
		swprintf(szNumber,BONUS_NUMBER_MAX_LEN,L"%.2f",fNum);
		}
	else
		{
		long lNum = wcstol(pszNumber,NULL,10);
		lNum += iIncrease;
		if (lNum > BONUS_NUMBER_MAX) lNum = BONUS_NUMBER_MAX;
		if (lNum < BONUS_NUMBER_MIN) lNum = BONUS_NUMBER_MIN;
		if (!bSigned && lNum < 0) lNum = 0;
		swprintf(szNumber,BONUS_NUMBER_MAX_LEN,L"%d",lNum);
		}

	Game_BonusCleanNumber(szNumber,bFloat,bSigned);
	wcscpy(pszNumber,szNumber);
	return;
}


// «»»» Détermine si un texte est un nombre positif «««««««««««««««««««««»

WCHAR* Game_BonusGetPositiveSign(WCHAR *pszText)
{
	if (!pszText) return(szPlus);
	if (!*pszText) return(szPlus);
	if (*pszText == L'-') return(szEmpty);
	return(szPlus);
}


// «»»» Détermine si un texte est vide ««««««««««««««««««««««««««««««««««»

BOOL Game_BonusIsEmptyNumber(WCHAR *pszText)
{
	int	i;

	if (!pszText) return(TRUE);
	if (wcslen(pszText) == 0) return(TRUE);

	for (i = 0; pszText[i] != 0; i++)
		{
		if (pszText[i] == L'-') continue;
		if (pszText[i] == L'.') continue;
		if (pszText[i] != L'0') return(FALSE);
		}

	return(TRUE);
}


// «»»» Multiplie ou divise un nombre par 100 «««««««««««««««««««««««««««»

void Game_BonusShiftNumber(WCHAR *pszNumber, BOOL bLeft)
{
	double	fNum;

	fNum = wcstod(pszNumber,NULL);
	if (bLeft) fNum /= 100.0f;
	else fNum *= 100.0f;
	swprintf(pszNumber,BONUS_NUMBER_MAX_LEN,L"%.2f",fNum);
	Game_BonusCleanNumber(pszNumber,TRUE,TRUE);
	return;
}


// «»»» Nettoie un nombre «««««««««««««««««««««««««««««««««««««««««««««««»

void Game_BonusCleanNumber(WCHAR *pszNumber, BOOL bFloat, BOOL bSigned)
{
	WCHAR	szResult[BONUS_NUMBER_MAX_LEN+1];
	int	i,j,e,p;

	// Main cleanup loop
	for (i = 0, j = 0, e = 1, p = 0; pszNumber[i] != 0; i++)
		{
		if (j >= BONUS_NUMBER_MAX_LEN)
			{
			#if _DEBUG
			MessageBox(App.hWnd,L"Buffer overflow in Game_BonusCleanNumber(), that was not supposed to happen !",L"Debug",MB_ICONEXCLAMATION|MB_OK);
			#endif
			break;
			}
		if (bSigned && pszNumber[i] == L'-' && j)		// "-" only at the beginning
			continue;
		if (bSigned && pszNumber[i] == L'-' && !j)
			{
			szResult[j++] = pszNumber[i];
			continue;
			}
		if (bFloat && pszNumber[i] == L'.' && e)		// Insert "0" if ".number"
			{
			szResult[j++] = L'0';
			szResult[j++] = pszNumber[i];
			e = 0;
			p = 1;
			continue;
			}
		if (bFloat && pszNumber[i] == L'.' && !p)		// Only one "."
			{
			szResult[j++] = pszNumber[i];
			p = 1;
			continue;
			}
		if (pszNumber[i] < L'0') continue;
		if (pszNumber[i] > L'9') continue;
		szResult[j++] = pszNumber[i];
		e = 0;
		}
	szResult[j] = L'\0';

	// Set to 0 if empty
	i = wcslen(szResult);
	if (!i) wcscpy(szResult,bFloat?L"0.0":L"0");
	else if (bFloat && szResult[i-1] == '.') wcscat(szResult,L"0");

	// Remove leading 0's
	for (i = szResult[0] == L'-'?1:0; szResult[i] != 0; i++)
		{
		if (szResult[i] == L'0')
			{
			if (szResult[i+1] == 0) break;
			if (szResult[i+1] == L'.') break;
			continue;
			}
		break;
		}
	if (szResult[0] == L'-') szResult[--i] = L'-';

	// Remove trailing 0's
	if (bFloat && wcslen(&szResult[i]) > 1)
		{
		WCHAR*	szPoint;
		WCHAR*	szPtr;

		szPoint = wcschr(&szResult[i],L'.');
		if (szPoint)
			{
			szPtr = &szResult[i+wcslen(&szResult[i])-1];
			while (szPtr != szPoint)
				{
				if (*szPtr != L'0') break;
				if (szPtr-1 == szPoint) break;
				*szPtr-- = 0;
				}
			}
		}

	wcscpy(pszNumber,&szResult[i]);
	return;
}


// «»»» Supprime ".0" à la fin d'un nombre à virgule (pour affichage) «««»

void Game_BonusCleanFloat(WCHAR *pszNumber)
{
	WCHAR*	pszEnd;

	pszEnd = wcsstr(pszNumber,L".0");
	if (!pszEnd) return;
	if (wcslen(pszEnd) != 2) return;
	*pszEnd = 0;
	return;
}
