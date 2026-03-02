#include "mouse.h"

double Mouse::x = 0;
double Mouse::y = 0;

double Mouse::lastX = 0;
double Mouse::lastY = 0;

double Mouse::dx = 0;
double Mouse::dy = 0;

double Mouse::scrollDx = 0;
double Mouse::scrollDy = 0;

bool Mouse::firstMouse = true;

bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = { 0 };
bool Mouse::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = { 0 };

void Mouse::CursorPosCallback(GLFWwindow* window, double _x, double _y) {
    x = _x;
    y = _y;

    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    dx = x - lastX;
    dy = lastY - y; // y coordinates are inverted
    lastX = x;
    lastY = y;
}

void Mouse::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action != GLFW_RELEASE) {
        if (!buttons[button]) {
            buttons[button] = true;
        }
    }
    else {
        buttons[button] = false;
    }
    buttonsChanged[button] = action != GLFW_REPEAT;
}

void Mouse::MouseWheelCallback(GLFWwindow* window, double dx, double dy) {
    scrollDx = dx;
    scrollDy = dy;
}

double Mouse::GetMouseX() {
    return x;
}

double Mouse::GetMouseY() {
    return y;
}

double Mouse::GetDX() {
    double _dx = dx;
    // set to 0 because change no longer new
    dx = 0;
    return _dx;
}

double Mouse::GetDY() {
    double _dy = dy;
    dy = 0;
    return _dy;
}

double Mouse::GetScrollDX() {
    double _scrollDx = scrollDx;
    // set to 0 because change no longer new
    scrollDx = 0;
    return _scrollDx;
}

double Mouse::GetScrollDY() {
    double _scrollDy = scrollDy;
    // set to 0 because change no longer new
    scrollDy = 0;
    return _scrollDy;
}

bool Mouse::Button(int button) {
    return buttons[button];
}

bool Mouse::ButtonChanged(int button) {
    bool ret = buttonsChanged[button];
    // set to false because change no longer new
    buttonsChanged[button] = false;
    return ret;
}

bool Mouse::ButtonWentUp(int button) {
    return !buttons[button] && ButtonChanged(button);
}

bool Mouse::ButtonWentDown(int button) {
    return buttons[button] && ButtonChanged(button);
}