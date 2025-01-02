#include "include.h"

void GetClients() {
	if (USettings::fov_changer || USettings::BunnyHop || USettings::No_Flash) {
		DWORD64 LocalPlayer = E->GetLocal();
		if (LocalPlayer == NULL)
			return;

		int health = E->GetHealth(LocalPlayer);

		if (health < 1 || health > 100)
			return;

		DWORD64 CameraServices;
		if (!read<DWORD64>(LocalPlayer, C_BasePlayerPawn::m_pCameraServices, CameraServices))
			return;
		if (USettings::fov_changer) {
			int currentfov; read<int>(CameraServices, CCSPlayerBase_CameraServices::m_iFOV, currentfov);
			bool isscoped; read<bool>(LocalPlayer, C_CSPlayerPawn::m_bIsScoped, isscoped);
			if (!isscoped && currentfov != USettings::fov_value) {
				write<int>(CameraServices, CCSPlayerBase_CameraServices::m_iFOV, USettings::fov_value);
			}
		}
		if (USettings::BunnyHop) {
			int fFlag; read<int>(LocalPlayer, C_BaseEntity::m_fFlags, fFlag);
			E->bunnyHop(fFlag);
		}
		if (USettings::No_Flash) {
			E->noFlash(LocalPlayer);
		}
	}
}

void TriggerBot() {
	if (USettings::triggerbot) {

		if (!GetAsyncKeyState(USettings::THotKey))
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

		std::this_thread::sleep_for(std::chrono::milliseconds(USettings::triggerbot_delayms));
		s->SimClick();
	}
}

void Aimbot(int index) {
	if (USettings::Aimbot) {
		if (USettings::Head_Target || USettings::Body_Target) {
			DWORD64 LocalPlayer = E->GetLocal();
			if (LocalPlayer == NULL)
				return;

			int health = E->GetHealth(LocalPlayer);

			if (health < 1 || health > 100)
				return;

			if (USettings::Aimbot && GetAsyncKeyState(USettings::HotKey)) {
				DWORD64 CurrentPawn = E->GetEnt(index);
				DWORD64 sceneNode; read<DWORD64>(CurrentPawn, C_BaseEntity::m_pGameSceneNode, sceneNode);
				if (sceneNode == NULL)
					return;
				DWORD64 BoneArray; read<DWORD64>(sceneNode, CSkeletonInstance::m_modelState + 0x80, BoneArray);
				if (BoneArray == NULL)
					return;
				Vector3 aimpos;
				if (USettings::Head_Target)
					aimpos = E->GetBonePos3D(BoneArray, bones::head);
				if (USettings::Body_Target)
					aimpos = E->GetBonePos3D(BoneArray, bones::spine);
				aimpos.z -= 1.f;
				if (PosToScreen(aimpos).dist({ X_Screen / 2,Y_Screen / 2 }) < USettings::AimFov)
					AimBot(LocalPlayer, E->GetCameraPos(), aimpos);
			}
			setRCS();
		}
	}
}