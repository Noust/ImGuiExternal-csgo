#pragma once
#include "include.h"

void TopDownToScreen(const Vector3& pos, DWORD64 local);
bool WorldToScreen(const Vector3& Pos, Vector2& ToPos);
Vector2 PosToScreen(Vector3 pos);