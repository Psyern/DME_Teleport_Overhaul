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
