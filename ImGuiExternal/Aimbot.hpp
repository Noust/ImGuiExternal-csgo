#pragma once
#include "include.h"
#include <math.h>

void AimBot(DWORD64 local, Vector3 LocalPos, Vector3 AimPos)
{
	int ShotsFired;
	float Yaw, Pitch;
	float Distance, Norm;
	Vector3 OppPos;
	Vector3 Angles;

	OppPos = AimPos - LocalPos;

	Distance = sqrt(pow(OppPos.x, 2) + pow(OppPos.y, 2));

	Yaw = atan2f(OppPos.y, OppPos.x) * 57.295779513 - E->GetViewAnles().y;
	Pitch = -atan(OppPos.z / Distance) * 57.295779513 - E->GetViewAnles().x;
	Norm = sqrt(pow(Yaw, 2) + pow(Pitch, 2));

	Yaw = Yaw * (1 - USettings::Smooth) + E->GetViewAnles().y;
	Pitch = Pitch * (1 - USettings::Smooth) + E->GetViewAnles().x;

	// Recoil control
	if (!read<int>(local + C_CSPlayerPawn::m_iShotsFired, ShotsFired))
		return;

	if (ShotsFired > USettings::RCSBullet)
	{
		Vector2 PunchAngle;
		C_UTL_VECTOR AimPunchCache;
		if (read<C_UTL_VECTOR>(local + C_CSPlayerPawn::m_aimPunchCache, AimPunchCache)) {
			if (AimPunchCache.Count <= 0 && AimPunchCache.Count > 0xFFFF)
				return;
			if (!ProcessMgr.ReadMemory<Vector2>(AimPunchCache.Data + (AimPunchCache.Count - 1) * sizeof(Vector3), PunchAngle))
				return;

			Yaw = Yaw - PunchAngle.y * USettings::RCSScale.x;
			Pitch = Pitch - PunchAngle.x * USettings::RCSScale.y;
		}
	}
	Angles = clampAngles({ Pitch,Yaw });
	E->SetViewAngles(Angles.y, Angles.x);
}

void setRCS() {
	USettings::RCSScale = { 0.7f + (0.7f - USettings::Smooth) * 2,0.58f + (0.7f - USettings::Smooth) * 2 };
}