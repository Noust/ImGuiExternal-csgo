#pragma once
#include "include.h"

struct Vector2 {
	float x, y;

	float dist(Vector2 V) {
		return sqrt(pow(x - V.x, 2) + pow(y - V.y, 2));
	}
};

struct Vector3 {
	float x, y, z;

	float dist(Vector3 V) {
		return sqrt(pow(x - V.x, 2) + pow(y - V.y, 2) + pow(z - V.z, 2));
	}
};

struct Vector4 {
	float x, y, z, w;

	float dist(Vector4 V) {
		return sqrt(pow(x - V.x, 2) + pow(y - V.y, 2) + pow(z - V.z, 2) + pow(w - V.w, 2));
	}
};