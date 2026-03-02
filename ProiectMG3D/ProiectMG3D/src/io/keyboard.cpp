#include "io/keyboard.h"

bool Keyboard::keys[GLFW_KEY_LAST] = { 0 };
bool Keyboard::keysChanged[GLFW_KEY_LAST] = { 0 };

void Keyboard::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_RELEASE) {
        if (!keys[key]) {
            keys[key] = true;
        }
    }
    else {
        keys[key] = false;
    }
    keysChanged[key] = action != GLFW_REPEAT;
}

bool Keyboard::Key(int key) {
    return keys[key];
}

bool Keyboard::KeyChanged(int key) {
    bool ret = keysChanged[key];
    // set to false because change no longer new
    keysChanged[key] = false;
    return ret;
}

bool Keyboard::KeyWentDown(int key) {
    return keys[key] && KeyChanged(key);
}

bool Keyboard::KeyWentUp(int key) {
    return !keys[key] && KeyChanged(key);
}