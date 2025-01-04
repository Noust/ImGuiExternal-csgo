#pragma once
#include "include.h"

Vector2 TopDownToScreen(const Vector3& pos);
bool WorldToScreen(const Vector3& Pos, Vector2& ToPos);
Vector2 PosToScreen(Vector3 pos);