#pragma once
#include "include.h"

void DrawLine(Vector2 pos1, Vector2 pos2, ImColor color, float Thicknes, bool checkpoints);
void drawbox(Vector2 pos, float height, float width, ImColor color, float thickness);
void DrawString(Vector2 pos, ImColor color, float divide, const char* text);
void drawhealthbar(Vector2 pos, float height, float width, ImColor color, float thickness);
void drawarmorbar(Vector2 pos, float height, float width, ImColor color, float thickness);
void DrawFilledRect(Vector2 pos, float height, float width, ImColor color);
void Draw3DBox(DWORD64 Ents, ImColor color, float Thickness, float width);
ImColor HealthBarColor(DWORD64 ent);
ImColor ArmorBarColor(DWORD64 ent);
void DrawCircle(Vector2 pos, int radious, int thickness, ImColor color);
void DrawCornerEsp(float W, float H, Vector2 pos, ImColor color, int thickness);
void DrawBones(DWORD64 BoneAddr, int thickness, ImColor color);

struct animator {
	float alpha;
	bool tick;
	float speed;
};