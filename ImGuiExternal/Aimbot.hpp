#pragma once
#include "include.h"
#include <math.h>

inline ImVec2 AngleToScreenOffset(float angleX, float angleY, float previousX, float previousY) {
	const float fov = 90.0f;
	int width = X_Screen;
	int height = Y_Screen;

	float fovRad = fov * (M_PI / 180.0f);

	double thetaX = (angleX - previousX) * (M_PI / 180.0f);
	double offsetX = (width * tan(thetaX)) / (2 * tan(fovRad / 2));

	double thetaY = (angleY - previousY) * (M_PI / 180.0f);
	double offsetY = (height * tan(thetaY)) / (2 * tan(fovRad / 2));

	return ImVec2(-offsetX, offsetY);
}

void AimBot(DWORD64 local, Vector3 LocalPos, Vector3 AimPos) {
    Vector3 OppPos = AimPos - LocalPos;
    float Distance = sqrt(OppPos.x * OppPos.x + OppPos.y * OppPos.y);
    
    float Yaw = atan2f(OppPos.y, OppPos.x) * 57.295779513f - E->GetViewAnles(local).y;
    float Pitch = -atan(OppPos.z / Distance) * 57.295779513f - E->GetViewAnles(local).x;

    const float smoothFactor = 1.0f - USettings.Smooth;
    Vector2 currentView = E->GetViewAnles(local);
    Yaw = Yaw * smoothFactor + currentView.y;
    Pitch = Pitch * smoothFactor + currentView.x;

    int ShotsFired;
    if (read<int>(local, C_CSPlayerPawn::m_iShotsFired, ShotsFired) && ShotsFired > USettings.RCSBullet) {
        C_UTL_VECTOR AimPunchCache;
        if (read<C_UTL_VECTOR>(local, C_CSPlayerPawn::m_aimPunchCache, AimPunchCache)) {
            if (AimPunchCache.Count > 0 && AimPunchCache.Count <= 0xFFFF) {
                Vector2 PunchAngle;
                if (ProcessMgr.ReadMemory<Vector2>(AimPunchCache.Data + (AimPunchCache.Count - 1) * sizeof(Vector3), PunchAngle)) {
                    Yaw -= PunchAngle.y * USettings.RCSScale.x;
                    Pitch -= PunchAngle.x * USettings.RCSScale.y;
                }
            }
        }
    }

    Vector3 Angles = clampAngles({Pitch, Yaw});
    ImVec2 screenOffset = AngleToScreenOffset(Angles.y, Angles.x, currentView.y, currentView.x);

    std::this_thread::sleep_for(std::chrono::microseconds(50));
    mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(screenOffset.x), static_cast<DWORD>(screenOffset.y), 0, 0);
}

void setRCS() {
	USettings.RCSScale = { 0.7f + (0.7f - USettings.Smooth) * 2,0.58f + (0.7f - USettings.Smooth) * 2 };
}