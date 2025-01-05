#include "include.h"
#define listEntry2Offset(handle)  0x8 * ((handle & 0x7FFF) >> 9) + 0x10

DWORD64 Entitys::GetEnt(int index) {
    DWORD64 entitylist;
    if (!read<DWORD64>(client, ClientDll::dwEntityList, entitylist))
        return NULL;

    DWORD64 listentry;
    if (!read<DWORD64>(entitylist, 0x10, listentry))
        return NULL;

    DWORD64 CurrentController;
    if (!read<DWORD64>(listentry, (index * 0x78), CurrentController))
        return NULL;

    int pawnhandle;
    if (!read<int>(CurrentController, CCSPlayerController::m_hPlayerPawn, pawnhandle))
        return NULL;

    DWORD64 ListEntry2;
    if (!read<DWORD64>(entitylist, listEntry2Offset(pawnhandle), ListEntry2))
        return NULL;

    const DWORD64 currentPawnOffset = 0x78 * (pawnhandle & 0x1FF);
    DWORD64 CurrentPawn;
    if (!read<DWORD64>(ListEntry2, currentPawnOffset, CurrentPawn))
        return NULL;

    return CurrentPawn;
}

DWORD64 Entitys::GetEntt(int handle) {
    if (handle == NULL)
        return NULL;

    DWORD64 entitylist;
    if (!read<DWORD64>(client, ClientDll::dwEntityList, entitylist))
        return NULL;

    DWORD64 listentry;
    if (!read<DWORD64>(entitylist, 0x10, listentry))
        return NULL;

    DWORD64 ListEntry2;
    if (!read<DWORD64>(entitylist, listEntry2Offset(handle), ListEntry2))
        return NULL;

    DWORD64 CurrentPawn;
    if (!read<DWORD64>(ListEntry2, (0x78 * (handle & 0x1FF)), CurrentPawn))
        return NULL;

    return CurrentPawn;
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

Vector2 Entitys::GetViewAnles(DWORD64 addr) {
	Vector2 ViewAngles;
	if (!read<Vector2>(addr, C_CSPlayerPawnBase::m_angEyeAngles, ViewAngles))
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
	static bool jumping = false;
	
	if (flags == STANDING) {
		if (!jumping) {
			Sleep(7);
			// Presionar espacio
			INPUT input = { 0 };
			input.type = INPUT_KEYBOARD;
			input.ki.wVk = VK_SPACE;
			SendInput(1, &input, sizeof(INPUT));
			
			// Pequeña espera para asegurar que el juego registre la pulsación
			Sleep(10);
			
			// Soltar espacio
			input.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &input, sizeof(INPUT));
			
			jumping = true;
		}
	}
	else {
		jumping = false;
	}
}