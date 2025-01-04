#include "include.h"
float Matrix[4][4]{};

Vector2 TopDownToScreen(const Vector3& pos) {
    DWORD64 LocalPlayer = E->GetLocal();
    Vector3 localPos = E->GetPos(LocalPlayer);
    Vector2 viewAngles = E->GetViewAnles();
    
    Vector3 relativePos = pos - localPos;

    float yawRad = -viewAngles.y * (M_PI / 180.0f);
    
    float rotatedX = -(relativePos.x * sin(yawRad) + relativePos.y * cos(yawRad));
    float rotatedY = -(relativePos.x * cos(yawRad) - relativePos.y * sin(yawRad));
    
    Vector2 screenPos;
    screenPos.x = USettings.center.x + (rotatedX * USettings.scale);
    screenPos.y = USettings.center.y + (rotatedY * USettings.scale);

    float distanceFromCenter = screenPos.dist(USettings.center);
    if (distanceFromCenter <= USettings.radarRadius) {
        ImGui::GetBackgroundDrawList()->AddCircleFilled(
            ImVec2(screenPos.x, screenPos.y),
            3.0f,
            ImColor(255, 0, 0, 255)
        );
    }
    
    return screenPos;
}


bool WorldToScreen(const Vector3& Pos, Vector2& ToPos)
{
	float View = 0.f;
	float SightX = X_Screen / 2, SightY = Y_Screen / 2;

	View = Matrix[3][0] * Pos.x + Matrix[3][1] * Pos.y + Matrix[3][2] * Pos.z + Matrix[3][3];

	if (View <= 0.01)
		return false;

	ToPos.x = SightX + (Matrix[0][0] * Pos.x + Matrix[0][1] * Pos.y + Matrix[0][2] * Pos.z + Matrix[0][3]) / View * SightX;
	ToPos.y = SightY - (Matrix[1][0] * Pos.x + Matrix[1][1] * Pos.y + Matrix[1][2] * Pos.z + Matrix[1][3]) / View * SightY;

	return true;
}

Vector2 PosToScreen(Vector3 pos) {
	Vector2 posscreen;
	if (ProcessMgr.ReadMemory(client + ClientDll::dwViewMatrix, Matrix, 64)) {
		if (WorldToScreen(pos, posscreen)) {
			return posscreen;
		}
	}
	return { -1,-1 };
}