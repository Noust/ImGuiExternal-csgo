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
    // Reducir pasos para movimiento más rápido pero aún suave
    const int steps = 15;
    
    // Calcular incrementos por paso
    float stepX = (float)dx / steps;
    float stepY = (float)dy / steps;
    
    float accX = 0.0f;
    float accY = 0.0f;

    for (int i = 0; i < steps; i++) {
        accX += stepX;
        accY += stepY;
        
        int moveX = (int)accX;
        int moveY = (int)accY;
        
        // Solo enviar evento si hay movimiento real
        if (moveX != 0 || moveY != 0) {
            mouse_event(MOUSEEVENTF_MOVE, moveX, moveY, 0, 0);
            accX -= moveX;
            accY -= moveY;
        }

        // Menor delay entre movimientos
        Sleep(0);
    }
}

void sim::MoveToTarget(int targetX, int targetY) {
    // Usar centro de pantalla directamente sin GetCursorPos
    int dx = targetX - (X_Screen / 2);
    int dy = targetY - (Y_Screen / 2);
    
    // Aplicar un límite máximo al movimiento para evitar saltos bruscos
    const int maxMove = 100;
    dx = (dx > maxMove) ? maxMove : (dx < -maxMove) ? -maxMove : dx;
    dy = (dy > maxMove) ? maxMove : (dy < -maxMove) ? -maxMove : dy;
    
    SimulateMouseMove(dx, dy);
}