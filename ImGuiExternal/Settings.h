#pragma once
#include "include.h"

namespace CSettings {
	int MenuWindow = 0;

	struct Settings {
		//Gun
		bool triggerbot = false;
		int triggerbot_delayms = 1;
		int TriggerHotKey = 0;
		int THotKey = VK_XBUTTON2;
		std::vector<int> THotKeyList{ VK_XBUTTON2, VK_LMENU, VK_RBUTTON, VK_XBUTTON1, VK_LBUTTON, VK_CAPITAL, VK_LSHIFT, VK_LCONTROL };// added new button VK_LBUTTON

		inline void TSetHotKey(int Index)
		{
			THotKey = THotKeyList.at(Index);
		}

		bool Aimbot = false;
		float AimFov = 200;
		float Smooth = 0.7;
		int RCSBullet = 1;
		Vector2 RCSScale = { 0.7f,0.58f };
		bool Head_Target = true;
		bool Body_Target = false;
		bool ShowFov = false;
		int FovThickness = 0;
		ImColor FovColor = { 255,255,255 };
		bool ShowTarget = false;
		int TargetThickness = 0;
		ImColor TargetColor = { 255,0,0 };
		bool FilledCircle = false;
		ImColor FilledCircleColor = { 0,0,0,80 };
		int AimBotHotKey = 0;
		int HotKey = VK_LBUTTON;
		std::vector<int> HotKeyList{ VK_LBUTTON, VK_LMENU, VK_RBUTTON, VK_XBUTTON1, VK_XBUTTON2, VK_CAPITAL, VK_LSHIFT, VK_LCONTROL };// added new button VK_LBUTTON

		inline void SetHotKey(int Index)
		{
			HotKey = HotKeyList.at(Index);
		}

		bool Recoil_Control = false;
		Vector2 RRCSScale = { 0.695f,0.66f };

		//visuals
		bool DrawCrosshair = false;
		bool whennotaiming = false;
		ImColor Crosshair_Color = { 255,255,255 };
		float Crosshair_size = 10;
		int Crosshair_thickness = 0;
		bool circle = true;
		bool Cross = true;

		bool radar_hack = false;
		float scale = 0.08f;
		Vector2 center = { 100,100 };
		float radarRadius = 100.0f;

		bool ShowEyeRay = false;
		float length = 25;
		ImColor EyeRay_Color = { 255,0,0 };
		int EyeRay_Thickness = 0;

		//ESP
		bool Show_Enemy = true;
		bool Show_Squad = true;

		int HealthBar_Esp_Thickness = 2;
		bool HealthBar_ESP = false;

		int ArmorBar_Esp_Thickness = 2;
		bool ArmorBar_ESP = false;

		ImColor Enemy_Box_Esp_Color = { 255,0,0 };
		ImColor Squad_Box_Esp_Color = { 0,0,255 };
		int Box_Esp_Thickness = 0;
		bool Box_ESP = false;

		ImColor Enemy_CornerBox_Esp_Color = { 255,0,0 };
		ImColor Squad_CornerBox_Esp_Color = { 0,0,255 };
		int Box_CornerEsp_Thickness = 0;
		bool CornerBox_ESP = false;

		ImColor Enemy_Name_ESP_Color = { 255,255,255 };
		ImColor Squad_Name_ESP_Color = { 255,255,255 };
		bool Name_ESP = false;

		ImColor Enemy_Distance_Esp_Color = { 255,0,0 };
		ImColor Squad_Distance_Esp_Color = { 0,0,255 };
		bool Distance_Esp = false;

		ImColor Enemy_Bone_Esp_Color = { 255,0,0 };
		ImColor Squad_Bone_Esp_Color = { 0,0,255 };
		int Bone_Esp_Thickness = 0;
		bool Bone_Esp = false;

		Vector2 SnaplLine_Esp_Start_Point = { X_Screen / 2,Y_Screen };
		bool SnaplLine_Esp_End_Point = false;
		ImColor Enemy_SnaplLine_Esp_Color = { 255,0,0 };
		ImColor Squad_SnaplLine_Esp_Color = { 0,0,255 };
		int SnaplLine_Esp_Thickness = 0;
		bool SnaplLine_Esp = false;

		ImColor Enemy_FilledBox_Esp_Color = { 0,0,0,70 };
		ImColor Squad_FilledBox_Esp_Color = { 0,0,0,70 };
		bool FilledBox_Esp = false;

		ImColor Enemy_GunName_Color = { 255,0,0 };
		ImColor Squad_GunName_Color = { 0,0,255 };
		bool GunName_Esp = false;

		ImColor Enemy_Box3D_Esp_Color = { 255,0,0 };
		ImColor Squad_Box3D_Esp_Color = { 0,0,255 };
		int Box3D_Esp_Thickness = 0;
		bool Box3D_Esp = false;
		float Box3D_Width = 20;

		int ESP_Distance = 50;
		float Text_Size = 0.9f;

		bool Night_Mode = false;
		bool FullBright_Mode = false;

		//Player
		bool BunnyHop = false;
		bool MoneyServices = false;

		//settings
		bool window_animation = true;
		bool navigationwindow_animation = false;
		bool optionswindow_animation = false;
		bool show_watermark = true;
		bool OBSBypass = false;
		bool Show_Fps = false;

		bool Backround_Animation = true;
	};

	inline Settings settings;
};

#define USettings CSettings::settings