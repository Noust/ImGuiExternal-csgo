#include "include.h"

/*USettings FManager::ReadSettings() {
    USettings opciones;
    FILE* archivo = fopen("sarilla.bin", "rb");
    fread(&opciones, sizeof(opciones), 1, archivo);
    fclose(archivo);
    return opciones;
}
void FManager::SaveSettings(USettings opciones) {
    FILE* archivo = fopen("sarilla.bin", "wb");
    fwrite(&opciones, sizeof(opciones), 1, archivo);
    fclose(archivo);
}
*/

void FManager::ReadSettings() {
    std::ifstream archivo("sarilla.bin", std::ios::binary);
    if (archivo.is_open()) {
        archivo.read(reinterpret_cast<char*>(&USettings::MenuWindow), sizeof(USettings::MenuWindow));
        archivo.read(reinterpret_cast<char*>(&USettings::triggerbot), sizeof(USettings::triggerbot));
        archivo.read(reinterpret_cast<char*>(&USettings::triggerbot_delayms), sizeof(USettings::triggerbot_delayms));
        archivo.read(reinterpret_cast<char*>(&USettings::Silent_Aim), sizeof(USettings::Silent_Aim));
        archivo.read(reinterpret_cast<char*>(&USettings::Head_Target), sizeof(USettings::Head_Target));
        archivo.read(reinterpret_cast<char*>(&USettings::Body_Target), sizeof(USettings::Body_Target));
        archivo.read(reinterpret_cast<char*>(&USettings::ShowFov), sizeof(USettings::ShowFov));
        archivo.read(reinterpret_cast<char*>(&USettings::AimbotFov), sizeof(USettings::AimbotFov));
        archivo.read(reinterpret_cast<char*>(&USettings::FovThickness), sizeof(USettings::FovThickness));
        archivo.read(reinterpret_cast<char*>(&USettings::FovColor), sizeof(USettings::FovColor));
        archivo.read(reinterpret_cast<char*>(&USettings::ShowTarget), sizeof(USettings::ShowTarget));
        archivo.read(reinterpret_cast<char*>(&USettings::TargetThickness), sizeof(USettings::TargetThickness));
        archivo.read(reinterpret_cast<char*>(&USettings::TargetColor), sizeof(USettings::TargetColor));
        archivo.read(reinterpret_cast<char*>(&USettings::FilledCircle), sizeof(USettings::FilledCircle));
        archivo.read(reinterpret_cast<char*>(&USettings::FilledCircleColor), sizeof(USettings::FilledCircleColor));
        archivo.read(reinterpret_cast<char*>(&USettings::fov_changer), sizeof(USettings::fov_changer));
        archivo.read(reinterpret_cast<char*>(&USettings::fov_value), sizeof(USettings::fov_value));
        archivo.read(reinterpret_cast<char*>(&USettings::oldfov_value), sizeof(USettings::oldfov_value));
        archivo.read(reinterpret_cast<char*>(&USettings::DrawCrosshair), sizeof(USettings::DrawCrosshair));
        archivo.read(reinterpret_cast<char*>(&USettings::whennotaiming), sizeof(USettings::whennotaiming));
        archivo.read(reinterpret_cast<char*>(&USettings::Crosshair_Color), sizeof(USettings::Crosshair_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::Crosshair_size), sizeof(USettings::Crosshair_size));
        archivo.read(reinterpret_cast<char*>(&USettings::Crosshair_thickness), sizeof(USettings::Crosshair_thickness));
        archivo.read(reinterpret_cast<char*>(&USettings::circle), sizeof(USettings::circle));
        archivo.read(reinterpret_cast<char*>(&USettings::Cross), sizeof(USettings::Cross));
        archivo.read(reinterpret_cast<char*>(&USettings::radar_hack), sizeof(USettings::radar_hack));
        archivo.read(reinterpret_cast<char*>(&USettings::Show_Enemy), sizeof(USettings::Show_Enemy));
        archivo.read(reinterpret_cast<char*>(&USettings::Show_Squad), sizeof(USettings::Show_Squad));
        archivo.read(reinterpret_cast<char*>(&USettings::HealthBar_Esp_Thickness), sizeof(USettings::HealthBar_Esp_Thickness));
        archivo.read(reinterpret_cast<char*>(&USettings::HealthBar_ESP), sizeof(USettings::HealthBar_ESP));
        archivo.read(reinterpret_cast<char*>(&USettings::ArmorBar_Esp_Thickness), sizeof(USettings::ArmorBar_Esp_Thickness));
        archivo.read(reinterpret_cast<char*>(&USettings::ArmorBar_ESP), sizeof(USettings::ArmorBar_ESP));
        archivo.read(reinterpret_cast<char*>(&USettings::Enemy_Box_Esp_Color), sizeof(USettings::Enemy_Box_Esp_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::Squad_Box_Esp_Color), sizeof(USettings::Squad_Box_Esp_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::Box_Esp_Thickness), sizeof(USettings::Box_Esp_Thickness));
        archivo.read(reinterpret_cast<char*>(&USettings::Box_ESP), sizeof(USettings::Box_ESP));
        archivo.read(reinterpret_cast<char*>(&USettings::Enemy_CornerBox_Esp_Color), sizeof(USettings::Enemy_CornerBox_Esp_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::Squad_CornerBox_Esp_Color), sizeof(USettings::Squad_CornerBox_Esp_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::Box_CornerEsp_Thickness), sizeof(USettings::Box_CornerEsp_Thickness));
        archivo.read(reinterpret_cast<char*>(&USettings::CornerBox_ESP), sizeof(USettings::CornerBox_ESP));
        archivo.read(reinterpret_cast<char*>(&USettings::Enemy_Name_ESP_Color), sizeof(USettings::Enemy_Name_ESP_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::Squad_Name_ESP_Color), sizeof(USettings::Squad_Name_ESP_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::Name_ESP), sizeof(USettings::Name_ESP));
        archivo.read(reinterpret_cast<char*>(&USettings::Enemy_Distance_Esp_Color), sizeof(USettings::Enemy_Distance_Esp_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::Squad_Distance_Esp_Color), sizeof(USettings::Squad_Distance_Esp_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::Distance_Esp), sizeof(USettings::Distance_Esp));
        archivo.read(reinterpret_cast<char*>(&USettings::Enemy_Bone_Esp_Color), sizeof(USettings::Enemy_Bone_Esp_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::Squad_Bone_Esp_Color), sizeof(USettings::Squad_Bone_Esp_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::Bone_Esp_Thickness), sizeof(USettings::Bone_Esp_Thickness));
        archivo.read(reinterpret_cast<char*>(&USettings::Bone_Esp), sizeof(USettings::Bone_Esp));
        archivo.read(reinterpret_cast<char*>(&USettings::SnaplLine_Esp_Start_Point), sizeof(USettings::SnaplLine_Esp_Start_Point));
        archivo.read(reinterpret_cast<char*>(&USettings::SnaplLine_Esp_End_Point), sizeof(USettings::SnaplLine_Esp_End_Point));
        archivo.read(reinterpret_cast<char*>(&USettings::Enemy_SnaplLine_Esp_Color), sizeof(USettings::Enemy_SnaplLine_Esp_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::Squad_SnaplLine_Esp_Color), sizeof(USettings::Squad_SnaplLine_Esp_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::SnaplLine_Esp_Thickness), sizeof(USettings::SnaplLine_Esp_Thickness));
        archivo.read(reinterpret_cast<char*>(&USettings::SnaplLine_Esp), sizeof(USettings::SnaplLine_Esp));
        archivo.read(reinterpret_cast<char*>(&USettings::Enemy_FilledBox_Esp_Color), sizeof(USettings::Enemy_FilledBox_Esp_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::Squad_FilledBox_Esp_Color), sizeof(USettings::Squad_FilledBox_Esp_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::FilledBox_Esp), sizeof(USettings::FilledBox_Esp));
        archivo.read(reinterpret_cast<char*>(&USettings::Enemy_GunName_Color), sizeof(USettings::Enemy_GunName_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::Squad_GunName_Color), sizeof(USettings::Squad_GunName_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::GunName_Esp), sizeof(USettings::GunName_Esp));
        archivo.read(reinterpret_cast<char*>(&USettings::Enemy_Box3D_Esp_Color), sizeof(USettings::Enemy_Box3D_Esp_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::Squad_Box3D_Esp_Color), sizeof(USettings::Squad_Box3D_Esp_Color));
        archivo.read(reinterpret_cast<char*>(&USettings::Box3D_Esp_Thickness), sizeof(USettings::Box3D_Esp_Thickness));
        archivo.read(reinterpret_cast<char*>(&USettings::Box3D_Esp), sizeof(USettings::Box3D_Esp));
        archivo.read(reinterpret_cast<char*>(&USettings::Box3D_Width), sizeof(USettings::Box3D_Width));
        archivo.read(reinterpret_cast<char*>(&USettings::ESP_Distance), sizeof(USettings::ESP_Distance));
        archivo.read(reinterpret_cast<char*>(&USettings::Night_Mode), sizeof(USettings::Night_Mode));
        archivo.read(reinterpret_cast<char*>(&USettings::ColorBlack), sizeof(USettings::ColorBlack));
        archivo.read(reinterpret_cast<char*>(&USettings::FullBright_Mode), sizeof(USettings::FullBright_Mode));
        archivo.read(reinterpret_cast<char*>(&USettings::ColorWhite), sizeof(USettings::ColorWhite));
        archivo.read(reinterpret_cast<char*>(&USettings::BunnyHop), sizeof(USettings::BunnyHop));
        archivo.read(reinterpret_cast<char*>(&USettings::window_animation), sizeof(USettings::window_animation));
        archivo.read(reinterpret_cast<char*>(&USettings::navigationwindow_animation), sizeof(USettings::navigationwindow_animation));
        archivo.read(reinterpret_cast<char*>(&USettings::optionswindow_animation), sizeof(USettings::optionswindow_animation));
        archivo.read(reinterpret_cast<char*>(&USettings::show_watermark), sizeof(USettings::show_watermark));
        archivo.close();
    }
}

// Función para guardar la configuración en un archivo binario
void FManager::SaveSettings() {
    std::ofstream archivo("sarilla.bin", std::ios::binary);
    if (archivo.is_open()) {
        archivo.write(reinterpret_cast<const char*>(&USettings::MenuWindow), sizeof(USettings::MenuWindow));
        archivo.write(reinterpret_cast<const char*>(&USettings::triggerbot), sizeof(USettings::triggerbot));
        archivo.write(reinterpret_cast<const char*>(&USettings::triggerbot_delayms), sizeof(USettings::triggerbot_delayms));
        archivo.write(reinterpret_cast<const char*>(&USettings::Silent_Aim), sizeof(USettings::Silent_Aim));
        archivo.write(reinterpret_cast<const char*>(&USettings::Head_Target), sizeof(USettings::Head_Target));
        archivo.write(reinterpret_cast<const char*>(&USettings::Body_Target), sizeof(USettings::Body_Target));
        archivo.write(reinterpret_cast<const char*>(&USettings::ShowFov), sizeof(USettings::ShowFov));
        archivo.write(reinterpret_cast<const char*>(&USettings::AimbotFov), sizeof(USettings::AimbotFov));
        archivo.write(reinterpret_cast<const char*>(&USettings::FovThickness), sizeof(USettings::FovThickness));
        archivo.write(reinterpret_cast<const char*>(&USettings::FovColor), sizeof(USettings::FovColor));
        archivo.write(reinterpret_cast<const char*>(&USettings::ShowTarget), sizeof(USettings::ShowTarget));
        archivo.write(reinterpret_cast<const char*>(&USettings::TargetThickness), sizeof(USettings::TargetThickness));
        archivo.write(reinterpret_cast<const char*>(&USettings::TargetColor), sizeof(USettings::TargetColor));
        archivo.write(reinterpret_cast<const char*>(&USettings::FilledCircle), sizeof(USettings::FilledCircle));
        archivo.write(reinterpret_cast<const char*>(&USettings::FilledCircleColor), sizeof(USettings::FilledCircleColor));
        archivo.write(reinterpret_cast<const char*>(&USettings::fov_changer), sizeof(USettings::fov_changer));
        archivo.write(reinterpret_cast<const char*>(&USettings::fov_value), sizeof(USettings::fov_value));
        archivo.write(reinterpret_cast<const char*>(&USettings::oldfov_value), sizeof(USettings::oldfov_value));
        archivo.write(reinterpret_cast<const char*>(&USettings::DrawCrosshair), sizeof(USettings::DrawCrosshair));
        archivo.write(reinterpret_cast<const char*>(&USettings::whennotaiming), sizeof(USettings::whennotaiming));
        archivo.write(reinterpret_cast<const char*>(&USettings::Crosshair_Color), sizeof(USettings::Crosshair_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::Crosshair_size), sizeof(USettings::Crosshair_size));
        archivo.write(reinterpret_cast<const char*>(&USettings::Crosshair_thickness), sizeof(USettings::Crosshair_thickness));
        archivo.write(reinterpret_cast<const char*>(&USettings::circle), sizeof(USettings::circle));
        archivo.write(reinterpret_cast<const char*>(&USettings::Cross), sizeof(USettings::Cross));
        archivo.write(reinterpret_cast<const char*>(&USettings::radar_hack), sizeof(USettings::radar_hack));
        archivo.write(reinterpret_cast<const char*>(&USettings::Show_Enemy), sizeof(USettings::Show_Enemy));
        archivo.write(reinterpret_cast<const char*>(&USettings::Show_Squad), sizeof(USettings::Show_Squad));
        archivo.write(reinterpret_cast<const char*>(&USettings::HealthBar_Esp_Thickness), sizeof(USettings::HealthBar_Esp_Thickness));
        archivo.write(reinterpret_cast<const char*>(&USettings::HealthBar_ESP), sizeof(USettings::HealthBar_ESP));
        archivo.write(reinterpret_cast<const char*>(&USettings::ArmorBar_Esp_Thickness), sizeof(USettings::ArmorBar_Esp_Thickness));
        archivo.write(reinterpret_cast<const char*>(&USettings::ArmorBar_ESP), sizeof(USettings::ArmorBar_ESP));
        archivo.write(reinterpret_cast<const char*>(&USettings::Enemy_Box_Esp_Color), sizeof(USettings::Enemy_Box_Esp_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::Squad_Box_Esp_Color), sizeof(USettings::Squad_Box_Esp_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::Box_Esp_Thickness), sizeof(USettings::Box_Esp_Thickness));
        archivo.write(reinterpret_cast<const char*>(&USettings::Box_ESP), sizeof(USettings::Box_ESP));
        archivo.write(reinterpret_cast<const char*>(&USettings::Enemy_CornerBox_Esp_Color), sizeof(USettings::Enemy_CornerBox_Esp_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::Squad_CornerBox_Esp_Color), sizeof(USettings::Squad_CornerBox_Esp_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::Box_CornerEsp_Thickness), sizeof(USettings::Box_CornerEsp_Thickness));
        archivo.write(reinterpret_cast<const char*>(&USettings::CornerBox_ESP), sizeof(USettings::CornerBox_ESP));
        archivo.write(reinterpret_cast<const char*>(&USettings::Enemy_Name_ESP_Color), sizeof(USettings::Enemy_Name_ESP_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::Squad_Name_ESP_Color), sizeof(USettings::Squad_Name_ESP_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::Name_ESP), sizeof(USettings::Name_ESP));
        archivo.write(reinterpret_cast<const char*>(&USettings::Enemy_Distance_Esp_Color), sizeof(USettings::Enemy_Distance_Esp_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::Squad_Distance_Esp_Color), sizeof(USettings::Squad_Distance_Esp_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::Distance_Esp), sizeof(USettings::Distance_Esp));
        archivo.write(reinterpret_cast<const char*>(&USettings::Enemy_Bone_Esp_Color), sizeof(USettings::Enemy_Bone_Esp_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::Squad_Bone_Esp_Color), sizeof(USettings::Squad_Bone_Esp_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::Bone_Esp_Thickness), sizeof(USettings::Bone_Esp_Thickness));
        archivo.write(reinterpret_cast<const char*>(&USettings::Bone_Esp), sizeof(USettings::Bone_Esp));
        archivo.write(reinterpret_cast<const char*>(&USettings::SnaplLine_Esp_Start_Point), sizeof(USettings::SnaplLine_Esp_Start_Point));
        archivo.write(reinterpret_cast<const char*>(&USettings::SnaplLine_Esp_End_Point), sizeof(USettings::SnaplLine_Esp_End_Point));
        archivo.write(reinterpret_cast<const char*>(&USettings::Enemy_SnaplLine_Esp_Color), sizeof(USettings::Enemy_SnaplLine_Esp_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::Squad_SnaplLine_Esp_Color), sizeof(USettings::Squad_SnaplLine_Esp_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::SnaplLine_Esp_Thickness), sizeof(USettings::SnaplLine_Esp_Thickness));
        archivo.write(reinterpret_cast<const char*>(&USettings::SnaplLine_Esp), sizeof(USettings::SnaplLine_Esp));
        archivo.write(reinterpret_cast<const char*>(&USettings::Enemy_FilledBox_Esp_Color), sizeof(USettings::Enemy_FilledBox_Esp_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::Squad_FilledBox_Esp_Color), sizeof(USettings::Squad_FilledBox_Esp_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::FilledBox_Esp), sizeof(USettings::FilledBox_Esp));
        archivo.write(reinterpret_cast<const char*>(&USettings::Enemy_GunName_Color), sizeof(USettings::Enemy_GunName_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::Squad_GunName_Color), sizeof(USettings::Squad_GunName_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::GunName_Esp), sizeof(USettings::GunName_Esp));
        archivo.write(reinterpret_cast<const char*>(&USettings::Enemy_Box3D_Esp_Color), sizeof(USettings::Enemy_Box3D_Esp_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::Squad_Box3D_Esp_Color), sizeof(USettings::Squad_Box3D_Esp_Color));
        archivo.write(reinterpret_cast<const char*>(&USettings::Box3D_Esp_Thickness), sizeof(USettings::Box3D_Esp_Thickness));
        archivo.write(reinterpret_cast<const char*>(&USettings::Box3D_Esp), sizeof(USettings::Box3D_Esp));
        archivo.write(reinterpret_cast<const char*>(&USettings::Box3D_Width), sizeof(USettings::Box3D_Width));
        archivo.write(reinterpret_cast<const char*>(&USettings::ESP_Distance), sizeof(USettings::ESP_Distance));
        archivo.write(reinterpret_cast<const char*>(&USettings::Night_Mode), sizeof(USettings::Night_Mode));
        archivo.write(reinterpret_cast<const char*>(&USettings::ColorBlack), sizeof(USettings::ColorBlack));
        archivo.write(reinterpret_cast<const char*>(&USettings::FullBright_Mode), sizeof(USettings::FullBright_Mode));
        archivo.write(reinterpret_cast<const char*>(&USettings::ColorWhite), sizeof(USettings::ColorWhite));
        archivo.write(reinterpret_cast<const char*>(&USettings::BunnyHop), sizeof(USettings::BunnyHop));
        archivo.write(reinterpret_cast<const char*>(&USettings::window_animation), sizeof(USettings::window_animation));
        archivo.write(reinterpret_cast<const char*>(&USettings::navigationwindow_animation), sizeof(USettings::navigationwindow_animation));
        archivo.write(reinterpret_cast<const char*>(&USettings::optionswindow_animation), sizeof(USettings::optionswindow_animation));
        archivo.write(reinterpret_cast<const char*>(&USettings::show_watermark), sizeof(USettings::show_watermark));
        archivo.close();
    }
}