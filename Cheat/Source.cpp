bool exitLoop = false;
bool isMenuVisible = true;
bool sizeChanged = false;
int maxPlayerCount = 500;
int loopDelay = 10;

#include <mjLib/mjLib.hpp>
#include <DirectX/D3D11.h>
#include <DirectX/D3DX11.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_internal.h>
#include "DrawImGui.hpp"

#include "Game.hpp"
#include "Memory.hpp"
#include "Menu.hpp"
#include "SimpleMath.h"
#include "Cheat.hpp"
#include "Overlay.hpp"

int main(){ 
    mjLib::Console::Enable();
    SetConsoleTitle(mjLib::String::Random(30).c_str());

    mjLib::Logger::WriteLog(XorString("Searching for the game process..."), mjLib::Logger::LogLevel::LOG_DEBUG);
    while (!Game.pID) {
        Game.pID = FindGame();
    }

    mjLib::Logger::WriteLog(XorString("Game process found!"), mjLib::Logger::LogLevel::LOG_SUCCESSFUL);
    mjLib::Logger::WriteLog(XorString("Requesting access to the game..."), mjLib::Logger::LogLevel::LOG_DEBUG);

    Sleep(5000);
    Game.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Game.pID);
    if (!Game.hProcess) {
        mjLib::Logger::WriteLog(XorString("Failed to gain access to the game!"), mjLib::Logger::LogLevel::LOG_ERROR);
        mjLib::Console::ExitError();
    }

    mjLib::Logger::WriteLog(XorString("Searching for the game window..."), mjLib::Logger::LogLevel::LOG_DEBUG);
    while (!Game.hWnd) {
        Game.hWnd = FindWindow(Game.lpClassName, NULL);
        Offsets.GameBase = GetBaseAddress();
        Offsets.CitizenPlayernamesBase = GetBaseAddress(XorString("citizen-playernames-five.dll"));
    }

    mjLib::Logger::WriteLog(XorString("Game window found!"), mjLib::Logger::LogLevel::LOG_SUCCESSFUL);
    ReadOffsets();

    mjLib::Logger::WriteLog(XorString("Creating overlay..."), mjLib::Logger::LogLevel::LOG_DEBUG);
    std::thread([&]() { UpdateOverlay(); }).detach();
    std::thread([&]() { UpdatePeds(); }).detach();
    std::thread([=]() { SetAim(); }).detach();
    std::thread([=]() { TriggerBot(); }).detach();
    std::thread([=]() { AddPlayerList(); }).detach();
    std::thread([=]() { Exploits(); }).detach();

    while (!exitLoop) {
        if (CreateOverlay()) {
            mjLib::Logger::WriteLog(XorString("Overlay created successfully!"), mjLib::Logger::LogLevel::LOG_SUCCESSFUL);
            mjLib::Console::Close();
            OverlayMain();
        }
    }
}