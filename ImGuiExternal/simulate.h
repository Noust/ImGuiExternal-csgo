#pragma once
#include "include.h"

class sim {
public:
	void SimKey(int KeyID);
	void SimClick();
	void SimulateMouseMove(int dx, int dy);
};

sim* s;