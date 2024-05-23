#include "include.h"


DWORD64 Entitys::GetEnt(int index) {
	DWORD64 entitylist; read<DWORD64>(client, ClientDll::dwEntityList, entitylist);
	DWORD64 listentry;
	if (read<DWORD64>(entitylist, 0x10, listentry)) {
		DWORD64 CurrentController;
		if (read<DWORD64>(listentry, (index * 0x78), CurrentController)) {
			int pawnhandle;
			if (read<int>(CurrentController, CCSPlayerController::m_hPlayerPawn, pawnhandle)) {
				DWORD64 ListEntry2; read<DWORD64>(entitylist, (0x8 * ((pawnhandle & 0x7FFF) >> 9) + 0x10), ListEntry2);
				DWORD64 CurrentPawn;
				if (read<DWORD64>(ListEntry2, (0x78 * (pawnhandle & 0x1FF)), CurrentPawn)) {
					return CurrentPawn;
				}
			}
		}
	}
	return NULL;
}

DWORD64 Entitys::GetEntt(int handle) {
	DWORD64 entitylist; read<DWORD64>(client, ClientDll::dwEntityList, entitylist);
	DWORD64 listentry;
	if (read<DWORD64>(entitylist, 0x10, listentry)) {
		if (handle != NULL) {
			DWORD64 ListEntry2; read<DWORD64>(entitylist, (0x8 * ((handle & 0x7FFF) >> 9) + 0x10), ListEntry2);
			DWORD64 CurrentPawn;
			if (read<DWORD64>(ListEntry2, (0x78 * (handle & 0x1FF)), CurrentPawn)) {
				return CurrentPawn;
			}
		}
	}
	return NULL;
}

DWORD64 Entitys::GetLocal() {
	DWORD64 local;
	if (!read<DWORD64>(client, ClientDll::dwLocalPlayerPawn, local))
		return NULL;
	return local;
}

DWORD64 Entitys::GetEntInfo(int index) {
	DWORD64 entitylist; read<DWORD64>(client, ClientDll::dwEntityList, entitylist);
	DWORD64 listentry;
	if (read<DWORD64>(entitylist, 0x10, listentry)) {
		DWORD64 CurrentController;
		if (read<DWORD64>(listentry, (index * 0x78), CurrentController)) {
			return CurrentController;
		}
	}
	return NULL;
}

Vector3 Entitys::GetPos(DWORD64 ent) {
	Vector3 pos;
	if (!read<Vector3>(ent, CBaseAnimGraph::m_vLastSlopeCheckPos, pos))
		return { 0,0,0 };
	return pos;
}

int Entitys::GetHealth(DWORD64 ent) {
	int health;
	if (!read<int>(ent, C_BaseEntity::m_iHealth, health))
		return 0;
	return health;
}

int Entitys::GetArmor(DWORD64 ent) {
	int Armor;
	if (!read<int>(ent, C_CSPlayerPawn::m_ArmorValue, Armor))
		return 0;
	return Armor;
}

int Entitys::GetTeam(DWORD64 ent) {
	int team;
	if (!read<int>(ent, C_BaseEntity::m_iTeamNum, team))
		return 0;
	return team;
}

int Entitys::GetMaxHealth(DWORD64 ent) {
	int MaxHealth;
	if (!read<int>(ent, C_BaseEntity::m_iMaxHealth, MaxHealth))
		return 0;
	return MaxHealth;
}

Vector2 Entitys::GetBonePos(DWORD64 BoneAddr, int BoneId) {
	Vector3 BonePos; read<Vector3>(BoneAddr, BoneId * 32, BonePos);
	return PosToScreen(BonePos);
}

Vector3 Entitys::GetBonePos3D(DWORD64 BoneAddr, int BoneId) {
	Vector3 BonePos; read<Vector3>(BoneAddr, BoneId * 32, BonePos);
	return BonePos;
}

bool Entitys::SetViewAngles(float Yaw, float Pitch) {
	Vector2 Angle{ Pitch,Yaw };

	if (!ProcessMgr.WriteMemory<Vector2>(client + ClientDll::dwViewAngles, Angle))
		return false;

	return true;
}

Vector2 Entitys::GetViewAnles() {
	Vector2 ViewAngles;
	if (!read<Vector2>(client, ClientDll::dwViewAngles, ViewAngles))
		return { 0,0 };
	return ViewAngles;
}

Vector3 Entitys::GetCameraPos() {
	Vector3 CameraPos;
	if (!read<Vector3>(GetLocal(), C_CSPlayerPawnBase::m_vecLastClipCameraPos, CameraPos))
		return { 0,0,0 };
	return CameraPos;
}

void Entitys::bunnyHop(int flags) {
	if (GetAsyncKeyState(VK_SPACE) && flags & bhopInAir) {
		Sleep(14);
		write<int>(client, ClientDll::dwForceJump, PLUS_JUMP);
	}
	else {
		write<int>(client, ClientDll::dwForceJump, MINUS_JUMP);
	}
}

void Entitys::noFlash(DWORD64 ent) {
	float flash = 0.f;
	float flashtime; 
	if (!read<float>(ent, C_CSPlayerPawnBase::m_flFlashBangTime, flashtime))
		return;
	if (flashtime > 0) {
		write<float>(ent, C_CSPlayerPawnBase::m_flFlashBangTime, flash);
	}
}