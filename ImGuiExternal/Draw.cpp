#include "include.h"

void DrawLine(Vector2 pos1, Vector2 pos2, ImColor color, float Thicknes, bool checkpoints) {
	if (checkpoints) {
		if (pos1.x >= 0 && pos1.y >= 0 && pos1.x <= X_Screen && pos1.y <= Y_Screen && pos2.x >= 0 && pos2.y >= 0 && pos2.x <= X_Screen && pos2.y <= Y_Screen) {
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos1.x, pos1.y), ImVec2(pos2.x, pos2.y), ImColor(0, 0, 0), Thicknes + 2);
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos1.x, pos1.y), ImVec2(pos2.x, pos2.y), color, Thicknes);
		}
	}
	else {
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos1.x, pos1.y), ImVec2(pos2.x, pos2.y), ImColor(0, 0, 0), Thicknes + 2);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos1.x, pos1.y), ImVec2(pos2.x, pos2.y), color, Thicknes);
	}
}

void drawbox(Vector2 pos, float height, float width, ImColor color, float thickness) {
	DrawLine({ pos.x + width, pos.y }, { pos.x - width,pos.y }, color, thickness, false);
	DrawLine({ pos.x + width, pos.y }, { pos.x + width,pos.y - height }, color, thickness, false);
	DrawLine({ pos.x + width,pos.y - height }, { pos.x - width,pos.y - height }, color, thickness, false);
	DrawLine({ pos.x - width,pos.y - height }, { pos.x - width,pos.y }, color, thickness, false);
}

void DrawString(Vector2 pos, ImColor color, float divide, const char* text) {
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(pos.x - ImGui::CalcTextSize(text).x / 2, pos.y - ImGui::CalcTextSize(text).y / divide), color, text);
}

void drawhealthbar(Vector2 pos, float height, float width, ImColor color, float thickness) {
	DrawLine({ pos.x - width,pos.y - height }, { pos.x - width,pos.y }, color, thickness, false);
}

void drawarmorbar(Vector2 pos, float height, float width, ImColor color, float thickness) {
	DrawLine({ pos.x + width,pos.y - height }, { pos.x + width,pos.y }, color, thickness, false);
}

ImColor HealthBarColor(DWORD64 ent) {
	if (E->GetHealth(ent) <= E->GetMaxHealth(ent) && E->GetHealth(ent) > E->GetMaxHealth(ent) * 0.75)
		return ImColor(0, 255, 0);
	else if (E->GetHealth(ent) <= E->GetMaxHealth(ent) * 0.75 && E->GetHealth(ent) > E->GetMaxHealth(ent) * 0.5)
		return ImColor(85, 170, 0);
	else if (E->GetHealth(ent) <= E->GetMaxHealth(ent) * 0.5 && E->GetHealth(ent) > E->GetMaxHealth(ent) * 0.25)
		return ImColor(170, 85, 0);
	else if (E->GetHealth(ent) <= E->GetMaxHealth(ent) * 0.25f && E->GetHealth(ent) > 0)
		return ImColor(255, 0, 0);
	return ImColor(0, 0, 0, 0);
}

ImColor ArmorBarColor(DWORD64 ent) {
	if (E->GetArmor(ent) <= E->GetMaxHealth(ent) && E->GetArmor(ent) > E->GetMaxHealth(ent) * 0.75)
		return ImColor(0, 0, 255);
	else if (E->GetArmor(ent) <= E->GetMaxHealth(ent) * 0.75 && E->GetArmor(ent) > E->GetMaxHealth(ent) * 0.5)
		return ImColor(0, 0, 170);
	else if (E->GetArmor(ent) <= E->GetMaxHealth(ent) * 0.5 && E->GetArmor(ent) > E->GetMaxHealth(ent) * 0.25)
		return ImColor(0, 0, 85);
	else if (E->GetArmor(ent) <= E->GetMaxHealth(ent) * 0.25f && E->GetArmor(ent) > 0)
		return ImColor(0, 0, 0);
	return ImColor(0, 0, 0, 0);
}

void DrawFilledRect(Vector2 pos, float height, float width, ImColor color) {
	ImGui::GetBackgroundDrawList()->AddRectFilled({ pos.x - width,pos.y - height }, { pos.x + width, pos.y }, color);
}

void Draw3DBox(DWORD64 Ents, ImColor color, float Thickness, float width, int fFlags) {
	Vector3 pos1 = E->GetPos(Ents);
	Vector3 pos2 = E->GetPos(Ents);
	Vector3 pos3 = E->GetPos(Ents);
	Vector3 pos4 = E->GetPos(Ents);
	Vector3 pos5 = E->GetPos(Ents);
	Vector3 pos6 = E->GetPos(Ents);
	Vector3 pos7 = E->GetPos(Ents);
	Vector3 pos8 = E->GetPos(Ents);

	//Bottom Points
	pos1.x += width;
	pos1.y += width;
	pos2.x += width;
	pos2.y -= width;
	pos3.x -= width;
	pos3.y -= width;
	pos4.x -= width;
	pos4.y += width;

	//Top Points
	pos5.x += width;
	pos5.y += width;
	pos6.x += width;
	pos6.y -= width;
	pos7.x -= width;
	pos7.y -= width;
	pos8.x -= width;
	pos8.y += width;

	if (fFlags == CROUCHING) {
		pos8.z += 55;
		pos5.z += 55;
		pos6.z += 55;
		pos7.z += 55;
	}
	else {
		pos8.z += 70;
		pos5.z += 70;
		pos6.z += 70;
		pos7.z += 70;
	}

	//to screen
	Vector2 posscreen1 = PosToScreen(pos1);
	Vector2 posscreen2 = PosToScreen(pos2);
	Vector2 posscreen3 = PosToScreen(pos3);
	Vector2 posscreen4 = PosToScreen(pos4);

	Vector2 posscreen5 = PosToScreen(pos5);
	Vector2 posscreen6 = PosToScreen(pos6);
	Vector2 posscreen7 = PosToScreen(pos7);
	Vector2 posscreen8 = PosToScreen(pos8);

	//Draw Top
	DrawLine(posscreen1, posscreen2, color, Thickness, true);
	DrawLine(posscreen2, posscreen3, color, Thickness, true);
	DrawLine(posscreen3, posscreen4, color, Thickness, true);
	DrawLine(posscreen4, posscreen1, color, Thickness, true);

	DrawLine(posscreen5, posscreen6, color, Thickness, true);
	DrawLine(posscreen6, posscreen7, color, Thickness, true);
	DrawLine(posscreen7, posscreen8, color, Thickness, true);
	DrawLine(posscreen8, posscreen5, color, Thickness, true);

	DrawLine(posscreen5, posscreen1, color, Thickness, true);
	DrawLine(posscreen6, posscreen2, color, Thickness, true);
	DrawLine(posscreen7, posscreen3, color, Thickness, true);
	DrawLine(posscreen8, posscreen4, color, Thickness, true);
}

void DrawCircle(Vector2 pos, int radious, int thickness, ImColor color) {
	ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(pos.x, pos.y), radious, ImColor(0, 0, 0), 0, thickness + 2);
	ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(pos.x, pos.y), radious, color, 0, thickness);
}

void DrawCornerEsp(float W, float H, Vector2 pos, ImColor color, int thickness) {
	float lineW = (W / 5);
	float lineH = (H / 6);

	//outline
	DrawLine({ pos.x - W / 2, pos.y - H }, { pos.x - W / 2 + lineW, pos.y - H }, color, thickness, false);//top left
	DrawLine({ pos.x - W / 2, pos.y - H }, { pos.x - W / 2, pos.y - H + lineH }, color, thickness, false);
	DrawLine({ pos.x - W / 2, pos.y - lineH }, { pos.x - W / 2, pos.y }, color, thickness, false); //bot left
	DrawLine({ pos.x - W / 2, pos.y }, { pos.x - W / 2 + lineW, pos.y }, color, thickness, false);
	DrawLine({ pos.x + W / 2 - lineW, pos.y - H }, { pos.x + W / 2, pos.y - H }, color, thickness, false); // top right
	DrawLine({ pos.x + W / 2, pos.y - H }, { pos.x + W / 2, pos.y - H + lineH }, color, thickness, false);
	DrawLine({ pos.x + W / 2, pos.y - lineH }, { pos.x + W / 2, pos.y }, color, thickness, false); // bot right
	DrawLine({ pos.x + W / 2 - lineW, pos.y }, { pos.x + W / 2, pos.y }, color, thickness, false);
}

void DrawBones(DWORD64 BoneAddr, int thickness, ImColor color) {
    const std::pair<int, int> bonePairs[] = {
        {bones::head, bones::neck},
        {bones::neck, bones::spine}, 
        {bones::spine, bones::spine_1},
        {bones::spine_1, bones::hip},
        {bones::neck, bones::left_shoulder},
        {bones::left_shoulder, bones::left_arm},
        {bones::left_arm, bones::left_hand},
        {bones::neck, bones::right_shoulder},
        {bones::right_shoulder, bones::right_arm},
        {bones::right_arm, bones::right_hand},
        {bones::hip, bones::left_hip},
        {bones::left_hip, bones::left_knee},
        {bones::left_knee, bones::left_feet},
        {bones::hip, bones::right_hip},
        {bones::right_hip, bones::right_knee},
        {bones::right_knee, bones::right_feet}
    };

    Vector2 headPos = E->GetBonePos(BoneAddr, bones::head);
    
	if (headPos.x > 0 || headPos.y > 0 || headPos.x < X_Screen || headPos.y < Y_Screen) {
		float distanceToHead = E->GetCameraPos().dist(E->GetBonePos3D(BoneAddr, bones::head));
		float headRadius = 43 * (100 / distanceToHead);
		DrawCircle(headPos, headRadius, thickness, color);
	}

    Vector2 startPos, endPos;
    for (const auto& [bone1, bone2] : bonePairs) {
        startPos = E->GetBonePos(BoneAddr, bone1);
        endPos = E->GetBonePos(BoneAddr, bone2);
        DrawLine(startPos, endPos, color, thickness, true);
    }
}