#pragma once
#include <cstdio>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <Tlhelp32.h>
#include <atlconv.h>
#include <thread>
#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <dwmapi.h>
#include "ProcessManager.hpp"
#include "Overlay.hpp"
#include <DirectX/d3d9.h>
#include <DirectX/D3dx9tex.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx9.h>
#include <ImGui/imgui_impl_win32.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "D3dx9")

std::string targetProcessName = "cs2.exe";
float X_Screen;
float Y_Screen;
DWORD64 client;
#define M_PI       3.14159265358979323846   // pi

#include "image.h"
#include "Vector.h"
#include "Settings.h"
#include "Offsets.h"
#include "Entitys.h"
#include "WorldToScreen.h"
Entitys* E;
#include "bones.h"
#include "Draw.h"
#include "FileManager.h"
#include "simulate.h"
#include "Threads.h"
#include "guns.h"
#include "Aimbot.hpp"