#pragma once
#include "include.h"

inline Vector2 AngleToScreenOffset(float angleX, float angleY, float previousX, float previousY) {
	const float fov = 90.0f;
	int width = X_Screen;
	int height = Y_Screen;

	float fovRad = fov * (M_PI / 180.0f);

	float thetaX = (angleX - previousX) * (M_PI / 180.0f);
    float offsetX = (width * tan(thetaX)) / (2 * tan(fovRad / 2));

    float thetaY = (angleY - previousY) * (M_PI / 180.0f);
    float offsetY = (height * tan(thetaY)) / (2 * tan(fovRad / 2));

    return { -offsetX, offsetY };
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
    Vector2 screenOffset = AngleToScreenOffset(Angles.y, Angles.x, currentView.y, currentView.x);

    std::this_thread::sleep_for(std::chrono::microseconds(50));
    mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(screenOffset.x), static_cast<DWORD>(screenOffset.y), 0, 0);
}

void setRCS() {
    USettings.RCSScale = { 0.7f + (0.7f - USettings.Smooth) * 2,0.58f + (0.7f - USettings.Smooth) * 2 };
}

void RecoilControl() {
    if (!USettings.Recoil_Control)
        return;

    int ShotsFired;
    static int previousShots = 0;
    static Vector2 oldPunch = {0.0f, 0.0f};
    DWORD64 local = E->GetLocal();
    
    if (!read<int>(local, C_CSPlayerPawn::m_iShotsFired, ShotsFired) || 
        ShotsFired <= 1) {
        if (ShotsFired == 0 || ShotsFired < previousShots) {
            oldPunch = {0.0f, 0.0f};
        }
        previousShots = ShotsFired;
        return;
    }

    Vector2 currentView = E->GetViewAnles(local);
    
    C_UTL_VECTOR AimPunchCache;
    if (read<C_UTL_VECTOR>(local, C_CSPlayerPawn::m_aimPunchCache, AimPunchCache)) {
        if (AimPunchCache.Count > 0 && AimPunchCache.Count <= 0xFFFF) {
            Vector2 PunchAngle;
            if (ProcessMgr.ReadMemory<Vector2>(AimPunchCache.Data + (AimPunchCache.Count - 1) * sizeof(Vector3), PunchAngle)) {
                if (previousShots <= 1 && ShotsFired > 1) {
                    oldPunch = PunchAngle;
                }

                Vector2 deltaPunch;
                deltaPunch.x = (PunchAngle.x - oldPunch.x) * USettings.RRCSScale.x;
                deltaPunch.y = (PunchAngle.y - oldPunch.y) * USettings.RRCSScale.y;

                float pixelScale = 100;

                float moveX = deltaPunch.y * pixelScale;
                float moveY = -deltaPunch.x * pixelScale;

                if (ShotsFired > 30) {
                    moveY -= 0.16f;
                }

                if (abs(moveX) > 0.01f || abs(moveY) > 0.01f) {
                    mouse_event(MOUSEEVENTF_MOVE, 
                        static_cast<DWORD>(moveX), 
                        static_cast<DWORD>(moveY), 
                        0, 0);
                }
                
                oldPunch = PunchAngle;
            }
        }
    }
    
    previousShots = ShotsFired;
}