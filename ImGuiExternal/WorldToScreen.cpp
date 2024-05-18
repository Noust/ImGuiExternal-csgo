#include "include.h"

bool WorldToScreen(Vector3 pos, Vector2& posscreen, float matrix[16]) {
	Vector4 clc;
	clc.w = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

	if (clc.w < 0.1f)
		return false;

	clc.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
	clc.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
	clc.z = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];

	Vector3 N;
	N.x = clc.x / clc.w;
	N.y = clc.y / clc.w;
	N.z = clc.z / clc.w;

	posscreen.x = (X_Screen / 2 * N.x) + (N.x + X_Screen / 2);
	posscreen.y = -(Y_Screen / 2 * N.y) + (N.y + Y_Screen / 2);
	return true;
}

Vector2 PosToScreen(Vector3 pos) {
	Vector2 posscreen;
	float Matrix[16];
	memcpy(&Matrix, (PBYTE*)(client + ClientDll::dwViewMatrix), sizeof(Matrix));
	if (WorldToScreen(pos, posscreen, Matrix)) {
		return posscreen;
	}
	return { -1,-1 };
}