#include "include.h"

void FManager::ReadSettings() {
    std::ifstream file("sarilla.bin", std::ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&CSettings::settings), sizeof(CSettings::settings));
        file.close();
    }
}

// Funcin para guardar la configuracin en un archivo binario
void FManager::SaveSettings() {
    std::ofstream file("sarilla.bin", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(&CSettings::settings), sizeof(CSettings::settings));
        file.close();
    }
}