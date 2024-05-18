#pragma once
#include "include.h"

bool WorldToScreen(Vector3 pos, Vector2& posscreen, float Matrix[16]);
Vector2 PosToScreen(Vector3 pos);