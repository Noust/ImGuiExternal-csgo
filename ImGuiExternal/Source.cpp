#include "include.h"
#include <algorithm>
bool createConsole = true;
bool isInitialized = false;
bool isMenuVisible = true;

/*checar el otro source y agregar cosas
visibility check
bomb hack
speed hack
recoil control
sking changer*/

IDirect3DTexture9* pTexture = nullptr;

int FindClosestEnemy() {
    DWORD64 LocalPlayer = E->GetLocal();
    if (!LocalPlayer || E->GetHealth(LocalPlayer) <= 0 || E->GetHealth(LocalPlayer) > 100)
        return 100;

    float closestDistance = FLT_MAX;
    int closestEntity = 100;
    const Vector3 localPos = E->GetPos(LocalPlayer);
    const int localTeam = E->GetTeam(LocalPlayer);
    const Vector2 screenCenter = { X_Screen / 2.0f, Y_Screen / 2.0f };

    for (int i = 0; i < 64; i++) {
        DWORD64 entityAddr = E->GetEnt(i);
        DWORD64 sceneNode, boneArray;
        
        if (!entityAddr || 
            !read<DWORD64>(entityAddr, C_BaseEntity::m_pGameSceneNode, sceneNode) || !sceneNode ||
            !read<DWORD64>(sceneNode, CSkeletonInstance::m_modelState + 0x80, boneArray) || !boneArray)
            continue;

        float entityHealth = E->GetHealth(entityAddr);
        if (entityHealth <= 0 || entityHealth > 100 || E->GetTeam(entityAddr) == localTeam)
            continue;

        if (E->GetBonePos3D(boneArray, 28).dist(localPos) / 100.0f > USettings.ESP_Distance)
            continue;

        Vector3 aimPos = E->GetBonePos3D(boneArray, USettings.Head_Target ? bones::head : bones::spine);
        Vector2 screenPos = PosToScreen(aimPos);
        float screenDist = screenPos.dist(screenCenter);

        if (screenDist <= USettings.AimFov && screenDist < closestDistance) {
            closestDistance = screenDist;
            closestEntity = i;
        }
    }

    return closestEntity;
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
		if (USettings.Backround_Animation)
			DrawBackgroundAnimation();
		inputHandler();
		if (USettings.Show_Fps)
			drawItem();
		
		// Animate menu
		static animator Animator = { 255, false, 0.5f };
		animatecontent(Animator);

		// Main window styling
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.11f, 0.11f, 0.11f, 1.00f));

		ImGui::Begin("CS Menu", nullptr, ImGuiWindowFlags_NoCollapse);
		ImVec2 windowpos = ImGui::GetWindowPos();
		ImVec2 windowsize = ImGui::GetWindowSize();

		// Navigation bar
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
		ImGui::BeginChild("##Navigation", ImVec2(ImGui::GetContentRegionAvail().x, 50), true);
		ImVec2 child_windowpos = ImGui::GetWindowPos();
		ImVec2 child_windowsize = ImGui::GetWindowSize();

		// Navigation buttons
		float windowWidth = ImGui::GetWindowSize().x;
		float buttonWidth = (ImGui::GetContentRegionAvail().x - 30) / 4;
		float spacing = (windowWidth - (buttonWidth * 4)) / 5;

		const char* tabs[] = { "Gun", "Legit", "Visuals", "Config" };
		int tabValues[] = { 1, 2, 3, 4 };

		for (int i = 0; i < 4; i++) {
			ImGui::SetCursorPosX(spacing * (i + 1) + buttonWidth * i);
			
			bool selected = (CSettings::MenuWindow == tabValues[i]);
			if (selected) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
			
			if (ImGui::Button(tabs[i], ImVec2(buttonWidth, 0))) {
				CSettings::MenuWindow = tabValues[i];
			}
			
			if (selected) ImGui::PopStyleColor();
			
			if (i < 3) ImGui::SameLine();
		}

		ImGui::EndChild();
		ImGui::PopStyleVar();

		// Content area
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
		ImGui::BeginChild("##Content", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
		ImVec2 child_windowpos1 = ImGui::GetWindowPos();
		ImVec2 child_windowsize1 = ImGui::GetWindowSize();

		// Main menu / Welcome screen
		if (CSettings::MenuWindow == 0) {
			ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x / 2) - 100);
			ImGui::Image((void*)pTexture, ImVec2(200, 170));
			centertext<float>("CSGO 2 Menu by Nova", 0.0f, 0.0f);
			centertext<float>("Window Size X:%0.f Y:%0.f", windowsize.x, windowsize.y);
		}

		// Gun menu
		else if (CSettings::MenuWindow == 1) {
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
			
			ImGui::Text("Combat Settings");

			// Trigger bot section
			ImGui::Checkbox("Enable Trigger Bot", &USettings.triggerbot);
			if (USettings.triggerbot) {
				if (ImGui::Combo("Trigger Key", &USettings.TriggerHotKey, "XBUTTON2\0MENU\0RBUTTON\0XBUTTON1\0LBUTTON\0CAPITAL\0SHIFT\0CONTROL"))
					USettings.TSetHotKey(USettings.TriggerHotKey);
				ImGui::SliderInt("Delay (ms)", &USettings.triggerbot_delayms, 0, 200);
				ImGui::Separator();
			}

			// Aimbot section
			ImGui::Checkbox("Enable Aimbot", &USettings.Aimbot);
			if (USettings.Aimbot) {
				if (ImGui::Combo("Aim Key", &USettings.AimBotHotKey, "LBUTTON\0MENU\0RBUTTON\0XBUTTON1\0XBUTTON2\0CAPITAL\0SHIFT\0CONTROL"))
					USettings.SetHotKey(USettings.AimBotHotKey);
				
				ImGui::Text("Core Settings");
				ImGui::SliderFloat("Smoothness", &USettings.Smooth, 0, 0.8f);
				ImGui::SliderFloat("FOV", &USettings.AimFov, 10, 1920);

				ImGui::Text("Target Selection");
				ImGui::Checkbox("Head Target", &USettings.Head_Target);
				ImGui::Checkbox("Body Target", &USettings.Body_Target);
				if (USettings.Head_Target) USettings.Body_Target = false;
				if (USettings.Body_Target) USettings.Head_Target = false;

				ImGui::Text("Visual Feedback");
				ImGui::Checkbox("Show FOV", &USettings.ShowFov);
				if (USettings.ShowFov) {
					ImGui::SliderInt("FOV Circle Thickness", &USettings.FovThickness, 0, 10);
					ImGui::ColorEdit3("FOV Circle Color", (float*)&USettings.FovColor);
					ImGui::Checkbox("Fill FOV Circle", &USettings.FilledCircle);
					ImGui::ColorEdit3("Fill Color", (float*)&USettings.FilledCircleColor);
				}

				ImGui::Checkbox("Show Target", &USettings.ShowTarget);
				if (USettings.ShowTarget) {
					ImGui::SliderInt("Target Indicator Thickness", &USettings.TargetThickness, 0, 10);
					ImGui::ColorEdit3("Target Indicator Color", (float*)&USettings.TargetColor);
				}
			}
			ImGui::PopStyleColor();
		}

		else if (CSettings::MenuWindow == 2) {
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));

			// Movement & Utility Section
			ImGui::Text("Movement & Utility");
			ImGui::Checkbox("Bunny Hop", &USettings.BunnyHop);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			// Money Manager Section
			ImGui::Text("Money Manager");
			ImGui::Checkbox("Enable Money Services", &USettings.MoneyServices);
			if (USettings.MoneyServices) {
				ImGui::BeginChild("MoneyServices", ImVec2(0, 200), true);
				for (int i = 0; i < 64; i++) {
					if (DWORD64 CurrentController = E->GetEntInfo(i)) {
						DWORD64 moneyservices;
						if (read<DWORD64>(CurrentController, CCSPlayerController::m_pInGameMoneyServices, moneyservices) && moneyservices != NULL) {
							char name[128];
							if (ProcessMgr.ReadMemory(CurrentController + CBasePlayerController::m_iszPlayerName, name, 128)) {
								int account, cashSpent, cashSpentTotal;
								if (read<int>(moneyservices, CCSPlayerController_InGameMoneyServices::m_iAccount, account) &&
									read<int>(moneyservices, CCSPlayerController_InGameMoneyServices::m_iCashSpentThisRound, cashSpent) &&
									read<int>(moneyservices, CCSPlayerController_InGameMoneyServices::m_iTotalCashSpent, cashSpentTotal)) {
									
									if (ImGui::TreeNode(name)) {
										ImGui::TextColored(ImColor(0, 255, 0), "Account: $%d", account);
										ImGui::TextColored(ImColor(255, 165, 0), "Round Spent: $%d", cashSpent);
										ImGui::TextColored(ImColor(255, 0, 0), "Total Spent: $%d", cashSpentTotal);
										ImGui::TreePop();
									}
								}
							}
						}
					}
				}
				ImGui::EndChild();
			}
			ImGui::PopStyleColor();
		}
		else if (CSettings::MenuWindow == 3) {
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));

			// View Settings Section
			ImGui::Text("View Settings Section");

			ImGui::Checkbox("Enable Radar", &USettings.radar_hack);
			if (USettings.radar_hack) {
				ImGui::Text("Radar Settings");
				ImGui::SliderFloat("Radar Scale", &USettings.scale, 0.05f, 0.5f, "%.2f");
				ImGui::SliderFloat("Radar Radius", &USettings.radarRadius, 10.0f, 200.0f, "%.0f px");
				
				ImGui::Text("Radar Position");
				ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.45f);
				ImGui::SliderFloat("X Position", &USettings.center.x, 0, X_Screen, "%.0f");
				ImGui::SameLine();
				ImGui::SliderFloat("Y Position", &USettings.center.y, 0, Y_Screen, "%.0f"); 
				ImGui::PopItemWidth();
				
				ImGui::Separator();
			}
			ImGui::Checkbox("Night Mode", &USettings.Night_Mode);
			ImGui::Checkbox("Full Bright", &USettings.FullBright_Mode);
			ImGui::Checkbox("Eye Ray", &USettings.ShowEyeRay);
			if (USettings.ShowEyeRay) {
				ImGui::SliderFloat("Eye Ray Length", &USettings.length, 5, 100, "%.0f");
				ImGui::ColorEdit3("Eye Ray Color", (float*)&USettings.EyeRay_Color);
				ImGui::SliderInt("Eye Ray Thickness", &USettings.EyeRay_Thickness, 0, 10);
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			// ESP Features Section
			ImGui::Text("ESP Features");

			// Snap Line ESP
			ImGui::Checkbox("Snap Lines", &USettings.SnaplLine_Esp);
			if (USettings.SnaplLine_Esp) {
				ImGui::ColorEdit3("Enemy Line Color", (float*)&USettings.Enemy_SnaplLine_Esp_Color);
				ImGui::ColorEdit3("Team Line Color", (float*)&USettings.Squad_SnaplLine_Esp_Color);
				ImGui::SliderInt("Line Thickness", &USettings.SnaplLine_Esp_Thickness, 0, 10);
				
				ImGui::Text("Line Start Position");
				if (ImGui::BeginTable("LinePositions", 2)) {
					ImGui::TableNextColumn();
					if (ImGui::Button("Bottom")) USettings.SnaplLine_Esp_Start_Point = { X_Screen / 2, Y_Screen };
					if (ImGui::Button("Left")) USettings.SnaplLine_Esp_Start_Point = { 0, Y_Screen / 2 };
					ImGui::TableNextColumn();
					if (ImGui::Button("Top")) USettings.SnaplLine_Esp_Start_Point = { X_Screen / 2, 0 };
					if (ImGui::Button("Right")) USettings.SnaplLine_Esp_Start_Point = { X_Screen, Y_Screen / 2 };
					ImGui::EndTable();
				}
				
				ImGui::SliderFloat("Custom X", &USettings.SnaplLine_Esp_Start_Point.x, 0, X_Screen);
				ImGui::SliderFloat("Custom Y", &USettings.SnaplLine_Esp_Start_Point.y, 0, Y_Screen);
				
				ImGui::Text("Line End Point");
				if (ImGui::Button(USettings.SnaplLine_Esp_End_Point ? "Head" : "Feet")) {
					USettings.SnaplLine_Esp_End_Point = !USettings.SnaplLine_Esp_End_Point;
				}
				ImGui::Separator();
			}

			// Box ESP Options
			ImGui::Checkbox("Box ESP", &USettings.Box_ESP);
			if (USettings.Box_ESP) {
				ImGui::ColorEdit3("Enemy Box Color", (float*)&USettings.Enemy_Box_Esp_Color);
				ImGui::ColorEdit3("Team Box Color", (float*)&USettings.Squad_Box_Esp_Color);
				ImGui::SliderInt("Box Thickness", &USettings.Box_Esp_Thickness, 0, 10);
				ImGui::Separator();
			}

			ImGui::Checkbox("Corner Box", &USettings.CornerBox_ESP);
			if (USettings.CornerBox_ESP) {
				ImGui::ColorEdit3("Enemy Corner Color", (float*)&USettings.Enemy_CornerBox_Esp_Color); 
				ImGui::ColorEdit3("Team Corner Color", (float*)&USettings.Squad_CornerBox_Esp_Color);
				ImGui::SliderInt("Corner Thickness", &USettings.Box_CornerEsp_Thickness, 0, 10);
				ImGui::Separator();
			}

			ImGui::Checkbox("3D Box", &USettings.Box3D_Esp);
			if (USettings.Box3D_Esp) {
				ImGui::ColorEdit3("Enemy 3D Box Color", (float*)&USettings.Enemy_Box3D_Esp_Color);
				ImGui::ColorEdit3("Team 3D Box Color", (float*)&USettings.Squad_Box3D_Esp_Color);
				ImGui::SliderInt("3D Box Thickness", &USettings.Box3D_Esp_Thickness, 0, 10);
				ImGui::SliderFloat("3D Box Width", &USettings.Box3D_Width, 10, 40);
				ImGui::Separator();
			}

			ImGui::Checkbox("Filled Box", &USettings.FilledBox_Esp);
			if (USettings.FilledBox_Esp) {
				ImGui::ColorEdit4("Enemy Fill Color", (float*)&USettings.Enemy_FilledBox_Esp_Color);
				ImGui::ColorEdit4("Team Fill Color", (float*)&USettings.Squad_FilledBox_Esp_Color);
				ImGui::Separator();
			}

			// Player Info ESP
			ImGui::Checkbox("Show Names", &USettings.Name_ESP);
			if (USettings.Name_ESP) {
				ImGui::ColorEdit3("Enemy Name Color", (float*)&USettings.Enemy_Name_ESP_Color);
				ImGui::ColorEdit3("Team Name Color", (float*)&USettings.Squad_Name_ESP_Color);
				ImGui::Separator();
			}

			ImGui::Checkbox("Show Distance", &USettings.Distance_Esp);
			if (USettings.Distance_Esp) {
				ImGui::ColorEdit3("Enemy Distance Color", (float*)&USettings.Enemy_Distance_Esp_Color);
				ImGui::ColorEdit3("Team Distance Color", (float*)&USettings.Squad_Distance_Esp_Color);
				ImGui::Separator();
			}

			ImGui::Checkbox("Show Weapon", &USettings.GunName_Esp);
			if (USettings.GunName_Esp) {
				ImGui::ColorEdit3("Enemy Weapon Color", (float*)&USettings.Enemy_GunName_Color);
				ImGui::ColorEdit3("Team Weapon Color", (float*)&USettings.Squad_GunName_Color);
				ImGui::Separator();
			}

			ImGui::Checkbox("Show Bones", &USettings.Bone_Esp);
			if (USettings.Bone_Esp) {
				ImGui::ColorEdit3("Enemy Bone Color", (float*)&USettings.Enemy_Bone_Esp_Color);
				ImGui::ColorEdit3("Team Bone Color", (float*)&USettings.Squad_Bone_Esp_Color);
				ImGui::SliderInt("Bone Thickness", &USettings.Bone_Esp_Thickness, 0, 10);
				ImGui::Separator();
			}

			ImGui::Checkbox("Health Bar", &USettings.HealthBar_ESP);
			ImGui::Checkbox("Armor Bar", &USettings.ArmorBar_ESP);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			// Crosshair Settings
			ImGui::Text("Crosshair Settings");
			ImGui::Checkbox("Custom Crosshair", &USettings.DrawCrosshair);
			if (USettings.DrawCrosshair) {
				ImGui::Checkbox("Show When Not Aiming", &USettings.whennotaiming);
				ImGui::ColorEdit3("Crosshair Color", (float*)&USettings.Crosshair_Color);
				ImGui::SliderFloat("Size", &USettings.Crosshair_size, 1, 30);
				ImGui::SliderInt("Thickness", &USettings.Crosshair_thickness, 0, 10);
				ImGui::Checkbox("Circle", &USettings.circle);
				ImGui::Checkbox("Cross", &USettings.Cross);
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			// Global ESP Settings
			ImGui::Text("Global ESP Settings");
			ImGui::SliderInt("Max ESP Distance", &USettings.ESP_Distance, 0, 100);
			ImGui::SliderFloat("Text Size", &USettings.Text_Size, 0.4f, 2.0f, "%.2f");
			
			ImGui::PopStyleColor();
		}
		else if (CSettings::MenuWindow == 4) {
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));

			// UI Settings
			ImGui::Text("Menu Animations");
			ImGui::Checkbox("Window Animation", &USettings.window_animation);
			ImGui::Checkbox("Navigation Animation", &USettings.navigationwindow_animation);
			ImGui::Checkbox("Options Animation", &USettings.optionswindow_animation);
			ImGui::Checkbox("Background Animation", &USettings.Backround_Animation);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Text("Menu Controls");
			ImGui::Text("Press INSERT to toggle menu");
			ImGui::Text("Press DELETE to exit");

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Text("Bypass");
			ImGui::Checkbox("OBS Bypass", &USettings.OBSBypass);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			// Save/Load Settings
			ImGui::Text("Configuration");
			ImGui::Checkbox("Show Overlay Fps", &USettings.Show_Fps);
			if (ImGui::Button("Save Settings")) {
				F->SaveSettings();
			}
			ImGui::SameLine();
			if (ImGui::Button("Load Settings")) {
				if (std::filesystem::exists("sarilla.bin"))
					F->ReadSettings();
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			// ESP Filters
			ImGui::Text("ESP Filters");
			ImGui::Checkbox("Show Teammates", &USettings.Show_Squad);
			ImGui::Checkbox("Show Enemies", &USettings.Show_Enemy);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			// Quick Actions
			ImGui::Text("Quick Actions");
			if (ImGui::Button("Disable All Features")) {
				USettings.Aimbot = false;
				USettings.triggerbot = false;
				USettings.ArmorBar_ESP = false;
				USettings.Head_Target = false;
				USettings.Body_Target = true;
				USettings.ShowFov = false;
				USettings.ShowTarget = false;
				USettings.FilledCircle = false;
				USettings.DrawCrosshair = false;
				USettings.whennotaiming = false;
				USettings.circle = true;
				USettings.Cross = true;
				USettings.ShowEyeRay = false;
				USettings.radar_hack = false;
				USettings.Show_Enemy = true;
				USettings.Show_Squad = true;
				USettings.HealthBar_ESP = false;
				USettings.Box_ESP = false;
				USettings.CornerBox_ESP = false;
				USettings.Name_ESP = false;
				USettings.Distance_Esp = false;
				USettings.Bone_Esp = false;
				USettings.SnaplLine_Esp_End_Point = false;
				USettings.SnaplLine_Esp = false;
				USettings.FilledBox_Esp = false;
				USettings.BunnyHop = false;
				USettings.Box3D_Esp = false;
				USettings.Night_Mode = false;
				USettings.FullBright_Mode = false;
				USettings.window_animation = true;
				USettings.navigationwindow_animation = false;
				USettings.optionswindow_animation = false;
				USettings.GunName_Esp = false;
				USettings.OBSBypass = false;
				USettings.Show_Fps = false;
			}

			ImGui::SameLine();
			if (ImGui::Button("Reset to Default")) {
				USettings.Show_Fps = false;
				USettings.triggerbot = false;
				USettings.triggerbot_delayms = 1;
				USettings.Aimbot = false;
				USettings.Head_Target = false;
				USettings.Body_Target = true;
				USettings.ShowFov = false;
				USettings.AimFov = 200;
				USettings.Smooth = 0.7;
				USettings.HotKey = VK_LBUTTON;
				USettings.FovThickness = 0;
				USettings.FovColor = { 255,255,255 };
				USettings.ShowTarget = false;
				USettings.TargetThickness = 0;
				USettings.TargetColor = { 255,0,0 };
				USettings.FilledCircle = false;
				USettings.FilledCircleColor = { 0,0,0,80 };
				USettings.DrawCrosshair = false;
				USettings.whennotaiming = false;
				USettings.Crosshair_Color = { 255,255,255 };
				USettings.Crosshair_size = 10;
				USettings.Crosshair_thickness = 0;
				USettings.circle = true;
				USettings.Cross = true;
				USettings.ShowEyeRay = false;
				USettings.length = 50;
				USettings.EyeRay_Color = { 255,0,0 };
				USettings.EyeRay_Thickness = 0;
				USettings.radar_hack = false;
				USettings.scale = 0.08f;
				USettings.center = { 100,100 };
				USettings.radarRadius = 100.0f;
				USettings.Show_Enemy = true;
				USettings.Show_Squad = true;
				USettings.HealthBar_Esp_Thickness = 2;
				USettings.HealthBar_ESP = false;
				USettings.Enemy_Box_Esp_Color = { 255,0,0 };
				USettings.Squad_Box_Esp_Color = { 0,0,255 };
				USettings.Box_Esp_Thickness = 0;
				USettings.Box_ESP = false;
				USettings.Enemy_CornerBox_Esp_Color = { 255,0,0 };
				USettings.Squad_CornerBox_Esp_Color = { 0,0,255 };
				USettings.Box_CornerEsp_Thickness = 0;
				USettings.CornerBox_ESP = false;
				USettings.Enemy_Name_ESP_Color = { 255,255,255 };
				USettings.Squad_Name_ESP_Color = { 255,255,255 };
				USettings.Name_ESP = false;
				USettings.Enemy_Distance_Esp_Color = { 255,0,0 };
				USettings.Squad_Distance_Esp_Color = { 0,0,255 };
				USettings.Distance_Esp = false;
				USettings.Enemy_Bone_Esp_Color = { 255,0,0 };
				USettings.Squad_Bone_Esp_Color = { 0,0,255 };
				USettings.Bone_Esp_Thickness = 0;
				USettings.Bone_Esp = false;
				USettings.SnaplLine_Esp_Start_Point = { X_Screen / 2,Y_Screen };
				USettings.SnaplLine_Esp_End_Point = false;
				USettings.Enemy_SnaplLine_Esp_Color = { 255,0,0 };
				USettings.Squad_SnaplLine_Esp_Color = { 0,0,255 };
				USettings.SnaplLine_Esp_Thickness = 0;
				USettings.SnaplLine_Esp = false;
				USettings.Enemy_FilledBox_Esp_Color = { 0,0,0,70 };
				USettings.Squad_FilledBox_Esp_Color = { 0,0,0,70 };
				USettings.FilledBox_Esp = false;
				USettings.Enemy_Box3D_Esp_Color = { 255,0,0 };
				USettings.Squad_Box3D_Esp_Color = { 0,0,255 };
				USettings.Box3D_Esp_Thickness = 0;
				USettings.Box3D_Esp = false;
				USettings.Box3D_Width = 20;
				USettings.ESP_Distance = 50;
				USettings.Night_Mode = false;
				USettings.FullBright_Mode = false;
				USettings.BunnyHop = false;
				USettings.window_animation = true;
				USettings.navigationwindow_animation = false;
				USettings.optionswindow_animation = false;
				USettings.show_watermark = true;
				USettings.GunName_Esp = false;
				USettings.HotKey = VK_LBUTTON;
				USettings.THotKey = VK_XBUTTON2;
				USettings.TriggerHotKey = 0;
				USettings.ArmorBar_ESP = false;
				USettings.AimBotHotKey = 0;
				USettings.Text_Size = 0.9f;
				USettings.Backround_Animation = true;
			}
			ImGui::PopStyleColor();
		}
		ImGui::EndChild();
		ImGui::PopStyleVar();

		// Window animations
		if (USettings.window_animation)
			ImGui::GetBackgroundDrawList()->AddRect(
				ImVec2(windowpos.x - 1, windowpos.y - 1),
				ImVec2(windowpos.x + windowsize.x + 1, windowpos.y + windowsize.y + 1),
				ImColor(255, 255, 255, (int)Animator.alpha), 12.0f
			);

		if (USettings.navigationwindow_animation)
			ImGui::GetForegroundDrawList()->AddRect(
				ImVec2(child_windowpos.x - 1, child_windowpos.y - 1),
				ImVec2(child_windowpos.x + child_windowsize.x + 1, child_windowpos.y + child_windowsize.y + 1),
				ImColor(255, 255, 255, (int)Animator.alpha), 12.0f
			);

		if (USettings.optionswindow_animation)
			ImGui::GetForegroundDrawList()->AddRect(
				ImVec2(child_windowpos1.x - 1, child_windowpos1.y - 1),
				ImVec2(child_windowpos1.x + child_windowsize1.x + 1, child_windowpos1.y + child_windowsize1.y + 1),
				ImColor(255, 255, 255, (int)Animator.alpha), 12.0f
			);

		if (USettings.show_watermark) {
			ImVec2 screenSize = ImGui::GetIO().DisplaySize;
			ImVec2 imageSize(128, 128); // Adjust size as needed
			ImVec2 pos(screenSize.x - imageSize.x - 10, 5); // 10px padding from edges
			ImGui::GetBackgroundDrawList()->AddImage((void*)pTexture, pos, ImVec2(pos.x + imageSize.x, pos.y + imageSize.y));
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();
		ImGui::End();
		SetFocus(overlayWindow);
	}

	if (USettings.ArmorBar_ESP || USettings.GunName_Esp || USettings.SnaplLine_Esp || USettings.Box_ESP || USettings.Name_ESP || USettings.Bone_Esp || USettings.Distance_Esp || USettings.HealthBar_ESP || USettings.CornerBox_ESP || USettings.FilledBox_Esp || USettings.Box3D_Esp || USettings.radar_hack || USettings.ShowEyeRay) {
		DWORD64 LocalPlayer = E->GetLocal();
		if (!LocalPlayer) return;

		if (USettings.radar_hack) {
			auto drawList = ImGui::GetBackgroundDrawList();
			const ImVec2 center(USettings.center.x, USettings.center.y);
			
			// Cache trig calculations
			static const float cos225 = cosf(225.0f * (M_PI / 180.0f));
			static const float sin225 = sinf(225.0f * (M_PI / 180.0f));
			static const float cos315 = cosf(315.0f * (M_PI / 180.0f));
			static const float sin315 = sinf(315.0f * (M_PI / 180.0f));

			// Draw radar elements
			drawList->AddCircleFilled(center, USettings.radarRadius, IM_COL32(0, 0, 0, 128));
			drawList->AddCircleFilled(center, 4.0f, IM_COL32(0, 255, 0, 255));

			// Draw lines using cached trig values
			const float radius = USettings.radarRadius;
			drawList->AddLine(
				center,
				ImVec2(center.x + cos225 * radius, center.y + sin225 * radius),
				IM_COL32(0, 255, 0, 180)
			);
			drawList->AddLine(
				center,
				ImVec2(center.x + cos315 * radius, center.y + sin315 * radius),
				IM_COL32(0, 255, 0, 180)
			);
		}

		for (int i = 0; i < 64; i++) {
			DWORD64 CurrentController = E->GetEntInfo(i);
			DWORD64 CurrentPawn = E->GetEnt(i);

			if (!CurrentPawn || !CurrentController || CurrentPawn == LocalPlayer)
				continue;

			int health = E->GetHealth(CurrentPawn);
			if (health < 1 || health > 100)
				continue;

			int TeamNum = E->GetTeam(CurrentPawn);
			int LTeamNum = E->GetTeam(LocalPlayer);

			float distance = E->GetPos(CurrentPawn).dist(E->GetPos(LocalPlayer)) / 100;
			if (distance > USettings.ESP_Distance)
				continue;

			DWORD64 CameraServices, sceneNode, BoneArray, CurrentWeapon;
			if (!read<DWORD64>(LocalPlayer, C_BasePlayerPawn::m_pCameraServices, CameraServices) ||
				!read<DWORD64>(CurrentPawn, C_BaseEntity::m_pGameSceneNode, sceneNode) ||
				!read<DWORD64>(sceneNode, CSkeletonInstance::m_modelState + 0x80, BoneArray) ||
				!read<DWORD64>(CurrentPawn, C_CSPlayerPawnBase::m_pClippingWeapon, CurrentWeapon))
				continue;

			Vector3 pos = E->GetBonePos3D(BoneArray, 28);
			Vector2 feetpos = PosToScreen(pos);
			Vector3 pos1 = pos;

			int fFlag;
			if (!read<int>(CurrentPawn, C_BaseEntity::m_fFlags, fFlag))
				continue;

			pos1.z += E->IsCrouching(fFlag) ? 55 : 70;
			Vector2 headpos = PosToScreen(pos1);
			float height = feetpos.y - headpos.y;

			bool isVisible = feetpos.x > 0 && feetpos.y > 0 && feetpos.x < X_Screen && feetpos.y < Y_Screen;
			bool showSquad = TeamNum == LTeamNum && USettings.Show_Squad;
			bool showEnemy = TeamNum != LTeamNum && USettings.Show_Enemy;

			if (USettings.radar_hack && showEnemy) {
				TopDownToScreen(pos, LocalPlayer);
			}

			if (!isVisible) continue;

			float boxWidth = E->IsCrouching(fFlag) ? (height + height * 0.25) / 4 : height / 4;

			if (USettings.FilledBox_Esp && (showSquad || showEnemy)) {
				ImColor color = TeamNum == LTeamNum ? USettings.Squad_FilledBox_Esp_Color : USettings.Enemy_FilledBox_Esp_Color;
				DrawFilledRect(feetpos, height, boxWidth, color);
			}

			if (USettings.SnaplLine_Esp && (showSquad || showEnemy)) {
				ImColor color = TeamNum == LTeamNum ? USettings.Squad_SnaplLine_Esp_Color : USettings.Enemy_SnaplLine_Esp_Color;
				Vector2 endPoint = USettings.SnaplLine_Esp_End_Point ? headpos : feetpos;
				DrawLine(USettings.SnaplLine_Esp_Start_Point, endPoint, color, USettings.SnaplLine_Esp_Thickness, true);
			}

			if (USettings.Box_ESP && (showSquad || showEnemy)) {
				ImColor color = TeamNum == LTeamNum ? USettings.Squad_Box_Esp_Color : USettings.Enemy_Box_Esp_Color;
				drawbox(feetpos, height, boxWidth, color, USettings.Box_Esp_Thickness);
			}

			if (USettings.Bone_Esp && (showSquad || showEnemy)) {
				ImColor color = TeamNum == LTeamNum ? USettings.Squad_Bone_Esp_Color : USettings.Enemy_Bone_Esp_Color;
				DrawBones(BoneArray, USettings.Bone_Esp_Thickness, color);
			}

			if (USettings.HealthBar_ESP && (showSquad || showEnemy)) {
				float healthRatio = (float)health / E->GetMaxHealth(CurrentPawn);
				float barWidth = E->IsCrouching(fFlag) ? (height + height * 0.25) / 3.6f : height / 3.6f;
				drawhealthbar(feetpos, height * healthRatio, barWidth, HealthBarColor(CurrentPawn), USettings.HealthBar_Esp_Thickness);
			}

			if (USettings.ArmorBar_ESP && (showSquad || showEnemy)) {
				int armor = E->GetArmor(CurrentPawn);
				float armorRatio = (float)armor / E->GetMaxHealth(CurrentPawn);
				float barWidth = E->IsCrouching(fFlag) ? (height + height * 0.25) / 3.6f : height / 3.6f;
				drawarmorbar(feetpos, height * armorRatio, barWidth, ArmorBarColor(CurrentPawn), USettings.ArmorBar_Esp_Thickness);
			}

			if (USettings.CornerBox_ESP && (showSquad || showEnemy)) {
				ImColor color = TeamNum == LTeamNum ? USettings.Squad_CornerBox_Esp_Color : USettings.Enemy_CornerBox_Esp_Color;
				float cornerWidth = E->IsCrouching(fFlag) ? (height + height * 0.25) / 2 : height / 2;
				DrawCornerEsp(cornerWidth, height, feetpos, color, USettings.Box_CornerEsp_Thickness);
			}

			if ((USettings.Name_ESP || USettings.Distance_Esp || USettings.GunName_Esp) && (showSquad || showEnemy)) {
				Vector3 topPos = pos;
				Vector3 bottomPos = pos;
				
				topPos.z += E->IsCrouching(fFlag) ? 55 : 75;
				bottomPos.z -= 10;
				
				float distance = E->GetPos(LocalPlayer).dist(E->GetBonePos3D(BoneArray, 28)) / 100;
				
				Vector2 topScreen = PosToScreen(topPos);
				Vector2 bottomScreen = PosToScreen(bottomPos);

				bool topVisible = topScreen.x > 0 && topScreen.y > 0 && topScreen.x < X_Screen && topScreen.y < Y_Screen;
				bool bottomVisible = bottomScreen.x > 0 && bottomScreen.y > 0 && bottomScreen.x < X_Screen && bottomScreen.y < Y_Screen;

				if (USettings.Name_ESP && topVisible) {
					char names[128];
					ProcessMgr.ReadMemory(CurrentController + CBasePlayerController::m_iszPlayerName, names, 128);
					ImColor color = TeamNum == LTeamNum ? USettings.Squad_Name_ESP_Color : USettings.Enemy_Name_ESP_Color;
					
					Vector2 namePos = topScreen;
					namePos.y -= 15;
					DrawString(namePos, color, USettings.Text_Size, names);
				}

				// Draw bottom info
				if (bottomVisible) {
					float yOffset = 0;
					
					if (USettings.Distance_Esp) {
						char distStr[32];
						sprintf_s(distStr, "[%0.fm]", distance);
						ImColor distColor = TeamNum == LTeamNum ? USettings.Squad_Distance_Esp_Color : USettings.Enemy_Distance_Esp_Color;
						Vector2 distPos = bottomScreen;
						distPos.y += yOffset;
						DrawString(distPos, distColor, USettings.Text_Size, distStr);
						yOffset += 15;
					}

					if (USettings.GunName_Esp) {
						short weaponDefinitionIndex;
						if (read<short>(CurrentWeapon, (C_EconEntity::m_AttributeManager + C_AttributeContainer::m_Item + C_EconItemView::m_iItemDefinitionIndex), weaponDefinitionIndex)) {
							auto it = weaponMap.find(weaponDefinitionIndex);
							if (it != weaponMap.end()) {
								ImColor gunColor = TeamNum == LTeamNum ? USettings.Squad_GunName_Color : USettings.Enemy_GunName_Color;
								Vector2 gunPos = bottomScreen;
								gunPos.y += yOffset;
								DrawString(gunPos, gunColor, USettings.Text_Size, it->second.c_str());
							}
						}
					}
				}
			}

			if (USettings.Box3D_Esp && (showSquad || showEnemy)) {
				ImColor color = TeamNum == LTeamNum ? USettings.Squad_Box3D_Esp_Color : USettings.Enemy_Box3D_Esp_Color;
				Draw3DBox(CurrentPawn, color, USettings.Box3D_Esp_Thickness, USettings.Box3D_Width, fFlag);
			}

			if (USettings.ShowEyeRay && (showSquad || showEnemy)) {
				ShowLosLine(BoneArray, CurrentPawn, USettings.length, USettings.EyeRay_Color, USettings.EyeRay_Thickness);
			}
		}
	}

	if (USettings.Aimbot) {
		DWORD64 local = E->GetLocal();
		if (local != NULL && E->GetHealth(local) > 0 && E->GetHealth(local) < 101) {
			if (USettings.ShowFov || USettings.FilledCircle) {
				ImVec2 screenCenter{ X_Screen / 2, Y_Screen / 2 };
				if (USettings.ShowFov) {
					DrawCircle({ X_Screen / 2, Y_Screen / 2 }, USettings.AimFov, USettings.FovThickness, USettings.FovColor);
				}
				if (USettings.FilledCircle) {
					ImGui::GetBackgroundDrawList()->AddCircleFilled(screenCenter, USettings.AimFov, USettings.FilledCircleColor);
				}
			}

			if (USettings.ShowTarget) {
				int closestEnemyIndex = FindClosestEnemy();
				if (closestEnemyIndex != 100) {
					DWORD64 ent = E->GetEnt(closestEnemyIndex);
					if (ent != NULL) {
						DWORD64 sceneNode, boneArray;
						if (read<DWORD64>(ent, C_BaseEntity::m_pGameSceneNode, sceneNode) &&
							read<DWORD64>(sceneNode, CSkeletonInstance::m_modelState + 0x80, boneArray)) {

							Vector3 aimPos = USettings.Head_Target ? 
								E->GetBonePos3D(boneArray, bones::head) :
								E->GetBonePos3D(boneArray, bones::spine);

							Vector2 screenPos = PosToScreen(aimPos);
							DrawLine({X_Screen / 2, Y_Screen / 2}, screenPos, USettings.TargetColor, USettings.TargetThickness, true);
						}
					}
				}
			}
		}
	}

	if (USettings.Night_Mode) {
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(X_Screen, Y_Screen), ImColor(0, 0, 0, 90));
	}
	if (USettings.FullBright_Mode) {
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(X_Screen, Y_Screen), ImColor(255, 255, 255, 80));
	}

	if (USettings.DrawCrosshair && E->GetHealth(E->GetLocal()) > 0) {
		bool shouldDraw = !USettings.whennotaiming || !GetAsyncKeyState(VK_RBUTTON);
		if (shouldDraw) {
			Vector2 screenCenter{ X_Screen / 2, Y_Screen / 2 };
			float size = USettings.Crosshair_size;
			float thickness = USettings.Crosshair_thickness;
			ImColor color = USettings.Crosshair_Color;

			if (USettings.Cross) {
				DrawLine(
					{screenCenter.x, screenCenter.y - size - 0.5f},
					{screenCenter.x, screenCenter.y + size - 0.5f},
					color, thickness, false
				);
				DrawLine(
					{screenCenter.x - size - 0.5f, screenCenter.y},
					{screenCenter.x + size - 0.5f, screenCenter.y},
					color, thickness, false
				);
			}
			if (USettings.circle) {
				DrawCircle(screenCenter, size, thickness, color);
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

void CleanupImGui() {
	isInitialized = false;
	isMenuVisible = false;
	
	Sleep(100);

	// Cleanup ImGui
	if (ImGui::GetCurrentContext() != nullptr) {
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}

void mainLoop() {
	static MSG msg;
	static RECT oldRect;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT && GetWindow(targetWindow, GW_HWNDPREV) && !GetAsyncKeyState(VK_DELETE)) {
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
		SetWindowDisplayAffinity(overlayWindow, USettings.OBSBypass ? WDA_EXCLUDEFROMCAPTURE : WDA_NONE);

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
		if (!isMenuVisible) {
			GetClients();
			TriggerBot();
			if (USettings.Aimbot) {
				int i = FindClosestEnemy();
				if (i == 100)
					continue;
				Aimbot(i);
			}
		}
	}

	CleanupImGui();

	if (pDevice) {
		pDevice->Release();
		pDevice = nullptr;
	}
	
	if (pDirect) {
		pDirect->Release();
		pDirect = nullptr;
	}

	// Cleanup window
	if (overlayWindow) {
		DestroyWindow(overlayWindow);
		UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
		overlayWindow = nullptr;
	}

	ExitProcess(0);
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
	gD3DPresentParams.BackBufferFormat = D3DFMT_A8R8G8B8;
	gD3DPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	gD3DPresentParams.MultiSampleType = D3DMULTISAMPLE_NONE;
	gD3DPresentParams.EnableAutoDepthStencil = TRUE;
	gD3DPresentParams.AutoDepthStencilFormat = D3DFMT_D16;

	if (pDirect->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, overlayWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &gD3DPresentParams, 0, &pDevice) != D3D_OK) {
		return FALSE;
	}
	return TRUE;
}

int main(int argc, char* argv[]) {
	std::string randomTitle = generateRandomString(generateRandomInt(10, 30));
	SetConsoleTitleA(randomTitle.c_str());
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