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
    mouse_event(MOUSEEVENTF_LEFTDOWN, X_Screen / 2, Y_Screen / 2, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, X_Screen / 2, Y_Screen / 2, 0, 0);
}