#include "include.h"


void sim::SimKey(int KeyID) {
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = KeyID;
    SendInput(1, &input, sizeof(INPUT));
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void sim::SimClick() {
    mouse_event(MOUSEEVENTF_LEFTDOWN, 1920 / 2, 1080 / 2, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 1920 / 2, 1080 / 2, 0, 0);
}

void sim::SimulateMouseMove(int dx, int dy) {
    POINT currentPos;
    GetCursorPos(&currentPos);

    const int steps = 30;

    double stepX = (double)dx / steps;
    double stepY = (double)dy / steps;

    double errorX = 0.0;
    double errorY = 0.0;

    for (int i = 0; i < steps; i++) {
        errorX += stepX;
        errorY += stepY;

        int moveX = (int)errorX;
        int moveY = (int)errorY;

        errorX -= moveX;
        errorY -= moveY;

        if (moveX | moveY) {
            mouse_event(MOUSEEVENTF_MOVE,
                (DWORD)moveX,
                (DWORD)moveY,
                0,
                0);
        }

        Sleep(1);
    }
}