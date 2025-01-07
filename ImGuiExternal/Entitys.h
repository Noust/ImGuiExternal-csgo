#pragma once
#include "include.h"
int JUMPING = 65664;
int STANDING = 65665;

int PLUS_JUMP = 65537;
int MINUS_JUMP = 256;

//Address of signature = engine2.dll + 0x001604F8
//"\x66\xC7\x44\x1F\x28", "xxxxx"
//"66 C7 44 1F 28"

class Entitys {
public:
	DWORD64 GetEnt(int index);
	DWORD64 GetEntt(int handle);
	DWORD64 GetLocal();
	DWORD64 GetEntInfo(int index);
	Vector3 GetPos(DWORD64 ent);
	int GetHealth(DWORD64 ent);
	int GetArmor(DWORD64 ent);
	int GetTeam(DWORD64 ent);
	int GetMaxHealth(DWORD64 ent);
	Vector2 GetBonePos(DWORD64 BoneAddr, int BoneId);
	Vector3 GetBonePos3D(DWORD64 BoneAddr, int BoneId);
	Vector2 GetViewAnles(DWORD64 addr);
	Vector3 GetCameraPos();
	void bunnyHop(int flags);
	bool IsCrouching(int flags);
};