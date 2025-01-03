#pragma once
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx9.h>
#include <ImGui/imgui_impl_win32.h>

void inputHandler();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#define clearVariable(x) if (x) { x->Release(); x = nullptr; delete x;}

std::string generateRandomString(int length) {
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string randomString;
    std::srand(std::time(0));
    for (int i = 0; i < length; ++i) {
        randomString += characters[std::rand() % characters.length()];
    }
    return randomString;
}

int generateRandomInt(int min, int max) {
    std::srand(std::time(0));
    return min + std::rand() % (max - min + 1);
}

DWORD getProcessID(std::string processName) {
    PROCESSENTRY32 processInfo;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    processInfo.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnapshot, &processInfo)) {
        do {
            if (!lstrcmpi(processInfo.szExeFile, processName.c_str())) {
                CloseHandle(hSnapshot);
                return processInfo.th32ProcessID;
            }
        } while (Process32Next(hSnapshot, &processInfo));
    }
    CloseHandle(hSnapshot);
    return 0;
}

typedef struct
{
	DWORD R;
	DWORD G;
	DWORD B;
	DWORD A;
}RGBA;

std::string stringToUTF8(const std::string& str) {
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);
	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
	std::string retStr(pBuf);
	delete[]pwBuf;
	delete[]pBuf;
	pwBuf = NULL;
	pBuf = NULL;
	return retStr;
}

void drawStrokeText(int x, int y, RGBA* color, const char* str) {
	ImFont a;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = stringToUTF8(utf_8_1);
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x - 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), utf_8_2.c_str());
}

void drawNewText(int x, int y, RGBA* color, const char* str) {
	ImFont a;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = stringToUTF8(utf_8_1);
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), utf_8_2.c_str());
}

void drawRect(int x, int y, int w, int h, RGBA* color, int thickness) {
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), 0, 0, thickness);
}

void drawFilledRect(int x, int y, int w, int h, RGBA* color) {
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), 0, 0);
}

void drawCircleFilled(int x, int y, int radius, RGBA* color) {
	ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}

void drawCircle(int x, int y, int radius, RGBA* color, int segments) {
	ImGui::GetForegroundDrawList()->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), segments);
}

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, RGBA* color, float thickne) {
	ImGui::GetForegroundDrawList()->AddTriangle(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), thickne);
}

void drawTriangleFilled(int x1, int y1, int x2, int y2, int x3, int y3, RGBA* color) {
	ImGui::GetForegroundDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}

void drawLine(int x1, int y1, int x2, int y2, RGBA* color, int thickness) {
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), thickness);
}

void drawCornerBox(int x, int y, int w, int h, int borderPx, RGBA* color) {
	drawFilledRect(x + borderPx, y, w / 3, borderPx, color);
	drawFilledRect(x + w - w / 3 + borderPx, y, w / 3, borderPx, color);
	drawFilledRect(x, y, borderPx, h / 3, color);
	drawFilledRect(x, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color);
	drawFilledRect(x + borderPx, y + h + borderPx, w / 3, borderPx, color);
	drawFilledRect(x + w - w / 3 + borderPx, y + h + borderPx, w / 3, borderPx, color);
	drawFilledRect(x + w + borderPx, y, borderPx, h / 3, color);
	drawFilledRect(x + w + borderPx, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color);
}


void DrawLine(Vector2 pos1, Vector2 pos2, ImColor color, float Thicknes, bool checkpoints) {
	if (checkpoints) {
		if (pos1.x >= 0 && pos1.y >= 0 && pos1.x <= X_Screen && pos1.y <= Y_Screen && pos2.x >= 0 && pos2.y >= 0 && pos2.x <= X_Screen && pos2.y <= Y_Screen) {
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos1.x, pos1.y), ImVec2(pos2.x, pos2.y), ImColor(0, 0, 0), Thicknes + 2);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos1.x, pos1.y), ImVec2(pos2.x, pos2.y), color, Thicknes);
		}
	}
	else {
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos1.x, pos1.y), ImVec2(pos2.x, pos2.y), ImColor(0, 0, 0), Thicknes + 2);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos1.x, pos1.y), ImVec2(pos2.x, pos2.y), color, Thicknes);
	}
}

void drawbox(Vector2 pos, float height, float width, ImColor color, float thickness) {
	DrawLine({ pos.x + width, pos.y }, { pos.x - width,pos.y }, color, thickness, false);
	DrawLine({ pos.x + width, pos.y }, { pos.x + width,pos.y - height }, color, thickness, false);
	DrawLine({ pos.x + width,pos.y - height }, { pos.x - width,pos.y - height }, color, thickness, false);
	DrawLine({ pos.x - width,pos.y - height }, { pos.x - width,pos.y }, color, thickness, false);
}

void DrawString(Vector2 pos, ImColor color, float divide, const char* text) {
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(pos.x - ImGui::CalcTextSize(text).x / 2, pos.y - ImGui::CalcTextSize(text).y / divide), color, text);
}

void drawhealthbar(Vector2 pos, float height, float width, ImColor color, float thickness) {
	DrawLine({ pos.x - width,pos.y - height }, { pos.x - width,pos.y }, color, thickness, false);
}

void drawarmorbar(Vector2 pos, float height, float width, ImColor color, float thickness) {
	DrawLine({ pos.x + width,pos.y - height }, { pos.x + width,pos.y }, color, thickness, false);
}

ImColor HealthBarColor(DWORD64 ent) {
	if (E->GetHealth(ent) <= E->GetMaxHealth(ent) && E->GetHealth(ent) > E->GetMaxHealth(ent) * 0.75)
		return ImColor(0, 255, 0);
	else if (E->GetHealth(ent) <= E->GetMaxHealth(ent) * 0.75 && E->GetHealth(ent) > E->GetMaxHealth(ent) * 0.5)
		return ImColor(85, 170, 0);
	else if (E->GetHealth(ent) <= E->GetMaxHealth(ent) * 0.5 && E->GetHealth(ent) > E->GetMaxHealth(ent) * 0.25)
		return ImColor(170, 85, 0);
	else if (E->GetHealth(ent) <= E->GetMaxHealth(ent) * 0.25f && E->GetHealth(ent) > 0)
		return ImColor(255, 0, 0);
	return ImColor(0, 0, 0, 0);
}

ImColor ArmorBarColor(DWORD64 ent) {
	if (E->GetArmor(ent) <= E->GetMaxHealth(ent) && E->GetArmor(ent) > E->GetMaxHealth(ent) * 0.75)
		return ImColor(0, 0, 255);
	else if (E->GetArmor(ent) <= E->GetMaxHealth(ent) * 0.75 && E->GetArmor(ent) > E->GetMaxHealth(ent) * 0.5)
		return ImColor(0, 0, 170);
	else if (E->GetArmor(ent) <= E->GetMaxHealth(ent) * 0.5 && E->GetArmor(ent) > E->GetMaxHealth(ent) * 0.25)
		return ImColor(0, 0, 85);
	else if (E->GetArmor(ent) <= E->GetMaxHealth(ent) * 0.25f && E->GetArmor(ent) > 0)
		return ImColor(0, 0, 0);
	return ImColor(0, 0, 0, 0);
}

void DrawFilledRect(Vector2 pos, float height, float width, ImColor color) {
	ImGui::GetBackgroundDrawList()->AddRectFilled({ pos.x - width,pos.y - height }, { pos.x + width, pos.y }, color);
}

void Draw3DBox(DWORD64 Ents, ImColor color, float Thickness, float width, int fFlags) {
	const Vector3 basePos = E->GetPos(Ents);

	Vector2 screenPos = PosToScreen(basePos);
	if (screenPos.x < 0 || screenPos.y < 0 || screenPos.x > X_Screen || screenPos.y > Y_Screen)
		return;

	const float height = (fFlags == CROUCHING) ? 55.0f : 70.0f;

	const float xPlus = basePos.x + width;
	const float xMinus = basePos.x - width;
	const float yPlus = basePos.y + width;
	const float yMinus = basePos.y - width;
	const float zTop = basePos.z + height;

	const Vector3 corners[8] = {
		{xPlus, yPlus, basePos.z},
		{xPlus, yMinus, basePos.z},
		{xMinus, yMinus, basePos.z},
		{xMinus, yPlus, basePos.z},
		{xPlus, yPlus, zTop},
		{xPlus, yMinus, zTop},
		{xMinus, yMinus, zTop},
		{xMinus, yPlus, zTop}
	};

	Vector2 screenCorners[8];
	for (int i = 0; i < 8; i++) {
		screenCorners[i] = PosToScreen(corners[i]);
	}

	// Draw lines with minimal calculations
	// Bottom square
	for (int i = 0; i < 4; i++) {
		int next = (i + 1) & 3; // Faster than % 4
		DrawLine(screenCorners[i], screenCorners[next], color, Thickness, true);
	}

	// Top square
	for (int i = 4; i < 8; i++) {
		int next = 4 + ((i + 1) & 3); // Faster than % 4
		DrawLine(screenCorners[i], screenCorners[next], color, Thickness, true);
	}

	// Vertical lines
	for (int i = 0; i < 4; i++) {
		DrawLine(screenCorners[i], screenCorners[i + 4], color, Thickness, true);
	}
}

void DrawCircle(Vector2 pos, int radious, int thickness, ImColor color) {
	ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(pos.x, pos.y), radious, ImColor(0, 0, 0), 0, thickness + 2);
	ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(pos.x, pos.y), radious, color, 0, thickness);
}

void DrawCornerEsp(float W, float H, Vector2 pos, ImColor color, int thickness) {
	float lineW = (W / 5);
	float lineH = (H / 6);

	//outline
	DrawLine({ pos.x - W / 2, pos.y - H }, { pos.x - W / 2 + lineW, pos.y - H }, color, thickness, false);//top left
	DrawLine({ pos.x - W / 2, pos.y - H }, { pos.x - W / 2, pos.y - H + lineH }, color, thickness, false);
	DrawLine({ pos.x - W / 2, pos.y - lineH }, { pos.x - W / 2, pos.y }, color, thickness, false); //bot left
	DrawLine({ pos.x - W / 2, pos.y }, { pos.x - W / 2 + lineW, pos.y }, color, thickness, false);
	DrawLine({ pos.x + W / 2 - lineW, pos.y - H }, { pos.x + W / 2, pos.y - H }, color, thickness, false); // top right
	DrawLine({ pos.x + W / 2, pos.y - H }, { pos.x + W / 2, pos.y - H + lineH }, color, thickness, false);
	DrawLine({ pos.x + W / 2, pos.y - lineH }, { pos.x + W / 2, pos.y }, color, thickness, false); // bot right
	DrawLine({ pos.x + W / 2 - lineW, pos.y }, { pos.x + W / 2, pos.y }, color, thickness, false);
}

void DrawBones(DWORD64 BoneAddr, int thickness, ImColor color) {
	const std::pair<int, int> bonePairs[] = {
		{bones::head, bones::neck},
		{bones::neck, bones::spine},
		{bones::spine, bones::spine_1},
		{bones::spine_1, bones::hip},
		{bones::neck, bones::left_shoulder},
		{bones::left_shoulder, bones::left_arm},
		{bones::left_arm, bones::left_hand},
		{bones::neck, bones::right_shoulder},
		{bones::right_shoulder, bones::right_arm},
		{bones::right_arm, bones::right_hand},
		{bones::hip, bones::left_hip},
		{bones::left_hip, bones::left_knee},
		{bones::left_knee, bones::left_feet},
		{bones::hip, bones::right_hip},
		{bones::right_hip, bones::right_knee},
		{bones::right_knee, bones::right_feet}
	};

	Vector2 headPos = E->GetBonePos(BoneAddr, bones::head);

	if (headPos.x > 0 || headPos.y > 0 || headPos.x < X_Screen || headPos.y < Y_Screen) {
		float distanceToHead = E->GetCameraPos().dist(E->GetBonePos3D(BoneAddr, bones::head));
		float headRadius = 43 * (100 / distanceToHead);
		DrawCircle(headPos, headRadius, thickness, color);
	}

	Vector2 startPos, endPos;
	for (const auto& [bone1, bone2] : bonePairs) {
		startPos = E->GetBonePos(BoneAddr, bone1);
		endPos = E->GetBonePos(BoneAddr, bone2);
		DrawLine(startPos, endPos, color, thickness, true);
	}
}

struct animator {
	float alpha;
	bool tick;
	float speed;
};