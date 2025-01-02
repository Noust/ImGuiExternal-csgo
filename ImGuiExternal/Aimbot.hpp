#pragma once
#include "include.h"
#include <math.h>

void AimBot(Vector2 AimPos)
{
	Vector2 d_x_y = { AimPos.x - X_Screen / 2, AimPos.y - Y_Screen / 2 };
	s->SimulateMouseMove(d_x_y.x, d_x_y.y);
}