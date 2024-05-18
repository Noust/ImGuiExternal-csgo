#include "include.h"

void GetClients() {
	while (true) {
		if (USettings::fov_changer || USettings::BunnyHop) {
			DWORD64 LocalPlayer = E->GetLocal();
			if (LocalPlayer == NULL)
				continue;

			int health = E->GetHealth(LocalPlayer);

			if (health < 1 || health > 100)
				continue;

			DWORD64 CameraServices;
			if (!read<DWORD64>(LocalPlayer, C_BasePlayerPawn::m_pCameraServices, CameraServices))
				continue;
			if (USettings::fov_changer) {
				int currentfov; read<int>(CameraServices, CCSPlayerBase_CameraServices::m_iFOV, currentfov);
				bool isscoped; read<bool>(LocalPlayer, C_CSPlayerPawn::m_bIsScoped, isscoped);
				if (!isscoped && currentfov != USettings::fov_value) {
					write<int>(CameraServices, CCSPlayerBase_CameraServices::m_iFOV, USettings::fov_value);
				}
			}
			if (USettings::BunnyHop) {
				int fFlag; read<int>(LocalPlayer, C_BaseEntity::m_fFlags, fFlag);
				if (GetAsyncKeyState(VK_SPACE)) {
					if (fFlag == STANDING || fFlag == CROUCHING) {
						std::this_thread::sleep_for(std::chrono::milliseconds(20));
						write<int>(client, ClientDll::dwForceJump, PLUS_JUMP);
					}
					else {
						write<int>(client, ClientDll::dwForceJump, MINUS_JUMP);
					}
				}
			}
		}
		Sleep(5);
	}
}

void TriggerBot() {
	while (true) {
		if (USettings::triggerbot) {
			DWORD64 LocalPlayer = E->GetLocal();
			int team = E->GetTeam(LocalPlayer);
			int entindex; read<int>(LocalPlayer, C_CSPlayerPawnBase::m_iIDEntIndex, entindex);

			if (entindex == -1)
				continue;

			DWORD64 CurrentPawn = E->GetEntt(entindex);
			int entityTeam = E->GetTeam(CurrentPawn);

			if (team == entityTeam)
				continue;

			if (GetAsyncKeyState(VK_XBUTTON2)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(USettings::triggerbot_delayms));
				s->SimClick();
			}
		}
		Sleep(1);
	}
}