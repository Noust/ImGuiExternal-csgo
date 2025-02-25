#include "include.h"

void GetClients() {
    if (!USettings.BunnyHop)
        return;

    DWORD64 LocalPlayer = E->GetLocal();
    if (!LocalPlayer)
        return;

    int health = E->GetHealth(LocalPlayer);
    if (health < 1 || health > 100) 
        return;

	if (!GetAsyncKeyState(VK_LMENU))
		return;

    if (USettings.BunnyHop) {
        int flags;
        if (read<int>(LocalPlayer, C_BaseEntity::m_fFlags, flags)) {
            E->bunnyHop(flags);
        }
    }
}

void TriggerBot() {
	if (USettings.triggerbot) {

		if (!GetAsyncKeyState(USettings.THotKey))
			return;

		DWORD64 LocalPlayer = E->GetLocal();

		int team = E->GetTeam(LocalPlayer);
		int entindex; read<int>(LocalPlayer, C_CSPlayerPawnBase::m_iIDEntIndex, entindex);

		if (entindex == -1)
			return;

		DWORD64 CurrentPawn = E->GetEntt(entindex);
		int entityTeam = E->GetTeam(CurrentPawn);

		if (team == entityTeam)
			return;

		int health = E->GetHealth(CurrentPawn);

		if (health < 1 || health > 100)
			return;

		std::this_thread::sleep_for(std::chrono::milliseconds(USettings.triggerbot_delayms));
		s->SimClick();
	}
}

void Aimbot(int index) {
	if (!USettings.Aimbot || (!USettings.Head_Target && !USettings.Body_Target))
		return;

	DWORD64 LocalPlayer = E->GetLocal();
	if (!LocalPlayer)
		return;

	int health = E->GetHealth(LocalPlayer);
	if (health < 1 || health > 100)
		return;

	if (GetAsyncKeyState(USettings.HotKey)) {
		DWORD64 CurrentPawn = E->GetEnt(index);
		DWORD64 sceneNode;
		if (!read<DWORD64>(CurrentPawn, C_BaseEntity::m_pGameSceneNode, sceneNode) || !sceneNode)
			return;

		DWORD64 BoneArray;
		if (!read<DWORD64>(sceneNode, CSkeletonInstance::m_modelState + 0x80, BoneArray) || !BoneArray)
			return;

		Vector3 aimpos = USettings.Head_Target ? 
			E->GetBonePos3D(BoneArray, bones::head) :
			E->GetBonePos3D(BoneArray, bones::spine);

		if (PosToScreen(aimpos).dist({ X_Screen / 2, Y_Screen / 2 }) < USettings.AimFov) {
			AimBot(LocalPlayer, E->GetCameraPos(), aimpos);
		}
	}

	/*setRCS();*/
}