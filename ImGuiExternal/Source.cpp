#include "include.h"
bool createConsole = true;
bool isInitialized = false;
bool isMenuVisible = true;
char distance[50];
char names[16];
char name[16];

/*checar el otro source y agregar cosas
visibility check
bomb hack
speed hack
recoil control
sking changer*/

IDirect3DTexture9* pTexture = nullptr;

int FindClosestEnemy() {
	int ClosestEntity = 100;
	DWORD64 LocalPlayer = E->GetLocal();
	if (LocalPlayer != NULL) {
		if (E->GetHealth(E->GetLocal()) > 0 && E->GetHealth(E->GetLocal()) < 101) {
			float Finish;
			float Closest = FLT_MAX;
			for (int i = 0; i < 64; i++) {
				DWORD64 EntityAddr = E->GetEnt(i);
				if (EntityAddr != 0) {
					DWORD64 sceneNode; read<DWORD64>(EntityAddr, C_BaseEntity::m_pGameSceneNode, sceneNode);
					if (sceneNode == NULL)
						continue;
					DWORD64 BoneArray; read<DWORD64>(sceneNode, CSkeletonInstance::m_modelState + 0x80, BoneArray);
					if (BoneArray == NULL)
						continue;
					float EnHealth = E->GetHealth(EntityAddr); 
					if (EnHealth == 0 || EnHealth > 100)
						continue;
					float Distance = E->GetBonePos3D(BoneArray, 28).dist(E->GetPos(E->GetLocal())) / 100; 
					if (Distance > USettings::ESP_Distance)
						continue;
					if (E->GetTeam(EntityAddr) == E->GetTeam(E->GetLocal()))
						continue;
					Vector3 aimpos;
					if (USettings::Head_Target)
						aimpos = E->GetBonePos3D(BoneArray, bones::head);
					if (USettings::Body_Target)
						aimpos = E->GetBonePos3D(BoneArray, bones::spine);
					aimpos.z -= 1.f;
					Vector2 posscreen = PosToScreen(aimpos);
					if (posscreen.dist({ X_Screen / 2,Y_Screen / 2 }) > USettings::AimFov)
						continue;

					Finish = posscreen.dist({ X_Screen / 2, Y_Screen / 2 });
					if (Finish < Closest) {
						Closest = Finish;
						ClosestEntity = i;
					}
				}
			}
		}
	}
	return ClosestEntity;
}

void Colors() {
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(11, 12);
	style.FrameRounding = 4.0f;
	style.ItemSpacing = { 14.0f,4.0f };
	style.ScrollbarSize = 11.0f;
	style.WindowBorderSize = 0.0f;
	style.WindowRounding = 12.0f;
	style.ChildRounding = 12.0f;
	style.ScrollbarRounding = 12.0f;
	style.WindowTitleAlign = { 0.50f,0.50f };
	style.WindowMinSize = ImVec2(701, 444);

	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

}

void animatecontent(animator& animator) {
	const int limit = 255;
	if (animator.tick || animator.alpha == limit) {
		animator.tick = true;
		if (!(animator.alpha <= 0))
			animator.alpha -= animator.speed;
		else if (animator.alpha <= 0)
			animator.tick ^= 1;
	}
	else if (!animator.tick || animator.alpha != limit) {
		animator.tick = false;
		if (!(animator.alpha >= limit))
			animator.alpha += animator.speed;
		else if (animator.alpha >= limit)
			animator.tick ^= 1;
	}
}

template<class T>
void centertext(const char* text, T value1, T value2) {
	char texto[100];
	sprintf_s(texto, sizeof(texto), text, value1, value2);
	ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(texto).x) / 2);
	ImGui::Text(texto, value1, value2);
}

struct WindowInfo {
	int Width;
	int Height;
	int Left;
	int Right;
	int Top;
	int Bottom;
};

WindowInfo* windowInfo;
WNDCLASSEX windowClass;
HWND targetWindow;
HWND overlayWindow;

std::string targetProcessName = "cs2.exe";
std::string ovarlayName = generateRandomString(generateRandomInt(30, 100));

IDirect3DDevice9Ex* pDevice = nullptr;
IDirect3D9Ex* pDirect = nullptr;
D3DPRESENT_PARAMETERS gD3DPresentParams = { NULL };

void drawItem() {
	char fpsInfo[64];
	RGBA textColor = { 255,255,255,255 };
	snprintf(fpsInfo, sizeof(fpsInfo), "Overlay FPS: %.0f", ImGui::GetIO().Framerate);
	drawStrokeText(30, 44, &textColor, fpsInfo);
}

void renderImGui() {
	if (!isInitialized) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		D3DXCreateTextureFromFileInMemory(pDevice, NOVA_LOGO, sizeof(NOVA_LOGO), &pTexture);

		ImGui_ImplWin32_Init(overlayWindow);
		ImGui_ImplDX9_Init(pDevice);
		ImGui_ImplDX9_CreateDeviceObjects();
		isInitialized = true;
	}

	if (GetAsyncKeyState(VK_INSERT) & 1) {
		isMenuVisible = !isMenuVisible;
		ImGui::GetIO().MouseDrawCursor = isMenuVisible;
	}
	Colors();
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	SetWindowLong(overlayWindow, GWL_EXSTYLE, isMenuVisible ? WS_EX_TOOLWINDOW : (WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW));
	UpdateWindow(overlayWindow);
	if (isMenuVisible) {
		inputHandler();
		drawItem();
		static animator Animator = { 255,false,0.5f };
		animatecontent(Animator);
		ImGui::Begin("CS Menu", 0, ImGuiWindowFlags_NoCollapse);
		ImVec2 windowpos{ ImGui::GetWindowPos() };
		ImVec2 windowsize{ ImGui::GetWindowSize() };
		ImGui::BeginChild("##top", ImVec2(ImGui::GetContentRegionAvail().x, 46), true);
		ImVec2 child_windowpos{ ImGui::GetWindowPos() };
		ImVec2 child_windowsize{ ImGui::GetWindowSize() };
		auto windowWidth = ImGui::GetWindowSize().x;
		auto buttonWidth = (ImGui::GetContentRegionAvail().x - 30) / 4;
		auto spacing = (windowWidth - (buttonWidth * 4)) / 5;
		ImGui::SetCursorPosX(spacing);
		if (ImGui::Button("Gun", ImVec2(buttonWidth, 0))) {
			USettings::MenuWindow = 1;
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(spacing * 2 + buttonWidth);
		if (ImGui::Button("Legit", ImVec2(buttonWidth, 0))) {
			USettings::MenuWindow = 2;
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(spacing * 3 + buttonWidth * 2);
		if (ImGui::Button("Visuals", ImVec2(buttonWidth, 0))) {
			USettings::MenuWindow = 3;
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(spacing * 4 + buttonWidth * 3);
		if (ImGui::Button("Config", ImVec2(buttonWidth, 0))) {
			USettings::MenuWindow = 4;
		}
		ImGui::EndChild();
		ImGui::BeginChild("##Bottom", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
		ImVec2 child_windowpos1{ ImGui::GetWindowPos() };
		ImVec2 child_windowsize1{ ImGui::GetWindowSize() };
		if (USettings::MenuWindow == 0) {
			ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (200 / 2));
			ImGui::Image((void*)pTexture, ImVec2(200, 170));
			centertext<float>("CSGO 2 Menu by Nova", 0.0f, 0.0f);
			centertext<float>("Window Size X:%0.f Y:%0.f", windowsize.x, windowsize.y);
		}
		else if (USettings::MenuWindow == 1) {
			ImGui::Checkbox("trigger Bot", &USettings::triggerbot);
			if (USettings::triggerbot) {
				if (ImGui::Combo("Trigger Key", &USettings::TriggerHotKey, "XBUTTON2\0MENU\0RBUTTON\0XBUTTON1\0LBUTTON\0CAPITAL\0SHIFT\0CONTROL")) {
					USettings::TSetHotKey(USettings::TriggerHotKey);
				}
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::SliderInt("Triggerbot Delay", &USettings::triggerbot_delayms, 0, 200);
				ImGui::Separator();
			}
			ImGui::Checkbox("AimBot", &USettings::Aimbot);
			if (USettings::Aimbot) {
				if (ImGui::Combo("Aim Key", &USettings::AimBotHotKey, "LBUTTON\0MENU\0RBUTTON\0XBUTTON1\0XBUTTON2\0CAPITAL\0SHIFT\0CONTROL")) {
					USettings::SetHotKey(USettings::AimBotHotKey);
				}
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Text("Settings");
				ImGui::SliderFloat("AimBot Smooth", &USettings::Smooth, 0, 0.7f);
				ImGui::SliderFloat("AimBot Fov", &USettings::AimFov, 10, 1920);
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Text("Target");
				ImGui::Checkbox("Head Target", &USettings::Head_Target);
				if (USettings::Head_Target)
					USettings::Body_Target = false;
				ImGui::Checkbox("Body Target", &USettings::Body_Target);
				if (USettings::Body_Target)
					USettings::Head_Target = false;
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Text("Aimbot Fov");
				ImGui::Checkbox("Show Fov", &USettings::ShowFov);
				if (USettings::ShowFov) {
					ImGui::SliderInt("Fov Thickness", &USettings::FovThickness, 0, 10);
					ImGui::ColorEdit3("Fov Color", (float*)&USettings::FovColor);
					ImGui::Checkbox("Filled Circle", &USettings::FilledCircle);
					ImGui::ColorEdit3("Filled Circle Color", (float*)&USettings::FilledCircleColor);
					ImGui::Spacing();
					ImGui::Spacing();
				}
				ImGui::Checkbox("Show Target", &USettings::ShowTarget);
				if (USettings::ShowTarget) {
					ImGui::SliderInt("Target Thickness", &USettings::TargetThickness, 0, 10);
					ImGui::ColorEdit3("Target Color", (float*)&USettings::TargetColor);
				}
				ImGui::Separator();
			}
		}
		else if (USettings::MenuWindow == 2) {
			ImGui::Checkbox("Buny Hop", &USettings::BunnyHop);
			ImGui::Checkbox("No Flash", &USettings::No_Flash);
			ImGui::Checkbox("Money Manager", &USettings::MoneyServices);
			if (USettings::MoneyServices) {
				for (int i = 0; i < 64; i++) {
					int health = E->GetHealth(E->GetEnt(i));
					if (health > 0 && health < 101) {
						DWORD64 CurrentController = E->GetEntInfo(i);
						if (CurrentController != NULL) {
							DWORD64 moneyservices; read<DWORD64>(CurrentController, CCSPlayerController::m_pInGameMoneyServices, moneyservices);
							if (moneyservices != NULL) {
								if (ProcessMgr.ReadMemory(CurrentController + CBasePlayerController::m_iszPlayerName, name, 16)) {
									int account;
									if (read<int>(moneyservices, CCSPlayerController_InGameMoneyServices::m_iAccount, account)) {
										int cashSpent;
										if (read<int>(moneyservices, CCSPlayerController_InGameMoneyServices::m_iCashSpentThisRound, cashSpent)) {
											int cashSpentTotal;
											if (read<int>(moneyservices, CCSPlayerController_InGameMoneyServices::m_iTotalCashSpent, cashSpentTotal)) {
												if (ImGui::TreeNode(name)) {
													ImGui::TextColored(ImColor(255, 255, 255), "Account: %d", account);
													ImGui::TextColored(ImColor(255, 255, 255), "Cash Spent this round: %d", cashSpent);
													ImGui::TextColored(ImColor(255, 255, 255), "Cash Spent total: %d", cashSpentTotal);
													ImGui::TreePop();
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else if (USettings::MenuWindow == 3) {
			ImGui::Checkbox("Fov Changer", &USettings::fov_changer);
			if (USettings::fov_changer) {
				ImGui::SliderInt("Fov value", &USettings::fov_value, 0, 170);
				if (ImGui::Button("Default")) {
					USettings::fov_value = USettings::oldfov_value;
				}
				ImGui::Separator();
			}
			ImGui::Checkbox("Radar Hack", &USettings::radar_hack);
			ImGui::Checkbox("Snap Line Esp", &USettings::SnaplLine_Esp);
			if (USettings::SnaplLine_Esp) {
				ImGui::ColorEdit3("Enemy Snap Line Esp Color", (float*)&USettings::Enemy_SnaplLine_Esp_Color);
				ImGui::ColorEdit3("Squad Snap Line Esp Color", (float*)&USettings::Squad_SnaplLine_Esp_Color);
				ImGui::SliderInt("Snap Line Esp Thickness", &USettings::SnaplLine_Esp_Thickness, 0, 10);
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Text("Start Point:  X:%f  Y:%f", USettings::SnaplLine_Esp_Start_Point.x, USettings::SnaplLine_Esp_Start_Point.y);
				if (ImGui::Button("Bottom")) {
					USettings::SnaplLine_Esp_Start_Point = { X_Screen / 2, Y_Screen };
				}
				if (ImGui::Button("Top")) {
					USettings::SnaplLine_Esp_Start_Point = { X_Screen / 2, 0 };
				}
				if (ImGui::Button("Left Side")) {
					USettings::SnaplLine_Esp_Start_Point = { 0, Y_Screen / 2 };
				}
				if (ImGui::Button("Right Side")) {
					USettings::SnaplLine_Esp_Start_Point = { X_Screen, Y_Screen / 2 };
				}
				ImGui::SliderFloat("Custom X", &USettings::SnaplLine_Esp_Start_Point.x, 0, X_Screen);
				ImGui::SliderFloat("Custom Y", &USettings::SnaplLine_Esp_Start_Point.y, 0, Y_Screen);
				ImGui::Spacing();
				ImGui::Spacing();
				if (USettings::SnaplLine_Esp_End_Point)
					ImGui::Text("End Point:  Head");
				else
					ImGui::Text("End Point:  Feet");
				if (ImGui::Button("Head")) {
					USettings::SnaplLine_Esp_End_Point = true;
				}
				if (ImGui::Button("Feet")) {
					USettings::SnaplLine_Esp_End_Point = false;
				}
				ImGui::Separator();
			}
			ImGui::Checkbox("Bone Esp", &USettings::Bone_Esp);
			if (USettings::Bone_Esp) {
				ImGui::ColorEdit3("Bone Enemy Color", (float*)(&USettings::Enemy_Bone_Esp_Color));
				ImGui::ColorEdit3("Bone Squad Color", (float*)(&USettings::Squad_Bone_Esp_Color));
				ImGui::SliderInt("Bone thickness", &USettings::Bone_Esp_Thickness, 0, 10);
				ImGui::Separator();
			}
			ImGui::Checkbox("Show Distance", &USettings::Distance_Esp);
			if (USettings::Distance_Esp) {
				ImGui::ColorEdit3("Enemy Distance Color", (float*)&USettings::Enemy_Distance_Esp_Color);
				ImGui::ColorEdit3("Squad Distance Color", (float*)&USettings::Squad_Distance_Esp_Color);
				ImGui::Separator();
			}
			ImGui::Checkbox("Show Health Bar", &USettings::HealthBar_ESP);
			if (USettings::HealthBar_ESP) {
				ImGui::SliderInt("Health Bar Thickness", &USettings::HealthBar_Esp_Thickness, 0, 10);
				ImGui::Separator();
			}
			ImGui::Checkbox("Show Armor Bar", &USettings::ArmorBar_ESP);
			if (USettings::ArmorBar_ESP) {
				ImGui::SliderInt("Armor Bar Thickness", &USettings::ArmorBar_Esp_Thickness, 0, 10);
				ImGui::Separator();
			}
			ImGui::Checkbox("Box Esp", &USettings::Box_ESP);
			if (USettings::Box_ESP) {
				ImGui::ColorEdit3("Enemy Box Esp Color", (float*)&USettings::Enemy_Box_Esp_Color);
				ImGui::ColorEdit3("Squad Box Esp Color", (float*)&USettings::Squad_Box_Esp_Color);
				ImGui::SliderInt("Box Esp Thickness", &USettings::Box_Esp_Thickness, 0, 10);
				ImGui::Separator();
			}
			ImGui::Checkbox("Corner Box Esp", &USettings::CornerBox_ESP);
			if (USettings::CornerBox_ESP) {
				ImGui::ColorEdit3("Enemy Corner Box Esp Color", (float*)&USettings::Enemy_CornerBox_Esp_Color);
				ImGui::ColorEdit3("Squad Corner Box Esp Color", (float*)&USettings::Squad_CornerBox_Esp_Color);
				ImGui::SliderInt("Box Corner Esp Thickness", &USettings::Box_CornerEsp_Thickness, 0, 10);
				ImGui::Separator();
			}
			ImGui::Checkbox("Filled Box Esp", &USettings::FilledBox_Esp);
			if (USettings::FilledBox_Esp) {
				ImGui::ColorEdit3("Enemy Filled Box Esp Color", (float*)&USettings::Enemy_FilledBox_Esp_Color);
				ImGui::ColorEdit3("Squad Filled Box Esp Color", (float*)&USettings::Squad_FilledBox_Esp_Color);
				ImGui::Separator();
			}
			ImGui::Checkbox("3D Box Esp", &USettings::Box3D_Esp);
			if (USettings::Box3D_Esp) {
				ImGui::ColorEdit3("Enemy 3D Box Esp Color", (float*)&USettings::Enemy_Box3D_Esp_Color);
				ImGui::ColorEdit3("Squad 3D Box Esp Color", (float*)&USettings::Squad_Box3D_Esp_Color);
				ImGui::SliderInt("3D Box Esp Thickness", &USettings::Box3D_Esp_Thickness, 0, 10);
				ImGui::SliderFloat("3D Box Esp Width", &USettings::Box3D_Width, 10, 40);
				ImGui::Separator();
			}
			ImGui::Checkbox("Name Esp", &USettings::Name_ESP);
			if (USettings::Name_ESP) {
				ImGui::ColorEdit3("Enemy Name Color", (float*)&USettings::Enemy_Name_ESP_Color);
				ImGui::ColorEdit3("Squad Name Color", (float*)&USettings::Squad_Name_ESP_Color);
				ImGui::Separator();
			}
			ImGui::Checkbox("Gun Name Esp", &USettings::GunName_Esp);
			if (USettings::GunName_Esp) {
				ImGui::ColorEdit3("Enemy Gun Name Color", (float*)&USettings::Enemy_GunName_Color);
				ImGui::ColorEdit3("Squad Gun Name Color", (float*)&USettings::Squad_GunName_Color);
				ImGui::Separator();
			}
			ImGui::Checkbox("Night Mode", &USettings::Night_Mode);
			ImGui::Checkbox("Full Bright", &USettings::FullBright_Mode);
			ImGui::Checkbox("Draw Crosshair", &USettings::DrawCrosshair);
			if (USettings::DrawCrosshair) {
				ImGui::Checkbox("When not Aiming", &USettings::whennotaiming);
				ImGui::ColorEdit3("Crosshair Color", (float*)(&USettings::Crosshair_Color));
				ImGui::SliderFloat("Crosshair size", &USettings::Crosshair_size, 1, 30);
				ImGui::SliderInt("Crosshair Thickness", &USettings::Crosshair_thickness, 0, 10);
				ImGui::Checkbox("Circle", &USettings::circle);
				ImGui::Checkbox("Cross", &USettings::Cross);
				ImGui::Separator();
			}
			ImGui::SliderInt("ESP Distance", &USettings::ESP_Distance, 0, 100);
		}
		else if (USettings::MenuWindow == 4) {
			ImGui::Text("Menu Animation Options");
			ImGui::Checkbox("Window Animation", &USettings::window_animation);
			ImGui::Checkbox("Navigation Window Animation", &USettings::navigationwindow_animation);
			ImGui::Checkbox("Options Window Animation", &USettings::optionswindow_animation);
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Text("Settings Options");
			if (ImGui::Button("Save Current Settings")) {
				F->SaveSettings();
			}
			if (ImGui::Button("Load Settings")) {
				if (std::filesystem::exists("sarilla.bin"))
					F->ReadSettings();
			}
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Text("ESP Options");
			ImGui::Checkbox("Show Team", &USettings::Show_Squad);
			ImGui::Checkbox("Show Enemy", &USettings::Show_Enemy);
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Text("Feature Options");
			if (ImGui::Button("Disable all")) {
				USettings::Aimbot = false;
				USettings::triggerbot = false;
				USettings::ArmorBar_ESP = false;
				USettings::Head_Target = false;
				USettings::Body_Target = true;
				USettings::ShowFov = false;
				USettings::ShowTarget = false;
				USettings::FilledCircle = false;
				USettings::fov_changer = false;
				USettings::DrawCrosshair = false;
				USettings::whennotaiming = false;
				USettings::circle = true;
				USettings::Cross = true;
				USettings::radar_hack = false;
				USettings::Show_Enemy = true;
				USettings::Show_Squad = true;
				USettings::HealthBar_ESP = false;
				USettings::Box_ESP = false;
				USettings::CornerBox_ESP = false;
				USettings::Name_ESP = false;
				USettings::Distance_Esp = false;
				USettings::Bone_Esp = false;
				USettings::SnaplLine_Esp_End_Point = false;
				USettings::SnaplLine_Esp = false;
				USettings::FilledBox_Esp = false;
				USettings::BunnyHop = false;
				USettings::Box3D_Esp = false;
				USettings::Night_Mode = false;
				USettings::FullBright_Mode = false;
				USettings::window_animation = true;
				USettings::navigationwindow_animation = false;
				USettings::optionswindow_animation = false;
				USettings::GunName_Esp = false;
			}
			if (ImGui::Button("Default")) {
				USettings::triggerbot = false;
				USettings::triggerbot_delayms = 1;
				USettings::Aimbot = false;
				USettings::Head_Target = false;
				USettings::Body_Target = true;
				USettings::ShowFov = false;
				USettings::AimFov = 200;
				USettings::Smooth = 0.7;
				USettings::HotKey = VK_LBUTTON;
				USettings::FovThickness = 0;
				USettings::FovColor = { 255,255,255 };
				USettings::ShowTarget = false;
				USettings::TargetThickness = 0;
				USettings::TargetColor = { 255,0,0 };
				USettings::FilledCircle = false;
				USettings::FilledCircleColor = { 0,0,0,80 };
				USettings::fov_changer = false;
				USettings::fov_value = 90;
				USettings::oldfov_value = 90;
				USettings::DrawCrosshair = false;
				USettings::whennotaiming = false;
				USettings::Crosshair_Color = { 255,255,255 };
				USettings::Crosshair_size = 10;
				USettings::Crosshair_thickness = 0;
				USettings::circle = true;
				USettings::Cross = true;
				USettings::radar_hack = false;
				USettings::Show_Enemy = true;
				USettings::Show_Squad = true;
				USettings::HealthBar_Esp_Thickness = 2;
				USettings::HealthBar_ESP = false;
				USettings::Enemy_Box_Esp_Color = { 255,0,0 };
				USettings::Squad_Box_Esp_Color = { 0,0,255 };
				USettings::Box_Esp_Thickness = 0;
				USettings::Box_ESP = false;
				USettings::Enemy_CornerBox_Esp_Color = { 255,0,0 };
				USettings::Squad_CornerBox_Esp_Color = { 0,0,255 };
				USettings::Box_CornerEsp_Thickness = 0;
				USettings::CornerBox_ESP = false;
				USettings::Enemy_Name_ESP_Color = { 255,255,255 };
				USettings::Squad_Name_ESP_Color = { 255,255,255 };
				USettings::Name_ESP = false;
				USettings::Enemy_Distance_Esp_Color = { 255,0,0 };
				USettings::Squad_Distance_Esp_Color = { 0,0,255 };
				USettings::Distance_Esp = false;
				USettings::Enemy_Bone_Esp_Color = { 255,0,0 };
				USettings::Squad_Bone_Esp_Color = { 0,0,255 };
				USettings::Bone_Esp_Thickness = 0;
				USettings::Bone_Esp = false;
				USettings::SnaplLine_Esp_Start_Point = { 1920 / 2,1080 };
				USettings::SnaplLine_Esp_End_Point = false;
				USettings::Enemy_SnaplLine_Esp_Color = { 255,0,0 };
				USettings::Squad_SnaplLine_Esp_Color = { 0,0,255 };
				USettings::SnaplLine_Esp_Thickness = 0;
				USettings::SnaplLine_Esp = false;
				USettings::Enemy_FilledBox_Esp_Color = { 0,0,0,70 };
				USettings::Squad_FilledBox_Esp_Color = { 0,0,0,70 };
				USettings::FilledBox_Esp = false;
				USettings::Enemy_Box3D_Esp_Color = { 255,0,0 };
				USettings::Squad_Box3D_Esp_Color = { 0,0,255 };
				USettings::Box3D_Esp_Thickness = 0;
				USettings::Box3D_Esp = false;
				USettings::Box3D_Width = 20;
				USettings::ESP_Distance = 50;
				USettings::Night_Mode = false;
				USettings::ColorBlack = { 0,0,0,90 };
				USettings::FullBright_Mode = false;
				USettings::ColorWhite = { 255,255,255,80 };
				USettings::BunnyHop = false;
				USettings::window_animation = true;
				USettings::navigationwindow_animation = false;
				USettings::optionswindow_animation = false;
				USettings::show_watermark = false;
				USettings::GunName_Esp = false;
				USettings::HotKey = VK_LBUTTON;
				USettings::THotKey = VK_XBUTTON2;
				USettings::TriggerHotKey = 0;
				USettings::ArmorBar_ESP = false;
				USettings::AimBotHotKey = 0;
			}
		}
		ImGui::EndChild();
		if (USettings::window_animation)
			ImGui::GetBackgroundDrawList()->AddRect({ windowpos.x - 1, windowpos.y - 1 }, { windowpos.x + windowsize.x + 1, windowpos.y + windowsize.y + 1 }, ImColor(255, 255, 255, (int)Animator.alpha), 12.0f, 0, 1);//84, 171, 219
		if (USettings::navigationwindow_animation)
			ImGui::GetForegroundDrawList()->AddRect({ child_windowpos.x - 1, child_windowpos.y - 1 }, { child_windowpos.x + child_windowsize.x + 1, child_windowpos.y + child_windowsize.y + 1 }, ImColor(255, 255, 255, (int)Animator.alpha), 12.0f, 0, 1);
		if (USettings::optionswindow_animation)
			ImGui::GetForegroundDrawList()->AddRect({ child_windowpos1.x - 1, child_windowpos1.y - 1 }, { child_windowpos1.x + child_windowsize1.x + 1, child_windowpos1.y + child_windowsize1.y + 1 }, ImColor(255, 255, 255, (int)Animator.alpha), 12.0f, 0, 1);
		if (USettings::show_watermark)
			ImGui::GetBackgroundDrawList()->AddImage((void*)pTexture, ImVec2(1726, -10), ImVec2(1906, 170));
		ImGui::End();
		SetFocus(overlayWindow);
	}
	if (USettings::ArmorBar_ESP || USettings::GunName_Esp || USettings::SnaplLine_Esp || USettings::Box_ESP || USettings::Name_ESP || USettings::Bone_Esp || USettings::Distance_Esp || USettings::HealthBar_ESP || USettings::CornerBox_ESP || USettings::FilledBox_Esp || USettings::Box3D_Esp || USettings::radar_hack) {
		for (int i = 0; i < 64; i++) {
			DWORD64 CurrentController = E->GetEntInfo(i);
			DWORD64 CurrentPawn = E->GetEnt(i);
			DWORD64 LocalPlayer = E->GetLocal();

			if (LocalPlayer == NULL)
				continue;
			if (CurrentPawn == NULL)
				continue;
			if (CurrentController == NULL)
				continue;

			int health = E->GetHealth(CurrentPawn);
			int armor = E->GetArmor(CurrentPawn);

			if (health < 1 || health > 100)
				continue;

			int TeamNum = E->GetTeam(CurrentPawn);
			int LTeamNum = E->GetTeam(LocalPlayer);

			if (LocalPlayer == CurrentPawn)
				continue;

			if (E->GetPos(CurrentPawn).dist(E->GetPos(LocalPlayer)) / 100 > USettings::ESP_Distance)
				continue;

			DWORD64 CameraServices; read<DWORD64>(LocalPlayer, C_BasePlayerPawn::m_pCameraServices, CameraServices);
			if (CameraServices == NULL)
				continue;
			DWORD64 sceneNode; read<DWORD64>(CurrentPawn, C_BaseEntity::m_pGameSceneNode, sceneNode);
			if (sceneNode == NULL)
				continue;
			DWORD64 BoneArray; read<DWORD64>(sceneNode, CSkeletonInstance::m_modelState + 0x80, BoneArray);
			if (BoneArray == NULL)
				continue;
			DWORD64 CurrentWeapon; read<DWORD64>(CurrentPawn, C_CSPlayerPawnBase::m_pClippingWeapon, CurrentWeapon);
			if (CurrentWeapon == NULL)
				continue;

			Vector3 pos = E->GetBonePos3D(BoneArray, 28); Vector2 feetpos = PosToScreen(pos);
			Vector3 pos1 = E->GetBonePos3D(BoneArray, 28);
			int fFlag;
			if (!read<int>(LocalPlayer, C_BaseEntity::m_fFlags, fFlag))
				continue;
			if (fFlag == CROUCHING)
				pos1.z += 55;
			else
				pos1.z += 70;

			Vector2 headpos = PosToScreen(pos1);
			float height = feetpos.y - headpos.y;

			if (USettings::radar_hack) {
				bool True = true;
				if (TeamNum != LTeamNum && USettings::Show_Enemy)
					write<bool>(CurrentPawn, (C_CSPlayerPawn::m_entitySpottedState + EntitySpottedState_t::m_bSpotted), True);
			}
			if (USettings::FilledBox_Esp) {
				ImColor color = TeamNum == LTeamNum ? USettings::Squad_FilledBox_Esp_Color : USettings::Enemy_FilledBox_Esp_Color;
				if (feetpos.x > 0 && feetpos.y > 0 && feetpos.x < X_Screen && feetpos.y < Y_Screen) {
					if (fFlag == CROUCHING) {
						if (TeamNum == LTeamNum && USettings::Show_Squad)
							DrawFilledRect(feetpos, height, (height + height * 0.25) / 4, color);
						else if (TeamNum != LTeamNum && USettings::Show_Enemy)
							DrawFilledRect(feetpos, height, (height + height * 0.25) / 4, color);
					}
					else {
						if (TeamNum == LTeamNum && USettings::Show_Squad)
							DrawFilledRect(feetpos, height, height / 4, color);
						else if (TeamNum != LTeamNum && USettings::Show_Enemy)
							DrawFilledRect(feetpos, height, height / 4, color);
					}
				}
			}
			if (USettings::SnaplLine_Esp) {
				ImColor color = TeamNum == LTeamNum ? USettings::Squad_SnaplLine_Esp_Color : USettings::Enemy_SnaplLine_Esp_Color;
				if (USettings::SnaplLine_Esp_End_Point) {
					if (TeamNum == LTeamNum && USettings::Show_Squad)
						DrawLine(USettings::SnaplLine_Esp_Start_Point, headpos, color, USettings::SnaplLine_Esp_Thickness, true);
					else if (TeamNum != LTeamNum && USettings::Show_Enemy)
						DrawLine(USettings::SnaplLine_Esp_Start_Point, headpos, color, USettings::SnaplLine_Esp_Thickness, true);
				}
				else {
					if (TeamNum == LTeamNum && USettings::Show_Squad)
						DrawLine(USettings::SnaplLine_Esp_Start_Point, feetpos, color, USettings::SnaplLine_Esp_Thickness, true);
					else if (TeamNum != LTeamNum && USettings::Show_Enemy)
						DrawLine(USettings::SnaplLine_Esp_Start_Point, feetpos, color, USettings::SnaplLine_Esp_Thickness, true);
				}
			}
			if (USettings::Box_ESP) {
				ImColor color = TeamNum == LTeamNum ? USettings::Squad_Box_Esp_Color : USettings::Enemy_Box_Esp_Color;
				if (feetpos.x > 0 && feetpos.y > 0 && feetpos.x < X_Screen && feetpos.y < Y_Screen) {
					if (fFlag == CROUCHING) {
						if (TeamNum == LTeamNum && USettings::Show_Squad)
							drawbox(feetpos, height, (height + height * 0.25) / 4, color, USettings::Box_Esp_Thickness);
						else if (TeamNum != LTeamNum && USettings::Show_Enemy)
							drawbox(feetpos, height, (height + height * 0.25) / 4, color, USettings::Box_Esp_Thickness);
					}
					else {
						if (TeamNum == LTeamNum && USettings::Show_Squad)
							drawbox(feetpos, height, height / 4, color, USettings::Box_Esp_Thickness);
						else if (TeamNum != LTeamNum && USettings::Show_Enemy)
							drawbox(feetpos, height, height / 4, color, USettings::Box_Esp_Thickness);
					}
				}
			}
			if (USettings::Bone_Esp) {
				if (feetpos.x > 0 && feetpos.y > 0 && feetpos.x < X_Screen && feetpos.y < Y_Screen) {
					ImColor color = TeamNum == LTeamNum ? USettings::Squad_Bone_Esp_Color : USettings::Enemy_Bone_Esp_Color;/*
					for (int j = 0; j < 32; j++) {
						Vector2 Bonepos = E->GetBonePos(BoneArray, j);
						sprintf_s(number, sizeof(number), "%d", j);
						DrawString(Bonepos, ImColor(255, 255, 255), 2, number);
					}*/
					if (TeamNum == LTeamNum && USettings::Show_Squad)
						DrawBones(BoneArray, USettings::Bone_Esp_Thickness, color);
					else if (TeamNum != LTeamNum && USettings::Show_Enemy)
						DrawBones(BoneArray, USettings::Bone_Esp_Thickness, color);
				}
			}
			if (USettings::HealthBar_ESP) {
				if (feetpos.x > 0 && feetpos.y > 0 && feetpos.x < X_Screen && feetpos.y < Y_Screen) {
					if (fFlag == CROUCHING) {
						if (TeamNum == LTeamNum && USettings::Show_Squad)
							drawhealthbar(feetpos, height * E->GetHealth(CurrentPawn) / E->GetMaxHealth(CurrentPawn), (height + height * 0.25) / 3.6f, HealthBarColor(CurrentPawn), USettings::HealthBar_Esp_Thickness);
						else if (TeamNum != LTeamNum && USettings::Show_Enemy)
							drawhealthbar(feetpos, height * E->GetHealth(CurrentPawn) / E->GetMaxHealth(CurrentPawn), (height + height * 0.25) / 3.6f, HealthBarColor(CurrentPawn), USettings::HealthBar_Esp_Thickness);
					}
					else {
						if (TeamNum == LTeamNum && USettings::Show_Squad)
							drawhealthbar(feetpos, height * E->GetHealth(CurrentPawn) / E->GetMaxHealth(CurrentPawn), height / 3.6f, HealthBarColor(CurrentPawn), USettings::HealthBar_Esp_Thickness);
						else if (TeamNum != LTeamNum && USettings::Show_Enemy)
							drawhealthbar(feetpos, height * E->GetHealth(CurrentPawn) / E->GetMaxHealth(CurrentPawn), height / 3.6f, HealthBarColor(CurrentPawn), USettings::HealthBar_Esp_Thickness);
					}
				}
			}
			if (USettings::ArmorBar_ESP) {
				if (feetpos.x > 0 && feetpos.y > 0 && feetpos.x < X_Screen && feetpos.y < Y_Screen) {
					if (fFlag == CROUCHING) {
						if (TeamNum == LTeamNum && USettings::Show_Squad)
							drawarmorbar(feetpos, height * E->GetArmor(CurrentPawn) / E->GetMaxHealth(CurrentPawn), (height + height * 0.25) / 3.6f, ArmorBarColor(CurrentPawn), USettings::ArmorBar_Esp_Thickness);
						else if (TeamNum != LTeamNum && USettings::Show_Enemy)
							drawarmorbar(feetpos, height * E->GetArmor(CurrentPawn) / E->GetMaxHealth(CurrentPawn), (height + height * 0.25) / 3.6f, ArmorBarColor(CurrentPawn), USettings::ArmorBar_Esp_Thickness);
					}
					else {
						if (TeamNum == LTeamNum && USettings::Show_Squad)
							drawarmorbar(feetpos, height * E->GetArmor(CurrentPawn) / E->GetMaxHealth(CurrentPawn), height / 3.6f, ArmorBarColor(CurrentPawn), USettings::ArmorBar_Esp_Thickness);
						else if (TeamNum != LTeamNum && USettings::Show_Enemy)
							drawarmorbar(feetpos, height * E->GetArmor(CurrentPawn) / E->GetMaxHealth(CurrentPawn), height / 3.6f, ArmorBarColor(CurrentPawn), USettings::ArmorBar_Esp_Thickness);
					}
				}
			}
			if (USettings::CornerBox_ESP) {
				ImColor color = TeamNum == LTeamNum ? USettings::Squad_CornerBox_Esp_Color : USettings::Enemy_CornerBox_Esp_Color;
				if (feetpos.x > 0 && feetpos.y > 0 && feetpos.x < X_Screen && feetpos.y < Y_Screen) {
					if (fFlag == CROUCHING) {
						if (TeamNum == LTeamNum && USettings::Show_Squad)
							DrawCornerEsp((height + height * 0.25) / 2, height, feetpos, color, USettings::Box_CornerEsp_Thickness);
						else if (TeamNum != LTeamNum && USettings::Show_Enemy)
							DrawCornerEsp((height + height * 0.25) / 2, height, feetpos, color, USettings::Box_CornerEsp_Thickness);
					}
					else {
						if (TeamNum == LTeamNum && USettings::Show_Squad)
							DrawCornerEsp(height / 2, height, feetpos, color, USettings::Box_CornerEsp_Thickness);
						else if (TeamNum != LTeamNum && USettings::Show_Enemy)
							DrawCornerEsp(height / 2, height, feetpos, color, USettings::Box_CornerEsp_Thickness);
					}
				}
			}
			if (USettings::Name_ESP) {
				ProcessMgr.ReadMemory(CurrentController + CBasePlayerController::m_iszPlayerName, names, 16);
				Vector3 pos2 = E->GetBonePos3D(BoneArray, 28);
				pos2.z += 84;
				Vector2 posscreen2 = PosToScreen(pos2);
				ImColor color = TeamNum == LTeamNum ? USettings::Squad_Name_ESP_Color : USettings::Enemy_Name_ESP_Color;
				if (posscreen2.x > 0 && posscreen2.y > 0 && posscreen2.x < X_Screen && posscreen2.y < Y_Screen) {
					if (TeamNum == LTeamNum && USettings::Show_Squad)
						DrawString(posscreen2, color, 1, names);
					else if (TeamNum != LTeamNum && USettings::Show_Enemy)
						DrawString(posscreen2, color, 1, names);
				}
			}
			if (USettings::Distance_Esp) {
				Vector3 pos2 = E->GetBonePos3D(BoneArray, 28);
				pos2.z += 77;
				Vector2 posscreen2 = PosToScreen(pos2);
				sprintf_s(distance, sizeof(distance), "[%0.fm]", E->GetPos(LocalPlayer).dist(E->GetBonePos3D(BoneArray, 28)) / 100);
				ImColor color = TeamNum == LTeamNum ? USettings::Squad_Distance_Esp_Color : USettings::Enemy_Distance_Esp_Color;
				if (posscreen2.x > 0 && posscreen2.y > 0 && posscreen2.x < X_Screen && posscreen2.y < Y_Screen) {
					if (TeamNum == LTeamNum && USettings::Show_Squad)
						DrawString(posscreen2, color, 2, distance);
					else if (TeamNum != LTeamNum && USettings::Show_Enemy)
						DrawString(posscreen2, color, 2, distance);
				}
			}
			if (USettings::GunName_Esp) {
				short weaponDefinitionIndex; read<short>(CurrentWeapon, (C_EconEntity::m_AttributeManager + C_AttributeContainer::m_Item + C_EconItemView::m_iItemDefinitionIndex), weaponDefinitionIndex);
				if (weaponDefinitionIndex != -1) {
					auto it = weaponMap.find(weaponDefinitionIndex);
					Vector3 pos2 = E->GetBonePos3D(BoneArray, 28);
					pos2.z -= 10;
					Vector2 posscreen2 = PosToScreen(pos2);
					if (it != weaponMap.end()) {
						ImColor color = TeamNum == LTeamNum ? USettings::Squad_GunName_Color : USettings::Enemy_GunName_Color;
						if (posscreen2.x > 0 && posscreen2.y > 0 && posscreen2.x < X_Screen && posscreen2.y < Y_Screen) {
							if (TeamNum == LTeamNum && USettings::Show_Squad)
								DrawString(posscreen2, color, 2, it->second.c_str());
							else if (TeamNum != LTeamNum && USettings::Show_Enemy)
								DrawString(posscreen2, color, 2, it->second.c_str());
						}
					}
				}
			}
			if (USettings::Box3D_Esp) {
				ImColor color = TeamNum == LTeamNum ? USettings::Squad_Box3D_Esp_Color : USettings::Enemy_Box3D_Esp_Color;
				if (feetpos.x > 0 && feetpos.y > 0 && feetpos.x < X_Screen && feetpos.y < Y_Screen) {
					if (TeamNum == LTeamNum && USettings::Show_Squad)
						Draw3DBox(CurrentPawn, color, USettings::Box3D_Esp_Thickness, USettings::Box3D_Width, fFlag);
					else if (TeamNum != LTeamNum && USettings::Show_Enemy)
						Draw3DBox(CurrentPawn, color, USettings::Box3D_Esp_Thickness, USettings::Box3D_Width, fFlag);
				}
			}
		}
	}
	if (USettings::Aimbot) {
		if (USettings::ShowFov || USettings::FilledCircle) {
			DWORD64 local = E->GetLocal();
			if (local != NULL) {
				int health = E->GetHealth(local);
				if (health > 0 && health < 101) {
					if (USettings::ShowFov)
						DrawCircle({ X_Screen / 2, Y_Screen / 2 }, USettings::AimFov, USettings::FovThickness, USettings::FovColor);
					if (USettings::FilledCircle)
						ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(X_Screen / 2, Y_Screen / 2), USettings::AimFov, USettings::FilledCircleColor);
				}
			}
		}
	}
	if (USettings::ShowTarget && USettings::Aimbot) {
		int i = FindClosestEnemy();
		if (i != 100) {
			DWORD64 ent = E->GetEnt(FindClosestEnemy());
			if (ent != NULL) {
				DWORD64 sceneNode;
				if (read<DWORD64>(ent, C_BaseEntity::m_pGameSceneNode, sceneNode)) {
					DWORD64 BoneArray;
					if (read<DWORD64>(sceneNode, CSkeletonInstance::m_modelState + 0x80, BoneArray)) {
						Vector3 aimpos;
						if (USettings::Head_Target)
							aimpos = E->GetBonePos3D(BoneArray, bones::head);
						if (USettings::Body_Target)
							aimpos = E->GetBonePos3D(BoneArray, bones::spine);
						aimpos.z -= 1.f;
						Vector2 posscreen = PosToScreen(aimpos);
						DrawLine({ X_Screen / 2, Y_Screen / 2 }, posscreen, USettings::TargetColor, USettings::TargetThickness, true);
					}
				}
			}
		}
	}
	if (USettings::Night_Mode) {
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(X_Screen, Y_Screen), USettings::ColorBlack);
	}
	if (USettings::FullBright_Mode) {
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(X_Screen, Y_Screen), USettings::ColorWhite);
	}
	if (USettings::DrawCrosshair) {
		if (E->GetHealth(E->GetLocal()) > 0) {
			if (!USettings::whennotaiming) {
				if (USettings::Cross) {
					DrawLine({ X_Screen / 2,(Y_Screen / 2) - USettings::Crosshair_size - 0.5f }, { X_Screen / 2,(Y_Screen / 2) + USettings::Crosshair_size - 0.5f }, USettings::Crosshair_Color, USettings::Crosshair_thickness, false);
					DrawLine({ (X_Screen / 2) - USettings::Crosshair_size - 0.5f,Y_Screen / 2 }, { (X_Screen / 2) + USettings::Crosshair_size - 0.5f,Y_Screen / 2 }, USettings::Crosshair_Color, USettings::Crosshair_thickness, false);
				}
				if (USettings::circle) {
					DrawCircle({ X_Screen / 2, Y_Screen / 2 }, USettings::Crosshair_size, USettings::Crosshair_thickness, USettings::Crosshair_Color);
				}
			}
			else {
				if (!GetAsyncKeyState(VK_RBUTTON)) {
					if (USettings::Cross) {
						DrawLine({ X_Screen / 2,(Y_Screen / 2) - USettings::Crosshair_size }, { X_Screen / 2,(Y_Screen / 2) + USettings::Crosshair_size }, USettings::Crosshair_Color, USettings::Crosshair_thickness, false);
						DrawLine({ (X_Screen / 2) - USettings::Crosshair_size,Y_Screen / 2 }, { (X_Screen / 2) + USettings::Crosshair_size,Y_Screen / 2 }, USettings::Crosshair_Color, USettings::Crosshair_thickness, false);
					}
					if (USettings::circle) {
						DrawCircle({ X_Screen / 2, Y_Screen / 2 }, USettings::Crosshair_size, USettings::Crosshair_thickness, USettings::Crosshair_Color);
					}
				}
			}
		}
	}
	ImGui::EndFrame();

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (pDevice->BeginScene() >= 0) {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		pDevice->EndScene();
	}

	if (pDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST && pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		ImGui_ImplDX9_InvalidateDeviceObjects();
		pDevice->Reset(&gD3DPresentParams);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

void mainLoop() {
	static MSG msg;
	static RECT oldRect;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT && GetWindow(targetWindow, GW_HWNDPREV)) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (GetForegroundWindow() == targetWindow) {
			SetWindowPos(overlayWindow, GetWindow(GetForegroundWindow(), GW_HWNDPREV), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		RECT windowRect;
		POINT windowPoint;
		ZeroMemory(&windowRect, sizeof(RECT));
		ZeroMemory(&windowPoint, sizeof(POINT));

		GetClientRect(targetWindow, &windowRect);
		ClientToScreen(targetWindow, &windowPoint);

		if (memcmp(&windowPoint, &oldRect.top, sizeof(POINT)) || memcmp(&windowRect, &oldRect, sizeof(RECT))) {
			oldRect = windowRect;
			windowInfo->Width = windowRect.right;
			windowInfo->Height = windowRect.bottom;
			SetWindowPos(overlayWindow, (HWND)0, windowPoint.x, windowPoint.y, windowInfo->Width, windowInfo->Height, SWP_NOREDRAW);
			pDevice->Reset(&gD3DPresentParams);
		}
		renderImGui();
		GetClients();
		TriggerBot();
		if (USettings::Aimbot) {
			int i = FindClosestEnemy();
			if (i == 100)
				continue;
			Aimbot(FindClosestEnemy());
		}
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	clearVariable(pDevice);
	clearVariable(pDirect);
	if (overlayWindow) {
		DestroyWindow(overlayWindow);
		UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
		overlayWindow = nullptr;
	}
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (isInitialized && isMenuVisible) {
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return TRUE;
	}
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		TerminateProcess(GetCurrentProcess(), 0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void inputHandler() {
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
	for (int i = 1; i < 5; i++) {
		io.MouseDown[i] = false;
	}
}

bool createOverlay() {	
	windowClass = { sizeof(WNDCLASSEX), NULL, WndProc, NULL, NULL, NULL, NULL, NULL, NULL, NULL, ovarlayName.c_str(), NULL };
	RegisterClassEx(&windowClass);

	RECT windowRect;
	POINT windowPoint;
	ZeroMemory(&windowRect, sizeof(RECT));
	ZeroMemory(&windowPoint, sizeof(POINT));

	GetClientRect(targetWindow, &windowRect);
	ClientToScreen(targetWindow, &windowPoint);

	MARGINS margins = { -1 };
	windowInfo->Width = windowRect.right;
	windowInfo->Height = windowRect.bottom;

	overlayWindow = CreateWindowEx(NULL, ovarlayName.c_str(), ovarlayName.c_str(), WS_POPUP | WS_VISIBLE, windowInfo->Left, windowInfo->Top, windowInfo->Width, windowInfo->Height, NULL, NULL, windowClass.hInstance, NULL);
	DwmExtendFrameIntoClientArea(overlayWindow, &margins);
	SetWindowLong(overlayWindow, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	ShowWindow(overlayWindow, SW_SHOW);
	UpdateWindow(overlayWindow);
	return TRUE;
}

bool createDirectX() {
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &pDirect))) {
		return FALSE;
	}

	ZeroMemory(&gD3DPresentParams, sizeof(gD3DPresentParams));
	gD3DPresentParams.Windowed = TRUE;
	gD3DPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	gD3DPresentParams.BackBufferFormat = D3DFMT_UNKNOWN;
	gD3DPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	gD3DPresentParams.BackBufferFormat = D3DFMT_A8R8G8B8;
	gD3DPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE; //VSync (Vertical Synchronization)

	if (pDirect->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, overlayWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &gD3DPresentParams, 0, &pDevice) != D3D_OK) {
		return FALSE;
	}
	return TRUE;
}

int main(int argc, char* argv[]) {
	X_Screen = GetSystemMetrics(SM_CXSCREEN);
	Y_Screen = GetSystemMetrics(SM_CYSCREEN);
	if (createConsole == false) {
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	}	
	auto ProcessStatus = ProcessMgr.Attach("cs2.exe");

	if (ProcessStatus != StatusCode::SUCCEED)
	{
		std::cout << "[ERROR] Failed to attach process, StatusCode:" << ProcessStatus << std::endl;
		system("pause");
		return 0;
	}
	std::cout << "[Game] Pid:" << ProcessMgr.ProcessID;
	client = GetHandle("client.dll");

	windowInfo = new WindowInfo();
	bool WindowFocus = false;
	while (WindowFocus == false) {
		DWORD ForegroundWindowProcessID;
		GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundWindowProcessID);
		if (getProcessID(targetProcessName) == ForegroundWindowProcessID) {
			targetWindow = GetForegroundWindow();
			RECT windowRect;
			GetWindowRect(targetWindow, &windowRect);
			windowInfo->Width = windowRect.right - windowRect.left;
			windowInfo->Height = windowRect.bottom - windowRect.top;
			windowInfo->Left = windowRect.left;
			windowInfo->Right = windowRect.right;
			windowInfo->Top = windowRect.top;
			windowInfo->Bottom = windowRect.bottom;
			WindowFocus = true;
		}
	}
	createOverlay();
	createDirectX();
	mainLoop();
}