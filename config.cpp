class CfgPatches
{
	class DME_Teleport_Overhaul
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_scripts",
			"DZ_Sounds_Effects",
			"JM_CF_Scripts"
		};
	};
};
class CfgMods
{
	class DME_Teleport_Overhaul
	{
		dir="DME_Teleport_Overhaul";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="DME_Teleport_Overhaul";
		tooltip="tooltip";
		overview="DME_Teleport_Overhaul";
		credits="Psyern";
		author="Psyern";
		authorID="0";
		version=1;
		extra=0;
		type="mod";
		dependencies[]=
		{
			"Game",
			"World",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"DME_Teleport_Overhaul/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"DME_Teleport_Overhaul/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"DME_Teleport_Overhaul/scripts/5_Mission"
				};
			};
		};
	};
};
class CfgSoundShaders
{
	class DME_TeleportPortal_SoundShader
	{
		samples[] = {{"DME_Teleport_Overhaul\sounds\teleport", 1}};
		volume = 1.0;
		range = 60;
		rangeCurve[] = {{0, 1}, {20, 1}, {60, 0}};
	};
	class DME_TeleportPortalOut_SoundShader
	{
		samples[] = {{"DME_Teleport_Overhaul\sounds\teleport_out", 1}};
		volume = 1.0;
		range = 60;
		rangeCurve[] = {{0, 1}, {20, 1}, {60, 0}};
	};
};
class CfgSoundSets
{
	class DME_TeleportPortal_SoundSet
	{
		soundShaders[] = {"DME_TeleportPortal_SoundShader"};
		volumeFactor = 1.0;
		frequencyFactor = 1.0;
		spatial = 1;
		loop = 0;
		sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		distanceFilter = "defaultDistanceFilter";
		doppler = 0;
	};
	class DME_TeleportPortalOut_SoundSet
	{
		soundShaders[] = {"DME_TeleportPortalOut_SoundShader"};
		volumeFactor = 1.0;
		frequencyFactor = 1.0;
		spatial = 1;
		loop = 0;
		sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		distanceFilter = "defaultDistanceFilter";
		doppler = 0;
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class DME_Teleport_OverhaulPoint: Inventory_Base
	{
		scope=1;
		storageCategory=10;
		model="\DME_Teleport_Overhaul\Data\ActionPoint.p3d";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
		};
		hiddenSelections[]=
		{
			"camo"
		};
		hiddenSelectionsTextures[]=
		{
			"\DME_Teleport_Overhaul\Data\ProxySkin.paa"
		};
	};
};
